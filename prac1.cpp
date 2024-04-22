#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

const int MAX_NODES = 100;
vector<int> graph[MAX_NODES];

void parallelBFS(int start) {
    bool visited[MAX_NODES] = {false};
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
#pragma omp parallel for
        for (int i = 0; i < graph[current].size(); ++i) {
            int neighbor = graph[current][i];
            #pragma omp critical
            {
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }

    cout << "BFS Visited Nodes: ";
    for (int i = 0; i < MAX_NODES; ++i) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

void parallelDFS(int start, bool visited[]) {
    visited[start] = true;
#pragma omp parallel for
    for (int i = 0; i < graph[start].size(); ++i) {
        int neighbor = graph[start][i];
        if (!visited[neighbor]) {
            parallelDFS(neighbor, visited);
        }
    }
}

int main() {
    int num_nodes;
    cout << "Enter the number of nodes in the graph: ";
    cin >> num_nodes;

    cout << "Enter the connections for each node (enter -1 to terminate):" << endl;
    for (int i = 0; i < num_nodes; ++i) {
        int node, neighbor;
        cout << "Enter node " << i << ": ";
        cin >> node;
        while (true) {
            cout << "Enter neighbor node (-1 to stop): ";
            cin >> neighbor;
            if (neighbor == -1)
                break;
            graph[node].push_back(neighbor);
        }
    }

    int start_node;
    cout << "Enter the starting node for traversal: ";
    cin >> start_node;

    parallelBFS(start_node);

    bool visited[MAX_NODES] = {false};
    parallelDFS(start_node, visited);

    cout << "DFS Visited Nodes: ";
    for (int i = 0; i < MAX_NODES; ++i) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    return 0;
}
