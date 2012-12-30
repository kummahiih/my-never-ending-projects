#for parsing
from parse_data import get_data

#plotting
import matplotlib.pyplot as plt
#from matplotlib.ticker import Formatter
from matplotlib import dates




def makeplot():
    data = get_data()
    timestamps = [ r[0] for r in data]
    temperature = [ r[1] for r in data]

    fig = plt.figure()
    ax = fig.add_subplot(111)

    hfmt = dates.DateFormatter('%Y.%m.%d')
    ax.plot_date(timestamps,temperature, '.-')
    ax.xaxis.set_major_locator(dates.DayLocator())
    ax.xaxis.set_major_formatter(hfmt)

    plt.xticks(rotation='vertical')
    plt.yticks([ -40.+i for i in range(0,80,2)])
    plt.subplots_adjust(bottom=.3)
    plt.grid(True)
    plt.show()

    

makeplot()

            
