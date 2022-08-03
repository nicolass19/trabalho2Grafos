//
// Created by Marcos Paulo on 13/06/2022.
//

#ifndef TESTEGRAFOS_GRAPHKRUSKAL_H
#define TESTEGRAFOS_GRAPHKRUSKAL_H

#include <vector>
#include "Graph.h"

using namespace std;


struct Subset {
    int parent;
    int rank;
};

class EdgeKruskal
{
public:
    EdgeKruskal(int source, int target, float weight);

    int source, target;
    float weight;
};

class GraphKruskal{

public:

    GraphKruskal(int V, int E);
    GraphKruskal(Graph *graph);

    ~GraphKruskal();

    vector<EdgeKruskal> kruskalMST();
    void addEdge(int source, int target, float weight);


private:
    int number_edges, V;
    vector<EdgeKruskal>* edges;
    Graph* graphSource;

    ///Métodos
    int find(Subset subsets[], int i);
    void joinSet(Subset subsets[], int x, int y);

};

#endif //TESTEGRAFOS_GRAPHKRUSKAL_H
