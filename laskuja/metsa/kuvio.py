# -*- coding: cp1252 -*-
"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""
from plot_tools import makeplot_from_lists, yearly
import datetime


class Lyhenne:
    def __init__(self, lyhyt, pitka ):
        self.lyhyt = lyhyt
        self.pitka = pitka
    def __str__(self):
        return self.lyhyt


class Vakiot:
    paikat= [
        Lyhenne("Lehto", "Lehto tai vastaaa suo"),
        Lyhenne("OMT", "Lehtomainen kangas tai vastaava suo"),
        Lyhenne("MT","Tuore kangas tai vastaava suo"),
        Lyhenne("VT","Kuivahko kangas tai vastaava suo"),
        Lyhenne("CT","Kuiva kangas tai vastaava suo"),
        Lyhenne("CIT","Karukkokangas tai vastaava suo"),
        Lyhenne("Kitu","Kitumaa"),
        Lyhenne("Jouto","Joutomaa"),
        Lyhenne("Muu","Muu maa")
        ]
    
    luokat = [
        Lyhenne("A0", "Aukea"),
        Lyhenne("S0", "Siemenmetsikkö"),
        Lyhenne("T1", "Taimikko, alle 1.3 m"),
        Lyhenne("T2", "Taimikko, yli 1.3 m"),
        Lyhenne("Y1", "Yksipuolinen taimikko"),
        Lyhenne("02", "Nuori kasvumetsikkö"),
        Lyhenne("03", "Varttunut kasvumetsikkö"),
        Lyhenne("04", "Uudistuskypsä metsikkö"),
        Lyhenne("05", "Suojuspuumetsikkö"),
        Lyhenne("06", "Vajaamuotoinen metsikkö")
        ]

    puut = [
        Lyhenne("Mät", "Mänty tukki"),
        Lyhenne("Kut", "Kuusitukki"),
        Lyhenne("Kot", "Koivutuki"),
        Lyhenne("Mut", "muutukki"),
        Lyhenne("Mäe", "Mäntykuitu"),
        Lyhenne("Kuk", "kuusikuitu"),
        Lyhenne("Kok", "koivukuitu"),
        Lyhenne("Muk", " muukuitu"),
        Lyhenne("Enp", "Energiapuu")
        ]
    
    paikka = dict([(p.lyhyt, p) for p in  paikat])
    luokka= dict([(p.lyhyt, p) for p in  luokat])
    puu = dict([(p.lyhyt, p) for p in  puut])
    
    puun_arvo = {
        puu["Mät"]:50.58,
        puu["Kut"]:47.9,
        puu["Kot"]:46.74,
        puu["Mut"]:6.00,
        puu["Mäe"]:14.18,
        puu["Kuk"]:21.88,
        puu["Kok"]:13.9,
        puu["Muk"]:5.0,
        puu["Enp"]:3.00
        }
    
    paikan_arvo = {
        paikka["Lehto"]:0.0,
        paikka["OMT"]:440.,
        paikka["MT"]:320.,
        paikka["VT"]:230,
        paikka["CT"]:170,
        paikka["CIT"]:0.0,
        paikka["Kitu"]:0.0,
        paikka["Jouto"]:0.0,
        paikka["Muu"]:0.0,        
        }
    
    """Männyn kasvuprosentti. laske ite muille. lisäksi absoluuttinen kasvu olisi parempi kuin kasvupros, mutta tuli jo arvioitua ..."""
    kasvuprosentti = {
       (paikka["MT"],luokka["A0"]):0.0,
       (paikka["MT"],luokka["T1"]):0.41,
       (paikka["MT"],luokka["T2"]):0.40,
       (paikka["MT"],luokka["02"]):0.091,
       (paikka["MT"],luokka["03"]):0.0285,
       (paikka["MT"],luokka["04"]):0.017,
       
       (paikka["VT"],luokka["A0"]):0.0,
       (paikka["VT"],luokka["T1"]):0.32,
       (paikka["VT"],luokka["T2"]):0.31,
       (paikka["VT"],luokka["02"]):0.0545,
       (paikka["VT"],luokka["03"]):0.027,
       (paikka["VT"],luokka["04"]):0.0175,
       
       #nämä on harusta vedettyjä
       (paikka["CT"],luokka["A0"]):0.0,
       (paikka["CT"],luokka["T1"]):0.28,
       (paikka["CT"],luokka["T2"]):0.28,
       (paikka["CT"],luokka["02"]):0.0445,
       (paikka["CT"],luokka["03"]):0.027,
       (paikka["CT"],luokka["04"]):0.0175,
       
       #aivan hatusta
       (paikka["OMT"],luokka["A0"]):0.0,
       (paikka["OMT"],luokka["T1"]):0.28,
       (paikka["OMT"],luokka["T2"]):0.28,
       (paikka["OMT"],luokka["02"]):0.0445,
       (paikka["OMT"],luokka["03"]):0.027,
       (paikka["OMT"],luokka["04"]):0.0175
       
       }
    

#TODO: taimikon hoitokustannukset
#TODO: raivauskustannukset

class ProsenttiKuvio:
    def __init__(self, ID, ala, paikka,  ika, luokka,  kuutiot_per_ala):
        self.ID = ID
        self.paikka = paikka
        self.luokka = luokka
        self.ala = ala
        self.ika = ika
        self.kuutiot = kuutiot_per_ala * ala
    
    def __repr__(self):
        return "ID:" +self.ID +" ala:" +str(self.ala) +": ("+ str(self.paikka) +"," +str(self.luokka)+ ") ika:" + str(self.ika) + " m3: "+  str(self.kuutiot)
    
    def simuloi(self):
        print self
        self.ika += 1
        kasvuprosentti = Vakiot.kasvuprosentti[ (self.paikka, self.luokka)]
        if self.luokka == Vakiot.luokka["A0"] or  ( self.kuutiot == 0 and self.luokka  in [Vakiot.luokka["T1"], Vakiot.luokka["T2"]] ) :
            self.kuutiot += 0.1 * self.ala
            print "aukio kasvaa taimea 0.1 m3 / ha /v",
        else:
            self.kuutiot += kasvuprosentti * self.kuutiot
            print 'kasvuprosentti:',kasvuprosentti,
        print '-> uusi m3:', self.kuutiot
        if self.luokka == Vakiot.luokka["A0"] and self.ika >= 10:
            self.kuutiot = 10 * self.ala
            self.luokka = Vakiot.luokka["T2"]
            print "A0 -> T2:"
            print self
            return {
                }
        if self.luokka == Vakiot.luokka["T1"] and self.ika >= 10:            
            self.luokka = Vakiot.luokka["T2"]
            print "T1 -> T2:"
            print self
            return {                
                }
        if self.luokka == Vakiot.luokka["T2"] and self.ika >= 35:
            self.luokka = Vakiot.luokka["02"]
            print "T2 -> 02 (aika):"
            print  self
            return {
                }
        if self.luokka == Vakiot.luokka["T2"] and self.kuutiot >= 28 * self.ala:
            self.luokka = Vakiot.luokka["02"]
            print "T2 -> 02 (kuutiot >= 28 m3 / ha):"
            print  self
            return {
                }
        if self.luokka == Vakiot.luokka["02"] and self.kuutiot >= 90 * self.ala:
            uudet_kuutiot = 75. * self.ala
            kuutiot_pois = self.kuutiot - uudet_kuutiot
            tulo = 14. * kuutiot_pois
            self.kuutiot = uudet_kuutiot
            print "Ensiharvennus (kuutiot)"
            self.luokka = Vakiot.luokka["03"]
            print "02 -> 03"
            print  self
            return {'tulo': tulo, 'kuutiot_pois':kuutiot_pois }
        
        if self.luokka == Vakiot.luokka["02"] and self.ika >= 55:
            returned = {}
            if self.kuutiot >=  75. * self.ala:
                print "Ensiharvennus (ikä)"
                uudet_kuutiot = 75. * self.ala
                kuutiot_pois = self.kuutiot - uudet_kuutiot
                tulo = 14. * kuutiot_pois  
                self.kuutiot = uudet_kuutiot  
                returned = {'tulo': tulo, 'kuutiot_pois':kuutiot_pois }
            else:
                print "ei tarpeeksi puuta ensiharvennukseen"
            self.luokka = Vakiot.luokka["03"]
            print "02 -> 03"
            print  self
            return returned
        
        if self.luokka == Vakiot.luokka["03"] and self.kuutiot >= 130 * self.ala:
            uudet_kuutiot = 50. * self.ala
            kuutiot_pois = self.kuutiot - uudet_kuutiot
            tulo = 20.* kuutiot_pois
            self.kuutiot = uudet_kuutiot
            self.luokka = Vakiot.luokka["04"]
            print "Harvennushakkuu (kuutiot)"
            print "03 -> 04"
            print  self
            return {'tulo': tulo, 'kuutiot_pois':kuutiot_pois }
        
        if self.luokka == Vakiot.luokka["03"] and self.ika >= 80:
            returned = {}
            if self.kuutiot >=  50. * self.ala:
                print "Harvennushakkuu (ikä)"
                uudet_kuutiot = 50. * self.ala
                kuutiot_pois = self.kuutiot - uudet_kuutiot
                tulo = 20.* kuutiot_pois
                self.kuutiot = uudet_kuutiot
                returned = {'tulo': tulo, 'kuutiot_pois':kuutiot_pois }
            else:
               print "ei tarpeeksi puuta Harvennushakkuuseen" 
            self.luokka = Vakiot.luokka["04"]
            print "03 -> 04"
            print  self
            return returned
        
        if self.luokka == Vakiot.luokka["04"] and ( self.kuutiot >= 130 * self.ala or self.ika >= 110):
            kuutiot_pois = self.kuutiot
            tukkia = 100. / 130. * self.kuutiot
            kuitua = (1 - 100. / 130.) * self.kuutiot
            
            metsasta_saatava_myyntitulo_tukkipuu = tukkia * 50.
            metsasta_saatava_myyntitulo_kuitupuu = kuitua * 20.
            
            menot_metsan_uudistus_kulut_tukki = metsasta_saatava_myyntitulo_tukkipuu * 0.1
            print 'uudistuskulut:', menot_metsan_uudistus_kulut_tukki
            
            tulo = metsasta_saatava_myyntitulo_tukkipuu  + metsasta_saatava_myyntitulo_kuitupuu - menot_metsan_uudistus_kulut_tukki
            
            self.kuutiot = 0
            self.ika = 0
            self.luokka = Vakiot.luokka["A0"]
            print "Päätehakkuu (%s)"%("ikä" if self.ika >= 110 else "kuutiot")
            print "04 -> A0"
            print  self
            return {'tulo': tulo, 'kuutiot_pois':kuutiot_pois }



if __name__ == "__main__":
    print Vakiot.paikka["Lehto"]
    print Vakiot.luokka["T1"]
    print Vakiot.kasvuprosentti[(Vakiot.paikka["MT"],Vakiot.luokka["T1"])]
    print Vakiot.puun_arvo[Vakiot.puu["Mät"]]
    print Vakiot.paikan_arvo[Vakiot.paikka["MT"]]
    
    #(self, ID, paikka, luokka, ala, ika, kuutiot):
    testikuvio = ProsenttiKuvio("12", 2.2,Vakiot.paikka["MT"],  50, Vakiot.luokka["02"], 110 )
    
    #(self, ID, paikka, luokka, ala, ika, kuutiot):
    #testikuvio = ProsenttiKuvio("16", 2.0, Vakiot.paikka["MT"], 50, Vakiot.luokka["02"],  85  )
    aika_list = []
    kuutiot_list = []
    for i in range(200):
        vuosi = i+2013
        print "-"*60
        print "vuosi:",vuosi
        aika_list.append(datetime.datetime(vuosi,1,1))
        kuutiot_list.append(testikuvio.kuutiot)
        print testikuvio.simuloi()
    
    makeplot_from_lists(aika_list, kuutiot_list, yearly, step = 100)
    
    
    
    
    
