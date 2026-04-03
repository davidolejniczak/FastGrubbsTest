# Stat Grubb

High speed Grubbs Test used to calculate the the z-score data points in a data set. I created this to learn about c++ and python integration and to have an fast alternate to the major python libaries. You can view the package on pypi [here](https://pypi.org/project/grubbstest/). 

## How the Algorithm Works

### `run_Grubbs` — Grubbs Test with Outlier Removal

This is an iterative algorithm that progressively removes outliers from the dataset before computing z-scores.

### `run_NoOutlier` — Standard Z-Score (No Outlier Removal)

A single-pass calculation with no iterative removal:

1. Compute mean and standard deviation across the entire dataset.
2. Compute `z = (x − mean) / sd` for every data point.

Because no points are removed, outliers pull the mean and standard deviation toward themselves, which compresses z-scores across the rest of the dataset.

---

### Implementation Specifics
- The dataset is assumed to follow a normal distribution.
- The Grubbs test is designed for detecting **one outlier per iteration**; the loop repeats to handle multiple outliers.
- Mean and standard deviation are computed with Welford's algorithm to avoid floating-point cancellation errors.

### Performance

Benchmarked against an equivalent pure-Python/NumPy/SciPy implementation:

| Dataset | `run_Grubbs` speedup | `run_NoOutlier` speedup |
|---------|---------------------|------------------------|
| 1k      | **43×**             | 1.3×                   |
| 10k     | **8×**              | 1.2×                   |
| 100k    | **2.5×**            | 1.3×                   |

The large gains in `run_Grubbs` come from O(1) outlier removal (swap-to-end) and incremental mean/variance updates (reverse Welford), avoiding repeated full-array passes.

## Installing GrubbsTest

> **macOS only** (Intel and Apple Silicon)

```bash
pip install grubbstest
```

## Usage

```python
import fastgrubbstest as g
```

### Functions
- `run_Grubbs(data, alpha=0.05)`
  - Calculates z-scores with iterative outlier removal from mean and standard deviation calculation
  - Outliers still receive a z-score, computed relative to the cleaned distribution
- `run_NoOutlier(data)`
  - Calculates z-scores without removing outliers from mean and standard deviation

### Inputs

- `run_Grubbs`
  - `data`: data in dict format
  - `alpha`: significance level for Student's t-distribution (default `0.05`)

- `run_NoOutlier`
  - `data`: data in dict format

### Input Format
- A dictionary where each key is the ID and the value is the number
  ```python
  {"ab": 85, "cd": 4, ...}
  ```

### Output Format
- A dictionary where each key is the ID and the value is a list of `[original_value, z_score]`
  ```python
  {"ab": [85, 1.23], "cd": [4, -0.56], ...}
  ```

## License

This project is licensed under the MIT License
