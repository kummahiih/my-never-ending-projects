from grapheqgen import *

import numpy as np
import matplotlib
import matplotlib.pyplot as plt

import math


def getComplexities(eq):
    return (
        len(eq.operations),
        len(str(eq))
        )

def generateData(L, n=100):
    x = []
    y = []
    for i in range(n):
        try:
            k = L.generate()[0]
            l = k.evaluate()
        except RuntimeError:
            continue
        a,b = getComplexities(l)
        x.append(a)
        y.append(math.log(b,2))
    return x,y



if __name__ == "__main__":
    #     12345
    #kk = "qwert"
    #kk = list(range(0,11))
    kk = list(range(0,24))
    
    L, _ = get_grapheq_grammar(kk)

    #k = L.generate()[0]
    #print(k)
    
    #l = k.evaluate()
    #print(l)
    #print(getComplexities(l))
    #l.evaluate()

    x,y = generateData(L, 10000)

    matplotlib.rcParams['axes.unicode_minus'] = False
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(x, y, '.')
    ax.set_title('Graafilausekkeiden pituus vs generoidun graafin kaarien lukumäärä')
    ax.set_xlabel('Verkon kaarien lukumäärä')
    ax.set_ylabel('Graafilausekkeen merkkien kaksikantainen logaritmi')
    plt.show()
