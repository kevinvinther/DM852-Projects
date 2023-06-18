#include "../src/graph/adjacency_list.hpp"
#include "../src/graph/depth_first_search.hpp"
#include "../src/graph/topological_sort.hpp"
#include <iostream>
#include <cassert>

using vertex = graph::AdjacencyList<graph::tags::Directed>::VertexDescriptor;
using edge = graph::AdjacencyList<graph::tags::Directed>::EdgeDescriptor;


int test_directed_graph_creation() {
    graph::AdjacencyList<graph::tags::Directed> g(0);

    return 0;
}

int test_bidirectional_graph_creation() {
    graph::AdjacencyList<graph::tags::Bidirectional> g(0);

    return 0;
}

int test_undirected_graph_creation() {
    graph::AdjacencyList<graph::tags::Undirected> g(0);

    return 0;
}

int test_topo_sort () {
    graph::AdjacencyList<graph::tags::Directed> g(0);

    vertex v1 = addVertex(g);
    vertex v2 = addVertex(g);
    vertex v3 = addVertex(g);
    vertex v4 = addVertex(g);
    vertex v5 = addVertex(g);
    vertex v6 = addVertex(g);

    addEdge(v6, v3, g);
    addEdge(v6, v1, g);
    addEdge(v5, v1, g);
    addEdge(v5, v2, g);
    addEdge(v3, v4, g);
    addEdge(v4, v2, g);

    std::vector<vertex> vertices(numVertices(g));
    topoSort(g, vertices.begin());

    return 0;
}

int test_getIndex() {
    graph::AdjacencyList<graph::tags::Directed> g(0);

    vertex v1 = addVertex(g);
    vertex v2 = addVertex(g);
    vertex v3 = addVertex(g);
    vertex v4 = addVertex(g);
    vertex v5 = addVertex(g);
    vertex v6 = addVertex(g);

    addEdge(v6, v3, g);
    addEdge(v6, v1, g);
    addEdge(v5, v1, g);
    addEdge(v5, v2, g);
    addEdge(v3, v4, g);
    addEdge(v4, v2, g);

    assert(getIndex(v1, g) == 0);
    assert(getIndex(v2, g) == 1);
    assert(getIndex(v3, g) == 2);
    assert(getIndex(v4, g) == 3);
    assert(getIndex(v5, g) == 4);
    assert(getIndex(v6, g) == 5);

    return 0;
}

int main() {
    test_directed_graph_creation();
    test_bidirectional_graph_creation();
    test_undirected_graph_creation();
    test_topo_sort();
    test_getIndex();

    return 0;
}