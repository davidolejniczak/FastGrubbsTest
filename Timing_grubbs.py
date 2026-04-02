"""
Benchmark: C++ fastgrubbstest  vs  pure Python (scipy/numpy) Grubbs test
"""

import sys
import ast
import time
import numpy as np
from scipy import stats

sys.path.insert(0, "build")
import fastgrubbstest as cpp


def python_grubbs(data: dict, alpha: float = 0.05) -> dict:
    keys = list(data.keys())
    values = np.array(list(data.values()), dtype=np.float64)
    working = values.copy()

    while len(working) > 1:
        n = len(working)
        mean = np.mean(working)
        std = np.std(working)
        if std == 0:
            break
        t = stats.t.ppf(1.0 - alpha / (2 * n), n - 2)
        G = ((n - 1) / np.sqrt(n)) * (t ** 2 / np.sqrt(n - 2 + t ** 2))
        residuals = np.abs(working - mean)
        if residuals.max() > G:
            working = np.delete(working, residuals.argmax())
        else:
            break

    mean_c = np.mean(working)
    std_c = np.std(working)
    if std_c == 0:
        raise ValueError("Standard deviation of cleaned data is zero")
    zscores = (values - mean_c) / std_c
    return {k: [float(v), float(z)] for k, v, z in zip(keys, values, zscores)}


def python_no_outlier(data: dict) -> dict:
    keys = list(data.keys())
    values = np.array(list(data.values()), dtype=np.float64)
    mean = np.mean(values)
    std = np.std(values)
    if std == 0:
        raise ValueError("Standard deviation is zero")
    zscores = (values - mean) / std
    return {k: [float(v), float(z)] for k, v, z in zip(keys, values, zscores)}


def load(path: str) -> dict:
    with open(path) as f:
        return ast.literal_eval(f.read())


def bench(fn, *args, runs: int = 10) -> float:
    """Return median elapsed seconds over `runs` calls."""
    times = []
    for _ in range(runs):
        t0 = time.perf_counter()
        fn(*args)
        times.append(time.perf_counter() - t0)
    times.sort()
    return times[len(times) // 2]


def report(label: str, py_s: float, cpp_s: float) -> None:
    speedup = py_s / cpp_s if cpp_s > 0 else float("inf")
    print(f"  {'Python':>8}  {py_s * 1000:8.3f} ms")
    print(f"  {'C++':>8}  {cpp_s * 1000:8.3f} ms")
    print(f"  {'Speedup':>8}  {speedup:.1f}x")

RUNS = 1
ALPHA = 0.05
DATASETS = [
    ("1k",   "Timing/data1k.txt"),
    ("10k",  "Timing/data10k.txt"),
    ("100k", "Timing/data100k.txt"),
]

if __name__ == "__main__":
    print(f"Benchmark  (median of {RUNS} runs per dataset)\n")

    for name, path in DATASETS:
        data = load(path)
        n = len(data)
        print(f"── {name} ({n:,} points) ─────────────────────")

        print(" run_Grubbs:")
        py_t  = bench(python_grubbs,       data, ALPHA, runs=RUNS)
        cpp_t = bench(cpp.run_Grubbs,      data, ALPHA, runs=RUNS)
        report(name, py_t, cpp_t)

        print(" run_NoOutlier:")
        py_t2  = bench(python_no_outlier,   data, runs=RUNS)
        cpp_t2 = bench(cpp.run_NoOutlier,   data, runs=RUNS)
        report(name, py_t2, cpp_t2)

        print()
