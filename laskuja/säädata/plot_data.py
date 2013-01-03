#for parsing
from parse_data import get_data

#plotting
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
    
def no_locator_set(ax):
    return

def makeplot(data, formattersetter, step = 2):
    timestamps = [ r[0] for r in data]
    temperature = [ r[1] for r in data]

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

    
if __name__ == "__main__":
    makeplot(get_data(), daily)
    makeplot(get_data(), monthly)

            
