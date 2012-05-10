import copy
from grapheq import *
from itertools import product
import random

from functools import reduce

def emptylistIfNone(alist): return alist if alist != None else []

class RuleSring:
    def __init__(self, alist  = None):
        alist=emptylistIfNone(alist)
        self.alist = tuple(alist)

    def __repr__(self):
        return 'RS %s' % str(self.alist)

    def __lt__(self, other):
        return self.alist < other.alist
    def __le__(self, other):
        return self.alist <= other.alist
    def __eq__(self, other):
        return self.alist == other.alist
    def __ne__(self, other):
        return self.alist != other.alist
    def __gt__(self, other):
        return self.alist > other.alist
    def __ge__(self, other):
        return self.alist >= other.alist

    def __hash__(self):
        #print(self.alist)
        return self.alist.__hash__()

    def getChoises(self):
        choices = []
        for i in self.alist:
            if isTerminal(i):
                #print('terminal')
                choices.append((i,))
            else:
                #print('rule')
                choices.append(i.getChoises())
        #print('choices', len(choices), choices)
        for i in product(*choices):
            yield RuleSring(i)

    def canEvaluate(self):
        return False

    def generate(self):
        out = []
        for i in self.alist:
            if isTerminal(i):
                out.append(i)
            else:
                i = i.generate()
                out.extend(i)
                
        return out



class CfgRule(RuleSring):
    def __init__(self, alist = None,**d):        
        alist=emptylistIfNone(alist)
        self.name = d['name'] if 'name' in d else ""
        self.alist = tuple(alist)

    def __repr__(self):
        return 'R(%s) %s' % (self.name,str(self.alist))

    def __hash__(self):
        return id(self)
    
    def add(self, *alist):
        self.alist = tuple(list(self.alist) + [tuple(alist)] )

    def canEvaluate(self):
        return all([isTerminal(i) for i in self.alist])

    def getChoises(self):
        #print(self)
        return [RuleSring(i) for i in self.alist]

    def generate(self):
        #print('generate',self.name)
        #print(self.name)
        if len(self.alist) == 0:
            return []
        r = random.choice(self.alist)
 
        return RuleSring( r ).generate()

def isNotNone(i): return type(None) != type(i)

def isTerminal(i):
    return not isinstance(i, CfgRule) and (i.canEvaluate() if isinstance(i, RuleSring) else True)

def isNonTerminal(i):
    return isinstance(i, CfgRule) and not (i.canEvaluate() if isinstance(i, NestedFreezed_fnc) else True)

def tryEvaluate(i):
    return i.evaluate() if isinstance(i, Freezed_fnc) else i

def tryMutate(i, mutater, tn):
    if random.uniform(0,1) < tn:
        return mutater(i).generate()[0]
    elif isinstance(i, Freezed_fnc):
        i.mutate(mutater, tn)[0]
    return i
    

class NestedFreezed_fnc(Freezed_fnc, RuleSring):
    def __init__(self, fnc, alist = [], adict = {}, **d):
        Freezed_fnc.__init__(self, fnc, alist, adict)

    def __repr__(self):
        return 'ES "%s" "%s"' % (self.fnc.__name__, str(self.alist))

    def evaluate(self):        
        
        returned = Freezed_fnc(self.fnc)
        returned.alist = tuple([ tryEvaluate(i) for i in self.alist ])
        returned.adict = tuple([ (tryEvaluate(i), tryEvaluate(j)) for i,j in self.adict])
        #print('evaluate',self)
        return returned.evaluate()
    
    def getChoises(self):
        for i in RuleSring.getChoises(self):
            yield NestedFreezed_fnc(self.fnc, i.alist)

    def generate(self):
         i = RuleSring.generate(self)
         return [NestedFreezed_fnc(self.fnc, i)]

    def mutate(self, mutater, tn):
        self.alist = tuple([ tryMutate(i,mutater, tn) for i in self.alist ])
        return self.generate()

class ECfgRule(CfgRule, NestedFreezed_fnc):
    def __init__(self, fnc, *l, **d):
        d['name']= fnc.__name__
        CfgRule.__init__(self, *l, **d)
        NestedFreezed_fnc.__init__(self, fnc, *l, **d)

    def __repr__(self):
        return 'ER %s %s' % (self.fnc.__name__, str(self.alist))

    def __hash__(self):
        return id(self)
    

    def getChoises(self):
        return [NestedFreezed_fnc(self.fnc, i.alist) for i in CfgRule.getChoises(self)]


    def generate(self):
        #print('generate',self.name)
        i = CfgRule.generate(self)
        if isNotNone(i):
            return [NestedFreezed_fnc(self.fnc, i)]
        
        return [NestedFreezed_fnc(self.fnc,[])]


def w_choice(lst, totalSum):
    n = random.uniform(0, totalSum)
    #print(n)
    for item, weight in enumerate(lst):
        if n < weight:
                return item
        n = n - weight
    return len(lst) - 1

           

class WCfgRule(CfgRule):
    def __init__(self, alist = None, weights = None,**d):
        alist=emptylistIfNone(alist)
        weights=emptylistIfNone(weights)
        CfgRule.__init__(self, alist,**d)
        self.weights = weights
        self.weights_sum = sum(weights)

    def add(self, *l, weight = 1):
        CfgRule.add(self, *l)
        self.weights.append(weight)
        self.weights_sum += weight

    def generate(self):
        #print('generate',self.name)
        index = w_choice(self.weights, self.weights_sum)
        #if index >= len(self.alist):
        #    print('index', index, len(self.alist ),len(self.weights), self.weights_sum)
        return RuleSring( self.alist[index] ).generate()
    
       
##class CfgGraphConstant(yaastar.Map):
##    def neighbor_nodes(self, aRuleSring):
##        for i in aRuleSring.getChoises():
##            yield i
##    
##    def heuristic_estimate_of_distance(self, start, goal):
##        return 1
##
##    def dist_between(self, x, y):
##        return 1



def get_grapheq_grammar(symbols, operator=debug):

    #C = from_operator(operator)
    I, O = get_I_and_O()

    caps = from_operator(operator)

    def c(*l):
        return caps(*l)

    def remove(a,b):
        return a-b
        
    def r(*l):
        return reduce(remove, l)

    def mul(a,b):
        return a * b
    
    def m(*l):
        return reduce(mul, l)
    
    L = ECfgRule(c)
    B = WCfgRule(name="B")
    B2 = WCfgRule(name="B2")

    S = WCfgRule(name="S")
    for i in symbols:
        S.add(i,weight = 2)
    S.add(I,weight=len(symbols)/6)
    S.add(O,weight=len(symbols)/10)
        
    V = ECfgRule(r)
    V.add(B2)
    
    M = ECfgRule(m)
    M.add(B2)
    
    L.add(B)
    
    B.add(S, weight = 4.2)
    B.add(S, B,weight = 2.5)
    B.add(V, weight = 0.5)
    B.add(M, weight = 2)

    B2.add(L,B2,weight = 1)
    B2.add(L,L,weight = 3)

    Start = WCfgRule(name="Start")
    Start.add(L)
    Start.add(M)
    Start.add(V)
    
    def dummy(*a):
        #print(a, a[0])
        return a[0]

    def mutater(mutated):
        if not isinstance(mutated, NestedFreezed_fnc):
            if random.uniform(0, 1) < 0.5:
                return B
        #if isinstance(mutated, NestedFreezed_fnc):
        #    if mutated.fnc == capsulate:       
        return Start
    
    return NestedFreezed_fnc(dummy, [Start]), mutater






        

        
    
