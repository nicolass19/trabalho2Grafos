//
// Created by Marcos Paulo on 31/07/2022.
//

#include "../Headers/Graph.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "../Headers/Cluster.h"
#include "../Headers/ClusterNode.h"

using namespace std;

bool compareNodes(const ClusterNode& node1, const ClusterNode& node2){
    return node1.getValue() > node2.getValue();
}
bool allVisited(const bool* visited, int size){
    for (int i = 0; i < size; ++i) {
        if (!visited[i])
            return false;
    }
    return true;
}

vector<ClusterNode> Graph::orderVertices() {
    vector<ClusterNode> vertices;

    for (Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        float sum = 0;
        int numEdges = 0;
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            sum += edge->getWeight();
            numEdges++;
        }
        vertices.emplace_back(ClusterNode(node->getId(),node->getIdExternal(), node->getWeight(), sum, numEdges));

        //cout << "No:" << node->getIdExternal() << " peso: " << node->getWeight() << endl;
    }

    sort(vertices.begin(), vertices.end(), compareNodes);

    cout << "Vetor ordenado: ";
    for (auto & vertice : vertices){
        cout << vertice.getExternalId() << " ";
    }
    cout << endl;

    return vertices;
}

Cluster* Graph::agmGuloso() {

    int order = this->getOrder();
    if (numClusters > order) {
        cout << "ERRO: numero de clusters maior que o numero de vertices" << endl;
        abort();
    }

    auto vertices = orderVertices();
    bool visited[order];
    float *lowerLimits = this->getLowerLimits();
    float *higherLimits = this->getHigherLimits();
    Cluster *clusters = new Cluster[numClusters];

    for (int i = 0; i < numClusters; i++){
        clusters[i].setLowerLimit(lowerLimits[i]);
        clusters[i].setHigherLimit(higherLimits[i]);
        cout << "Limites: " << endl;
        cout << clusters[i].getLowerLimit() << " " << clusters[i].getHigherLimit() << endl;
    }


    for (int i = 0; i < order; i++) {
        Node *n = this->getNode(vertices.at(i).getId());
        int bestCluster = 0;
        float auxValue = 0;
        float value = 0;
        float sum;
        int j;

        for (j = 0; j < numClusters; j++) {

            if (clusters[j].getPrimeiro() == nullptr) {
                float v = clusters[j].getHigherLimit() - n->getWeight();
                cout << "V[" << j << "]: " << clusters[j].getHigherLimit() << "-" << n->getWeight();
                cout << "=" << v << endl;
                if (v > auxValue) {
                    value = 0;
                    bestCluster = j;
                    auxValue = v;
                    cout << "Valor: " << value << endl;
                }
            } else {
                sum = 0;
                for (ClusterNode *c = clusters[j].getPrimeiro(); c != nullptr; c = c->getProx()) {
                    Edge *edge = n->hasEdgeBetween(c->getId());
                    if (edge != nullptr &&
                        (clusters[j].getActualLimit() + n->getWeight() <= clusters[j].getHigherLimit())) {
                        //cout << "NO:" << n->getIdExternal() << "ENTROU!!" << endl;
                        //cout << clusters[j].getActualLimit() << "+" << n->getWeight() << "<=" << clusters[j].getHigherLimit() << endl;
                        sum += edge->getWeight();
                    }
                }
                if (sum * (clusters[j].getHigherLimit() - clusters[j].getActualLimit()) > value) {
                    value = sum;
                    bestCluster = j;
                }
            }
        }
        cout << "Value=" << value << " ";
        cout << "Inserindo ";
        cout << "NO: " << n->getIdExternal() << "- Peso: " << n->getWeight();
        cout << " no cluster " << bestCluster << " Soma parcial: ";
        clusters[bestCluster].insereFinal(n->getId(), n->getWeight(), value);
        cout << clusters[bestCluster].getSumEdges() << endl;
    }
    float quality = 0;
    cout << "Soma final de cada cluster: " << endl;
    for (int i = 0; i < numClusters; ++i) {
        cout << "Cluster[" << i << "] = " << clusters[i].getSumEdges() << endl;
        quality += clusters[i].getSumEdges();
    }
    cout << "QUALIDADE: " << quality << endl;


    return clusters;
}