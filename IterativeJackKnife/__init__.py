from .main import JackKnifeConfig, NoOutlierConfig

__version__ = "0.0.1"

class JackKnife:
    def __init__(self, percentile=0.95, dof1=3.0, use_list_output=True, use_id_field=False):
        self._config = JackKnifeConfig()
        self.percentile = percentile
        self.dof1 = dof1
        self.use_list_output = use_list_output
        self.use_id_field = use_id_field

    @property
    def percentile(self):
        return self._config.getPercentile()

    @percentile.setter
    def percentile(self, value):
        self._config.setPercentile(value)

    @property
    def dof1(self):
        return self._config.getDof1()

    @dof1.setter
    def dof1(self, value):
        self._config.setDof1(value)

    @property
    def use_list_output(self):
        return self._config.getUseListOutput()

    @use_list_output.setter
    def use_list_output(self, value):
        self._config.setUseListOutput(value)

    @property
    def use_id_field(self):
        return self._config.getUseIdField()

    @use_id_field.setter
    def use_id_field(self, value):
        self._config.setUseIdField(value)

    def run(self, data):
        return self._config.runJackknife(data)


class NoOutlier:
    def __init__(self, use_list_output=True, use_id_field=False):
        self._config = NoOutlierConfig()
        self.use_list_output = use_list_output
        self.use_id_field = use_id_field

    @property
    def use_list_output(self):
        return self._config.getUseListOutput()

    @use_list_output.setter
    def use_list_output(self, value):
        self._config.setUseListOutput(value)

    @property
    def use_id_field(self):
        return self._config.getUseIdField()

    @use_id_field.setter
    def use_id_field(self, value):
        self._config.setUseIdField(value)

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
    'JackKnifeConfig',
    'NoOutlierConfig',
    'run_JackKnife',
    'run_NoOutlier',
]