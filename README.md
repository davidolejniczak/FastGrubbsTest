# GrubbsTest 

## Installation

### Prerequisites

Before installing please install the following:
- Python 3.7 or higher

### Installing GrubbsTest

You can install the package directly from PyPI:

```bash
pip install GrubbsTest
```

## Usage
### Functions


- `run_NoOutlier(data, use_list_output=True, use_id_field=False)`
  - Calculates standard Z-scores without removing outliers

### Inputs

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

