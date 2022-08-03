//
// Created by Marcos Paulo on 02/08/2022.
//
#include <iostream>
#include "../Headers/Cluster.h"


Cluster::Cluster()
{
    primeiro = NULL;
}

Cluster::~Cluster()
{
    No *p = primeiro;

    while(p != NULL)
    {
        primeiro = primeiro->getProx();
        delete p;
        p = primeiro;
    }
}

void Cluster::insereInicio(int val)
{
    ///insere um novo nó no inicio da lista com valor val

    No *p = new No();
    p->setInfo(val);
    p->setProx(primeiro);
    primeiro = p;
}

void Cluster::insereFinal(int val)
{
    No *t = new No();
    t->setInfo(val);
    t->setProx(NULL);

    if (primeiro != NULL){
        No *p;
        for (p = primeiro; p->getProx() != NULL; p = p->getProx());
        p->setProx(t);

    }
    else{
        primeiro = t;
    }
}

void Cluster::removeInicio()
{
    if(primeiro != NULL)
    {
        No *p = primeiro;

        primeiro = primeiro->getProx();

        delete p;
    }
}

void Cluster::removeFinal()
{
    if (primeiro != NULL){

        No *ap = NULL, *p;
        for (p = primeiro; p->getProx() != NULL; p = p->getProx()){
            ap = p;
        }

        if (ap == NULL){ // lista com 1 nó
            primeiro = NULL;
        }
        else{
            ap->setProx(NULL);
        }
        delete p;
    }
}

void Cluster::imprimir()
{
    No *p = primeiro;
    while(p != NULL)
    {
        std::cout<<p->getInfo()<<"    ";
        p = p->getProx();
    }
    std::cout<<std::endl<<std::endl<<std::endl;
}
