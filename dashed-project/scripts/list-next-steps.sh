#!/usr/bin/env bash
set -euo pipefail

cat >&2 <<'MESSAGE'
This monolithic bootstrap script has been retired.

The native Gecode module now lives under gecode/list and is maintained through
reviewable three-checkpoint bundles executed by:

  dashed-project/scripts/run-3-step-bundle.sh

Use bundle-template.sh for new changes.
MESSAGE

exit 2
