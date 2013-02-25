"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


from parse_cetml1659on import get_data

from plot_tools import makeplot, monthly, no_locator_set


if __name__ == "__main__":
    makeplot(get_data(), no_locator_set, step = 2)
    
