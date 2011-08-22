#!/usr/bin/env python2.6

from optparse import OptionParser
import scipy.stats

import matplotlib.pyplot as plt

#import matplotlib.cm as cm
#import pylab

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option(
        "-n", 
        "--number-of-bits", 
        dest="n",
        default =  100*195*8
        )
    parser.add_option(
        "-p", 
        "", 
        dest="p",
        help= "probability that more extreme values observed",
        default =  '0.05'
        )
    parser.add_option(
        "-M", 
        "", 
        dest="m",
        help= "probability that more extreme values observed",
        default =  100*195*8
        )
    (opt, _) = parser.parse_args()

    n = int(opt.n)
    p = float(opt.p)
    m = int(opt.m)

    print 'bits',n
    print 'probability that more extreme values observed',p
    print 'maximum interval',m
    
    print 'calculating the upper limit .. ',p/2.0

    border1 = [0]
    border2 = [0]
    
    for k in range(1,min(n,m)):
        b = scipy.stats.binom(n-k, 0.5)
        ppf = b.ppf(p/2.0)
        #print ppf
        point = 1/float(n-k) * ppf - 0.5
        border1.append(point)
        border2.append(-point)
        
    plt.plot(border1)
    plt.plot(border2)

    plt.title(str((1-p) * 100) +'% confidence interval for a single autocorrelation value. n='+str(n))
    plt.show()
