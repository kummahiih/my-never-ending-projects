"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from data_to_wav import generate_wavs
from parse_willab_data import get_data_airpressure
from signal_tools import interpolateSignal
import datetime

def takeOnlyValuesBetveen(signal, fromValue, toValue):
    for i in signal:
        if i[1] >= fromValue and i[1] <= toValue:
            yield i

if __name__ == "__main__":
    delta = datetime.timedelta(0,600,0)
    signal = get_data_airpressure()
    #airpressure_signal = list( interpolateSignal(signal, delta) )    
    #generate_wavs(airpressure_signal,"willab_airpressure_interpolated")

    airpressure_signal = list(
        takeOnlyValuesBetveen(
            interpolateSignal(signal, delta),
            961,1100
            )
        )
    generate_wavs(airpressure_signal,"willab_airpressure_sanitazed_interpolated")
