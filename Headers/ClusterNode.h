//
// Created by Marcos Paulo on 02/08/2022.
//

#ifndef TESTEGRAFOS_CLUSTERNODE_H
#define TESTEGRAFOS_CLUSTERNODE_H
class No
{
public:
    No(){};
    ~No(){};
    int getInfo(){return info;};
    No* getProx(){return prox;};
    void setInfo(int val){info = val;};
    void setProx(No *p){prox = p;};
private:
    int info; // informacao
    No *prox; // ponteiro para o proximo
};
#endif //TESTEGRAFOS_CLUSTERNODE_H
