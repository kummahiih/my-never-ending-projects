"""
Data is from ilmatieteen laitos ( http://ilmatieteenlaitos.fi/ )
COPYRIGHT: ILMATIETEEN LAITOS
"""

"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from parse_ilmatieteenlaitos import get_fields_sodankyla, get_fields_jyvaskyla, get_fields_helsinki, headers
from plot_tools import makeplot, monthly, no_locator_set


if __name__ == "__main__":
    print 'sodankyla T'
    signal = list(get_fields_sodankyla(['timestamp','T']))
    print signal[0]
    makeplot(signal, monthly, step = 2)
    
    
    print 'jyvaskyla T'
    signal = list(get_fields_jyvaskyla(['timestamp','T']))
    print signal[0]
    makeplot(signal, monthly, step = 2)
    
    print 'helsinki T'
    signal = list(get_fields_helsinki(['timestamp','T']))
    print signal[0]
    makeplot(signal, monthly, step = 2)
    
    print 'sodankyla P'
    signal = list(get_fields_sodankyla(['timestamp','P']))
    print signal[0]
    makeplot(signal, monthly, step = 2)
    
    print 'jyvaskyla P'
    signal = list(get_fields_jyvaskyla(['timestamp','P']))
    print signal[0]
    makeplot(signal, monthly, step = 2)
    
    print 'helsinki P'
    signal = list(get_fields_helsinki(['timestamp','P']))
    print signal[0]
    makeplot(signal, monthly, step = 2)

