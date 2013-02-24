"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


from parse_data import get_data

import numpy as np
import pylab as pl
from sklearn.decomposition import PCA

data = get_data()
temperature_0 = [ r[1] for r in data[:-2]]
temperature_1 = [ r[1] for r in data[1:-1]]
temperature_2 = [ r[1] for r in data[2:]]

X = np.c_[temperature_0,temperature_1,temperature_2]

pca = PCA(n_components=3)
X_r = pca.fit(X).transform(X)


# Percentage of variance explained for each components
print 'explained variance ratio (first two components):', \
    pca.explained_variance_ratio_

#pl.figure()
chanel_0 = X_r[:,0]
chanel_1 = X_r[:,1]
chanel_2 = X_r[:,2]

pl.figure()

pl.subplot(4,1,1)
pl.plot(temperature_0)
pl.title('True source')
pl.subplot(4,1,2)
pl.plot(chanel_0)
pl.title('PCA 1')

pl.subplot(4,1,3)
pl.plot(chanel_1)
pl.title('PCA 2')

pl.subplot(4,1,4)
pl.plot(chanel_2)
pl.title('PCA 3')

pl.show()
