"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


import csv
import datetime
import time
import re

headers = "measuretime,tempnow,temphi,templo,dewpoint,humidity,airpressure,windspeed,windspeedmax,winddir,precipitation1h,precipitation24h,solarrad".split(',')


def convertWillabField(item):
    header = item[0]
    data = item[1]
    try:
        if header == "measuretime":
            gmt_offset_str = data[-3:]
            gmt_offset_seconds = int(gmt_offset_str)*60*60
            timestamp = time.strptime(data[:-4], '%Y-%m-%d %H:%M:%S')
            got_time = time.mktime(timestamp)-gmt_offset_seconds
            timestamp = datetime.datetime.fromtimestamp(got_time)
            

            return (header, timestamp )
        return (header, float(data))
    except:
        return None

def filterEmpty( adict):
    return dict( [ i for i in adict.items() if i[1] != ''])

def convertRow(row, convertter):
        return dict([i for i in map(convertter, row.items()) if i != None])

def get_fields(filename ='weather.csv', included_fields = headers, convertter = convertWillabField):
    included_fields = set(included_fields)
    with open(filename, 'rb') as csvfile:
        reader = csv.DictReader(csvfile, delimiter=',')
        for row in reader:
            row = filterEmpty(row)
            foundkeys = row.viewkeys() & included_fields
            if foundkeys == included_fields:
                returned  = [ convertRow(row,convertter)[header] for header in included_fields]
                yield returned

def get_data():
    return list(
        get_fields(included_fields = "measuretime,tempnow".split(','))
        )

def get_data_airpressure():
    return list(
        get_fields(included_fields = "measuretime,airpressure".split(','))
        )
    

if __name__ == "__main__":
    data = get_data()
