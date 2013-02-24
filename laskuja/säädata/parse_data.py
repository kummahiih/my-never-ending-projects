"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


import csv
import datetime
import time

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
