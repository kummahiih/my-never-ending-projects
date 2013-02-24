"""
@copyright: 2013 by Pauli Rikula <pauli.rikula@gmail.com>
@license: MIT <http://www.opensource.org/licenses/mit-license.php>
"""


from parse_data import get_data

import numpy as np
import pylab as pl

from sklearn import cluster #, datasets
#from sklearn.metrics import euclidean_distances
#from sklearn.neighbors import kneighbors_graph
from sklearn.preprocessing import Scaler

from sklearn.decomposition import PCA

data = get_data()[:5000]

temperature_0 = [ r[1] for r in data[:-50]]
temperature_1 = [ r[1] for r in data[50:]]


X = np.c_[temperature_0,temperature_1]

#X = Scaler().fit_transform(X)

pca = PCA(n_components=3)
X = pca.fit(X).transform(X)


#spectral = cluster.SpectralClustering(n_clusters=2, mode='arpack',
#            affinity="nearest_neighbors")
#two_means = cluster.MiniBatchKMeans(n_clusters=4)
#ward_five = cluster.Ward(n_clusters=2, connectivity=connectivity)
dbscan = cluster.DBSCAN(eps=.2)

algorithm = dbscan

algorithm.fit(X)

if hasattr(algorithm, 'labels_'):
    y_pred = algorithm.labels_.astype(np.int)
else:
    y_pred = algorithm.predict(X)


for i in range(4):
    pl.plot([ v if y_pred[ind] == i else 0 for ind,v in enumerate(X[:,0])] )


pl.show()
