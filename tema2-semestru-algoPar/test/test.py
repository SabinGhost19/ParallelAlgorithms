import re
import sys
from collections import defaultdict

# ============================================================
# Read output from results.txt
# change this path if the file is located elsewhere
# ============================================================
try:
    with open("results.txt", "r", encoding="utf-8") as f:
        OUTPUT = f.read()
except FileNotFoundError:
    print("[ERROR] File 'results.txt' not found. Make sure it exists in the same folder.")
    sys.exit(1)

# ============================================================
# Regex patterns
# ============================================================
ALG_PATTERN = re.compile(r"Algorithm:\s+(.*)")
MAX_PATTERN = re.compile(r"MaxRequests\s*=\s*(\d+)")

TEST_PATTERN = re.compile(
    r"(RaceTest|AccuracyTest|DistributedTest)"
    r"(?:\:\s+Approved\s+<=\s+(\d+))?"
    r"(?:\s*\|\s*Requests=(\d+))?.*Time=(\d+)ms"
)

EXPECTED_ALGORITHMS = {"Fixed Window", "Sliding Window", "Token Bucket"}
EXPECTED_TESTS = {"RaceTest", "AccuracyTest", "DistributedTest"}
EXPECTED_LIMITS = {100, 500, 1000}

# ============================================================
# Validation & detailed reporting
# ============================================================
def validate(output: str):
    errors = []
    warnings = []
    details = defaultdict(lambda: defaultdict(dict))  # [alg][limit][test] = info

    current_alg = None
    current_max = None

    # -------------------------
    # Parse output
    # -------------------------
    for line in output.splitlines():
        line = line.strip()
        alg_match = ALG_PATTERN.search(line)
        if alg_match:
            current_alg = alg_match.group(1)
            continue

        max_match = MAX_PATTERN.search(line)
        if max_match:
            current_max = int(max_match.group(1))
            continue

        test_match = TEST_PATTERN.search(line)
        if test_match and current_alg and current_max is not None:
            test, approved, requests, time = test_match.groups()
            approved = int(approved) if approved else None
            requests = int(requests) if requests else None
            time = int(time)

            details[current_alg][current_max][test] = {
                "approved": approved,
                "requests": requests,
                "time": time,
                "status": "Unknown"
            }

    # -------------------------
    # Check completeness
    # -------------------------
    algs_found = set(details.keys())
    if algs_found != EXPECTED_ALGORITHMS:
        errors.append(f"[FAIL] Algorithms mismatch. Found={algs_found}, Expected={EXPECTED_ALGORITHMS}")

    for alg in EXPECTED_ALGORITHMS:
        if alg not in details:
            continue
        limits_found = set(details[alg].keys())
        if limits_found != EXPECTED_LIMITS:
            errors.append(f"[FAIL] {alg}: MaxRequests mismatch. Found={limits_found}, Expected={EXPECTED_LIMITS}")
        for limit in EXPECTED_LIMITS:
            if limit not in details[alg]:
                continue
            tests_found = set(details[alg][limit].keys())
            if tests_found != EXPECTED_TESTS:
                errors.append(f"[FAIL] {alg} | MaxRequests={limit}: Missing tests. Found={tests_found}, Expected={EXPECTED_TESTS}")

    # -------------------------
    # Detailed per-test checks
    # -------------------------
    for alg, limits in details.items():
        for limit, tests in limits.items():
            for test, info in tests.items():
                approved = info["approved"]
                requests = info["requests"]

                # Default pass
                info["status"] = "Passed"

                if test in {"RaceTest", "AccuracyTest"}:
                    if approved is None or approved > limit:
                        info["status"] = f"Failed: Approved {approved} > MaxRequests {limit}"
                        errors.append(f"{alg} | {limit} | {test}: Approved {approved} > MaxRequests {limit}")
                    if test == "AccuracyTest" and requests and approved > requests:
                        info["status"] = f"Failed: Approved {approved} > Requests {requests}"
                        errors.append(f"{alg} | {limit} | AccuracyTest: Approved {approved} > Requests {requests}")

                elif test == "DistributedTest":
                    # Consider it passed if line exists (Approved not required)
                    if approved and approved > limit:
                        info["status"] = f"Failed: Approved {approved} > MaxRequests {limit}"

    # -------------------------
    # Monotonic scalability check
    # -------------------------
    for alg in EXPECTED_ALGORITHMS:
        prev_approved = None
        for limit in sorted(EXPECTED_LIMITS):
            if limit not in details.get(alg, {}):
                continue
            approved_candidates = [info["approved"] for info in details[alg][limit].values() if info["approved"]]
            if not approved_candidates:
                continue
            current = approved_candidates[0]
            if prev_approved is not None and current < prev_approved:
                warnings.append(f"[WARN] {alg}: Approved decreased from {prev_approved} → {current} at MaxRequests={limit}")
            prev_approved = current

    # -------------------------
    # Heuristic warnings
    # -------------------------
    for limit in EXPECTED_LIMITS:
        if "Fixed Window" in details and "Sliding Window" in details:
            fw_time = details["Fixed Window"][limit]["RaceTest"]["time"]
            sw_time = details["Sliding Window"][limit]["RaceTest"]["time"]
            if sw_time < fw_time:
                warnings.append(f"[WARN] Sliding Window faster than Fixed Window at MaxRequests={limit}")

    tb_times = [details["Token Bucket"][limit]["RaceTest"]["time"] for limit in EXPECTED_LIMITS if limit in details.get("Token Bucket", {})]
    if tb_times and max(tb_times) - min(tb_times) > 10:
        warnings.append("[WARN] Token Bucket latency varies significantly across limits")

    return details, errors, warnings

# ============================================================
# Main
# ============================================================
if __name__ == "__main__":
    details, errors, warnings = validate(OUTPUT)

    print("\n===== DETAILED RATE LIMITER VALIDATION =====\n")

    # Per-algorithm, per-MaxRequests, per-test report
    for alg, limits in details.items():
        print(f"\nAlgorithm: {alg}")
        for limit, tests in sorted(limits.items()):
            print(f"  MaxRequests = {limit}")
            for test, info in tests.items():
                approved = info["approved"]
                requests = info["requests"]
                time = info["time"]
                status = info["status"]
                req_str = f"/ Requests {requests}" if requests else ""
                print(f"    {test}: Approved {approved} {req_str}, Time={time}ms -> {status}")

    # Errors and warnings summary
    if errors:
        print("\n===== ERRORS =====")
        for e in errors:
            print(e)
    else:
        print("\nNo errors detected. All tests passed basic validation.")

    if warnings:
        print("\n===== WARNINGS =====")
        for w in warnings:
            print(w)
