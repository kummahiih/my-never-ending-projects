"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from parse_willab_data import get_data_airpressure
from signal_tools import interpolateSignal
from willab_airpressure_to_wav import takeOnlyValuesBetveen

from plot_tools import makeplot, monthly, no_locator_set
import datetime

if __name__ == "__main__":
    delta = datetime.timedelta(0,600,0)
    signal = get_data_airpressure()
    makeplot(signal, monthly, step = 100)
    
    new_signal = list(
        takeOnlyValuesBetveen( #this takes only values between 961 and 1100
            interpolateSignal(signal, delta),  #this respamples data to 'delta' time intervals
            961,1100)
        )
    makeplot(new_signal, monthly, step = 100)
