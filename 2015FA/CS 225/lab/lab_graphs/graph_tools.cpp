/**
 * @file graph_tools.cpp
 * This is where you will implement several functions that operate on graphs.
 * Be sure to thoroughly read the comments above each function, as they give
 *  hints and instructions on how to solve the problems.
 */

#include "graph_tools.h"

/**
 * Returns the shortest distance (in edges) between the Vertices
 *  start and end.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @param start - the vertex to start the search from
 * @param end - the vertex to find a path to
 * @return the minimum number of edges between start and end
 *
 * @todo Label each edge "MINPATH" if it is part of the minimum path
 *
 * @note Remember this is the shortest path in terms of edges,
 *  not edge weights.
 * @note Again, you may use the STL stack and queue.
 * @note You may also use the STL's unordered_map, but it is possible
 *  to solve this problem without it.
 *
 * @hint In order to draw (and correctly count) the edges between two
 *  vertices, you'll have to remember each vertex's parent somehow.
 */
int GraphTools::findShortestPath(Graph & graph, Vertex start, Vertex end)
{
	//Get the graph prepared
	vector<Vertex> vertices = graph.getVertices();
	for (auto itr = vertices.begin(); itr != vertices.end(); itr++)
		graph.setVertexLabel(*itr, "UNKNOWN");

	graph.setVertexLabel(start, std::to_string(0));

	queue<Vertex> vertexQueue;
	//Start BFS
	vertexQueue.push(start);
	unordered_map<Vertex, Vertex> prev;
	prev.insert(make_pair(start, start));
	while (vertexQueue.front() != end)
	{
		vector<Vertex> adjs = graph.getAdjacent(vertexQueue.front());

		for (auto itr = adjs.begin(); itr != adjs.end(); itr++)
		{
			if (graph.getVertexLabel(*itr) == "UNKNOWN")
			{
				graph.setVertexLabel(*itr, std::to_string(1 + std::stoi(graph.getVertexLabel(vertexQueue.front()))));
				prev.insert(make_pair(*itr, vertexQueue.front()));			
			}
			else if (graph.getVertexLabel(*itr) == "VISITED")
				continue;
			else
			{
				if (std::stoi(graph.getVertexLabel(*itr)) > 1 + std::stoi(graph.getVertexLabel(vertexQueue.front())))
				{
					graph.setVertexLabel(*itr, std::to_string(1 + std::stoi(graph.getVertexLabel(vertexQueue.front()))));
					prev.insert(make_pair(*itr, vertexQueue.front()));
				}
			}
			vertexQueue.push(*itr);
		}
		graph.setVertexLabel(vertexQueue.front(), "VISITED");
		vertexQueue.pop();
	}
	Vertex curr = end;
	while (curr != prev[curr])
	{
		Vertex temp = prev[curr];
		graph.setEdgeLabel(curr, temp, "MINPATH");
		curr = temp;
	}
	return std::stoi(graph.getVertexLabel(end));
}

/**
 * Finds the minimum edge weight in the Graph graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @return the minimum weighted edge
 *
 * @todo Label the minimum edge as "MIN". It will appear blue when
 *  graph.savePNG() is called in minweight_test.
 *
 * @note You must do a traversal.
 * @note You may use the STL stack and queue.
 * @note You may assume the graph is connected.
 *
 * @hint Initially label vertices and edges as unvisited.
 */
int GraphTools::findMinWeight(Graph & graph)
{
	vector<Edge> edges = graph.getEdges();
	Edge min = edges[0];
	for (auto itr = edges.begin(); itr != edges.end(); itr++)
	{
		if  (*itr < min)
			min = *itr;
	}

	graph.setEdgeLabel(min.source, min.dest, "MIN");
    return graph.getEdgeWeight(min.source, min.dest);
}

/**
 * Finds a minimal spanning tree on a graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to find the MST of
 *
 * @todo Label the edges of a minimal spanning tree as "MST"
 *  in the graph. They will appear blue when graph.savePNG() is called.
 *
 * @note Use your disjoint sets class from MP 7.1 to help you with
 *  Kruskal's algorithm. Copy the files into the libdsets folder.
 * @note You may call std::sort (http://www.cplusplus.com/reference/algorithm/sort/)
 *  instead of creating a priority queue.
 */
void GraphTools::findMST(Graph & graph)
{
	vector<Vertex> V = graph.getVertices();
	vector<Edge> E = graph.getEdges();
	DisjointSets set;
	set.addelements(V.size());
	std::sort(E.begin(), E.end());

	for (size_t i = 0; i < E.size(); i++)
	{
		if (set.find(E[i].source) != set.find(E[i].dest))
		{
			set.setunion(E[i].source, E[i].dest);
			graph.setEdgeLabel(E[i].source, E[i].dest, "MST");
		}
	}
}
