#include <iostream>
#include "class.hpp"

using namespace std;

int main() {

    int n = 5;
    cout << "Creating Directed Graph with " << n << " vertices..." << endl;
    Graph* g = new DirectedGraph(n);

    cout << "Generating random edges (Erdos-Renyi model, p=0.5)..." << endl;
    g->generateRandom(0.5, 1, 10);
    g->printGraph();

    cout << "Converting to Matrix..." << endl;
    g->convertToMatrix();
    g->printGraph();

    cout << "Adding new vertex (Auto-convert to List)..." << endl;
    g->addVertex(); 
    g->addEdge(6, 1, 99);
    g->printGraph();

    delete g;
    return 0;
}