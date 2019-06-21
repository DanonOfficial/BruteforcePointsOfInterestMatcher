import matplotlib.pyplot as plt
import networkx as nx

G = nx.read_edgelist('resultList.txt', nodetype=int,
  data=(('weight',int),), )

pos = nx.kamada_kawai_layout(G)
nx.draw(G, pos, with_labels=True, font_weight='bold')


plt.savefig("Graph.png", format="PNG")
plt.show()
