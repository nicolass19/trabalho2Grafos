//
// Created by Marcos Paulo on 06/06/2022.
//

#include <iostream>
#include <sstream>
#include "../Headers/Graph.h"
#include "../Headers/Menu.h"

Menu::Menu(Graph *graph, const string& outputPath){
    this->outputFile.open(outputPath);
    this->graph = graph;
    this->graph->indexId();
}

void Menu::start(){
    bool hold = true;

    while(hold){
        hold = this->UI();
    }

    this->outputFile.close();
}

bool Menu::UI(){
    int option;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Fecho transitivo direto" << endl;
    cout << "[2] Fecho transitivo indireto" << endl;
    cout << "[3] Coeficiente de agrupamento local" << endl;
    cout << "[4] Coeficiente de agrupamento medio" << endl;
    cout << "[5] Caminho minimo Moore-Dijkstra" << endl;
    cout << "[6] Caminho minimo Floyd-Warshall" << endl;
    cout << "[7] Arvore Geradora Minima de Prim" << endl;
    cout << "[8] Arvore Geradora Minima de Kruskal" << endl;
    cout << "[9] Imprimir caminhamento em profundidade" << endl;
    cout << "[0] Sair" << endl;
    cout << "Digite uma opcao: ";

    cin >> option;

    if(option == 0)
        return false;

    selectMenu(option);

    return true;
}

void Menu::selectMenu(int option) {

    switch (option) {
        case 1: {
            fechoTransitivoDireto();
            break;
        }
        case 2: {
            fechoTransitivoIndireto();
            break;
        }
        case 3: {
            coefDeClusterLocal();
            break;
        }
        case 4: {
            coefDeClusterGlobal();
            break;
        }

        case 5: {
            dijkstraMenu();
            break;
        }

        case 6: {
            floydMenu();
            break;
        }

        case 7: {
            primMenu();
            break;
        }

        case 8: {
            kruskalMenu();
            break;
        }
        case 9:{
            caminhamentoProfundidade();
            break;
        }

        case 0: {
            break;
        }

        default: {
            cout << "Erro!" << endl <<"Opcao invalida." << endl;
        }

    }
}

void Menu::fechoTransitivoDireto() {

    string id;
    cout << "No id: " << endl;
    cin >> id;
    auto no = graph->getNodeFromExternalId(id);
    if(no == nullptr){
        cout << "No nao encontrado" << endl;
        return;
    }
    auto result = graph->FTD(no);

    this->printFTD(this->graph, result);
}

void Menu::fechoTransitivoIndireto(){

    string id;
    cout << "Digite o id do vertice: ";
    cin >> id;
    auto node = this->graph->getNodeFromExternalId(id);
    auto result = this->graph->FTI(node->getId());

    this->printFTI(this->graph, result);
}

void Menu::coefDeClusterLocal(){

    string id;
    cout << "Digite o id do vertice: ";
    cin >> id;
    Node* node = this->graph->getNodeFromExternalId(id);
    float cda = this->graph->CDA(node->getId());

    this->outputFile << "Coeficiente de agrupamento local do no " << id << ": "<< cda << endl;
    cout << "No "<< id << " - CDA:" << cda << endl;
}

void Menu::coefDeClusterGlobal(){

    float cdag = this->graph->CDAG();

    this->outputFile << "Coeficiente de agrupamento global: " << cdag << endl;
    cout << "CDAG: " << cdag << endl;
}

void Menu::dijkstraMenu() {
    string sourceIdExternal, targetIdExternal;

    if (!graph->getWeightedEdge()) {
        cout << "ERRO: Grafo nao ponderado" << endl;
        return;
    }

    cout << "Source node id: " << endl;
    cin >> sourceIdExternal;

    cout << "Target node id: " << endl;
    cin >> targetIdExternal;

    auto result = graph->dijkstra(sourceIdExternal, targetIdExternal);

    this->printDijkstra(this->graph, result);
}

void Menu::floydMenu() {
    string sourceIdExternal, targetIdExternal;

    if (!graph->getWeightedEdge()) {
        cout << "ERRO: Grafo nao ponderado" << endl;
        return;
    }
    cout << "Source node id: " << endl;
    cin >> sourceIdExternal;

    cout << "Target node id: " << endl;
    cin >> targetIdExternal;

    auto result = graph->floydWarshall(sourceIdExternal, targetIdExternal);
    this->printFloyd(this->graph, result);

}

void Menu::primMenu() {

    if (this->graph->getDirected()) {
        cout << "ERRO: grafo direcionado" << endl;
        return;
    }
    if (!this->graph->getWeightedEdge()){
        cout << "ERRO: grafo nao ponderado" << endl;
        return;
    }

    string input;
    cout << "Digite os vertices separados por virgula: " << endl;
    cin >> input;

    auto listNodeId = this->splitString(input, ',');
    auto result = this->graph->agmPrim(&listNodeId);
    printPrim(&result);
}
void Menu::kruskalMenu(){

    if (this->graph->getDirected()) {
        cout << "ERRO: grafo direcionado" << endl;
        return;
    }
    if (!this->graph->getWeightedEdge()){
        cout << "ERRO: grafo nao ponderado" << endl;
        return;
    }

    string input;
    cout << "Digite os vertices separados por virgula: " << endl;
    cin >> input;

    auto listNodeId = this->splitString(input, ',');
    auto result = this->graph->kruskal(&listNodeId);
    printKruskal(result);

}

vector<string> Menu::splitString(const string& str, char delimiter) { ///Auxiliar do Prim e Kruskal

    vector<string> internal;
    stringstream ss(str);
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}
void Menu::caminhamentoProfundidade(){

    string id;
    cout << "Digite o id do vertice: ";
    cin >> id;
    Node* node = this->graph->getNodeFromExternalId(id);
    if(this->graph->searchNode(node->getId())) {
        int ordem = this->graph->getOrder();

        int *visited = new int[ordem]; //vetor para os ranks dos nos
        for (int i = 0; i < ordem; i++) {
            visited[i] = -1;
        }

        auto result = this->graph->caminhoProfundidade(id, visited);

        printCP(this->graph, result, visited);
    }
    else{
        cout << "No invalido" << endl;
    }
}

