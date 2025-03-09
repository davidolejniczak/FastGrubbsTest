#!/usr/bin/env python3
from IterativeJackKnife import main
import numpy as np

def test_jackknife():
    # Create test data
    data = [1.0, 2.0, 3.0, 10.0, 2.5, 2.7, 2.8, 3.1]
    
    # Create JackKnife configuration
    jk = main.JackKnifeConfig()
    
    # Test dictionary output
    print("Testing dictionary output:")
    jk.setUseListOutput(False)
    result_dict = jk.runJackknife(data)
    print("Dictionary result:")
    for value, zscore in result_dict.items():
        print(f"Value: {value:.2f}, Z-score: {zscore:.2f}")
    
    print("\n" + "="*50 + "\n")
    
    # Test list output
    print("Testing list output:")
    jk.setUseListOutput(True)
    result_list = jk.runJackknife(data)
    print("List result:")
    for value, zscore in result_list:
        print(f"Value: {value:.2f}, Z-score: {zscore:.2f}")
    
    print("\n" + "="*50 + "\n")
    
    print("Testing integer dictionary input:")
    int_dict = {1: 0, 2: 0, 3: 0, 10: 0}  # Integer keys
    jk.setUseListOutput(False)
    int_dict_result = jk.runJackknife(int_dict)
    print("Integer dictionary result:")
    for value, zscore in int_dict_result.items():
        print(f"Value: {value}, Z-score: {zscore:.2f}")
    
    print("\n" + "="*50 + "\n")
    
    print("Testing float dictionary input:")
    float_dict = {1.5: 0, 2.7: 0, 3.14: 0, 10.5: 0}  # Float keys
    result_dict = jk.runJackknife(float_dict)
    print("Float dictionary result:")
    for value, zscore in result_dict.items():
        print(f"Value: {value}, Z-score: {zscore:.2f}")
    
    print("\n" + "="*50 + "\n")
    
    print("Testing mixed numeric dictionary input:")
    mixed_dict = {1: 0, 2.5: 0, 3: 0, 10.7: 0}  # Mixed int/float keys
    result_dict = jk.runJackknife(mixed_dict)
    print("Mixed numeric dictionary result:")
    for value, zscore in result_dict.items():
        print(f"Value: {value}, Z-score: {zscore:.2f}")

if __name__ == "__main__":
    test_jackknife()