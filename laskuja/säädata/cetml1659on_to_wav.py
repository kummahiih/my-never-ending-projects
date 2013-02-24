"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


from data_to_wav import generate_wavs
from parse_cetml1659on import get_data

if __name__ == "__main__":
    generate_wavs(get_data(),"cetml1659on")
