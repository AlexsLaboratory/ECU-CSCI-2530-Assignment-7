// CSCI 2530
// Assignment: 7
// Author:     Alex Lowe
// File:       dijkstra
// Tab stops:  None

/*
This program takes and reads a weighted graph from the user. It also takes
in a start and finish point on the weighted graph then prints the
path with lowest weight on the graph.
*/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "pqueue.h"
//#include "pqueue.cpp"

typedef PriorityQueue EventQueue;

using namespace std;

// Defines that tracing is off by default.

int tracing = 0;


/*
This edge contains the two integers that it connects labeled as from and
to as well as its weight as a double. It uses the pointer next to point to
the next edge on vertex from.
*/

struct Edge
{
    double weight;
    Edge *next;
    int from;
    int to;

    Edge(int f, int t, double w, Edge *n)
    {
      from = f;
      to = t;
      weight = w;
      next = n;
    }
};

/*
Vertex contains a pointer head that points to the list of Edges that are
connected to this vertex. An integer time to indicate when a signal arrives
at this vertex and an integer fromVertex to indicate where the signal came
from.
*/

struct Vertex
{
    Edge *list;
    double time;
    int fromVertex;

    Vertex()
    {
      time = -1;
      fromVertex = -1;
      list = NULL;
    }
};


/*
Graph contains two integers, the first numVert which shows the total number
of vertices in the graph, while numEdges stores the total number of edges.
the pointer vertices leads to a list of all the vertices in the Graph.
*/

struct Graph
{
    int numVert;
    int numEdge;
    Vertex *vertices;

    Graph(int nv)
    {
      numEdge = 0;
      numVert = nv;
      vertices = new Vertex[nv + 1];
    }
};

/*
This is a helper function that inserts an edge into on adjacency list. It
takes in a u (from) a v (to), w (weight), and finally a pointer of type
Graph*.
 */

void insertDirectionalEdge(int u, int v, double w, Graph* g)
{
  g->vertices[u].list = new Edge(u, v, w, g->vertices[u].list);
}

/*
This is a helper function that takes in a Vertex structure. If tracing is
equal to 0 it will only print one side of the vertices. If tracing is
equal to 1 it will print both sides fo the vertices (forwards and backwards).
*/

void printVert(Vertex &v)
{
  for (Edge *p = v.list; p != NULL; p = p->next)
  {
    if (tracing == 1 || p->from < p->to)
    {
      printf("(%i, %i) weight %lf\n", p->from, p->to, p->weight);
    }
  }
}


/*
Inserts an edge to Graph v where u is the first vertex and b is the second.
w is the weight of the vertex.
*/

void insertEdge(int u, int v, double w, Graph* g)
{
  insertDirectionalEdge(u, v, w, g);
  insertDirectionalEdge(v, u, w, g);
  g->numEdge++;
}

/*
Prints the number of vertices and edges with the pointer to a Graph
structure being an argument taken in by the function. For each vertex it
runs the printVert function which prints the from vertex, the to vertex,
and the weight.
*/

void writeGraph(const Graph* g)
{
  printf("There are %i vertices and %i edges\n", g->numVert, g->numEdge);
  for (int i = 1; i <= g->numVert; ++i)
  {
    printVert(g->vertices[i]);
  }
}

/*
Takes in the user input and then uses that input to insert edge(s) into
the graph.
*/

Graph* readGraph()
{
  int numVert;
  int v1;
  int v2;
  double weight;
  scanf("%i", &numVert);
  Graph *g = new Graph(numVert);
  while (scanf("%i", &v1) == 1 && v1 != 0)
  {
    scanf("%i", &v2);
    scanf("%lf", &weight);
    insertEdge(v1, v2, weight, g);
  }
  return g;
}

/*
Takes the number of arguments and if there are two arguments it looks at
the second one and if it is equal to "-t" then it turns tracing one else
it prints invalid parameter back to the user.
*/

void setTracing(int argc, char *argv[])
{
  switch (argc)
  {
    case 1:
      break;
    case 2:
      if (strcmp(argv[1], "-t") == 0)
      {
        tracing = 1;
      }
      else
      {
        printf("Invalid parameter argument");
        exit(0);
      }
  }
}

/*
 Sends a signal to the event queue e (EventQueue) using s (Send) and r
(Receive) as vertexes and t as the time or weight of the signal.
*/

void sendSignal(int s, int r, double t, EventQueue &e)
{
  insert(e, new Event(t, s, r), t);
  if (tracing == 1)
  {
    printf("%lf Sending a signal from %i to %i\n", t, s, r);
  }
}

/*
Sends a signal to every vertex v searching through Graph g for
each vertex and then adding each that hasn't been touched to event queue q.
*/

void propagateSignal(Graph *g, int v, EventQueue &e)
{
  for (Edge *p = g->vertices[v].list; p != NULL; p = p->next)
  {
    if (g->vertices[p->to].time == -1)
    {
      sendSignal(v, p->to, g->vertices[v].time + p->weight, e);
    }
  }
}

/*
A helper function that takes in a graph (g) a vertex (v), a s (sender) and
t (time) and installs it into the Vertex structure.
*/

void installInfo(Graph *g, int v, int s, double t)
{
  g->vertices[v].time = t;
  g->vertices[v].fromVertex = s;
}

/*
Helper function that checks if the time equal to -1 and if it is it will
run installInfo and propogateSignal functions.
*/

void processEvent(Graph *g, EventQueue &eq, Event *e)
{
  if (g->vertices[e->receiver].time == -1)
  {
    if (tracing == 1)
    {
      printf("%lf signal arrival from %i to %i\n", e->time, e->sender,
             e->receiver);
    }
    installInfo(g, e->receiver, e->sender, e->time);
    propagateSignal(g, e->receiver, eq);
  }
  else
  {
    if (tracing == 1)
    {
      printf("%lf signal from %i to %i ignored\n", e->time, e->sender,
      e->receiver);
    }
  }

}

/*
Finds the smallest path (by weight) in graph g from vertex vStart to
vertex vEnd.
*/

void eventLoop(Graph *g, int vStart, int vEnd)
{
  EventQueue e;
  sendSignal(0, vStart, 0, e);
  Event *i;
  double p;
  while (g->vertices[vEnd].fromVertex < 0)
  {
    remove(e, i, p);
    processEvent(g, e, i);
    delete i;
  }
}

/*
Prints the graph g from teh starting point vStart to the end point vEnd.
*/

void showPath(Graph *g, int vStart, int vEnd)
{
  if (vStart == vEnd)
  {
    cout << vStart;
  }
  else
  {
    showPath(g, vStart, g->vertices[vEnd].fromVertex);
    cout << " -> " << vEnd;
  }
}

int main(int argc, char *argv[])
{
  int vStart;
  int vEnd;
  setTracing(argc, argv);
  Graph *test = readGraph();
  scanf("%i", &vStart);
  scanf("%i", &vEnd);
  writeGraph(test);
  eventLoop(test, vStart, vEnd);
  printf("The shortest path from %i to %i has length %lf and is\n", vStart,
         vEnd, test->vertices[vEnd].time);
  showPath(test, vStart, vEnd);
  cout << endl;
  return 0;
}