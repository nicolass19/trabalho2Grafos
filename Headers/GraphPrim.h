//
// Created by Marcos Paulo on 15/06/2022.
//

#ifndef TESTEGRAFOS_GRAPHPRIM_H
#define TESTEGRAFOS_GRAPHPRIM_H
#include<bits/stdc++.h>
#include "Graph.h"

using namespace std;

typedef pair<int, int> Pair;

class GraphPrim{

    Graph* graphSource;
    int V;
    list< pair<int, int> > *adj;

public:
    GraphPrim(Graph *graph);
    ~GraphPrim();

    void addEdge(int u, int v, int w);

    vector<EdgeStruct> primMST();
};
#endif //TESTEGRAFOS_GRAPHPRIM_H
