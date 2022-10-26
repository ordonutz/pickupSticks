/*
 * Name: Leslie Ordonez
 * Description: This program simulates the playing of pick up sticks. The game
				requires players to avoid picking up sticks that are underneath
				other sticks. Through the help of a graph to store the order
				of the sticks and using top sort we are able to simulate the
				playing of this game. If the graph of a game has a cycle than
				it is impossible to win the game so we first check to see if a
				cycle is detected. The program modifies depth first search to 
				help detect cycles and to push the sticks onto a myStack object
				in top sort order.
 * Input: The input for this program is a text file. The text file contains the
          number of vertices, edges, and the dependencies. 
 * Output: The results for each test case are printed with n lines of integers.
		   Prints error message if there is no such correct order to p/u sticks.
*/

#include <iostream>
#include <fstream>
#include <string>
#include "cs302hw8.h"
using namespace std;

bool dfsCycleCheck(const int& currNode, vertex<int>* graph, int* visited);

bool detectCycle(const int& vertices, vertex<int>* graph, int* visited);

void topSort(const int& currNode, vertex<int>* graph, bool* visited, myStack<int>& dfsStack);

int main()
{
	bool cycleDetect = true;      // stores results of detectCycle function
	ifstream inFileObj;           // object for our file input
	int vertices = 0;             // number of sticks
	int edgeNum = 0;              // number of edges in graph
	int from = 0;                 // used to create edge starting at from
	int to = 0;                   // used to create edge ending at to
	
	string userInput;             // stores filename from user
	string currInput;             // current string read in

	int* visited;                 // keeps track of visited nodes to detect cycles
	bool* topSrtVisited;          // keeps track of visited nodes for top sort
	vertex<int>* sticks;          // object to store dependencies
	vertex<int>::edgeIterator it; // object to iterate each index of sticks 
	myStack <int> dfsStack;       // object use for top sort
	
	// Prompt user for file input
	cout << endl << "Enter filename: ";
	getline(cin, userInput);
	inFileObj.open(userInput);

	// Check for error in opening file
	while (!inFileObj.is_open()) {
		cout << "Enter filename: ";
		getline(cin, userInput);
		inFileObj.open(userInput);
	}

	cout << endl; // separate file input line(s) from output 
	
	inFileObj >> currInput;
	vertices = stoi(currInput);

	inFileObj >> currInput;
	edgeNum = stoi(currInput);

	sticks = new vertex<int>[vertices + 1];   // store dependencies
	visited = new int[vertices + 1]();        // initializes [] with 0
	topSrtVisited = new bool[vertices + 1](); // initializes [] with false

	for (int i = 0; i < edgeNum; i++)        
	{
		inFileObj >> currInput;
		from = stoi(currInput);
		inFileObj >> currInput;
		to = stoi(currInput);

		sticks[from].addEdge(to);
	}

	// detect cycle using a modified DFS
	cycleDetect = detectCycle(vertices, sticks, visited);
	
	if (cycleDetect) // Cannot win the game if a cycle was found
	{
		cout << "IMPOSSIBLE! YOU CANNOT WIN!" << endl;
	}

	// print topological sort using myStack object
	else 
	{
		for (int i = 1; i < vertices + 1; i++) // visit all verticies and push
		{
			if (!topSrtVisited[i])
			{
				topSort(i, sticks, topSrtVisited, dfsStack);
			}
		}

		// dfsStack will be in top sort form from reverse pushing from the DFS
		while (!dfsStack.isEmpty())
		{
			cout << dfsStack.pop() << endl;
		}
	}

	// separate output
	cout << endl;

	// deallocate
	delete[] sticks;
	delete[] visited;
	delete[] topSrtVisited;

	// close file
	inFileObj.close();

	return 0;
}

/*
* dfs: Checks if there is a cycle in a graph through a modification to a DFS.
	   Keeps track of vertices in function call stack with 0, 1, and 2. 
	   0 = unvisited vertex, 1 = visited vertex but currently exploring that
	   path, and 2 = fully explored vertex. Instead of using two arrays to 
	   check if the visited node is in the function call stack array, I use one 
	   array to keep track of the 3 states. If the vertex is in state = 1 than
	   it is in the function call stack and a cycle was detected.
* parameters: const int& currNode to start exploring from in the graph
		      vertex<int>* graph that stores dependencies  
			  int* visited array that uses (0, 1, 2) to denote visiting state
* return value: Returns true if a cylce was detected false otherwise
*/
bool dfsCycleCheck(const int& currNode, vertex<int>* graph, int* visited)
{
	visited[currNode] = 1;        // mark as visited but not fully explored

	vertex<int>::edgeIterator it; // iterates through path 

	// != NULL or graph[currNode].end();
	for (it = graph[currNode].begin(); it != NULL; it++)
	{
		if (visited[*it] == 1) // in the func call stack cycle detected
		{
			return true;
		}

		// call dfs on unvisited nodes 
		// if cycle is detected from previous if statment than
		// it will keep returning true from the path the cycle was found 
		else if ((visited[*it] == 0) && (dfsCycleCheck(*it, graph, visited)))
		{
			return true;
		}
	}

	// Can't explore current path any further mark currNode as fully explored
	visited[currNode] = 2;

	// return false if we don't encounter a visited but not fully explored 
	// vertex / visited vertex in the call stack no cycle was found
	return false; 
}

/*
* detectCycle: Iterates through all the vertices of a graph to check if a cycle
			   was found by calling helper function dfsCycleCheck. 
* parameters: const int& number of vertices in the graph
		      vertex<int>* graph that stores dependencies  
			  int* visited array that uses (0, 1, 2) to denote visiting state
* return value: Returns true if a cycle was detected in a graph false otherwise.
*/
bool detectCycle(const int& vertices, vertex<int>* graph, int* visited)
{
	for (int i = 1; i < vertices + 1; i++) // loop through all vertices
	{
		// returns if cycle is detected in modified dfs function
		if ((visited[i] == 0) && (dfsCycleCheck(i, graph, visited))) 
		{
			return true;
		}

		// returns here if no cycle is detected 
		return false;
	}

	// should never return this only here to remove warning
	return true;
}

/*
* topSort: Modified DFS that pushes elements onto the stack in a post order
		   traversal. Once all elements are pushed on in this order they can
		   then be popped off in top sort form.
* parameters: const int& currNode to start exploring in the graph
			  vertex<int>* graph to sort 
			  bool* visited keeps track of visited/unvisited vertices
			  myStack<int>& dfsStack stack object used to push and print 
* return value: none
*/
void topSort(const int& currNode, vertex<int>* graph, bool* visited, myStack<int>& dfsStack)
{
	if (visited[currNode])
	{
		return;
	}

	visited[currNode] = true; // mark vertex as visited
	
	vertex<int>::edgeIterator it;

	// != NULL or graph[currNode].end();
	for (it = graph[currNode].begin(); it != NULL; it++)
	{
		if (!visited[*it]) // call function again if node is unvisited
		{
			topSort(*it, graph, visited, dfsStack);
		}
	}

	// At this point we cannot further explore the path of the graph. 
	// We have reached the "bottom stick" so we pop starting from this stick to 
	// get the reverse ordering popped onto the stack 
	dfsStack.push(currNode);
}
