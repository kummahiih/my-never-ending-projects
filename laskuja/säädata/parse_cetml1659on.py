#data for this file is available at
#http://www.metoffice.gov.uk/hadobs/hadcet/cetml1659on.dat

##http://www.metoffice.gov.uk/hadobs/hadcet/terms_and_conditions.html :

##HadCET Data: Terms and Conditions
##HadCET is subject to Crown copyright protection. The material may be downloaded to file or printer for the purposes of private study and scientific research. Any other proposed use of the material is subject to a copyright licence available from the Met Office. Licences and further information can be obtained from the Met Office IPR Officer, FitzRoy Road, Exeter, EX1 3PB E-mail: ipr@metoffice.gov.uk.
##For further information on Crown Copyright policy and licensing arrangements, see the guidance featured on HMSO's web site.
##When publishing work using the data, please use the following citation:
##Parker, D.E., T.P. Legg, and C.K. Folland. 1992. A new daily Central England Temperature Series, 1772-1991. Int. J. Clim., Vol 12, pp 317-342 
##The source should also be quoted in the acknowledgements section as www.metoffice.gov.uk/hadobs.

# I think this is private research and study ..

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
