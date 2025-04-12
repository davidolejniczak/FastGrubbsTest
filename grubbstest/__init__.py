from ._grubbstest_impl import NoOutlierConfig, GrubbsConfig

__version__ = "0.0.1"

class NoOutlier:
    def __init__(self, useList=True, useID=False):
        self._config = NoOutlierConfig()
        self.useList = useList
        self.useID = useID

    @property
    def useList(self):
        return self._config.getUseList()

    @useList.setter
    def useList(self, value):
        self._config.UseList = value

    @property
    def useID(self):
        return self._config.getUseId()

    @useID.setter
    def useID(self, value):
        self._config.UseId = value

    def run(self, data):
        return self._config.runNoOutlier(data)


class Grubbs:
    def __init__(self, alpha=0.05, useList=True, useID=False):
        self._config = GrubbsConfig()
        self.alpha = alpha
        self.useList = useList
        self.useID = useID
        
    @property
    def alpha(self):
        return self._config.getAlpha()
        
    @alpha.setter
    def alpha(self, value):
        self._config.Alpha = value
        
    @property
    def useList(self):
        return self._config.getUseList()
        
    @useList.setter
    def useList(self, value):
        self._config.UseList = value
        
    @property
    def useID(self):
        return self._config.getUseId()
        
    @useID.setter
    def useID(self, value):
        self._config.UseId = value
        
    def run(self, data):
        return self._config.runGrubbs(data)


def run_NoOutlier(data, use_list_output=True, use_id_field=False):
    """
    Calculate z-scores for all values.
    
    Args:
        data: Input data (list or dict)
        use_list_output: Whether to return results as list (True) or dict (False) (default: True)
        use_id_field: Input data has ID fields (True) (default: False)
        
    Returns:
        List or dict with data values and their corresponding z-scores
    """
    return NoOutlier(use_list_output, use_id_field).run(data)

def run_Grubbs(data, alpha=0.05, use_list_output=True, use_id_field=False):
    """
    Run the Grubb's test for z-scores.
    
    Args:
        data: Input data (list or dict)
        alpha: Significance level (default: 0.05)
        use_list_output: Whether to return results as list (True) or dict (False) (default: True)
        use_id_field: Input data has ID fields (True) (default: False)
        
    Returns:
        List or dict with data values and their corresponding z-scores
    """
    return Grubbs(alpha, use_list_output, use_id_field).run(data)

__all__ = [
    'NoOutlier',
    'run_NoOutlier',
    'Grubbs',
    'run_Grubbs',
]