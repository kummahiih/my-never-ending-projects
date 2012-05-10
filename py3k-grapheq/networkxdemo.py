import grapheq

import networkx as nx
import matplotlib.pyplot as plt

def graph_maker(G):
    def connect(a, b):
        print (a, b)
        G.add_edges_from([(a, b)])
    return grapheq.from_operator(connect)



if __name__ == "__main__":
    G = nx.DiGraph()
    I, O = grapheq.get_I_and_O()
    C = graph_maker(G)
    network = C(1, 2) * C(3, 4, I) * C(5, 6) * C(7)
    network.evaluate()

    nx.draw(G)
    plt.show()
    input('>')
    
