 # -*- coding: latin-1 -*-
"""
Data is from ilmatieteen laitos ( http://ilmatieteenlaitos.fi/ )
"""


"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

import csv
import datetime
import time
import re
from dateutil import tz

headers = "Vuosi;Kuukausi;Päivä;Tunti;Minuutti;T (°C);P_sea (hPa)".split(';')


def parse_file(csvfile):
    reader = csv.DictReader(csvfile, delimiter=';', quotechar='"')
    for row in reader:
        year = int(row['Vuosi'])
        month = int(row['Kuukausi'])
        day = int(row['Päivä'])
        hour= int(row['Tunti'])
        minute = int(row['Minuutti'])
        stamp = datetime.datetime(year,month,day,hour,minute)
        returned =  {'timestamp':stamp,'T':float(row['T (°C)'].replace(',','.')), 'P':float(row['P_sea (hPa)'].replace(',','.'))}
        yield returned

def get_fields(included_fields, filename , skipcount):
    
    included_fields = set(included_fields)
    
    with open(filename, 'rb') as csvfile:
        #empty strings
        for _ in range(skipcount):
            print csvfile.readline()
                
        for row in parse_file(csvfile):
            foundkeys = row.viewkeys() & included_fields
            if foundkeys == included_fields:
                returned  = [ row[header] for header in included_fields]
                yield returned

def get_fields_sodankyla(included_fields):
    filename = "Lämpötila- ja painehavainnot 2010-2012 Sodankylä.csv"
    for r in get_fields(included_fields, filename,14):
        yield r

def get_fields_jyvaskyla(included_fields):
    filename = "Lämpötila- ja painehavainnot 2010-2012 Jyväskylä lentoasema.csv"
    for r in get_fields(included_fields, filename,8):
        yield r

def get_fields_helsinki(included_fields):
    filename = "Lämpötila- ja painehavainnot 2010-2012 Helsinki Kaisaniemi.csv"
    for r in get_fields(included_fields, filename,8):
        yield r

if __name__ == "__main__":
    
    #tests:
    
    for r in get_fields_sodankyla(['timestamp', 'T','P']):
        print r
        break
    
    for r in get_fields_jyvaskyla(['timestamp', 'T','P']):
        print r
        break
    
    for r in get_fields_helsinki(['timestamp', 'T','P']):
        print r
        break
