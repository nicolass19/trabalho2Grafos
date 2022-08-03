//
// Created by Marcos Paulo on 04/06/2022.
//

#ifndef TESTEGRAFOS_GRAPH_H
#define TESTEGRAFOS_GRAPH_H

#include <vector>
#include "Node.h"
#include "Edge.h"
#include "Cluster.h"

struct NodeStruct{
    int id;
    string external_id; //necessario?
    float weight;
    float sumEdgesWeights;
    int numEdges;
    float value;

    NodeStruct(int id, string external_id, float weight, float sumEdgesWeights, int numEdges){
        this->id = id;
        this->external_id = external_id;
        this->weight = weight;
        this->sumEdgesWeights = sumEdgesWeights;
        this->numEdges = numEdges;
        this->value = sumEdgesWeights + weight - numEdges;
    }
};

struct MinimalPathResult {
    vector<int>* path;
    long distance;
    bool founded;

    MinimalPathResult(vector<int>* path, long distance, bool founded) {
        this->path = path;
        this->distance = distance;
        this->founded = founded;
    }
    MinimalPathResult(bool founded) {
        this->founded = founded;
    }
};

struct EdgeStruct{
    string fromNode;
    string toNode;
    float weight;

    EdgeStruct(string fromNode, string toNode, float weight) {
        this->fromNode = fromNode;
        this->toNode = toNode;
        this->weight = weight;
    }

    EdgeStruct(){
        weight = 1;
    };
};

class Graph{

    //Atributos
private:
    int insertion_position;
    int order;
    unsigned int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node;
    Node* last_node;
    string* mapInternalIdToExternalId;

public:

    //Construtor
    Graph(bool directed, bool weighted_edge, bool weighted_node = false);

    //Destrutor
    ~Graph();

    //Métodos
    bool getWeightedEdge() const;
    bool getDirected() const;
    int getOrder() const;
    int getInsertionPosition();
    unsigned int getNumberEdges() const;
    bool getWeightedNode() const;
    Node *getFirstNode();
    Node* getNodeFromExternalId(const string& id_external);
    Node* getLastNode();
    string getExternalId(int id);


    void indexId();
    void insertEdge(const string& id_external, const string& target_id_external, float weight);
    void removeNode(const string& id_external);
    bool searchNode(int id);
    bool searchNodeFromExternalId(const string& id_external);
    Node* getNode(int id);

    ///FTD
    vector<EdgeStruct> FTD(Node* no);

    ///FTI
    vector<string> FTI(int id);

    ///CDA
    float CDA(int id);

    ///CDA Global
    float CDAG();

    ///Dijkstra
    vector<string> dijkstra(const string& source_id_external, const string& target_id_external);

    ///Floyd
    vector<string> floydWarshall(const string& source_id_external, const string& target_id_external);

    ///Prim
    vector<EdgeStruct> agmPrim(vector<string> *listNodeId);

    ///Kruskal
    vector<EdgeStruct> kruskal(vector<string> *listNode);

    ///Caminhamento em profundidade
    vector<EdgeStruct> caminhoProfundidade(const string& id, int* visited);

    ///CCP
    vector<NodeStruct> orderVertices(Graph *graph);
    Cluster* agmGuloso(const int& numClusters);

private:

    void removeFirstNode(int id);
    void removeLastNode(int id);

    Node* insertNode(string id_external);
    void insertEdge(Node* source_node, Node* target_node, float weight);

    ///FTD
    void FTDRec(Node *node, bool *visited, vector<EdgeStruct>* edges);

    ///FTI
    void FTIRec(int id, Node *node, bool* visited, bool* reach);

    ///Dijkstra
    vector<int> dijkstraDist(int source_id, vector<int> &path);
    MinimalPathResult dijkstra(int idSource, int idTarget);
    bool getPathDijkstra(vector<int> &dijkstraPath, vector<int> *pathToTarget, int target, int source);

    ///Floyd
    MinimalPathResult floydWarshall(int idSource, int idTarget);

    ///Caminhamento em profundidade
    void caminhoProfundidadeRec(Node *node, int* visited, vector<EdgeStruct>* edges, int rank);


    //Other methods
    //Node* getLastNode();
    //Node * insertNode(string id_external);

};
#endif //TESTEGRAFOS_GRAPH_H
