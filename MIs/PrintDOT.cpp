//
// Created by Marcos Paulo on 09/06/2022.
//
#include <iostream>
#include <iomanip>
#include "../Headers/Menu.h"

///AUXILIARES
string Menu::getEdgeShape(bool isDirected) {
    if (isDirected)
        return " -> ";
    else
        return " -- ";
}

string Menu::getGraphType(bool directed, bool isStrict){
    string graphType = directed ? this->graphDotDirected : this->graphDot;
    string strictName = isStrict ? this->strict : "";
    return strictName + " " + graphType;
}

string Menu::getWeightLabel(bool isWeighted, float weight) {
    stringstream stream;
    stream << fixed << setprecision(2) << weight;
    string weightStr = isWeighted ? " [label=" + stream.str() + "]" : "";
    return weightStr;
}

void Menu::printEdgeFromGraph(Graph* graphSource) {

    string edgeShape = this->getEdgeShape(graphSource->getDirected());
    bool isWeighted = graphSource->getWeightedEdge();

    for(auto node = graphSource->getFirstNode(); node != nullptr; node = node->getNextNode()) {
        for(auto edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()) {
            string weightStr = getWeightLabel(isWeighted, edge->getWeight());
            this->outputFile << "   " << node->getIdExternal() << edgeShape << graphSource->getExternalId(edge->getTargetId()) << weightStr << ";" << endl;
        }
    }
}

void Menu::printGraph() {
    printEdgeFromGraph(this->graph);
}

/// ----------------------------------

/// FTD
void Menu::printSubgraph(const vector<EdgeStruct>& subgraph, const string& subgraphName, int clusterNumber, bool isDirected) {

    string labelName = "label=\""+subgraphName+"\";";
    auto edgeShape = this->getEdgeShape(isDirected);
    auto clusterName = "cluster_" + to_string(clusterNumber);

    this->outputFile << "   " << "subgraph "<< clusterName <<" {" << endl;
    this->outputFile << "      " << labelName << endl;
    for (auto & i : subgraph) {
        string weightLabel = getWeightLabel(isDirected, i.weight);
        this->outputFile << "      " << i.fromNode << edgeShape << i.toNode << weightLabel <<";" << endl;
    }
    this->outputFile << "   " << "}" << endl;
}

void Menu::printFTD(Graph* grafo, const vector<EdgeStruct>& subgrafo) {

    string graphType = this->getGraphType(grafo->getDirected(), true);

    this->outputFile << graphType << " fechoTransitivoDireto {" << endl;

    printSubgraph(subgrafo, "FechoTransitivoDireto", 0, grafo->getDirected());

    this->outputFile << "}" << endl;
}

/// FTI
void Menu::printSubgraphFromSequence(const vector<string>& subgraph, const string& subgraphName, int clusterNumber) {

    string labelName = "label=\""+subgraphName+"\";";
    auto clusterName = "cluster_" + to_string(clusterNumber);
    int cont = 0;

    this->outputFile << "   " << "subgraph "<< clusterName <<" {" << endl;
    this->outputFile << "      " << labelName << endl << "      ";

    for (const auto & i : subgraph) {
        this->outputFile << i << "; ";
        cont++;
        if(cont == 30) {
            this->outputFile << endl << "      ";
            cont = 0;
        }
    }
    this->outputFile << endl << "   " << "}" << endl;
}

void Menu::printFTI(Graph* graphSource, const vector<string>& sequence){ // usar osage

    string graphType = this->getGraphType(graphSource->getDirected(), true);

    this->outputFile << graphType << " fechoTransitivoIndireto {" << endl;

    printSubgraphFromSequence(sequence, "FechoTransitivoIndireto", 1);

    this->outputFile << "}" << endl;
}
///Dijkstra
void Menu::printMinimalPath(Graph* graphSource, const vector<string>& minimalPath, string type){
    string graphType = this->getGraphType(graphSource->getDirected(), true);
    string edgeShape = this->getEdgeShape(graphSource->getDirected());
    string name = " caminhoMinimo" + type;
    int size = minimalPath.size();

    this->outputFile << graphType << name <<" {" << endl;

    string labelName = "label=\"CaminhoMinimo"+type+"\";";
    auto clusterName = "cluster_0";

    this->outputFile << "   " << "subgraph "<< clusterName <<" {" << endl;
    this->outputFile << "      " << labelName << endl << "      ";

    for (int i = 0; i < size-2; i++) {
        this->outputFile << minimalPath[i] << edgeShape;
    }
    if (size-2 >= 0){
        this->outputFile << minimalPath[size-2] << ";" << endl;
    }
    this->outputFile << "   " << "}" << endl;

    this->outputFile << "   " << "subgraph "<< "cluster_1" <<" {" << endl;
    this->outputFile << "      " << "label=\"Custo:\";" << endl;
    this->outputFile << "      " << minimalPath[minimalPath.size()-1] << ";" << endl;
    this->outputFile << "   " << "}" << endl;

    this->outputFile << "}" << endl;
}
void Menu::printDijkstra(Graph* graphSource, const vector<string>& minimalPath){
    this->printMinimalPath(graphSource, minimalPath, "Dijkstra");
}

///Floyd
void Menu::printFloyd(Graph* graphSource, const vector<string>& minimalPath){
    this->printMinimalPath(graphSource, minimalPath, "Floyd");
}

/// Kruskal
void Menu::printKruskal(const vector<EdgeStruct>& kruskalEdges){
    string graphType = this->getGraphType(false, true);
    string edgeShape = this->getEdgeShape(false);

    this->outputFile << graphType << " kruskal {" << endl;

    for (const auto& i: kruskalEdges) {
        this->outputFile << "    " << i.fromNode << edgeShape << i.toNode << this->getWeightLabel(true, i.weight) << endl;
    }

    this->outputFile << "}" << endl;
}

/// Prim
void Menu::printPrim(vector<EdgeStruct>* primEdges){
    string graphType = this->getGraphType(false, true);
    string edgeShape = this->getEdgeShape(false);

    this->outputFile << graphType << " prim {" << endl;

    for (const auto& i: *primEdges) {
        this->outputFile << "    " << i.fromNode << edgeShape << i.toNode << this->getWeightLabel(true, i.weight) << endl;
    }

    this->outputFile << "}" << endl;
}

///Caminhamento em Profundidade
void Menu::printCP(Graph* grafo, const vector<EdgeStruct>& subgrafo, int* visited) {

    string graphType = this->getGraphType(grafo->getDirected(), true);
    this->outputFile << graphType << " caminhamentoEmProfundidade {" << endl;

    int clusterNumber = 0;
    string labelName = "label=\"CaminhamentoEmProfundidade\";";
    auto edgeShape = this->getEdgeShape(grafo->getDirected());
    auto clusterName = "cluster_" + to_string(clusterNumber);

    this->outputFile << "   " << "subgraph "<< clusterName <<" {" << endl;
    this->outputFile << "      " << labelName << endl;
    for (auto &i : subgrafo) {
        string weightLabel = getWeightLabel(grafo->getDirected(), i.weight);
        Node* node = grafo->getNodeFromExternalId(i.fromNode);
        Node* targetNode = grafo->getNodeFromExternalId(i.toNode);
        if(visited[node->getId()] - visited[targetNode->getId()] >= 2)
            this->outputFile << "      " << i.fromNode << edgeShape << i.toNode << " " << weightLabel << edgeRedAttribute <<";" << endl;
        else
            this->outputFile << "      " << i.fromNode << edgeShape << i.toNode << " " << weightLabel <<";" << endl;
    }
    this->outputFile << "   " << "}" << endl;

    this->outputFile << "}" << endl;
}