"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


print __doc__

import numpy as np
import pylab as pl
from sklearn.decomposition import FastICA

from parse_data import get_data

###############################################################################
# Generate sample data

data = get_data()
temperature_0 = [ r[1] for r in data[:-1]]
temperature_1 = [ r[1] for r in data[1:]]

X = np.c_[temperature_0,temperature_1]

# Compute ICA
ica = FastICA()
S_ = ica.fit(X).transform(X)  # Get the estimated sources
A_ = ica.get_mixing_matrix()  # Get estimated mixing matrix
assert np.allclose(X, np.dot(S_, A_.T))

#################################################################################
### Plot results

chanel_0 = S_[:,0]
chanel_1 = S_[:,1]


pl.figure()

pl.subplot(3,1,1)
pl.plot(temperature_0)
pl.title('True source')
pl.subplot(3,1,2)
pl.plot(chanel_0)
pl.title('ICA 1')

pl.subplot(3,1,3)
pl.plot(chanel_1)
pl.title('ICA 2')

pl.show()


