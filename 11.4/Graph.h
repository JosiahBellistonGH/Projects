#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;


template <typename Type>
class Graph;

template <typename Type>
ostream& operator << (ostream & out, const Graph<Type> &g);

template <typename Type>
class Graph {
private:
	vector<Type> verticies;
  vector<vector<Type>> edges;

public:
	Graph();
	void addVertex(Type vertex);
	void addEdge(Type, Type);
	int getVertexPos(Type item);
	int getNumVertices()const;
	bool isEdge(Type, Type);
  int getIndex(vector<Type> vector, Type val);
  bool inStack (stack<Type> stack, Type val);
	friend ostream& operator << <>(ostream & out, const Graph<Type> &g);
	vector<Type>getPath(Type, Type);
};



/*********************************************
* Constructs an empty graph
*
*********************************************/
template<typename Type>
Graph<Type>::Graph() {

}

/*********************************************
* Adds a Vertex to the Graphs. Note that the vertex may not be an int value
*********************************************/
template <typename Type>
void Graph<Type>::addVertex(Type vertex) {
  verticies.push_back(vertex);
  vector<Type>lst;
  edges.push_back(lst);
}

/*********************************************
* Returns the current number of vertices
*
*********************************************/
template<typename Type>
int Graph<Type>::getNumVertices()const {
	return verticies.size();
}



/*********************************************
* Returns the position in the verticies list where the given vertex is located, -1 if not found.
*
*********************************************/
template <typename Type>
int Graph<Type>::getVertexPos(Type item) {
	for(unsigned int i = 0; i < verticies.size(); i++) {
    if (item == verticies[i]) {
      return i;
    }
  }
	return -1; //return negative one
}//End findVertexPos

/*********************************************
* Adds an edge going in the direction of source going to target
*
*********************************************/
template <typename Type>
void Graph<Type>::addEdge(Type source, Type target) {
	int srcPos = getVertexPos(source);
  if (srcPos < 0) {
    throw runtime_error("Vertex not found");
  }
  edges[srcPos].push_back(target);
}

template <typename Type>
bool Graph<Type>::isEdge(Type source, Type dest) {
	int srcPos = getVertexPos(source);
  if (srcPos < 0) {
    throw runtime_error("Vertex not found");
  }
  for (unsigned int i = 0; i < edges[i].size(); i++) {
    if(edges[srcPos][i] == dest) {
      return true;
    }
  }
	return false;
}



/*********************************************
* Returns a display of the graph in the format
* vertex: edge edge
* Note: This is not a traversal, just output
*********************************************/
template <typename Type>
ostream& operator << (ostream & out, const Graph<Type> &g) {
  for (unsigned int i = 0; i < g.verticies.size(); i++) {
    out << g.verticies[i] << ": ";
    for(unsigned int e = 0; e < g.edges[i].size(); e++) {
      out << g.edges[i][e] << " ";
    }
    out << "\n";
  }
	return out;
}

template<typename Type>
bool Graph<Type>::inStack (stack<Type> stack, Type val) {
    while (!stack.empty() && stack.top() != val) {
      stack.pop();
    }
    if (!stack.empty()) {
      return true;
    }
    return false;
}

template<typename Type>
int Graph<Type>::getIndex(vector<Type> vector, Type val) {
    auto item = find(vector.begin(), vector.end(), val);
    if (item != vector.end()) {
        return item - vector.begin();
    }
    else {
        return -1;
    }
}

/*
  getPath will return the shortest path from source to dest.
  You are welcome to use any solution not limited to the following, depth first search to traverse
  graph to find the solution, breadth first, shortest path first, or any
  other graph algorithm.

  You will return a vector with the solution from the source to the destination.
  IE: The source will be in position 1 the destination is in the last position of the solution, and each node in between
  are the verticies it will travel to get to the destination.  There will not be any
  other verticies in the list.
*/
template <typename Type>
vector<Type> Graph<Type>::getPath(Type source, Type dest) {
	vector<Type> solution;
  stack<Type> open;
  open.push(dest);
  stack<Type> closed;
  int size = verticies.size();
  int parents [size];
  for (int i = 0; i < size; i++) {
    parents[i] = -1;
  }
  while (!open.empty()) {
    auto curr = open.top();
    open.pop();
    closed.push(curr); 
    for (int i = 0; i < size; i++) {
      if (isEdge(curr, verticies[i])) {
        if (!inStack(closed, verticies[i])) {
          open.push(verticies[i]);
          if (parents[i] == -1) {
            parents[i] = getIndex(verticies, curr);
          }
        }
      }
    }
  }
  bool found = false;
  int solStep = getIndex(verticies, source);
  while (!found) {
    solution.push_back(verticies[solStep]);
    solStep = parents[solStep];
    if (solStep == getIndex(verticies, dest)) {
      found = true;
      solution.push_back(verticies[solStep]);
    }
  }
	return solution;
}

