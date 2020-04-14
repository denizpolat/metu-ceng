#include "the3.h"

void DFStraverse(int edge, int* visited, int**& edgeList, int n) {
	visited[edge] = 1;
	for (int i = 0; i < n; i++) {
		if (edgeList[edge][i] && !visited[i]) {
			DFStraverse(i, visited, edgeList, n);
		}
	}
}


int findComponentNumber(int n, int**& edgeList) {
	int visited[n];
	int number = 0;
	for (int i = 0; i < n; i++) {
		visited[i] = 0;
	}
	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			DFStraverse(i, visited, edgeList, n);
			number++;
		}
	}
	return number;
}



void fillDistancesTable(int n, int** dist, int**& edgeList) {
	std::vector<int> queue;
	for (int source = 0; source < n; source++) {
		for (int i = 0; i < n; i++) {
			dist[source][i] = INT_MAX;
		}
		dist[source][source] = 0;
		queue.push_back(source);
		while (!queue.empty()) {
			int u = queue[0];
			queue.erase(queue.begin());
			for (int i = 0; i < n; i++) {
				if (edgeList[u][i]) {
					int temp = dist[source][u] + edgeList[u][i];
					if (temp < dist[source][i]) {
						dist[source][i] = temp;
						queue.push_back(i);
					}
				}
			}
		}
	}
}



// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value : 
//                 number of disconnected components



int Important(int n, int**& edgeList, double*& scores) {
	int** distances = new int*[n];
	for (int i = 0; i < n; i++) {
		distances[i] = new int[n];
		scores[i] = 0;
	}

	fillDistancesTable(n, distances, edgeList);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int payda = distances[i][j];
			if (payda == INT_MAX) continue;
			if (!payda) payda = 1;
			for (int k = 0; k < n; k++) {
				if (distances[i][k] == INT_MAX || i == k || k == j) continue;
				else scores[k] += double(distances[i][k] + distances[k][j]) / payda;
			}
		}
	}

	for (int i = 0; i < n; i++) delete distances[i];
	delete distances;


	return findComponentNumber(n, edgeList);
}
