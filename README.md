# Kruskal's Algorithm
Kruskal's algorithm to find the minimum cost spanning tree uses the greedy approach. This algorithm treats the graph as a forest and every node it has as an individual tree. A tree connects to another only and only if, it has the least cost among all available options and does not violate Minimum Spanning Tree properties.

Kruskal's Algorithm makes use of the Greedy Algorithm strategy.

Working of Kruskal's Algorithm:
1. Let G = { V, E } and T = { A, B }
2. A = V and B = {}, |A| = n and |B| = 0
3. while (|B| <= n-1) do
          begin
                  find edge <u,v> of minimum length and add to B only if addition of edge <u,v> does not complete a cycle in T.
          end
4. stop
