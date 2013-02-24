"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from parse_willab_data import get_data
from willab_to_wav import interpolateSignal

from plot_data import makeplot, monthly, no_locator_set
import datetime

if __name__ == "__main__":
    delta = datetime.timedelta(0,600,0)
    signal = get_data()
    makeplot(signal, monthly, step = 2)
    new_signal = list( interpolateSignal(signal, delta) )
    makeplot(new_signal, monthly, step = 2)
