"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


import matplotlib.pyplot as plt
#from matplotlib.ticker import Formatter
from matplotlib import dates

def daily(ax):
    hfmt = dates.DateFormatter('%Y.%m.%d')    
    ax.xaxis.set_major_locator(dates.DayLocator())
    ax.xaxis.set_major_formatter(hfmt)

def monthly(ax):
    hfmt = dates.DateFormatter('%Y.%m') 
    ax.xaxis.set_major_locator(dates.MonthLocator())
    ax.xaxis.set_major_formatter(hfmt)
    
def yearly(ax):
    hfmt = dates.DateFormatter('%Y') 
    ax.xaxis.set_major_locator(dates.YearLocator())
    ax.xaxis.set_major_formatter(hfmt)
    
def no_locator_set(ax):
    return

def makeplot(data, formattersetter, step = 2):
    timestamps = [ r[0] for r in data]
    temperature = [ r[1] for r in data]
    makeplot_from_lists(timestamps, temperature, formattersetter, step)

def makeplot_from_lists(timestamps, temperature, formattersetter, step):

    fig = plt.figure()
    ax = fig.add_subplot(111)

    ax.plot_date(timestamps,temperature, '.-')
    
    formattersetter(ax)

    plt.xticks(rotation='vertical')
    miny = min(temperature)
    maxy = max(temperature)
    
    plt.yticks(list(range(int(miny - step), int(maxy + step), int(step))))
    plt.subplots_adjust(bottom=.3)
    plt.grid(True)
    plt.show()