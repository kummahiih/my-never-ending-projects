from grapheqgen import get_grapheq_grammar
#import yaastar
import heapq
#import bisect

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math

#import grapheq
import networkx as nx
from copy import copy

import random

class ParetoItem:
    def __init__(self, descArray, item):
        self.item = item
        self.descArray = tuple(descArray)

    def as_tuple(self):
        return tuple(list(self.descArray)+ [self.item])

    def __hash__(self):
         return self.descArray.__hash__()

    def __lt__(self, other):
        ok = True
        for i,j in enumerate(self.descArray):
            if not j.__lt__(self.descArray[i]):
                ok = False
        #print(self, other, ok)
        return ok
        #return self.descArray.__lt__(other.descArray)

    def __str__(self):
        return self.as_tuple().__str__()

    def __eq__(self, other):
        ok = True
        for i,j in enumerate(self.descArray):
            if not j.__eq__(self.descArray[i]):
                ok = False
        return ok
    #    return self.descArray.__eq__(other.descArray)

#pienimmät pois heapisat eka
def getComplexities(eq):
    return (
        len(eq.operations),
        -math.log(len(str(eq)), 2)
        )

def removeLesser(alist, item, maxamount):
    removed = []
    for i, j in enumerate(alist):
        if j < item:
            removed.append(i)

    #map(alist.pop, removed)
    
    
    if len(alist) >= maxamount and removed:
        alist.pop(random.choice(removed))

    if len(alist) < maxamount:
        alist.append(item)

def mutate(items, L):
    if not items:
        return generate(items, L)
    orginal = random.choice(items).item[1]
    return orginal.mutate(mutater, 0.15)[0]

def generate(items, L):
     return L.generate()[0]

def generateData(L, mutater, n = 1000000, nItems = 50):
    items = []
    done = set()
    k = None
    for i in range(n):
        for f in [generate,mutate,mutate,mutate]:
            try:
                
                #print('-'*60)
                k = f(items, L)
                #print('comparison:')
                if k in done:
                    continue
                
                done.add(k)

                if len(done) > 500:
                    done.pop()
                #print('evaluate:')
                l = k.evaluate()
                
                if len(str(l)) > 200:
                    continue
                #print('generated',k)
                a,b = getComplexities(l)
                if(a == 0):
                    continue
                item = ParetoItem((a,b),(l,k) )
                #heapq.heappush(items, item)
                removeLesser(items, item, nItems)
                
            except RuntimeError as e:
                #raise e
                print("Exception",e )
                continue

    return items

def plotItems(items):
    x = [  i.descArray[0] for i in items]
    y = [ - i.descArray[1] for i in items]


    matplotlib.rcParams['axes.unicode_minus'] = False
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(x, y, '.')
    ax.set_title('Graafilausekkeiden pituus vs generoidun graafin kaarien lukumäärä')
    ax.set_xlabel('Verkon kaarien lukumäärä')
    ax.set_ylabel('Graafilausekkeen merkkien kaksikantainen logaritmi')
    plt.show()

if __name__ == "__main__":
    #     12345
    G = nx.MultiDiGraph()
    def connect(a, b):
        print (a, b)
        G.add_edges_from([(a, b)])

    kk = range(24)#"qwertyasfg"
    L, mutater = get_grapheq_grammar(kk, operator=connect)

    
    items = generateData(L, mutater, 1000, 20)

    plotItems(items)
    input("verkko?")
    to_be_ordered = []
    for i in items:
        i, ii, iii = i.as_tuple()
        to_be_ordered.append((i,ii,iii[0]))

    to_be_ordered.sort()
    #map(print, to_be_ordered)
    for i,j in enumerate(to_be_ordered):
        print(i,j)

    #print( list(zip(x,y)))

    to_be_ordered[-1][-1].evaluate()
    nx.draw_circular(G)
    plt.show()

    
        

    
