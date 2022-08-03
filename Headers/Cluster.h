//
// Created by Marcos Paulo on 02/08/2022.
//
#include "./ClusterNode.h"

#ifndef TESTEGRAFOS_CLUSTER_H
#define TESTEGRAFOS_CLUSTER_H
class Cluster
{
private:
    No *primeiro;
public:
    Cluster();
    ~Cluster();

    void insereInicio(int val);
    void removeInicio();
    void insereFinal(int val);
    void imprimir();
    void removeFinal();
};
#endif //TESTEGRAFOS_CLUSTER_H
