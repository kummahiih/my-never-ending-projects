# -*- coding: cp1252 -*-
"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""
from kuvio import *


Kuviot = [
#    ProsenttiKuvio("16", 2.0, Vakiot.paikka["MT"], 50, Vakiot.luokka["02"],  85  ),
    ProsenttiKuvio("1.1", 1.8, Vakiot.paikka["MT"], 41, Vakiot.luokka["03"],  60  ),
    ProsenttiKuvio("1.2", 2.5, Vakiot.paikka["MT"], 41, Vakiot.luokka["02"],  30  ),
    ProsenttiKuvio("2.0", 3.8, Vakiot.paikka["CT"], 61, Vakiot.luokka["02"],  66 ),
    ProsenttiKuvio("3.0", 2.2, Vakiot.paikka["VT"], 131, Vakiot.luokka["04"],  193 ),
    ProsenttiKuvio("4.0", 2.6, Vakiot.paikka["CT"], 46, Vakiot.luokka["02"],  34 ),
    ProsenttiKuvio("5.0", 0.5, Vakiot.paikka["CT"], 131, Vakiot.luokka["04"],  195 ),
    ProsenttiKuvio("6.0", 1.0, Vakiot.paikka["VT"], 48, Vakiot.luokka["03"],  101 ),
    ProsenttiKuvio("6.1", 0.7, Vakiot.paikka["VT"], 36, Vakiot.luokka["02"],  32 ),
    ProsenttiKuvio("6.2", 1.8, Vakiot.paikka["VT"], 36, Vakiot.luokka["02"],  78 ),
    ProsenttiKuvio("6.3", 2.9, Vakiot.paikka["VT"], 36, Vakiot.luokka["02"],  55 ),
    ProsenttiKuvio("7.0", 5.1, Vakiot.paikka["MT"], 48, Vakiot.luokka["02"],  105 ),
    ProsenttiKuvio("7.1", 1.9, Vakiot.paikka["MT"], 54, Vakiot.luokka["02"],  53 )
 
    ]


if __name__ == "__main__":
    print "Lootin metsien ennusteet"
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
