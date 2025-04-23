from grubbstest import run_Grubbs
import time
import ast

def main():
    total1k = 0
    total10k = 0
    total100k = 0
    for i in range(30): 
        with open("Timing/data1k.txt","r") as f:
            file = f.read()
        time.sleep(1)
        data = ast.literal_eval(file)
        start = time.time()
        run_Grubbs(data, alpha=0.05, use_id_field=True, use_list_output=True)
        end = time.time()
        total = end - start
        total1k += total
        print(f"Elasped time for 1k: {total} seconds")
        
        with open("Timing/data10k.txt","r") as f:
            file = f.read()
        data2 = ast.literal_eval(file)
        time.sleep(1)
        start2 = time.time()
        run_Grubbs(data2, alpha=0.05, use_id_field=True, use_list_output=True)
        end2 = time.time()
        total2 = end2 - start2
        total10k += total2
        print(f"Elasped time for 10k: {total2} seconds")
        
        with open("Timing/data100k.txt","r") as f:
            file = f.read()
        time.sleep(1)
        data3 = ast.literal_eval(file)
        start3 = time.time()
        run_Grubbs(data3, alpha=0.05, use_id_field=True, use_list_output=True)
        end3 = time.time()
        total3 = end3 - start3
        total100k += total3
        print(f"Elasped time for 100k: {total3} seconds")
        
    print(f"Elasped average time for 1k: {total1k/30} seconds")
    print(f"Elasped average time for 10k: {total10k/30} seconds")
    print(f"Elasped average time for 100k: {total100k/30} seconds")

if __name__ == "__main__":
    main()