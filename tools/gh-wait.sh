#!/bin/sh
# Wait for one job of a GitHub Actions run on the mirror to finish and fetch
# its artifact. Only makes outbound requests, so it works from behind a
# firewall. Waiting on a single job (not the whole run) lets each Jenkins
# stage report its own platform as soon as that job is done.
#
# usage: tools/gh-wait.sh <repo> <workflow file> <commit sha> <job name> <artifact> <outdir>
# The artifact's files end up directly in <outdir>.
# needs: gh, with GH_TOKEN set
set -e

repo=$1
workflow=$2
sha=$3
job=$4
artifact=$5
outdir=$6

# The mirror only syncs from forgejo every 10 minutes, so the run may not
# exist yet. Wait up to 20 minutes: a full sync interval, plus slack for a
# late sync and for GitHub to queue the run.
appear_timeout=${GH_WAIT_APPEAR_TIMEOUT:-1200}
waited=0
while :; do
	id=$(gh run list -R "$repo" -w "$workflow" -c "$sha" -L 1 --json databaseId -q '.[0].databaseId')
	if [ -n "$id" ]; then
		break
	fi
	if [ "$waited" -ge "$appear_timeout" ]; then
		echo "no $workflow run for $sha on $repo after ${appear_timeout}s" >&2
		exit 1
	fi
	sleep 15
	waited=$((waited + 15))
done

echo "https://github.com/$repo/actions/runs/$id"

# Poll the job; the overall time limit is the Jenkins stage's timeout.
last=
while :; do
	# "<status> <conclusion> <job id>"; conclusion is empty until it's done
	info=$(gh run view "$id" -R "$repo" --json jobs -q ".jobs[] | select(.name == \"$job\") | \"\(.status) \(.conclusion) \(.databaseId)\"")
	status=${info%% *}
	if [ "$info" != "$last" ]; then
		echo "$job: ${info:-not created yet}"
		last=$info
	fi
	if [ "$status" = "completed" ]; then
		break
	fi
	sleep 30
done

rest=${info#* }
conclusion=${rest%% *}
jobid=${rest#* }
echo "https://github.com/$repo/actions/runs/$id/job/$jobid"

if [ "$conclusion" != "success" ]; then
	echo "$job finished with: $conclusion" >&2
	gh run view -R "$repo" --job "$jobid" --log-failed || true
	exit 1
fi

# Artifacts can be downloaded as soon as their job uploads them, even while
# other jobs in the run are still going.
rm -rf "$outdir"
gh run download "$id" -R "$repo" -n "$artifact" -D "$outdir"
