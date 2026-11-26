#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shlex
from enum import Enum
from itertools import zip_longest
import subprocess
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Tuple
import tempfile


TASK_NAME_RE = re.compile(r"^task_\d+$")


class CaseStatus(Enum):
    OK = "ok"
    FAIL = "fail"
    TIMEOUT = "timeout"
    NO_EXPECTED = "no_expected"
    EXEC_MISSING = "exec_missing"
    SLOW = "slow"
    ERROR = "error"
    MEM_EXCEEDED = "mem_exceeded"


CASE_VERDICTS = {
    CaseStatus.OK: "OK",
    CaseStatus.FAIL: "Wrong Answer",
    CaseStatus.TIMEOUT: "Time Limit Exceeded",
    CaseStatus.ERROR: "Runtime Error",
    CaseStatus.SLOW: "Slow",
    CaseStatus.MEM_EXCEEDED: "Memory Limit Exceeded",
    CaseStatus.NO_EXPECTED: "No Expected Output",
    CaseStatus.EXEC_MISSING: "Executable Missing",
}


@dataclass
class CaseResult:
    task: str
    case_name: str
    status: CaseStatus
    duration_ms: int
    message: str = ""
    peak_kb: Optional[int] = None


def find_repo_root(start: Path) -> Path:
    # Assumes this script is located at <root>/scripts/run_cases.py
    scripts_dir = start.parent
    return scripts_dir.parent


def discover_executables(build_dir: Path) -> Dict[str, Path]:
    executables: Dict[str, Path] = {}
    if not build_dir.exists():
        return executables

    for item in build_dir.iterdir():
        if not item.is_dir():
            continue
        name = item.name
        if not TASK_NAME_RE.match(name):
            continue
        exe_candidate = item / name
        if exe_candidate.exists() and exe_candidate.is_file() and os.access(exe_candidate, os.X_OK):
            executables[name] = exe_candidate
    return executables


def discover_tasks_from_tests(tests_dir: Path) -> List[str]:
    tasks: List[str] = []
    if not tests_dir.exists():
        return tasks
    for item in tests_dir.iterdir():
        if item.is_dir() and TASK_NAME_RE.match(item.name):
            tasks.append(item.name)
    return tasks


def discover_tasks_in_repo(repo_root: Path) -> List[str]:
    tasks: List[str] = []
    for item in repo_root.iterdir():
        if item.is_dir() and TASK_NAME_RE.match(item.name):
            tasks.append(item.name)
    return sorted(tasks)


def list_cases_for_task_central(tests_dir: Path, task: str) -> List[Tuple[Path, Path]]:
    """<tests_dir>/<task>/*.in with same-stem .out"""
    task_dir = tests_dir / task
    if not task_dir.exists():
        return []
    pairs: List[Tuple[Path, Path]] = []
    for input_file in sorted(task_dir.glob("*.in")):
        expected_file = input_file.with_suffix(".out")
        pairs.append((input_file, expected_file))
    return pairs


def list_cases_for_task_in_task(repo_root: Path, task: str) -> List[Tuple[Path, Path]]:
    """Search inside task dir: prefer task/tests/*.in, fallback to task/*.in"""
    task_dir = repo_root / task
    if not task_dir.exists():
        return []
    candidates: List[Path] = []
    tests_subdir = task_dir / "tests"
    if tests_subdir.exists():
        candidates.extend(sorted(tests_subdir.glob("*.in")))
        candidates.extend(sorted((tests_subdir / "custom").glob("*.in")))
    # Also allow inputs directly under the task directory
    candidates.extend(sorted(task_dir.glob("*.in")))
    # Deduplicate while preserving order
    seen: set[Path] = set()
    inputs: List[Path] = []
    for p in candidates:
        if p not in seen:
            seen.add(p)
            inputs.append(p)
    pairs: List[Tuple[Path, Path]] = []
    for input_file in inputs:
        expected_file = input_file.with_suffix(".out")
        pairs.append((input_file, expected_file))
    return pairs


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def write_text(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def normalize(s: str, mode: str) -> str:
    s = s.replace("\r\n", "\n").replace("\r", "\n")
    if mode == "strip":
        return s.rstrip()
    if mode == "keep":
        return s
    if mode == "lines":
        # Normalize per-line trailing whitespace
        return "\n".join(line.rstrip() for line in s.split("\n")).rstrip()
    return s.rstrip()


def run_executable(exe: Path, stdin_data: str, timeout_sec: float, env: Optional[Dict[str, str]] = None) -> Tuple[Optional[int], str, str, float]:
    start = time.perf_counter()
    try:
        completed = subprocess.run(
            [str(exe)],
            input=stdin_data,
            capture_output=True,
            text=True,
            timeout=timeout_sec,
            env=env,
        )
        duration = time.perf_counter() - start
        return completed.returncode, completed.stdout, completed.stderr, duration
    except subprocess.TimeoutExpired:
        duration = time.perf_counter() - start
        return None, "", "", duration


def find_time_tool() -> Optional[str]:
    # Prefer GNU time executable
    for candidate in ["/usr/bin/time", "/bin/time"]:
        if os.path.exists(candidate) and os.access(candidate, os.X_OK):
            return candidate
    return None


def run_executable_with_memory(
    exe: Path, stdin_data: str, timeout_sec: float, time_tool: str, env: Optional[Dict[str, str]] = None
) -> Tuple[Optional[int], str, str, float, Optional[int]]:
    # Use GNU time to measure peak RSS in KB via %M
    with tempfile.NamedTemporaryFile(prefix="runner_mem_", delete=False) as tmp:
        mem_file = tmp.name
    try:
        start = time.perf_counter()
        try:
            completed = subprocess.run(
                [time_tool, "-f", "MAXRSS=%M", "-o", mem_file, str(exe)],
                input=stdin_data,
                capture_output=True,
                text=True,
                timeout=timeout_sec,
                env=env,
            )
            duration = time.perf_counter() - start
            rc = completed.returncode
            stdout = completed.stdout
            stderr = completed.stderr
        except subprocess.TimeoutExpired:
            duration = time.perf_counter() - start
            rc = None
            stdout = ""
            stderr = ""

        # Read memory usage if file exists
        peak_kb: Optional[int] = None
        try:
            data = Path(mem_file).read_text(encoding="utf-8").strip()
            # Expect a single line like MAXRSS=12345
            for line in data.splitlines():
                if line.startswith("MAXRSS="):
                    value = line.split("=", 1)[1].strip()
                    if value.isdigit():
                        peak_kb = int(value)
                    break
        except Exception:
            peak_kb = None
        return rc, stdout, stderr, duration, peak_kb
    finally:
        try:
            os.remove(mem_file)
        except OSError:
            pass


def compare_outputs(actual: str, expected: str) -> bool:
    return actual == expected


def output_preview(value: Optional[str]) -> str:
    if value is None:
        return "<end of output>"
    return repr(value[:160]) + (" ..." if len(value) > 160 else "")


def print_output_difference(actual: str, expected: str) -> None:
    lines = zip_longest(expected.split("\n") if expected else [],
                        actual.split("\n") if actual else [])
    for number, (want, got) in enumerate(lines, 1):
        if want != got:
            print(f"  First difference at line {number}")
            print(f"  Expected: {output_preview(want)}")
            print(f"  Actual:   {output_preview(got)}")
            break


def print_replay(exe: Path, input_file: Path) -> None:
    command = shlex.quote(str(exe.absolute()))
    source = shlex.quote(str(input_file.absolute()))
    print(f"  Run manually: {command} < {source}")


def format_ms(seconds: float) -> int:
    return int(round(seconds * 1000))


def main(argv: Optional[Iterable[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Run task executables and compare outputs. Supports tests in each task folder "
            "(task_xx/tests/*.in|.out or task_xx/*.in|.out) or a central tests directory."
        )
    )
    parser.add_argument(
        "--tests-dir",
        type=Path,
        default=None,
        help="Path to central tests directory (used when --tests-layout=central).",
    )
    parser.add_argument(
        "--tests-layout",
        choices=["in-task", "central"],
        default="in-task",
        help="Where tests reside: in each task dir (default) or central tests dir.",
    )
    parser.add_argument(
        "--build-dir",
        type=Path,
        default=None,
        help="Path to build directory (default: <repo>/build)",
    )
    parser.add_argument(
        "--tasks",
        nargs="*",
        default=[],
        help="Specific task names to run (e.g., task_01 task_03). Default: autodetect",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=2.0,
        help="Per-case timeout in seconds (default: 2.0)",
    )
    parser.add_argument(
        "--mem-limit-mb",
        type=float,
        default=None,
        help=(
            "Soft per-case memory limit in MB. If exceeded, case is marked as MEM_EXCEEDED. "
            "Combine with --fail-on-mem to make it fail the run."
        ),
    )
    parser.add_argument(
        "--fail-on-mem",
        action="store_true",
        help=(
            "Exit with non-zero status if any case exceeds --mem-limit-mb (i.e., MEM_EXCEEDED)."
        ),
    )
    parser.add_argument(
        "--report-memtop",
        type=int,
        default=0,
        help=(
            "Print top-N highest memory usage cases in the summary (0 to disable)."
        ),
    )
    parser.add_argument(
        "--time-limit",
        type=float,
        default=None,
        help=(
            "Soft per-case time limit in seconds. If exceeded, case is marked as SLOW. "
            "Combine with --fail-on-slow to make it fail the run."
        ),
    )
    parser.add_argument(
        "--fail-on-slow",
        action="store_true",
        help=(
            "Exit with non-zero status if any case exceeds --time-limit (i.e., SLOW cases)."
        ),
    )
    parser.add_argument(
        "--report-slowest",
        type=int,
        default=0,
        help=(
            "Print top-N slowest cases in the summary (0 to disable)."
        ),
    )
    parser.add_argument(
        "--normalize",
        choices=["strip", "keep", "lines"],
        default="strip",
        help=(
            "Output normalization: strip=trim trailing whitespace; keep=exact; lines=trim per-line"
        ),
    )
    parser.add_argument(
        "--write-missing",
        action="store_true",
        help="If expected .out is missing, write it from current output",
    )
    parser.add_argument(
        "--update-expected",
        action="store_true",
        help="Overwrite existing .out files with current outputs",
    )
    parser.add_argument(
        "--save-actual",
        action="store_true",
        help="On mismatch, write <case>.out.actual with the produced output",
    )

    parser.add_argument("--example", choices=["bfs_visualization"],
                        help="Run the standalone BFS teaching example")
    parser.add_argument("--trace-dir", type=Path, nargs="?", const=Path(".graph-traces"),
                        help="Record steps per case (default: .graph-traces)")

    args = parser.parse_args(list(argv) if argv is not None else None)

    script_path = Path(__file__).resolve()
    repo_root = find_repo_root(script_path)
    tests_dir = args.tests_dir or (repo_root / "tests")
    build_dir = args.build_dir or (repo_root / "build")

    if args.example and (args.tasks or args.tests_layout != "in-task"):
        parser.error("--example cannot be combined with --tasks or --tests-layout=central")
    if args.trace_dir and (args.mem_limit_mb is not None or args.report_memtop or args.time_limit is not None):
        parser.error("Record traces separately from performance measurements")
    all_execs = discover_executables(build_dir)
    if args.example:
        example_exe = build_dir / "examples" / args.example / args.example
        if example_exe.is_file() and os.access(example_exe, os.X_OK):
            all_execs[args.example] = example_exe

    # Determine tasks to run
    requested: List[str] = [args.example] if args.example else list(args.tasks)
    if not requested:
        if args.tests_layout == "central":
            # Prefer tasks present in central tests dir; fallback to executables
            requested = sorted(
                set(discover_tasks_from_tests(tests_dir)) or set(all_execs.keys())
            )
        else:
            # From repo tasks; if none have .in files yet, still run for all discovered tasks
            repo_tasks = discover_tasks_in_repo(repo_root)
            requested = repo_tasks

    if not requested:
        print(
            "No tasks found. Ensure task folders like 'task_01' exist and are built."
        )
        return 2

    # Ensure executables for requested tasks
    missing_execs: List[str] = []
    task_to_exe: Dict[str, Path] = {}
    for task in requested:
        exe = all_execs.get(task)
        if exe is None:
            # Try an alternative common layout: build/<task>
            alt = build_dir / task
            if alt.exists() and alt.is_file() and os.access(alt, os.X_OK):
                exe = alt
        if exe is None:
            missing_execs.append(task)
        else:
            task_to_exe[task] = exe

    if missing_execs:
        for t in missing_execs:
            print(f"[WARN] Executable not found for {t}. Expected at '{build_dir / t / t}'")

    results: List[CaseResult] = []
    total_cases = 0
    result_inputs: List[dict] = []

    # Decide whether to measure memory
    time_tool = find_time_tool()
    measure_memory = bool(time_tool) and (args.mem_limit_mb is not None or args.report_memtop)

    for task in sorted(requested):
        exe = task_to_exe.get(task)
        if args.tests_layout == "central":
            cases = list_cases_for_task_central(tests_dir, task)
            no_cases_hint = str(tests_dir / task)
        else:
            cases = list_cases_for_task_in_task(repo_root / "examples" if args.example else repo_root, task)
            no_cases_hint = f"{repo_root / task}/(tests|.)"
        if not cases:
            print(f"[INFO] No test cases for {task} in '{no_cases_hint}'. Skipping.")
            continue
        print(f"\n=== {task} ({len(cases)} case(s)) ===")
        for input_file, expected_file in cases:
            total_cases += 1
            # Hash the exact snapshots used by this run, not files reread after it.
            input_bytes = input_file.read_bytes()
            expected_bytes = expected_file.read_bytes() if expected_file.exists() else None
            snapshot = {
                "id": os.path.relpath(input_file, repo_root).replace(os.sep, "/"),
                "inputHash": hashlib.sha256(input_bytes).hexdigest(),
                "expectedHash": hashlib.sha256(expected_bytes).hexdigest() if expected_bytes is not None else None,
            }
            result_inputs.append(snapshot)
            case_name = input_file.stem
            if args.save_actual or args.trace_dir:
                expected_file.with_suffix(".out.actual").unlink(missing_ok=True)
            run_env = os.environ.copy()
            # Ordinary checks must not inherit an unrelated recording destination.
            run_env.pop("GRAPH_TRACE", None)
            if args.trace_dir:
                # Separate equally named built-in, custom and task-root cases.
                group = "custom" if input_file.parent.name == "custom" else (
                    "root" if args.tests_layout == "in-task" and input_file.parent.name == task else ""
                )
                trace_path = args.trace_dir.resolve() / task / group / (case_name + ".jsonl")
                trace_path.parent.mkdir(parents=True, exist_ok=True)
                trace_path.unlink(missing_ok=True)
                run_env["GRAPH_TRACE"] = str(trace_path)
            if exe is None:
                results.append(CaseResult(task, case_name, CaseStatus.EXEC_MISSING, 0, "Executable is missing"))
                print(f"{case_name}: EXEC MISSING")
                continue
            stdin_data = input_bytes.decode("utf-8").replace("\r\n", "\n").replace("\r", "\n")
            if measure_memory:
                rc, stdout, stderr, duration, peak_kb = run_executable_with_memory(
                    exe, stdin_data, args.timeout, time_tool, run_env  # type: ignore[arg-type]
                )
            else:
                rc, stdout, stderr, duration = run_executable(exe, stdin_data, args.timeout, run_env)
                peak_kb = None
            duration_ms = format_ms(duration)

            if rc is None:
                results.append(CaseResult(task, case_name, CaseStatus.TIMEOUT, duration_ms, "Timed out", peak_kb))
                suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                print(f"{case_name}: TIMEOUT ({duration_ms} ms{suffix})")
                print(f"  Time limit: {args.timeout:g} s")
                print_replay(exe, input_file)
                continue

            if rc != 0:
                reason = f"signal {-rc}" if rc < 0 else f"exit code {rc}"
                results.append(CaseResult(task, case_name, CaseStatus.ERROR, duration_ms, reason, peak_kb))
                print(f"{case_name}: ERROR ({reason}, {duration_ms} ms)")
                if stderr:
                    print(f"  Stderr: {output_preview(stderr)}")
                print_replay(exe, input_file)
                continue

            actual_norm = normalize(stdout, args.normalize)

            if args.update_expected and expected_bytes is not None:
                write_text(expected_file, actual_norm + "\n")
                snapshot["expectedHash"] = hashlib.sha256(expected_file.read_bytes()).hexdigest()
                results.append(CaseResult(task, case_name, CaseStatus.OK, duration_ms, "updated expected", peak_kb))
                suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                print(f"{case_name}: UPDATED ({duration_ms} ms{suffix})")
                continue

            if expected_bytes is None:
                if args.write_missing:
                    write_text(expected_file, actual_norm + "\n")
                    snapshot["expectedHash"] = hashlib.sha256(expected_file.read_bytes()).hexdigest()
                    results.append(CaseResult(task, case_name, CaseStatus.OK, duration_ms, "wrote expected", peak_kb))
                    suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                    print(f"{case_name}: WROTE EXPECTED ({duration_ms} ms{suffix})")
                else:
                    if args.save_actual:
                        write_text(expected_file.with_suffix(".out.actual"), actual_norm + "\n")
                    results.append(
                        CaseResult(task, case_name, CaseStatus.NO_EXPECTED, duration_ms, "expected .out missing", peak_kb)
                    )
                    suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                    print(f"{case_name}: NO EXPECTED ({duration_ms} ms{suffix})")
                continue

            expected_norm = normalize(expected_bytes.decode("utf-8"), args.normalize)
            if compare_outputs(actual_norm, expected_norm):
                # Check soft time limit if provided
                # Time soft limit
                if args.time_limit is not None and duration > args.time_limit:
                    limit_ms = format_ms(args.time_limit)
                    results.append(
                        CaseResult(task, case_name, CaseStatus.SLOW, duration_ms, f"exceeded {limit_ms} ms", peak_kb)
                    )
                    suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                    print(f"{case_name}: SLOW ({duration_ms} ms > {limit_ms} ms{suffix})")
                else:
                    # Memory soft limit
                    if args.mem_limit_mb is not None and peak_kb is not None:
                        limit_kb = int(args.mem_limit_mb * 1024)
                        if peak_kb > limit_kb:
                            results.append(
                                CaseResult(
                                    task,
                                    case_name,
                                    CaseStatus.MEM_EXCEEDED,
                                    duration_ms,
                                    f"exceeded {limit_kb} KB",
                                    peak_kb,
                                )
                            )
                            print(
                                f"{case_name}: MEM_EXCEEDED ({peak_kb} KB > {limit_kb} KB, {duration_ms} ms)"
                            )
                        else:
                            results.append(CaseResult(task, case_name, CaseStatus.OK, duration_ms, "", peak_kb))
                            suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                            print(f"{case_name}: OK ({duration_ms} ms{suffix})")
                    else:
                        results.append(CaseResult(task, case_name, CaseStatus.OK, duration_ms, "", peak_kb))
                        suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                        print(f"{case_name}: OK ({duration_ms} ms{suffix})")
            else:
                if args.save_actual:
                    write_text(expected_file.with_suffix(".out.actual"), actual_norm + "\n")
                msg = "output differs"
                results.append(CaseResult(task, case_name, CaseStatus.FAIL, duration_ms, msg, peak_kb))
                suffix = f", mem {peak_kb} KB" if peak_kb is not None else ""
                print(f"{case_name}: FAIL ({duration_ms} ms{suffix})")
                print_output_difference(actual_norm, expected_norm)
                print_replay(exe, input_file)

    if args.trace_dir:
        reports = {}
        for result, snapshot in zip(results, result_inputs):
            reports.setdefault(result.task, []).append({
                **snapshot,
                "status": result.status.value,
                "verdict": CASE_VERDICTS[result.status],
                "durationMs": result.duration_ms,
            })
        for task, entries in reports.items():
            destination = args.trace_dir / task / "results.json"
            destination.parent.mkdir(parents=True, exist_ok=True)
            # Independent temporary names also allow two tasks/runs to finish concurrently.
            with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", dir=destination.parent,
                                             prefix="results-", suffix=".tmp", delete=False) as temporary:
                json.dump(entries, temporary)
            temporary_path = Path(temporary.name)
            try:
                temporary_path.replace(destination)
            finally:
                temporary_path.unlink(missing_ok=True)

    # Summary
    if not results:
        print("\nNo cases were executed.")
        return 2

    passed = sum(1 for r in results if r.status == CaseStatus.OK)
    failed = sum(1 for r in results if r.status == CaseStatus.FAIL)
    errors = sum(1 for r in results if r.status == CaseStatus.ERROR)
    timeouts = sum(1 for r in results if r.status == CaseStatus.TIMEOUT)
    missing_expected = sum(1 for r in results if r.status == CaseStatus.NO_EXPECTED)
    slow = sum(1 for r in results if r.status == CaseStatus.SLOW)
    missing_exec = sum(1 for r in results if r.status == CaseStatus.EXEC_MISSING)
    mem_exceeded = sum(1 for r in results if r.status == CaseStatus.MEM_EXCEEDED)

    print(
        f"\n=== Summary ===\n"
        f"Cases run: {passed + failed + errors + timeouts + missing_expected + missing_exec + slow + mem_exceeded}\n"
        f"OK: {passed}, FAIL: {failed}, ERROR: {errors}, TIMEOUT: {timeouts}, SLOW: {slow}, MEM_EXCEEDED: {mem_exceeded}, NO_EXPECTED: {missing_expected}, EXEC_MISSING: {missing_exec}"
    )

    print(f"Passed: {passed}/{len(results)}")
    results_by_task: Dict[str, List[CaseResult]] = {}
    for result in results:
        results_by_task.setdefault(result.task, []).append(result)
    for task, task_results in results_by_task.items():
        task_passed = sum(result.status == CaseStatus.OK for result in task_results)
        print(f"\n{task}: {task_passed}/{len(task_results)} passed")
        for number, result in enumerate(task_results, 1):
            print(f"  Test {number} ({result.case_name}): {CASE_VERDICTS[result.status]}")

    if args.report_slowest and results:
        # Consider only cases that actually ran (exclude exec_missing/no_expected) and sort by duration
        measured = [
            r for r in results
            if r.status in {CaseStatus.OK, CaseStatus.FAIL, CaseStatus.ERROR, CaseStatus.TIMEOUT, CaseStatus.SLOW}
        ]
        measured.sort(key=lambda r: r.duration_ms, reverse=True)
        top_n = measured[: max(0, args.report_slowest)]
        if top_n:
            print("\nTop slowest cases:")
            for r in top_n:
                print(f"- {r.task}/{r.case_name}: {r.duration_ms} ms [{r.status.value}] {r.message}")

    if args.report_memtop and results:
        mem_measured = [r for r in results if r.peak_kb is not None]
        mem_measured.sort(key=lambda r: r.peak_kb or 0, reverse=True)
        top_m = mem_measured[: max(0, args.report_memtop)]
        if top_m:
            print("\nTop memory usage cases:")
            for r in top_m:
                print(
                    f"- {r.task}/{r.case_name}: {r.peak_kb} KB [{r.status.value}] {r.message}"
                )

    if (
        failed
        or errors
        or timeouts
        or missing_exec
        or missing_expected
        or (args.fail_on_slow and slow)
        or (args.fail_on_mem and mem_exceeded)
    ):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())


