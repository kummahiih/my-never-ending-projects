# -*- coding: cp1252 -*-
"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

from kuvio import *
from plot_tools import makeplot_from_lists, yearly
import datetime

Kuviot = [
    ProsenttiKuvio("3",  2.3, Vakiot.paikka["MT"], 40, Vakiot.luokka["02"],  80  ),
    ProsenttiKuvio("4",  2.4, Vakiot.paikka["VT"], 45, Vakiot.luokka["02"],  70  ),
    ProsenttiKuvio("5",  0.4, Vakiot.paikka["VT"], 15, Vakiot.luokka["T2"],  0.0 ),
    ProsenttiKuvio("6",  2.8, Vakiot.paikka["VT"], 30, Vakiot.luokka["02"],  28  ),
    ProsenttiKuvio("9",  0.1, Vakiot.paikka["VT"], 130,Vakiot.luokka["04"],  140 ),
    ProsenttiKuvio("10", 0.7, Vakiot.paikka["VT"], 15, Vakiot.luokka["T2"],  0.0 ),
    ProsenttiKuvio("11", 0.7, Vakiot.paikka["VT"], 15, Vakiot.luokka["T2"],  0.0 ),
    ProsenttiKuvio("12", 2.2, Vakiot.paikka["MT"], 50, Vakiot.luokka["02"],  110 ),
    ProsenttiKuvio("13", 2.0, Vakiot.paikka["VT"], 5,  Vakiot.luokka["T1"],  0   ),
    ProsenttiKuvio("14", 0.9, Vakiot.paikka["VT"], 30, Vakiot.luokka["T2"],  0   ),
    ProsenttiKuvio("15", 0.8, Vakiot.paikka["VT"], 40, Vakiot.luokka["02"],  70  ),
    ProsenttiKuvio("16", 2.0, Vakiot.paikka["MT"], 50, Vakiot.luokka["02"],  85  ),
    ProsenttiKuvio("17", 1.4, Vakiot.paikka["MT"], 75, Vakiot.luokka["03"],  85  ),
    ProsenttiKuvio("19", 2.1, Vakiot.paikka["MT"], 75, Vakiot.luokka["03"],  147 )
    ]

if __name__ == "__main__":    
    print "perkkion metsien ennusteet"
    tulot = 0
    kuutiot = 0
    kuutiot_list = []
    tulot_list = []
    aika_list = []
    metsien_kuutiot = []
    
    for i in range(40):
        vuosi = 2013 + i
        print '*'*60
        print 'Vuosi', vuosi
        aika_list.append(datetime.datetime(vuosi,1,1))

        metsakuutiot = 0
        
        for kuvio in Kuviot:
            print '-'* 60
            r = kuvio.simuloi()
            metsakuutiot += kuvio.kuutiot
            if r:
                print r
                
                tulot += r['tulo']
                kuutiot += r['kuutiot_pois']
                
        metsien_kuutiot.append(metsakuutiot)
        kuutiot_list.append(kuutiot)
        tulot_list.append(tulot)
        
        print '-'* 60
        print 'tulot:', tulot
        print 'tulot / vuosi:', tulot / (i+1)
        print 'kuutiot:', kuutiot
        print 'kuutiot / vuosi:', kuutiot / (i+1)
    
    makeplot_from_lists(aika_list, metsien_kuutiot, yearly, step = 100)
    makeplot_from_lists(aika_list, kuutiot_list, yearly, step = 100)
    makeplot_from_lists(aika_list, tulot_list, yearly, step = 2000)
    
    
                
                
                
