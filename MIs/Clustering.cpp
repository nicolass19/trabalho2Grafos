//
// Created by Marcos Paulo on 31/07/2022.
//

#include "../Headers/Graph.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "../Headers/Cluster.h"

using namespace std;

bool compareNodes(const NodeStruct& node1, const NodeStruct& node2){
    return node1.value > node2.value;
}
bool allVisited(const bool* visited, int size){
    for (int i = 0; i < size; ++i) {
        if (!visited[i])
            return false;
    }
    return true;
}

vector<NodeStruct> Graph::orderVertices(Graph *graph) {
    vector<NodeStruct> vertices;

    for (Node* node = graph->getFirstNode(); node != nullptr; node = node->getNextNode()){
        float sum = 0;
        int numEdges = 0;
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            sum += edge->getWeight();
            numEdges++;
        }
        if(node->getIdExternal() == "3"){
            vertices.emplace_back(NodeStruct(node->getId(),node->getIdExternal(), 3, sum, numEdges));
        }
        else if(node->getIdExternal() == "2"){
            vertices.emplace_back(NodeStruct(node->getId(), node->getIdExternal(), 2, sum, numEdges));
        }
        else{
            vertices.emplace_back(NodeStruct(node->getId(), node->getIdExternal(), 1, sum, numEdges));
        }
        cout << "No:" << node->getIdExternal() << " sum: " << sum << endl;
    }

    sort(vertices.begin(), vertices.end(), compareNodes);

    cout << "Vetor ordenado: ";
    for (auto & vertice : vertices){
        cout << vertice.external_id << " ";
    }
    cout << endl;

    return vertices;
}

Cluster* Graph::agmGuloso(const int& numClusters){

    int order = this->getOrder();
    if (numClusters > order) {
        cout << "ERRO: numero de clusters maior que o numero de vertices" << endl;
    }

    auto vertices = orderVertices(this);
    int size = (int) vertices.size();
    bool visited[size];
    bool finish = false;
    Cluster* clusters = new Cluster[numClusters];

    for (int i = 0; i < size; ++i) {
        NodeStruct n = vertices.at(i);

        int melhorCluster = 0;
        for (int j = 0; j < numClusters; ++j) {

        }
    }


    return clusters;
}