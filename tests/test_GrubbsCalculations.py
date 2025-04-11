import unittest
from grubbstest import run_Grubbs

def find_max_abs_index(values):
    return max(range(len(values)), key=lambda i: abs(values[i]))

class TestGrubbsAlpha(unittest.TestCase):
    def setUp(self):
        self.outlier_data = [10, 11, 12, 13, 14, 50]
        self.normal_data = [10, 11, 12, 13, 14, 15]

    def test_grubbs_alpha_005(self):
        result = run_Grubbs(self.outlier_data, alpha=0.05)
        self.assertEqual(len(result), len(self.outlier_data))
        z_scores = [item[1] for item in result]
        max_abs_zscore_idx = find_max_abs_index(z_scores)
        self.assertEqual(self.outlier_data[max_abs_zscore_idx], 50)
        self.assertGreater(abs(z_scores[max_abs_zscore_idx]), 2.0)

    def test_grubbs_alpha_001(self):
        result = run_Grubbs(self.outlier_data, alpha=0.01)
        self.assertEqual(len(result), len(self.outlier_data))
        z_scores = [item[1] for item in result]
        max_abs_zscore_idx = find_max_abs_index(z_scores)
        self.assertEqual(self.outlier_data[max_abs_zscore_idx], 50)
        self.assertGreater(abs(z_scores[max_abs_zscore_idx]), 2.5)

    def test_normal_data_alpha_comparison(self):
        result_005 = run_Grubbs(self.normal_data, alpha=0.05)
        result_001 = run_Grubbs(self.normal_data, alpha=0.01)
        z_scores_005 = [item[1] for item in result_005]
        z_scores_001 = [item[1] for item in result_001]
        for z1, z2 in zip(z_scores_005, z_scores_001):
            self.assertAlmostEqual(z1, z2, places=5)

if __name__ == '__main__':
    unittest.main()