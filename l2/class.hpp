#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <utility> 
#include <iomanip> //гламур
#include <random> 

using namespace std;

class Graph {
protected:
    int n;
    vector<list<pair<int, int>>> adjList;
    vector<vector<int>> adjMatrix;
    bool isMatrixRepresentation;

public:
    Graph(int vertices) : n(vertices), isMatrixRepresentation(false) {
        adjList.resize(n + 1);
    }

    virtual ~Graph() {}

    // Генерація випадкового графу (Ердеша-Реньї)
    // p - ймовірність появи ребра (0.0 - 1.0)
    // minW, maxW - межі для ваги ребра
    virtual void generateRandom(double p, int minW, int maxW) = 0;

    void convertToMatrix() {
        if (isMatrixRepresentation) return;
        adjMatrix.assign(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (const auto& edge : adjList[i]) {
                adjMatrix[i][edge.first] = edge.second;
            }
        }
        adjList.clear();
        isMatrixRepresentation = true;
    }

    void convertToList() {
        if (!isMatrixRepresentation) return;
        adjList.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (adjMatrix[i][j] != 0) {
                    adjList[i].push_back({ j, adjMatrix[i][j] });
                }
            }
        }
        adjMatrix.clear();
        isMatrixRepresentation = false;
    }

    //Базові методи
    void addVertex() {
        if (isMatrixRepresentation) convertToList();
        n++;
        adjList.resize(n + 1);
    }

    void removeVertex(int k) {
        if (isMatrixRepresentation) convertToList();
        if (k < 1 || k > n) return;
        for (int i = 1; i <= n; ++i) {
            auto it = adjList[i].begin();
            while (it != adjList[i].end()) {
                if (it->first == k) it = adjList[i].erase(it);
                else {
                    if (it->first > k) it->first--;
                    ++it;
                }
            }
        }
        adjList.erase(adjList.begin() + k);
        n--;
    }

    virtual void addEdge(int u, int v, int weight = 1) = 0;
    virtual void removeEdge(int u, int v) = 0;

    void printGraph() const {
        if (isMatrixRepresentation) {
            cout << "Adjacency Matrix (V=" << n << "):" << endl;
            cout << "   ";
            for (int i = 1; i <= n; ++i) cout << setw(3) << i;
            cout << endl;
            for (int i = 1; i <= n; ++i) {
                cout << setw(2) << i << "|";
                for (int j = 1; j <= n; ++j) {
                    cout << setw(3) << adjMatrix[i][j];
                }
                cout << endl;
            }
        }
        else {
            cout << "Adjacency List (V=" << n << "):" << endl;
            for (int i = 1; i <= n; ++i) {
                cout << "[" << i << "]";
                for (const auto& edge : adjList[i]) {
                    cout << " -> " << edge.first << "(w:" << edge.second << ")";
                }
                cout << endl;
            }
        }
        cout << "-----------------------" << endl;
    }
};

class UndirectedGraph : public Graph {
public:
    UndirectedGraph(int vertices) : Graph(vertices) {}

    void addEdge(int u, int v, int weight = 1) override {
        if (u < 1 || u > n || v < 1 || v > n) return;
        if (isMatrixRepresentation) {
            adjMatrix[u][v] = weight;
            adjMatrix[v][u] = weight;
        }
        else {
            adjList[u].push_back({ v, weight });
            adjList[v].push_back({ u, weight });
        }
    }

    void removeEdge(int u, int v) override {
        if (u < 1 || u > n || v < 1 || v > n) return;
        if (isMatrixRepresentation) {
            adjMatrix[u][v] = 0;
            adjMatrix[v][u] = 0;
        }
        else {
            auto remover = [&](int src, int dest) {
                adjList[src].remove_if([dest](const pair<int, int>& edge) { return edge.first == dest; });
                };
            remover(u, v);
            remover(v, u);
        }
    }

    //Генерації для не ор. графу
    void generateRandom(double p, int minW, int maxW) override {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> probDist(0.0, 1.0);
        uniform_int_distribution<> weightDist(minW, maxW);

        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                if (probDist(gen) < p) {
                    addEdge(i, j, weightDist(gen));
                }
            }
        }
        cout << "Random Undirected Graph generated (p=" << p << ")" << endl;
    }
};

class DirectedGraph : public Graph {
public:
    DirectedGraph(int vertices) : Graph(vertices) {}

    void addEdge(int u, int v, int weight = 1) override {
        if (u < 1 || u > n || v < 1 || v > n) return;
        if (isMatrixRepresentation) {
            adjMatrix[u][v] = weight;
        }
        else {
            adjList[u].push_back({ v, weight });
        }
    }

    void removeEdge(int u, int v) override {
        if (u < 1 || u > n || v < 1 || v > n) return;
        if (isMatrixRepresentation) {
            adjMatrix[u][v] = 0;
        }
        else {
            adjList[u].remove_if([v](const pair<int, int>& edge) { return edge.first == v; });
        }
    }

    //Генерації для ор. графу
    void generateRandom(double p, int minW, int maxW) override {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> probDist(0.0, 1.0);
        uniform_int_distribution<> weightDist(minW, maxW);

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (i == j) continue;
                if (probDist(gen) < p) {
                    addEdge(i, j, weightDist(gen));
                }
            }
        }
        cout << "Random Directed Graph generated (p=" << p << ")" << endl;
    }
};