# IterativeJackKnife

A Python package implementing the Iterative jackknife algorithm for Z-score calculation. 

## IterativeJackKnife Explanation 

Calculates the the z-score of each data point in a data set using a mean and sd of the refined data set that doesn't have any outliers. 

1. Compute mean and sd of data set
2. Calculate the absolute residual for each data point
3. Calculate the C constant, which is based on fishers F distribution and number of observations left in data set that are not outliers. (Equation found in function `calcC`)
4. Remove data point with highest absolute residual if greater then C
5. Repeat steps 1-5 until no more outliers remaining in data set 
6. Calculate zscores for each data point in data set using the last mean and sd, which is from the reduced data set that does not have any outliers. 

## Installation

### Prerequisites

Before installing please install the following:
- Python 3.7 or higher
- GSL (GNU Scientific Library)

### Installing IterativeJackKnife

You can install the package directly from PyPI:

```bash
pip install IterativeJackKnife
```

## Usage
### Functions

- `run_JackKnife(data, percentile=0.95, dof1=3.0, use_list_output=True, use_id_field=False)`
  - Runs the iterative jackknife algorithm

- `run_NoOutlier(data, use_list_output=True, use_id_field=False)`
  - Calculates standard Z-scores without removing outliers

### Inputs

- `JackKnife`
  - `data`: data in list or dict format 
  - `percentile`: Confidence level for outlier detection (0-1)
  - `dof1`: Degrees of freedom 
  - `use_list_output`: Return results as list (True) or dict (False)
  - `use_id_field`: Input data has ID fields (True) or not (False)

- `NoOutlier`
  - `data`: data in list or dict format 
  - `use_list_output`: Return results as list (True) or dict (False)
  - `use_id_field`: Input data has ID fields (True) or not (False)

### Output Format
- List 
  - With ID (True): [[ID, value, z-score],[ID, value, z-score]] 
  - No ID (False): [[value, z-score],[value, z-score]]
- Dict 
  - With ID (True): {ID: }

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

David Olejniczak (david.olejniczak@icloud.com)

