//
// Created by Marcos Paulo on 13/06/2022.
//
#include <algorithm>
#include "../Headers/GraphKruskal.h"

GraphKruskal::GraphKruskal(int V, int E) {
    this->V = V;
    this->number_edges = E;
    this->edges = new vector<EdgeKruskal>();
}

GraphKruskal::GraphKruskal(Graph* graph) {
    this->graphSource = graph;
    this->V = graphSource->getOrder();
    this->number_edges = 0;

    this->edges = new vector<EdgeKruskal>();

    for(auto node = graphSource->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        for (auto next_edge = node->getFirstEdge(); next_edge != nullptr ; next_edge = next_edge->getNextEdge())
        {
            this->addEdge(node->getId(), next_edge->getTargetId(), next_edge->getWeight());
        }
    }
}

GraphKruskal::~GraphKruskal() {
    delete edges;
}

bool compareEdge(EdgeKruskal e1, EdgeKruskal e2)
{
    return (e1.weight < e2.weight);
}

vector<EdgeKruskal> GraphKruskal::kruskalMST() {
    int V = this->V;
    vector<EdgeKruskal> result;

    //Variaveis de apoio
    int e = 0;
    int i = 0;

    // Etapa 1: ordena todas as arestas em ordem crescente de peso
    sort(edges->begin(), edges->end(), compareEdge);

    auto* subsets = new Subset[V * sizeof(Subset)];

    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < this->number_edges)
    {
        // Etapa 2: escolha a menor aresta.
        EdgeKruskal next_edge = this->edges->at(i++);

        int x = find(subsets, next_edge.source);
        int y = find(subsets, next_edge.target);

        //Se a aresta não forma ciclo, adicione-a ao conjunto de arestas resultantes.
        if (x != y)
        {
            result.push_back(next_edge);
            joinSet(subsets, x, y);
        }
    }

    return result;
}

int GraphKruskal::find(Subset subsets[], int i) //encontra o primeiro nó da arvore (root)
{
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}


void GraphKruskal::joinSet(Subset subsets[], int x, int y)
{
    int xroot = find(subsets, x); ///x
    int yroot = find(subsets, y); ///y

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void GraphKruskal::addEdge(int source, int target, float weight) {
    this->edges->push_back(EdgeKruskal(source, target, weight));
    this->number_edges++;
}

EdgeKruskal::EdgeKruskal(int source, int target, float weight){
    this->source = source;
    this->target = target;
    this->weight = weight;
}