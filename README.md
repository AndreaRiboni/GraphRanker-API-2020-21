# GraphRanker-API-2020-21

**DSA Project, Politecnico di Milano A.Y. 2020/21**

The goal of this project is to efficiently implement a graph classifier, optimizing both time and space complexity. The system maintains a ranking of directed, weighted graphs, keeping track of the top-k graphs according to a specific metric.

Interaction with the program is done via **STDIN**, supporting two main commands:
- `AggiungiGrafo` (AddGraph): adds a new graph to the ranking
- `TopK`: displays the top-k graphs

The number of graphs and the value of k are always provided through **STDIN**.

The ranking metric is defined as the sum of the shortest paths from node 0 to all other nodes that are reachable from node 0.

**Final grade for this project: 30L (cum laude)**
