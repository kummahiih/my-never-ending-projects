# -*- coding: cp1252 -*-

"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""
from kuvio import *
from kuviot_perkkio import Kuviot
from plot_tools import makeplot_from_lists, yearly
import datetime
import math

def laske_lainakulut( maksettava, korko, aika, lyhennys ):
    for i in range(aika):
        maksettava_korko = korko * maksettava
        
        if lyhennys < maksettava:
            maksettava = maksettava - lyhennys
            menot = lyhennys + maksettava_korko
            yield {"maksettavaa": maksettava, 'menot':menot }
        else:
            menot = maksettava + maksettava_korko
            maksettava = 0
            yield {"maksettavaa": maksettava, 'menot':menot }
            

simulaation_kesto = 100

lohkomiskulut = 2500
lainhuudatusmaksu = 150
menot_metsakaupan_jarjestelykulut = 200
menot_lainan_jarjestelypalkkiot = 200


korkotukilaina = 3000
korkotukilainan_maksuaika = 15
korkotukilainan_korko = 0.033

korkotukilainan_lyhennys = korkotukilaina/ korkotukilainan_maksuaika

print 'simulaation kesto vuosina', simulaation_kesto

oma_raha = 12000

talletuskorko = 0.015
inflaatio = 0.03
print 'talletuskorko', talletuskorko
print 'inflaatio', inflaatio
print '-'
print 'sijoitettu oma raha ', oma_raha

print '-'

print 'otettu korkotukilaina', korkotukilaina
print 'korkotukilainan maksuaika', korkotukilainan_maksuaika
print 'korkotukilainan korko', korkotukilainan_korko
print 'korkotukilainan lyhennys', korkotukilainan_lyhennys

korkotukilaina_gen = laske_lainakulut( korkotukilaina, korkotukilainan_korko, simulaation_kesto, korkotukilainan_lyhennys)

print '-'

kovan_rahan_laina = 3000
kova_maksuaika = 10
kovan_rahan_korko = 0.05

kovan_rahan_lyhennys = kovan_rahan_laina / kova_maksuaika

print 'otettu kovan rahan laina', kovan_rahan_laina
print 'kovan rahan lainan maksuaika', kova_maksuaika
print 'kovan rahan lainan  korko', kovan_rahan_korko
print 'kovan rahan lainan  lyhennys', kovan_rahan_lyhennys

kovaraha_gen = laske_lainakulut( kovan_rahan_laina, kovan_rahan_korko, simulaation_kesto, kovan_rahan_lyhennys)

print '-'

print 'lohkomiskulut', lohkomiskulut 
print 'lainhuudatusmaksu', lainhuudatusmaksu 
print 'menot_metsakaupan_jarjestelykulut', menot_metsakaupan_jarjestelykulut 
print 'menot_lainan_jarjestelypalkkiot', menot_lainan_jarjestelypalkkiot 

alkukulut =  (lohkomiskulut + lainhuudatusmaksu + menot_metsakaupan_jarjestelykulut + menot_lainan_jarjestelypalkkiot)

print "alkukulut", alkukulut



print "*"* 100
alkutase = -alkukulut

maan_arvo = 0
for kuvio in Kuviot:
    maan_arvo += Vakiot.paikan_arvo[kuvio.paikka]
    
print 'ostetun maapohjan arvo', maan_arvo

alkutase += maan_arvo
print 'alkutase maapohjan arvo mukaanluettuna (maan arvo - alkukulut ):', alkutase
tase = alkutase

aika_list = []
tase_list = []


for i in range( simulaation_kesto ):
    vuosi = 2013 + i
    aika_list.append(datetime.datetime(vuosi,1,1))
    print "*" * 60
    print "Vuosi:", vuosi
    
    print 'korkotukilaina:' 
    maskutiedot = korkotukilaina_gen.next()    
    print 'lainan tiedot:', maskutiedot
    
    tase -= maskutiedot['menot']
    korkotukilaina_maksettavaa =  maskutiedot['maksettavaa']
    
    print 'kovan rahan laina'    
    maskutiedot = kovaraha_gen.next()
    print 'lainan tiedot:', maskutiedot
    
    tase -= maskutiedot['menot']
    kovarahalaina_maksettavaa =  maskutiedot['maksettavaa']
    
    print 'tase lainakulujen jälkeen:', tase
    
    print 'tase lainakulujen jälkeen maksamattomat lainat mukaanluettuna:', tase - korkotukilaina_maksettavaa - kovarahalaina_maksettavaa
    tase_list.append(tase - korkotukilaina_maksettavaa - kovarahalaina_maksettavaa)
    
    if tase > 0:
        korkotulo = tase * talletuskorko
        print 'tase plussalla -> positiivista korkoa:', korkotulo
        tase += korkotulo
    
    print "Kuvioiden tapahtumat:"
    for kuvio in Kuviot:
        print '-'* 60
        kuviotiedot = kuvio.simuloi()
        if kuviotiedot:
            print kuviotiedot
            tase += kuviotiedot['tulo'] * (1+inflaatio) ** i
    print 'tase kuvioiden tulojen jälkeen', tase
    print 'tase kuvioiden tulojen  jälkeen maksamaton laina mukaanluettuna:', tase - maskutiedot['maksettavaa']
    
    

tase = tase - maskutiedot['maksettavaa']


investoitu = (oma_raha + korkotukilaina +kovan_rahan_laina) * (1+inflaatio) ** simulaation_kesto
print "oletetetaan, että 120 v päästä metsästä saa myytynä saman hinnan eli",  investoitu
vuosikorko = math.log((tase + investoitu) / oma_raha ) / simulaation_kesto

print 'keskimääräinen vuosikorko', vuosikorko, 'kun simulaation kesto oli', simulaation_kesto," v. ja investoitu",oma_raha + korkotukilaina +kovan_rahan_laina, "(sis lainat) inflaatio oli ", inflaatio,' talletuskorko', talletuskorko


makeplot_from_lists(aika_list, tase_list, yearly, step = 2000)







