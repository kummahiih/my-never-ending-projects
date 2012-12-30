#for parsing
import csv
import datetime
import time

#plotting
import matplotlib.pyplot as plt
#from matplotlib.ticker import Formatter
from matplotlib import dates



def get_data(filename ='dataa.txt'):
    returned =  []
    
    with open(filename, 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=';')
        for row in reader:
            date = row[0].strip()
            clock = row[1].strip()
            got_time = datetime.datetime.strptime(date +";" + clock,"%d.%m.%Y;%H:%M:%S")
            
            temperature = float(row[2])
            returned.append([got_time, temperature])
    return returned


def makeplot():
    data = get_data()
    timestamps = [ r[0] for r in data]
    temperature = [ r[1] for r in data]

    fig = plt.figure()
    ax = fig.add_subplot(111)

    ax.plot_date(timestamps,temperature, '.-')
    ax.xaxis.set_major_locator(dates.DayLocator())
    ax.xaxis.set_major_formatter(hfmt)

    plt.xticks(rotation='vertical')
    plt.yticks([ -40.+i for i in range(0,80,2)])
    plt.subplots_adjust(bottom=.3)
    plt.grid(True)
    plt.show()

    

makeplot()

            
