from .main import JackKnifeConfig, NoOutlierConfig

__version__ = "0.0.1"

def run_JackKnife(data, percentile=0.95, dof1=3.0, use_list_output=True, use_id_field=False):
    """
    Run the Iterative JackKnife algorithm.
    Args:
        data: Input data (list or dict)
        percentile: Percentile value between 0 and 1 (default: 0.95)
        dof1: Degrees of freedom parameter (default: 3.0)
        use_list_output: Return results as list (True) or dict (False) (default: True)
        use_id_field: Input data has ID fields, yes if True (default: False)
        
    Returns:
        List or dict with data values and their corresponding Z-scores
    """
    config = JackKnifeConfig()
    config.setPercentile(percentile)
    config.setDof1(dof1)
    config.setUseListOutput(use_list_output)
    config.setUseIdField(use_id_field)
    return config.runJackknife(data)

def run_NoOutlier(data, use_list_output=True, use_id_field=False):
    """
    Calculate Z-scores for all values.
    
    Args:
        data: Input data (list or dict)
        use_list_output: Whether to return results as list (True) or dict (False) (default: True)
        use_id_field: Input data has ID fields (True) (default: False)
        
    Returns:
        List or dict with data values and their corresponding Z-scores
    """
    config = NoOutlierConfig()
    config.setUseListOutput(use_list_output)
    config.setUseIdField(use_id_field)
    return config.runNoOutlier(data)

__all__ = [
    'JackKnifeConfig',
    'NoOutlierConfig',
    'run_JackKnife',
    'run_OoOutlier',
]