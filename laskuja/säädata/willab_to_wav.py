"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from signal_tools import generate_wavs
from parse_willab_data import get_data

from signal_tools import interpolateSignal

import datetime
            
        
        

if __name__ == "__main__":
    delta = datetime.timedelta(0,600,0)
    
    print "getting data"
    signal = get_data()
    print "generating wavs"

    generate_wavs(signal,"willab_tempnow")
    
    print "interpolating signal"

    new_signal = list( interpolateSignal(signal, delta) )
    
    print "generating wavs"

    generate_wavs(new_signal,"willab_tempnow_interpolated")
