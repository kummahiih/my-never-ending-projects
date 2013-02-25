"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


#for parsing
from parse_data import get_data

from plot_tools import makeplot, daily, monthly

    
if __name__ == "__main__":
    signal = get_data()
    makeplot(signal, daily)
    makeplot(signal, monthly)
    
    
            
