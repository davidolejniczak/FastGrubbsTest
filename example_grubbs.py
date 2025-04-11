from grubbstest import run_Grubbs

def main():
    data = [['a', 85],['b', 4],['c', 5],['d', 3],['e', 2]]
    result = run_Grubbs(data, alpha=0.05, use_id_field=True, use_list_output=True)
    print(result)
    
if __name__ == "__main__":
    main()