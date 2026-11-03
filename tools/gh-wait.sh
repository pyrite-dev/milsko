#!/bin/sh
# Wait for a GitHub Actions run on the mirror to finish and fetch its artifact.
# Only makes outbound requests, so it works from behind a firewall.
#
# usage: tools/gh-wait.sh <repo> <workflow file> <commit sha> <artifact> <outdir>
# needs: gh, with GH_TOKEN set
set -e

repo=$1
workflow=$2
sha=$3
artifact=$4
outdir=$5

# The mirror can lag behind forgejo, so the run may not exist yet.
appear_timeout=${GH_WAIT_APPEAR_TIMEOUT:-900}
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

if ! gh run watch "$id" -R "$repo" --exit-status -i 30; then
	gh run view "$id" -R "$repo" --log-failed || true
	exit 1
fi

rm -rf "$outdir"
gh run download "$id" -R "$repo" -n "$artifact" -D "$outdir"
