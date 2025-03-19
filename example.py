#!/usr/bin/env python3
"""
IterativeJackKnife - Example Usage

This script demonstrates the various ways to use the IterativeJackKnife package
for outlier detection and Z-score calculation.
"""
import sys
import os
from pprint import pprint

# Make sure the package can be imported
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

# Import the package functions and classes
try:
    from IterativeJackKnife import run_JackKnife, run_NoOutlier, JackKnife, NoOutlier
    print("Successfully imported IterativeJackKnife package\n")
except ImportError as e:
    print(f"Error importing IterativeJackKnife package: {e}")
    sys.exit(1)

def separator(title):
    """Print a separator with a title"""
    print("\n" + "=" * 40)
    print(f"  {title}")
    print("=" * 40 + "\n")


# ==========================================================================
#  1. Simple List Example
# ==========================================================================
separator("1. Simple List Example")

# Simple data with an obvious outlier
data = [1.2, 2.3, 1.8, 1.6, 10.5, 1.9, 2.1]
print(f"Input data: {data}")

# Using the Iterative JackKnife algorithm
results = run_JackKnife(data)
print("\nJackKnife results:")
for item in results:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")

# Using the NoOutlier algorithm (standard Z-scores)
results = run_NoOutlier(data)
print("\nNoOutlier results:")
for item in results:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  2. List with Multiple Outliers
# ==========================================================================
separator("2. List with Multiple Outliers")

# Data with multiple outliers
data_multi_outliers = [1.2, 2.3, 1.8, 1.6, 10.5, 1.9, 2.1, 9.8, -5.0]
print(f"Input data: {data_multi_outliers}")

# Using the Iterative JackKnife algorithm
results = run_JackKnife(data_multi_outliers)
print("\nJackKnife results:")
for item in results:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  3. Dictionary Input
# ==========================================================================
separator("3. Dictionary Input")

# Data as a dictionary (key-value pairs)
data_dict = {
    "A": 1.2,
    "B": 2.3,
    "C": 1.8,
    "D": 1.6,
    "E": 10.5,
    "F": 1.9, 
    "G": 2.1
}
print("Input data dictionary:")
pprint(data_dict)

# Using dictionary as input, returning dictionary as output
results = run_JackKnife(data_dict, use_list_output=False)
print("\nJackKnife results as dictionary:")
for key, (value, zscore) in results.items():
    print(f"  Key: {key}, Value: {value:.2f}, Z-score: {zscore:.2f}, "
          f"Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  4. List of Lists (with ID)
# ==========================================================================
separator("4. List of Lists (with ID)")

# Each item is [id, value]
data_with_id = [
    ["Sample1", 1.2],
    ["Sample2", 2.3],
    ["Sample3", 1.8],
    ["Sample4", 1.6],
    ["Sample5", 10.5],
    ["Sample6", 1.9],
    ["Sample7", 2.1]
]
print("Input data with IDs:")
pprint(data_with_id)

# Use the use_id_field flag
results = run_JackKnife(data_with_id, use_id_field=True)
print("\nJackKnife results with IDs:")
for item in results:
    id_val, value, zscore = item
    print(f"  ID: {id_val}, Value: {value:.2f}, Z-score: {zscore:.2f}, "
          f"Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  5. Custom Parameters
# ==========================================================================
separator("5. Custom Parameters")

# Using custom percentile and dof1 values
data = [1.2, 2.3, 1.8, 1.6, 10.5, 1.9, 2.1, 2.5, 2.7]
print(f"Input data: {data}")

# Standard parameters (percentile=0.95, dof1=3.0)
results1 = run_JackKnife(data)

# More strict parameters (percentile=0.99, dof1=2.0)
results2 = run_JackKnife(data, percentile=0.99, dof1=2.0)

# Less strict parameters (percentile=0.90, dof1=4.0)
results3 = run_JackKnife(data, percentile=0.90, dof1=4.0)

print("\nStandard parameters (percentile=0.95, dof1=3.0):")
for item in results1:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")

print("\nStrict parameters (percentile=0.99, dof1=2.0):")
for item in results2:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")

print("\nLess strict parameters (percentile=0.90, dof1=4.0):")
for item in results3:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  6. Using Classes Directly
# ==========================================================================
separator("6. Using Classes Directly")

data = [1.2, 2.3, 1.8, 1.6, 10.5, 1.9, 2.1]
print(f"Input data: {data}")

# Create a JackKnife configuration
jk = JackKnife()
jk.percentile = 0.99  # Set higher percentile
jk.dof1 = 2.5         # Set custom dof1
jk.use_list = True    # Use list output
jk.use_id = False     # Don't use IDs

# Run the algorithm
results_jk = jk.run(data)
print("\nDirect JackKnife class usage:")
for item in results_jk:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")

# Create a NoOutlier configuration
no = NoOutlier()
no.use_list = True   # Use list output
no.use_id = False    # Don't use IDs

# Run the algorithm
results_no = no.run(data)
print("\nDirect NoOutlier class usage:")
for item in results_no:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")


# ==========================================================================
#  7. Edge Cases
# ==========================================================================
separator("7. Edge Cases")

# Empty list
print("Testing empty list:")
results = run_JackKnife([])
print(f"  Empty list result: {results}")

# Single value
print("\nTesting single value:")
results = run_JackKnife([5.0])
print(f"  Single value result: {results}")

# All identical values
print("\nTesting identical values:")
results = run_JackKnife([2.0, 2.0, 2.0, 2.0])
print(f"  Identical values result: {results}")

# Testing with very large outlier
print("\nTesting with very large outlier:")
results = run_JackKnife([1.2, 2.3, 1.8, 1.6, 1000.0, 1.9, 2.1])
for item in results:
    value, zscore = item
    print(f"  Value: {value:.2f}, Z-score: {zscore:.2f}, Outlier: {'Yes' if abs(zscore) > 2.0 else 'No'}")

print("\nAll tests completed successfully!")
