# GrubbsTest 

High speed Grubbs Test used to calculate the the z-score data points in a data set. I created this to learn about c++ and python integration and have an fast alternate to the major python libaries. You can view the package on pypi [here](https://pypi.org/project/grubbstest/). 

### Implentation Specifics   
- The Grubbs test statistic determines what is an outlier. The final mean and standard deviation is calculated without including the outliers in the dataset. The data set is assumed to be normal. 

- This is a Linux only package.  

## Installing GrubbsTest

A valid c++ complier and cmake is needed to be installed to run the package.
```bash 
sudo apt install cmake g++
```

You can install the package directly from PyPI:
```bash
pip install grubbstest
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

