import unittest
from grubbstest import run_Grubbs, Grubbs

class TestGrubbsIO(unittest.TestCase):
    def setUp(self):
        self.list_data = [85, 4, 5, 3, 2]
        self.dict_data = {'a': 85,'b': 4,'c': 5,'d': 3,'e': 2}
        self.list_data_with_id = [['a', 85],['b', 4],['c', 5],['d', 3],['e', 2]]

    def test_list_input(self):
        result = run_Grubbs(self.list_data)
        self.assertEqual(len(result), len(self.list_data))
        self.assertIsInstance(result, list)
        for item in result:
            self.assertIsInstance(item, list)
            self.assertEqual(len(item), 2)
            self.assertIsInstance(item[0], (int, float))
            self.assertIsInstance(item[1], float)

    def test_dict_input(self):
        result = run_Grubbs(self.dict_data, use_id_field=True)
        self.assertIsInstance(result, list)
        self.assertEqual(len(result), len(self.dict_data))
        for item in result:
            self.assertIsInstance(item, list)
            self.assertEqual(len(item), 3)
            self.assertIn(item[0], self.dict_data.keys())
            self.assertEqual(item[1], self.dict_data[item[0]])
            self.assertIsInstance(item[2], float)

    def test_list_output(self):
        result = run_Grubbs(self.list_data, use_list_output=True)
        self.assertIsInstance(result, list)
        self.assertEqual(len(result), len(self.list_data))
        for item in result:
            self.assertIsInstance(item, list)
            self.assertEqual(len(item), 2)

    def test_dict_output(self):
        result = run_Grubbs(self.list_data, use_list_output=False)
        self.assertIsInstance(result, dict)
        self.assertEqual(len(result), len(self.list_data))
        for key in result.keys():
            self.assertIn(key, self.list_data)
        for value in result.values():
            self.assertIsInstance(value, float)

    def test_list_input_with_ids(self):
        result = run_Grubbs(self.list_data_with_id, use_id_field=True)
        self.assertIsInstance(result, list)
        self.assertEqual(len(result), len(self.list_data_with_id))
        for i, item in enumerate(result):
            self.assertIsInstance(item, list)
            self.assertEqual(len(item), 3)
            self.assertEqual(item[0], self.list_data_with_id[i][0])
            self.assertEqual(item[1], self.list_data_with_id[i][1])
            self.assertIsInstance(item[2], float)

    def test_grubbs_class_interface(self):
        grubbs = Grubbs(alpha=0.05, useList=False, useID=True)
        result = grubbs.run(self.dict_data)
        self.assertIsInstance(result, dict)
        self.assertEqual(len(result), len(self.dict_data))
        for key in result.keys():
            self.assertIn(key, self.dict_data.keys())
        for key, value in result.items():
            self.assertIsInstance(value, tuple)
            self.assertEqual(len(value), 2)
            self.assertEqual(value[0], self.dict_data[key])
            self.assertIsInstance(value[1], float)

    def test_output_format_combinations(self):
        result1 = run_Grubbs(self.list_data, use_list_output=False)
        self.assertIsInstance(result1, dict)
        result2 = run_Grubbs(self.dict_data, use_id_field=True, use_list_output=True)
        self.assertIsInstance(result2, list)
        self.assertEqual(len(result2[0]), 3)
        result3 = run_Grubbs(self.dict_data, use_id_field=True, use_list_output=False)
        self.assertIsInstance(result3, dict)
        result4 = run_Grubbs(self.list_data_with_id, use_id_field=True, use_list_output=False)
        self.assertIsInstance(result4, dict)

if __name__ == '__main__':
    unittest.main()