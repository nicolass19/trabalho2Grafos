//
// Created by Marcos Paulo on 06/06/2022.
//

#ifndef TESTEGRAFOS_MENU_H
#define TESTEGRAFOS_MENU_H


#include <fstream>
#include "Graph.h"

using namespace std;

class Menu {
public:
    Menu(Graph *graph, const string& outputPath);

    bool UI();
    void start();
    void printGraph();
    void printEdgeFromGraph(Graph *graphSource);

private:

    ofstream outputFile;
    Graph* graph;

    ///Auxiliares
    string graphDot = "graph";
    string graphDotDirected = "digraph";
    string strict = "strict";
    string edgeRedAttribute = "[color=red,penwidth=3.0]";

    ///Menu
    void selectMenu(int option);

    ///Auxiliares
    static string getWeightLabel(bool isWeighted, float weight) ;
    static string getEdgeShape(bool isDirected);
    string getGraphType(bool directed, bool isStrict);
    //void printEdgeFromGraph(Graph *graphSource);

    ///PrintDOT's

    ///FTD
    void fechoTransitivoDireto();
    void printFTD(Graph *grafo, const vector<EdgeStruct> &subgrafo);
    void printSubgraph(const vector<EdgeStruct> &subgraph, const string &subgraphName, int clusterNumber,
                       bool isDirected);

    ///FTI
    void fechoTransitivoIndireto();
    void printFTI(Graph *graphSource, const vector<string> &sequence);
    void printSubgraphFromSequence(const vector<string>& subgraph, const string& subgraphName, int clusterNumber);

    ///Coeficiente de Agrupamento
    void coefDeClusterLocal();
    void coefDeClusterGlobal();

    ///Prim
    void primMenu();
    void printPrim(vector<EdgeStruct>* primEdges);

    ///Kruskal
    void kruskalMenu();
    vector<string> splitString(const string& str, char );
    void printKruskal(const vector<EdgeStruct>& kruskalEdges);

    ///Caminhamento em Profundidade
    void caminhamentoProfundidade();
    void printCP(Graph *grafo, const vector<EdgeStruct> &subgrafo, int* visited);

    ///Dijkstra
    void dijkstraMenu();
    void printDijkstra(Graph *graphSource, const vector<string> &minimalPath);
    void printMinimalPath(Graph *graphSource, const vector<string> &minimalPath, string type);

    ///Floyd
    void floydMenu();
    void printFloyd(Graph *graphSource, const vector<string> &minimalPath);

};



#endif //TESTEGRAFOS_MENU_H
