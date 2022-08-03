//
// Created by Marcos Paulo on 15/06/2022.
//
#include "../Headers/GraphPrim.h"

GraphPrim::GraphPrim(Graph* graph)
{
    graphSource = graph;
    graphSource->indexId();

    this->V = graphSource->getOrder();
    adj = new list<Pair> [this->V];

    for(auto node = graphSource->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        for (auto next_edge = node->getFirstEdge(); next_edge != nullptr ;next_edge = next_edge->getNextEdge())
        {
            this->addEdge(node->getId(), next_edge->getTargetId(), next_edge->getWeight());
        }
    }
}

GraphPrim::~GraphPrim()
{
    delete adj;
}

void GraphPrim::addEdge(int source, int target, int weight)
{
    adj[source].emplace_back(target, weight);
}


vector<EdgeStruct> GraphPrim::primMST()
{

    priority_queue <Pair, vector <Pair> , greater<Pair>> pq;

    int src = 0;

    vector<int> key(this->V, INT32_MAX);

    vector<int> parent(this->V, -1);

    vector<bool> inMST(this->V, false);


    pq.push(make_pair(0, src));
    key[src] = 0;

    while (!pq.empty())
    {

        int u = pq.top().second;
        pq.pop();

        if(inMST[u]){
            continue;
        }

        inMST[u] = true;

        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            int v = (*i).first;
            int weight = (*i).second;


            // Se V não estiver no MST e o peso da aresta (u,v) for menor que o atual key do v
            if (!inMST[v] && key[v] > weight)
            {
                // Atualiza o key de v
                key[v] = weight;
                pq.push(make_pair(key[v], v));
                parent[v] = u;
            }
        }
    }

    // Armazena o MST em um vetor de arestas
    vector<EdgeStruct> edges;
    for (int i = 1; i < this->V; ++i){
        edges.emplace_back(this->graphSource->getExternalId(parent[i]), this->graphSource->getExternalId(i), key[i]);
    }

    return edges;
}

