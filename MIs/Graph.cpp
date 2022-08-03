//
// Created by Marcos Paulo on 04/06/2022.
//
#include <iostream>
#include <utility>
#include "../Headers/Graph.h"
#include "../Headers/GraphKruskal.h"
#include "../Headers/GraphPrim.h"

using namespace std;

///Construtor
Graph::Graph(bool directed, bool weighted_edge, bool weighted_node)
{
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    this->insertion_position = 0;
    this->order = 0;
}

/// Destrutor
Graph::~Graph()
{
    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder() const
{
    return this->order;
}
unsigned int Graph::getNumberEdges() const
{
    return this->number_edges;
}
bool Graph::getDirected() const
{
    return this->directed;
}
bool Graph::getWeightedEdge() const
{
    return this->weighted_edge;
}
bool Graph::getWeightedNode() const
{
    return this->weighted_node;
}
Node *Graph::getFirstNode()
{
    return this->first_node;
}
Node *Graph::getLastNode()
{
    return this->last_node;
}

string Graph::getExternalId(int id) {
    return this->mapInternalIdToExternalId[id];
}

int Graph::getInsertionPosition() {
    auto aux = this->insertion_position;
    this->insertion_position++;
    return aux;
}

Node* Graph::insertNode(string id_external)
{
    auto node = new Node(this->getInsertionPosition(), move(id_external));

    if(this->getFirstNode() == nullptr)
    {
        this->first_node = this->last_node = node;
    }else{
        this->getLastNode()->setNextNode(node);
        this->last_node = node;
    }
    this->order++;
    return node;
}

void Graph::insertEdge(const string& id_external, const string& target_id_external, float weight) {
    auto source_node = this->getNodeFromExternalId(id_external);
    auto target_node = this->getNodeFromExternalId(target_id_external);
    if(source_node == nullptr)
    {
        source_node = this->insertNode(id_external);
        //if(this->weighted_node)
            //source_node->setWeight();
    }
    if(target_node == nullptr)
    {
        target_node = this->insertNode(target_id_external);
    }

    this->insertEdge(source_node, target_node, weight);
}

void Graph::insertEdge(Node* source_node, Node* target_node, float weight) {
    //cout << "FUNCIONOU" << endl;
    if (source_node == nullptr || target_node == nullptr) {
        cout << "Erro: no de origem e/ou de destino nulo(s)" << endl;
        abort();
    }
    if (!source_node->searchEdge(target_node->getId())){ ///evita multiarestas
        source_node->insertEdge(target_node->getId(), weight);
        this->number_edges++;
    }
    if (!this->directed)
    {
        if (!target_node->searchEdge(source_node->getId())) {
            target_node->insertEdge(source_node->getId(), weight);
        }
    }
}
void Graph::removeFirstNode(int id){
    auto node = this->getFirstNode();
    this->first_node = node->getNextNode();

    if(this->last_node == node)
    {
        this->last_node = this->first_node;
    }
    delete node;
}
void Graph::removeLastNode(int id){
    auto node = this->getFirstNode();

    if(node != last_node)
        while (node->getNextNode() != last_node)
            node->getNextNode();
    else{
        delete last_node;
        first_node = last_node = nullptr;
    }
    node->setNextNode(nullptr);
    delete last_node;
    last_node = node;
}
///------------------
void Graph::removeNode(const string& id_external){
    if (searchNodeFromExternalId(id_external)) {

        Node* targetNode = getNodeFromExternalId(id_external);
        int id = targetNode->getId();
        // Primeiro remove as arestas que apontam para o nó, para então excluí-lo
        for(Node* auxNode = getFirstNode(); auxNode != nullptr; auxNode = auxNode->getNextNode()){
            if(auxNode->searchEdge(id)){
                auxNode->removeEdge(getDirected(), targetNode);
                number_edges--;
            }
        }

        auto node = this->getFirstNode();
        if (first_node->getId() == id) {
            this->number_edges -= first_node->getOutDegree();
            removeFirstNode(id);
        } else if (last_node->getId() == id) {
            this->number_edges -= last_node->getOutDegree();
            removeLastNode(id);
        } else {
            while (node->getNextNode() != nullptr) {

                if (node->getNextNode()->getId() == id) {
                    auto tobeRemoved = node->getNextNode();
                    node->setNextNode(tobeRemoved->getNextNode());
                    this->number_edges -= tobeRemoved->getOutDegree();
                    delete tobeRemoved;
                }
                node = node->getNextNode();
            }
        }

        this->order--;
    }
    else {
        cout << "Id invalido" << endl;
    }
}


bool Graph::searchNode(int id)
{
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getId() == id)
        {
            return true;
        }
        node = node->getNextNode();
    }

    return false;
}


Node *Graph::getNode(int id)
{
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getId() == id)
        {
            return node;
        }
        node = node->getNextNode();
    }

    return node;
}

bool Graph::searchNodeFromExternalId(const string& id_external) {
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getIdExternal() == id_external)
        {
            return true;
        }
        node = node->getNextNode();
    }

    return false;
}

Node *Graph::getNodeFromExternalId(const string& id_external) {
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getIdExternal() == id_external)
        {
            return node;
        }
        node = node->getNextNode();
    }

    return node;
}

void Graph::indexId() {
    this->mapInternalIdToExternalId = new string[this->getOrder()];

    Node *node = this->first_node;
    while (node != nullptr)
    {
        this->mapInternalIdToExternalId[node->getId()] = node->getIdExternal();
        node = node->getNextNode();
    }
}
///--------------------------
/// Fecho Transitivo Direto
vector<EdgeStruct> Graph::FTD(Node* node){
    auto edges =  vector<EdgeStruct>();

    bool *visited = new bool[this->getOrder()];
    for(int i = 0; i < this->getOrder(); i++){
        visited[i] = false;
    }

    visited[node->getId()] = true;

    FTDRec(node, visited, &edges);

    return edges;
}

void Graph::FTDRec(Node *node, bool* visited, vector<EdgeStruct>* edges){

    for(Edge *auxEdge = node->getFirstEdge(); auxEdge != nullptr; auxEdge = auxEdge->getNextEdge())
    {
        auto targetId = auxEdge->getTargetId();
        if(!visited[targetId])
        {
            visited[targetId] = true;
            edges->emplace_back(
                    getExternalId(node->getId()),
                    getExternalId(auxEdge->getTargetId()),
                    auxEdge->getWeight());
            FTDRec(this->getNode(targetId), visited, edges);
        }
    }
}
/// --------------------------------
/// Fecho Transitivo Indireto
void Graph::FTIRec(int id, Node *node, bool* visited, bool* reach){

    if(!visited[node->getId()]){
        visited[node->getId()] = true;
        for(auto edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            if(edge->getTargetId() == id){
                reach[node->getId()] = true;
                break;
            }
            if(!visited[edge->getTargetId()]){
                FTIRec(id, this->getNode(edge->getTargetId()), visited, reach);
            }
            if(reach[edge->getTargetId()]){
                reach[node->getId()] = true;
                break;
            }
        }
    }

}
vector<string> Graph::FTI(int id){
    bool *visited = new bool[this->getOrder()];
    bool *reach = new bool[this->getOrder()];
    auto sequence = vector<string>();

    for(int i = 0; i < this->getOrder(); i++){
        visited[i] = false; reach[i] = false;
    }

    visited[id] = true; reach[id] = true;

    for(auto node = this->first_node; node != nullptr; node = node->getNextNode()){
        if(node->getId() != id){
            FTIRec(id, node, visited, reach);
        }
    }

    for(int i = 0; i < this->getOrder(); i++){
        if(reach[i]){
            sequence.push_back(mapInternalIdToExternalId[i]);
        }
    }

    return sequence;
}
/// -----------------------------------

/// Coeficiente de Agrupamento Local
float Graph::CDA(int id){
    Node* node = this->getNode(id);
    auto vizinhoString = vector<string>();
    bool *vizinhos = new bool[this->getOrder()];
    int somatorio = 0;
    int K = 0;

    for(int i = 0; i < this->getOrder(); i++){
        vizinhos[i] = false;
    }

    for(Edge* auxEdge = node->getFirstEdge(); auxEdge != nullptr; auxEdge = auxEdge->getNextEdge()){
        int targetId = auxEdge->getTargetId();
        vizinhos[targetId] = true;
        K++;
    }
    if(this->getDirected()) {
        for (Node *auxNode = this->getFirstNode(); auxNode != nullptr; auxNode = auxNode->getNextNode()) {
            if (auxNode->getId() != id) {
                for (Edge *auxEdge = auxNode->getFirstEdge(); auxEdge != nullptr; auxEdge = auxEdge->getNextEdge()) {
                    int targetId = auxEdge->getTargetId();
                    if (targetId == id && !vizinhos[auxNode->getId()]) {
                        vizinhos[auxNode->getId()] = true;
                        K++;
                        break;
                    }
                }
            }
        }
    }

    for (Node *auxNode = this->getFirstNode(); auxNode != nullptr; auxNode = auxNode->getNextNode()) {
        if (vizinhos[auxNode->getId()]) {
            int cont = 0;
            for (Edge *auxEdge = auxNode->getFirstEdge(); auxEdge != nullptr; auxEdge = auxEdge->getNextEdge()) {
                int targetId = auxEdge->getTargetId();
                if(vizinhos[targetId] && targetId != id) // nao conta o no passado por parametro
                    cont++;
            }
            somatorio += cont;
        }
    }

    if(K != 0 && K != 1) {
        float cda = (float) somatorio / (K * (K - 1));
        return cda;
    }
    else {
        return 0;
    }
}

/// Coeficiente de Agrupamento Global
float Graph::CDAG(){
    float somatorio = 0;
    for (Node *auxNode = this->getFirstNode(); auxNode != nullptr; auxNode = auxNode->getNextNode()) {
        float cda = this->CDA(auxNode->getId());
        somatorio += cda;
    }
    int v = this->getOrder();
    return somatorio/v;
}

/// Dijkstra
vector<string> Graph::dijkstra(const string& source_id_external, const string& target_id_external) {
    auto source_node = this->getNodeFromExternalId(source_id_external);
    auto target_node = this->getNodeFromExternalId(target_id_external);

    if(source_node == nullptr || target_node == nullptr){
        cout << "source_node e/ou target_node nulo(s)" << endl;
        abort();
    }

    auto dijkstraResult = this->dijkstra(source_node->getId(), target_node->getId());
    auto size = dijkstraResult.path->size();
    vector<string> pathDone(size+1); //adiciona mais uma posição para a distancia

    for (int i = 0; i < size; ++i) {
        pathDone[i] = mapInternalIdToExternalId[dijkstraResult.path->at(i)];
    }
    pathDone[size] = to_string(dijkstraResult.distance);

    return pathDone;
}

bool Graph::getPathDijkstra(vector<int>& dijkstraPath, vector<int> *pathToTarget, int target, int source)
{
    if (target != source)
    {
        if (dijkstraPath[target] == -1)
        {
            cout << "Caminho nao encontrado." << endl;
            return false;
        }
        getPathDijkstra(dijkstraPath, pathToTarget, dijkstraPath[target], source);
        pathToTarget->push_back(dijkstraPath[target]);
        return true;
    }
    return false;
}

MinimalPathResult Graph::dijkstra(int idSource, int idTarget){
    auto source_node = this->getNode(idSource);
    auto target_node = this->getNode(idTarget);

    if (source_node == nullptr || target_node == nullptr)
    {
        cout << "Caminho nao encontrado." << endl;
        return {false};
    }

    vector<int> dijkstraPath(this->order);
    vector<int> dijkstraDistances = dijkstraDist(idSource, dijkstraPath);

    auto pathToTarget = new vector<int>();
    long distance = 0;
    bool pathFound = getPathDijkstra(dijkstraPath, pathToTarget, idTarget, idSource);

    if (pathFound)
    {
        pathToTarget->push_back(idTarget);
        distance = dijkstraDistances[pathToTarget->at(pathToTarget->size()-1)];
    }

    return {pathToTarget, distance, pathFound};
}

vector<int> Graph::dijkstraDist(int s, vector<int> &path){
    // Armazena a distancia de cada vertice do vertice de origem
    vector<int> distance(this->order);

    bool visited[this->order];
    for (int i = 0; i < this->order; i++){
        visited[i] = false;
        path[i] = -1;
        distance[i] = INT32_MAX;
    }

    distance[s] = 0;
    path[s] = -1;
    int current = s;

    unordered_set<int> setNodeVisited;

    while (true){
        visited[current] = true;

        Edge *next_edge = this->getNode(current)->getFirstEdge();

        while (next_edge != nullptr){
            int v = next_edge->getTargetId();

            if (visited[v]){
                next_edge = next_edge->getNextEdge();
                continue;
            }

            setNodeVisited.insert(v);
            int alt = distance[current] + next_edge->getWeight();

            // Verifica se a distância calculada é menor que a distância anteriormente calculada e atualiza a distância
            if (alt < distance[v]){
                distance[v] = alt;
                path[v] = current;
            }
            next_edge = next_edge->getNextEdge();
        }

        setNodeVisited.erase(current);
        if (setNodeVisited.empty()){
            break;
        }

        int minDist = INT32_MAX;
        int index = 0;


        // Atualizar a distância dos vértices do gráfico
        for (int a : setNodeVisited){
            if (distance[a] < minDist){
                minDist = distance[a];
                index = a;
            }
        }
        current = index;
    }
    return distance;
}

/// Floyd-Warshall
vector<string> Graph::floydWarshall(const string &source_id_external, const string &target_id_external) {
    auto source_node = this->getNodeFromExternalId(source_id_external);
    auto target_node = this->getNodeFromExternalId(target_id_external);
    if(source_node == nullptr || target_node == nullptr)
    {
        cout<<"source_node e/ou target_node nulo(s)"<<endl;
        abort();
    }

    auto floydResult = this->floydWarshall(source_node->getId(), target_node->getId());
    auto size = floydResult.path->size();

    vector<string> pathDone(size+1); //adiciona o valor da distancia como um ultimo valor no caminho de string

    for (int i = 0; i < size; ++i) {
        pathDone[i] = mapInternalIdToExternalId[floydResult.path->at(i)];
    }
    pathDone[size] = to_string(floydResult.distance);

    return pathDone;
}

MinimalPathResult Graph::floydWarshall(int idSource, int idTarget) {
    static int graphSize = this->order;
    int **dis = new int*[graphSize];
    int **next = new int*[graphSize];
    int **matrix = new int*[graphSize];
    for(int i = 0; i < graphSize; i++){
        dis[i] = new int[graphSize];
        next[i] = new int[graphSize];
        matrix[i] = new int[graphSize];
    }
    for (int i = 0; i < graphSize; i++){
        for (int j = 0; j < graphSize; j++){
            if (i == j){
                matrix[i][j] = 0;
            }
            else{
                matrix[i][j] = INT32_MAX;
            }
        }
    }
    Node *next_node = this->first_node;
    while (next_node != nullptr){
        Edge *next_edge = next_node->getFirstEdge();
        while (next_edge != nullptr){
            matrix[next_node->getId()][next_edge->getTargetId()] = next_edge->getWeight();
            next_edge = next_edge->getNextEdge();
        }
        next_node = next_node->getNextNode();
    }
    // Inicializa a matriz de pesos (Grafo representado como matriz)
    for (int i = 0; i < graphSize; i++){
        for (int j = 0; j < graphSize; j++){
            dis[i][j] = matrix[i][j];

            if (matrix[i][j] == INT32_MAX)
                next[i][j] = -1;
            else
                next[i][j] = j;
        }
    }
    //Inicia o algoritmo de Floyd Warshall
    for (int k = 0; k < graphSize; k++){
        for (int i = 0; i < graphSize; i++){
            for (int j = 0; j < graphSize; j++){

                // Não podemos percorrer as arestas que não existem
                if (dis[i][k] == INT32_MAX || dis[k][j] == INT32_MAX){
                    continue;
                }

                if (dis[i][j] > dis[i][k] + dis[k][j])
                {
                    dis[i][j] = dis[i][k] + dis[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Se não houver um caminho entre os nós u e v, retorna um founded = false
    if (next[idSource][idTarget] == -1){
        return {false};
    }

    // Armazenando o caminho em um vetor
    long distance = 0;
    auto path = new vector<int>();
    path->push_back(idSource);

    while (idSource != idTarget){
        int idAux = next[idSource][idTarget];
        distance += dis[idSource][idAux];
        path->push_back(idAux);
        idSource = idAux;
    }
    // Retorna o caminho e a distância
    return {path, distance, true};
}


/// Kruskal
vector<EdgeStruct> Graph::kruskal(vector<string>* listNode){

    auto sizeList = listNode->size();

    if(sizeList == 0){
        cout << "Subgrafo vazio" << endl;
        return vector<EdgeStruct>(0);
    }

    vector<bool> participantNode(this->order, false);
    for(int i = 0; i < sizeList; i++){
        auto node = this->getNodeFromExternalId(listNode->at(i));
        if(node != nullptr){
            participantNode[node->getId()] = true;
        }else{
            cout << "No " << listNode->at(i) << " nao encontrado" << endl;
        }
    }

    auto graph = new Graph(false, true, false);
    for(auto node = this->first_node; node != nullptr; node = node->getNextNode()){
        if(!participantNode[node->getId()]){
            continue;
        }
        for (auto next_edge = node->getFirstEdge(); next_edge != nullptr; next_edge = next_edge->getNextEdge()){
            if(participantNode[next_edge->getTargetId()]){
                graph->insertEdge(
                        this->getExternalId(node->getId()),
                        this->getExternalId(next_edge->getTargetId()),
                        next_edge->getWeight());
            }
        }
    }

    graph->indexId();
    auto graphKruskal = new GraphKruskal(graph);
    auto result = graphKruskal->kruskalMST();
    auto size = result.size();
    vector<EdgeStruct> kruskalResult(size);

    for (int i = 0; i < size; ++i) {
        kruskalResult[i] = {graph->getExternalId(result[i].source),
                            graph->getExternalId(result[i].target),
                            result[i].weight};
    }

    return kruskalResult;
}
/// Prim
vector<EdgeStruct> Graph::agmPrim(vector<string>* listNodeId){
    auto sizeList = listNodeId->size();

    if(sizeList == 0){
        cout << "No nodes to load" << endl;
        return vector<EdgeStruct>(0);
    }

    vector<bool> participantNode(this->order, false);
    for(int i = 0; i < sizeList; i++){
        auto node = this->getNodeFromExternalId(listNodeId->at(i));
        if(node != nullptr){
            participantNode[node->getId()] = true;
        }else{
            cout << "Node " << listNodeId->at(i) << " not found" << endl;
        }
    }


    auto graph = new Graph(false, true);
    for(auto node = this->first_node; node != nullptr; node = node->getNextNode())
    {
        if(!participantNode[node->getId()]){
            continue;
        }
        for (auto next_edge = node->getFirstEdge(); next_edge != nullptr ;next_edge = next_edge->getNextEdge())
        {
            if(participantNode[next_edge->getTargetId()]){
                graph->insertEdge(
                        this->getExternalId(node->getId()),
                        this->getExternalId(next_edge->getTargetId()),
                        next_edge->getWeight());
            }
        }
    }

    auto graphPrim = new GraphPrim(graph);
    return graphPrim->primMST();
}

///Caminhamento em profundidade
vector<EdgeStruct> Graph::caminhoProfundidade(const string& id, int* visited){
    Node* node = this->getNodeFromExternalId(id);
    auto edges =  vector<EdgeStruct>();
    int rank = 0;

    visited[node->getId()] = 0;
    rank++;

    caminhoProfundidadeRec(node, visited, &edges, rank);

    return edges;
}

void Graph::caminhoProfundidadeRec(Node *node, int* visited, vector<EdgeStruct>* edges, int rank) {
    for (Edge *auxEdge = node->getFirstEdge(); auxEdge != nullptr; auxEdge = auxEdge->getNextEdge()) {
        auto targetId = auxEdge->getTargetId();
        if (visited[targetId] == -1) {
            visited[targetId] = rank;
            edges->emplace_back(
                    getExternalId(node->getId()),
                    getExternalId(auxEdge->getTargetId()),
                    auxEdge->getWeight());
            caminhoProfundidadeRec(this->getNode(targetId), visited, edges, rank+1);
        }
        else{
            if((visited[node->getId()] - visited[targetId]) >= 2){
                edges->emplace_back(
                        getExternalId(node->getId()),
                        getExternalId(targetId),
                        auxEdge->getWeight());
            }
        }
    }
}