"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


#for parsing
from parse_data import get_data
#for audio
from signal_tools import generate_wavs





if __name__ == "__main__":
    print "generating wavs"
    generate_wavs(get_data(),"temperature")
