#http://www.metoffice.gov.uk/hadobs/hadcet/cetml1659on.dat

import csv
import re
import datetime



def get_cetml1659on_rows(filename):
    with open(filename, 'rb') as csvfile:
        for row in csvfile.read().split('\n'):
            yield re.split(r'\s+',row.strip(' '))


def get_cetml1659on_numbers(filename):

    rows = list( get_cetml1659on_rows(filename))
    datarows = rows[7:-1]

    datarows = [ [ int(i[0]) ] + map(float,i[1:]) for i in datarows]

    return datarows


def get_cetml1659on_with_timespamps(filename):
    datarows = get_cetml1659on_numbers(filename)

    for row in datarows:
        year = row[0]
        monthly_data = row[1:-1]
        for i, value in enumerate(monthly_data):
            timestamp = datetime.date(year, i+1,1)
            yield timestamp, value
        
def get_data():
    return list(get_cetml1659on_with_timespamps('cetml1659on.dat'))


if __name__ == "__main__":
    data = get_data()
