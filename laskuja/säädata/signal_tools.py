"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""

import datetime

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
