//
// Created by Marcos Paulo on 02/08/2022.
//
#include "./ClusterNode.h"

#ifndef TESTEGRAFOS_CLUSTER_H
#define TESTEGRAFOS_CLUSTER_H
class Cluster
{
private:
    ClusterNode *primeiro;
    float lowerLimit;
    float higherLimit;
    float actualLimit;
    float sumEdges;

public:

    Cluster();
    ~Cluster();

    ClusterNode* getPrimeiro(){return primeiro;}

    float getLowerLimit() const;
    void setLowerLimit(float lowerLimit);
    float getHigherLimit() const;
    void setHigherLimit(float higherLimit);

    float getActualLimit() const;

    float getSumEdges() const;

    void insereInicio(int val);
    void removeInicio();
    void insereFinal(int id, float weight, float edgeWeight);
    void imprimir();
    void removeFinal();
};
#endif //TESTEGRAFOS_CLUSTER_H
