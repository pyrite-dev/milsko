#!/bin/sh
# waits for a GitHub Actions job to finish and downloads its artifact
# usage: tools/gh-wait.sh <repo> <workflow file> <commit sha> <job name> <artifact> <outdir>
# needs gh, with GH_TOKEN set
set -e

repo=$1
workflow=$2
sha=$3
job=$4
artifact=$5
outdir=$6

# the mirror syncs every 10 minutes, so the run may not exist yet
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

last=
while :; do
	# "<status> <conclusion> <job id>"
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

rm -rf "$outdir"
gh run download "$id" -R "$repo" -n "$artifact" -D "$outdir"
