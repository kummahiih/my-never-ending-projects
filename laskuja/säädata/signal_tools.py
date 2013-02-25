"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

import datetime

import wave
import struct


def interpolateValue(prevSample, nextSample, toStamp):
    prevStamp = prevSample[0]
    nextStamp = nextSample[0]

    assert(prevStamp < nextStamp)
    assert(prevStamp <= toStamp)
    assert(toStamp <= nextStamp)

    sample_delta = nextStamp - prevStamp

    to_sample_delta = toStamp - prevStamp

    multiplier = to_sample_delta.total_seconds() /  sample_delta.total_seconds()
    
    prevValue =  prevSample[1]
    nextValue =  nextSample[1]

    valueDelta = nextValue - prevValue

    newValue = prevValue + multiplier * valueDelta

    return toStamp, newValue


def getNewTimeStamps(startTime,endTime,delta):
    astamp = startTime
    while(astamp <= endTime):
        yield astamp
        astamp += delta


def interpolateSignal(signal, delta):
    nextIndex = 0
    prevSample = None
    for newStamp in getNewTimeStamps( signal[0][0], signal[-1][0], delta ):
        if prevSample == None:
            prevSample = signal[0]
            yield prevSample
            continue
            
        while ( nextIndex < len(signal) and signal[nextIndex][0] < newStamp ):
            prevSample = signal[nextIndex]
            nextIndex += 1
        if nextIndex >= len(signal):
            break
        prevSample = interpolateValue(prevSample , signal[nextIndex], newStamp)
        yield prevSample


def scale( aFloatList ,amin, amax):
    samples_max = max(aFloatList)
    samples_min = min(aFloatList)

    if samples_min - samples_max == 0:
        return [ (amin + amax) / 2 for _ in aFloatList ]

    returned = [
        amin +
        ((sample-samples_min) *
                (amax - amin) / (samples_max - samples_min)
         )

        for sample in aFloatList ]

    return returned

def generate_wavs(data,prefix):
    temperature = [ r[1] for r in data]
    scaled_temperature = scale(temperature,-32767,32767)

    for framerate in range(1000,128000,5000):
        noise_output = wave.open('results/%s_%d.wav'%(prefix,framerate), 'w')
        noise_output.setparams((2, 2, framerate, 0, 'NONE', 'not compressed'))

        for i in scaled_temperature:
                value = int(i +0.5)
                packed_value = struct.pack('h', value)
                noise_output.writeframes(packed_value)
                noise_output.writeframes(packed_value)

        noise_output.close()