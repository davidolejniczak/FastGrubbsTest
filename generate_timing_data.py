"""
Generate random timing data files for Timing_grubbs.py benchmarks.
Produces Timing/data1k.txt, Timing/data10k.txt, Timing/data100k.txt.

IDs are unique 2-character alphanumeric strings (base-62).
Values are normally distributed with a small number of injected outliers.
"""

import random
import string
import os

ALPHABET = string.ascii_lowercase + string.ascii_uppercase + string.digits  # 62 chars

SIZES = {
    "data1k.txt":   1_000,
    "data10k.txt":  10_000,
    "data100k.txt": 100_000,
}

MEAN   = 5000
STDEV  = 1000
OUTLIER_FRACTION = 0.005   # ~0.5% outliers, 5–10 sigma away
SEED   = 42


def unique_ids(n: int) -> list[str]:
    """Return n unique 2-char IDs, extending to 3 chars if needed."""
    ids = set()
    length = 2
    chars = ALPHABET
    while len(ids) < n:
        candidate = "".join(random.choices(chars, k=length))
        if candidate not in ids:
            ids.add(candidate)
        # If 2-char space (62^2 = 3844) is exhausted, bump to 3 chars
        if len(ids) >= len(chars) ** length:
            length += 1
    return list(ids)


def generate(n: int, seed: int) -> dict:
    rng = random.Random(seed)
    values = [round(rng.gauss(MEAN, STDEV)) for _ in range(n)]

    # Inject outliers
    n_outliers = max(1, int(n * OUTLIER_FRACTION))
    for i in rng.sample(range(n), n_outliers):
        direction = rng.choice([-1, 1])
        values[i] = round(MEAN + direction * rng.uniform(5, 10) * STDEV)

    ids = unique_ids(n)
    return dict(zip(ids, values))


def write(path: str, data: dict) -> None:
    with open(path, "w") as f:
        f.write("{\n")
        items = list(data.items())
        for i, (key, val) in enumerate(items):
            comma = "," if i < len(items) - 1 else ""
            f.write(f"'{key}': {val}{comma}\n")
        f.write("}\n")


if __name__ == "__main__":
    os.makedirs("Timing", exist_ok=True)
    for filename, n in SIZES.items():
        path = os.path.join("Timing", filename)
        data = generate(n, SEED)
        write(path, data)
        print(f"Wrote {path}  ({n:,} rows)")
