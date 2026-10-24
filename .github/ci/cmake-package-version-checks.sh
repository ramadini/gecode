#!/usr/bin/env bash
set -euo pipefail

prefix="${1:?usage: cmake-package-version-checks.sh <install-prefix>}"
config_version=$(find "$prefix" -type f -name GecodeConfigVersion.cmake -print -quit)
if [ -z "$config_version" ]; then
  echo "GecodeConfigVersion.cmake not found below $prefix" >&2
  exit 1
fi

version=$(sed -n 's/^set(PACKAGE_VERSION "\([0-9][0-9.]*\)")$/\1/p' "$config_version" | head -n 1)
IFS=. read -r major minor patch <<<"$version"
if [ -z "${major:-}" ] || [ -z "${minor:-}" ] || [ -z "${patch:-}" ]; then
  echo "Could not read a three-part package version from $config_version" >&2
  exit 1
fi

work=$(mktemp -d "${RUNNER_TEMP:-${TMPDIR:-/tmp}}/gecode-package-version-checks.XXXXXX")
trap 'rm -rf -- "$work"' EXIT
trap 'exit 1' HUP INT TERM

# Exercise the installed compatibility policy without configuring a compiler.
cat > "$work/check.cmake" <<'EOF'
cmake_minimum_required(VERSION 3.21)
set(PACKAGE_FIND_VERSION "${VERSION}")
set(PACKAGE_FIND_VERSION_MAJOR "${MAJOR}")
set(PACKAGE_FIND_VERSION_MINOR "${MINOR}")
set(PACKAGE_FIND_VERSION_PATCH "${PATCH}")
set(PACKAGE_FIND_VERSION_COUNT 3)
include("${CONFIG_VERSION}")
if(NOT PACKAGE_VERSION_COMPATIBLE OR NOT PACKAGE_VERSION_EXACT)
  message(FATAL_ERROR "Gecode package rejected its exact version ${VERSION}")
endif()

math(EXPR PACKAGE_FIND_VERSION_MINOR "${MINOR} - 1")
set(PACKAGE_FIND_VERSION_PATCH 0)
set(PACKAGE_FIND_VERSION "${MAJOR}.${PACKAGE_FIND_VERSION_MINOR}.0")
unset(PACKAGE_VERSION_COMPATIBLE)
unset(PACKAGE_VERSION_EXACT)
include("${CONFIG_VERSION}")
if(PACKAGE_VERSION_COMPATIBLE)
  message(FATAL_ERROR "Gecode ${VERSION} unexpectedly satisfied request ${PACKAGE_FIND_VERSION}")
endif()
EOF

cmake "-DCONFIG_VERSION=$config_version" "-DVERSION=$version" \
  "-DMAJOR=$major" "-DMINOR=$minor" "-DPATCH=$patch" -P "$work/check.cmake"
