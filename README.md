# Stat Grubb

High speed Grubbs Test used to calculate the the z-score data points in a data set. I created this to learn about c++ and python integration and to have an fast alternate to the major python libaries. You can view the package on pypi [here](https://pypi.org/project/grubbstest/). 

## How the Algorithm Works

### `run_Grubbs` — Grubbs Test with Outlier Removal

This is an iterative algorithm that progressively removes outliers from the dataset before computing z-scores.

**Step 1 — Compute mean and standard deviation**
Using [Welford's online algorithm](https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm) on the current dataset for numerical stability.

**Step 2 — Compute the critical value G**
The Grubbs critical value is derived from Student's t-distribution:

1. Look up the t-distribution critical value `T` at significance level `α / (2n)` with `n − 2` degrees of freedom.
2. Calculate G:

```
G = ((n−1) / sqrt(n)) × (T² / sqrt(n−2 + T²))
```

**Step 3 — Find the candidate outlier**
Identify the data point with the largest absolute deviation from the mean (i.e. `|x_i − mean|`).

**Step 4 — Test and remove**
If the largest residual exceeds G, that point is classified as an outlier and removed from the working dataset. Steps 1–4 repeat with the reduced dataset until no more outliers are found or only one point remains.

**Step 5 — Compute z-scores**
Once the cleaned dataset is finalized, mean and standard deviation are recalculated one last time on the outlier-free set. Z-scores for **all original data points** (including outliers) are then computed using those cleaned statistics:

```
z = (x − mean_clean) / sd_clean
```

This means outliers still receive a z-score — it is just computed relative to the clean distribution, so their deviation is clearly visible in the output.

---

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

## Installing GrubbsTest

```bash 
sudo apt install cmake g++
```

You can install the package directly from PyPI:
```bash
pip install grubbstest
```

## Usage
### Functions
- `run_Grubbs(data, alpha)`
  - Calcualtes z-score with removing outliers from mean and standard deviation calculation 
  - Still outputs outliers z-scores however they are not included when calculating the mean or standard deviation values that are used to calcuate z-scores
- `run_NoOutlier(data)`
  - Calculates z-scores without removing outliers in mean and standard deviation 

### Inputs

- `Grubbs` 
  - `data`: data in dict format 
  - `alpha`: alpha value using in student's t distribution calculation 

- `NoOutlier`
  - `data`: data in dict format 

### Input Format 
- A dictionary where each key is the ID and the value is the number
  ```python
    {ab: 85, cd: 4, ...}
  ```


### Output Format
- A dictionary where each key is the ID and the value is a list and its z-score 
  ```python
  {ab: [85, 1.23], cd: [4, -0.56], ...}
  ```

## License

This project is licensed under the MIT License
