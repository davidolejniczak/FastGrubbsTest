#!/usr/bin/env python3
from IterativeJackKnife import run_JackKnife

def list_examples():
    data = [1.2, 2.3, 1.8, 1.6, 10.5, 1.9, 2.1]
    
    results = run_JackKnife(data,dof1=3.0,percentile=0.95,use_list_output=True,use_id_field=False)
    print(results)
    
if __name__ == "__main__":
        list_examples()
