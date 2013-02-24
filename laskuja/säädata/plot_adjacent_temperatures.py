"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


from parse_data import get_data




import numpy as np
import matplotlib.pyplot as plt


data = get_data()

temperature_0 = [ r[1] for r in data[:-1]]
temperature_1 = [ r[1] for r in data[1:]]


H, xedges, yedges = np.histogram2d(temperature_0,temperature_1, bins=(128,128))

H_log = np.log(H)

extent = [yedges[0], yedges[-1], xedges[-1], xedges[0]]

plt.imshow(H_log, extent=extent, interpolation='nearest')

plt.colorbar()
plt.xlabel("T_n")
plt.ylabel("T_(n+1)")
plt.show()
