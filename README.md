# GrubbsTest 

Calculates the the z-score of each data point without including outliers in the mean and standard deviation calculation.

Providing z-scores for all data points including outliers while having the z-score based only data points that are not outliers

The Grubbs test statistic determines what is an outlier
The data set is assumed to be normal

## Installation

### Prerequisites

Before installing please install the following:
- Python 3.7 or higher

### Installing GrubbsTest

You can install the package directly from PyPI:

```bash
pip install FastGrubbsTest
```

## Usage
### Functions
- `run_Grubbs(data, alpha, use_list_output=True, use_id_field=False)`
  - Calcualtes z-score with removing outliers from mean and standard deviation calculation 
  - Still outputs outliers z-scores however they are not included when calculating the mean or standard deviation values that are used to calcuate z-scores
- `run_NoOutlier(data, use_list_output=True, use_id_field=False)`
  - Calculates z-scores without removing outliers in mean and standard deviation 

### Inputs

- `Grubbs` 
  - `data`: data in list or dict format 
  - `alpha`: alpha value using in student's t distribution calculation 
  - `use_list_output`: Return results as list (True) or dict (False)
  - `use_id_field`: Input data has ID fields (True) or not (False)

- `NoOutlier`
  - `data`: data in list or dict format 
  - `use_list_output`: Return results as list (True) or dict (False)
  - `use_id_field`: Input data has ID fields (True) or not (False)

### Output Format
- **List**  
  - With ID (True): A list of lists where each inner list contains the ID, value, and its z-score 
    ```python
    [[ab, 85, 1.23], [cd, 4, -0.56], ...]
    ```
  - No ID (False): A list of lists where each inner list contains the value and its z-score 
    ```python
    [[85, 1.23], [4, -0.56], ...]
    ```

- **Dict**  
  - With ID (True): A dictionary where each key is the ID and the value is a list and its z-score 
    ```python
    {ab: [85, 1.23], cd: [4, -0.56], ...}
    ```
  - No ID (False): A dictionary where each key is the value input and the value is its z-score.
    - **Warning**: If you have equal data points and use this output you will lose data
    ```python
    {85: 1.23, 4: -0.56, ...}
    ```

## License

This project is licensed under the MIT License

## Author

David Olejniczak

