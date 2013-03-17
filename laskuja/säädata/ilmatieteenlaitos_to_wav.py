"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from signal_tools import generate_wavs, interpolateSignal
import datetime

from parse_ilmatieteenlaitos import get_fields_sodankyla, get_fields_jyvaskyla, get_fields_helsinki, headers




if __name__ == "__main__":
    
    field_combos = [['timestamp','T'],['timestamp','P']]
    parsers = [get_fields_sodankyla, get_fields_jyvaskyla, get_fields_helsinki]
    names = ["ilmatieteenlaitos_sodankyla", "ilmatieteenlaitos_jyvaskyla", "ilmatieteenlaitos_helsinki"]
    
    delta = datetime.timedelta(0,600,0)
    
    def generate(parser, name, combo):
        print parser, name, combo
        print "getting data"
        
        signal = list(parser(combo))
        
        print "generating signal"
        
        new_signal = list( interpolateSignal(signal, delta) )
        
        print "generating wavs"
    
        generate_wavs(new_signal,name + "_"+combo[1])
        
    for combo in field_combos:
        for parser, name in zip(parsers,names):
            generate(parser, name, combo)
    