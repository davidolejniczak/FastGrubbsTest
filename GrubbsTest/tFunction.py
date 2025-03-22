import scipy.stats as sci

class TFunction:
    def __init__(self):
        pass

    def calcTDist(self,alpha,n):
        #return sci.t.ppf((1-(alpha/(2*n))),n-2)
        #return sci.t.ppf(1-(0.05/(2*n)),n-2)
        return sci.t.ppf(alpha, n)
    
def main():
    t_func = TFunction()
    alpha = 0.9981
    n = 11
    result = t_func.calcTDist(alpha, n)
    print(f"t-distribution value for alpha={alpha} and n={n}: {result}")

if __name__ == "__main__":
    main()