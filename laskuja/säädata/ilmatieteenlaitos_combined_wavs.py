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
    names = ["sodankyla", "jyvaskyla", "helsinki"]
    
    delta = datetime.timedelta(0,600,0)
    
    def get_signal(parser, combo):
        print parser, name, combo
        print "getting data"
        
        signal = list(parser(combo))
        
        print "interpoling signal"
        
        new_signal = list( interpolateSignal(signal, delta) )
        
        return new_signal
        
    for combo in field_combos:
        name = "ilmatieteenlaitos_"+"_".join(names) + "_combined_"+combo[1]
        signal_1 = get_signal(get_fields_sodankyla,combo)
        signal_2 = get_signal(get_fields_jyvaskyla,combo)
        signal_3 = get_signal(get_fields_helsinki,combo)
        
        print "summing"
        signal_sum = [[signal_1[i][0],signal_1[i][1]+ signal_2[i][1]+ signal_3[i][1] ] for i in range(len(signal_1))]
        
        print "making wavs"
        generate_wavs(signal_sum,name)
        