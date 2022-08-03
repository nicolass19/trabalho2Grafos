cmake_minimum_required(VERSION 3.22)
project(vai_p_caralho_git)

set(CMAKE_CXX_STANDARD 14)

include_directories(Headers)

add_executable(vai_p_caralho_git
        Headers/Cluster.h
        Headers/ClusterNode.h
        Headers/Edge.h
        Headers/Graph.h
        Headers/GraphKruskal.h
        Headers/GraphPrim.h
        Headers/Menu.h
        Headers/Node.h
        MIs/Cluster.cpp
        MIs/Clustering.cpp
        MIs/Edge.cpp
        MIs/Graph.cpp
        MIs/GraphKruskal.cpp
        MIs/GraphPrim.cpp
        MIs/Menu.cpp
        MIs/Node.cpp
        MIs/PrintDOT.cpp
        main.cpp)
