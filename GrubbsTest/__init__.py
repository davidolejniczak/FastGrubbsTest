from .main import JackKnifeConfig, NoOutlierConfig

__version__ = "0.0.1"

class JackKnife:
    def __init__(self, percentile=0.95, dof1=3.0, useList=True, useID=False):
        self._config = JackKnifeConfig()
        self.percentile = percentile
        self.dof1 = dof1
        self.useList = useList
        self.useID = useID

    @property
    def percentile(self):
        return self._config.getPercentile()

    @percentile.setter
    def percentile(self, value):
        self._config.Percentile = value

    @property
    def dof1(self):
        return self._config.Dof1

    @dof1.setter
    def dof1(self, value):
        self._config.Dof1 = value

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
        return self._config.runJackknife(data)


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
    return JackKnife(percentile, dof1, use_list_output, use_id_field).run(data)

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
    return NoOutlier(use_list_output, use_id_field).run(data)

__all__ = [
    'JackKnife',
    'NoOutlier',
    'run_JackKnife',
    'run_NoOutlier',
]