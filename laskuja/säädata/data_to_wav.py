"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


#for parsing
from parse_data import get_data

#for audio
import wave
import struct

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

if __name__ == "__main__":
    generate_wavs(get_data(),"temperature")
