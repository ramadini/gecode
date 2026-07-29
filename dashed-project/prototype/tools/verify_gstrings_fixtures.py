#!/usr/bin/env python3
"""Verify translated differential fixtures against preserved G-Strings source."""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
from pathlib import Path
from typing import Iterable

LEGACY_SOURCE = Path("gecode/string/tests/str_test2.cpp")


def normalized(text: str) -> str:
    return re.sub(r"\s+", "", text)


def extract_function(source: str, name: str) -> str:
    match = re.search(rf"\bvoid\s+{re.escape(name)}\s*\(\s*\)\s*\{{", source)
    if match is None:
        raise ValueError(f"could not find void {name}()")

    opening = source.find("{", match.start())
    depth = 0
    for index in range(opening, len(source)):
        if source[index] == "{":
            depth += 1
        elif source[index] == "}":
            depth -= 1
            if depth == 0:
                return source[match.start() : index + 1]
    raise ValueError(f"unterminated function {name}")


def candidate_roots(explicit: Iterable[Path]) -> list[Path]:
    roots = [path.expanduser() for path in explicit]
    environment = os.environ.get("DASHED_TESTS_OLD")
    if environment:
        roots.append(Path(environment).expanduser())
    roots.extend(
        [
            Path.home() / "Dashed" / "tests-old",
            Path.home() / "tests-old",
            Path.home() / "src" / "tests-old",
            Path.cwd().parent / "tests-old",
            Path.cwd() / "tests-old",
        ]
    )

    unique: list[Path] = []
    seen: set[Path] = set()
    for root in roots:
        resolved = root.resolve(strict=False)
        if resolved not in seen:
            seen.add(resolved)
            unique.append(resolved)
    return unique


def discover_source(roots: Iterable[Path]) -> Path | None:
    candidates: list[Path] = []
    for root in roots:
        if root.is_file() and root.name == LEGACY_SOURCE.name:
            candidates.append(root)
            continue
        if not root.is_dir():
            continue
        direct = root / LEGACY_SOURCE
        if direct.is_file():
            candidates.append(direct)
        candidates.extend(root.rglob(LEGACY_SOURCE.name))

    files = sorted(
        {path.resolve() for path in candidates if path.is_file()},
        key=lambda path: (len(path.parts), str(path)),
    )
    return files[0] if files else None


def git_text(repository: Path, revision: str, relative: Path) -> str | None:
    result = subprocess.run(
        ["git", "-C", str(repository), "show", f"{revision}:{relative.as_posix()}"],
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    )
    if result.returncode != 0:
        return None
    return result.stdout


def repository_root() -> Path | None:
    result = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"],
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    )
    if result.returncode != 0:
        return None
    return Path(result.stdout.strip()).resolve()


def discover_git_source(fetch_origin: bool) -> tuple[str, str] | None:
    repository = repository_root()
    if repository is None:
        return None

    revisions: list[str] = [
        "origin/master",
        "origin/main",
        "upstream/master",
        "upstream/main",
    ]
    history = subprocess.run(
        [
            "git",
            "-C",
            str(repository),
            "rev-list",
            "--all",
            "--",
            LEGACY_SOURCE.as_posix(),
        ],
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    )
    if history.returncode == 0:
        revisions = history.stdout.splitlines() + revisions

    seen: set[str] = set()
    for revision in revisions:
        if not revision or revision in seen:
            continue
        seen.add(revision)
        source = git_text(repository, revision, LEGACY_SOURCE)
        if source is not None:
            return (f"git:{revision}:{LEGACY_SOURCE.as_posix()}", source)

    if not fetch_origin:
        return None

    for branch in ("master", "main"):
        fetched = subprocess.run(
            ["git", "-C", str(repository), "fetch", "--quiet", "origin", branch],
            check=False,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        if fetched.returncode != 0:
            continue
        source = git_text(repository, "FETCH_HEAD", LEGACY_SOURCE)
        if source is not None:
            commit = subprocess.run(
                ["git", "-C", str(repository), "rev-parse", "FETCH_HEAD"],
                check=True,
                stdout=subprocess.PIPE,
                text=True,
            ).stdout.strip()
            return (f"git:{commit}:{LEGACY_SOURCE.as_posix()}", source)

    return None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--root", action="append", type=Path, default=[])
    parser.add_argument("--optional", action="store_true")
    parser.add_argument(
        "--fetch-origin",
        action="store_true",
        help="fetch origin/master or origin/main if no preserved local source is available",
    )
    arguments = parser.parse_args()

    manifest = json.loads(arguments.manifest.read_text(encoding="utf-8"))
    expected_source = Path(manifest.get("source", LEGACY_SOURCE.as_posix()))
    if expected_source != LEGACY_SOURCE:
        raise SystemExit(
            f"unsupported legacy source {expected_source}; expected {LEGACY_SOURCE}"
        )

    source_path = discover_source(candidate_roots(arguments.root))
    if source_path is not None:
        source_label = str(source_path)
        source = source_path.read_text(encoding="utf-8")
    else:
        git_source = discover_git_source(arguments.fetch_origin)
        if git_source is None:
            message = (
                "could not locate preserved G-Strings str_test2.cpp in the filesystem "
                "or local Git refs; set DASHED_TESTS_OLD, pass --root, or use "
                "--fetch-origin"
            )
            if arguments.optional:
                print(f"SKIP: {message}")
                return 0
            raise SystemExit(message)
        source_label, source = git_source

    verified = 0
    for case in manifest["cases"]:
        body = extract_function(source, case["function"])
        compact = normalized(body)
        missing = [
            anchor
            for anchor in case["anchors"]
            if normalized(anchor) not in compact
        ]
        if missing:
            details = "\n".join(f"  - {anchor}" for anchor in missing)
            raise SystemExit(
                f"{case['id']} no longer matches {source_label}; missing anchors:\n"
                f"{details}"
            )
        verified += 1

    print(f"verified {verified} G-Strings fixtures against {source_label}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
