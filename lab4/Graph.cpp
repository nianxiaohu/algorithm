#include <iostream>
#include <fstream>
#include <limits.h>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <list>
#define PRINT(n,t) {for ( int i = 0; i<t; i++ ) { std::cout << '\t' ;} std::cout << n << endl;}
using namespace std;
// there are two ways to keep track whether or not a vertex has been visited or not
// one is maintaining vist array
// two is including a variable in vertex class
int INFINITY = INT_MAX;

class Vertex {
public:
    string           name;   // Vertex name
    vector<Vertex *> adj;    // Adjacent vertices
    int              dist;   // Cost
    Vertex          *path;   // Previous vertex on shortest path
  int preN;
  int postN;
  Vertex( const string & nm,int pre = -1, int post = -1 ) : name( nm ),preN(pre),postN(post)
      { reset( ); }

    void reset( )
      { dist = INFINITY; path = NULL; }
};


typedef map<string,Vertex *> vmap;
typedef pair<string,Vertex *> vpair;


class Graph
{
public:
  Graph( ) { }
  ~Graph( );
  void addEdge( const string & sourceName, const string & destName );
  void printPath( const string & destName ) const;
  void unweighted( const string & startName );
  void dfs();   
private:
  Vertex * getVertex( const string & vertexName );// given a vertexName(string), return a pointer to that vertex
  void printPath( const Vertex & dest ) const;
  void clearAll( );
  void dfs(Vertex* vertex, int& time);
  vmap vertexMap;
  vector<Vertex *> allVertices;// vector of pointer
};

void Graph::addEdge( const string & sourceName, const string & destName )// call getVertex if the vertex is not there, you need to make a new one
{
    Vertex * v = getVertex( sourceName );
    Vertex * w = getVertex( destName );
    v->adj.push_back( w );
}

void Graph::printPath( const string & destName ) const
{
    vmap::const_iterator itr = vertexMap.find( destName );

    if( itr == vertexMap.end( ) )
    {
        cout << "Destination vertex not found" << endl;
        return;
    }

    const Vertex & w = *(*itr).second;// dot operator is higher in order preference than *, since the value of map is a pointer to vertex, so you need to deference to get its objects. declare w as just a reference to its objects.
    if( w.dist == INFINITY )
        cout << destName << " is unreachable";
    else
      printPath( w );// the same name, different argument lists overloading
    cout << endl;
}

// If vertexName is not present, add it to vertexMap
// In either case, return the Vertex
Vertex * Graph::getVertex( const string & vertexName )
{
    vmap::iterator itr = vertexMap.find( vertexName );

    if( itr == vertexMap.end( ) )
    {
        Vertex *newv = new Vertex( vertexName );
        allVertices.push_back( newv );
        vertexMap.insert( vpair( vertexName, newv ) );
        return newv;
    }
    return (*itr).second;
}

void Graph::printPath( const Vertex & dest ) const
{
    if( dest.path != NULL )
    {
        printPath( *dest.path );
        cout << " to ";
    }
    cout << dest.name;
}

void Graph::clearAll( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        allVertices[ i ]->reset( );
}

Graph::~Graph( )
{
    for( int i = 0; i < allVertices.size( ); i++ )
        delete allVertices[ i ];
}

void Graph::dfs() 
{
  clearAll();
  int time = 0;
  Vertex *start = allVertices[0];
  start->dist = 0;
  for( int i = 0; i < allVertices.size( ); i++ ) {
    Vertex *start = allVertices[i];
    if ( start->dist == INFINITY || i==0) {
      dfs(start,time);
    }
  }
  for( int i = 0; i < allVertices.size( ); i++ ) {
    cout << allVertices[i]->name << " "<< allVertices[i]-> preN << " "<< allVertices[i]->postN << endl;
  }
}
void Graph::dfs(Vertex* vertex,int & time) {
  vertex->preN = time;
  PRINT(vertex->name, time);
  for ( int i = 0; i < vertex->adj.size(); i++ ) {
    Vertex *w = vertex->adj[ i ];
    if ( w->dist == INFINITY ) {
      time++;
      w->dist = vertex->dist + 1;
      w->path = vertex;
      dfs(w,time);
    }
  }
  vertex->postN = time;
}


void Graph::unweighted( const string & startName )// unweighted use breadth first search algorithm 
{
  clearAll( );// whenever a new start node is specified, reset all nodes weight to INFINITY and prev node to NULL
  // weight INFINITY can be used to detect whether or not the node has been visited
    vmap::iterator itr = vertexMap.find( startName );

    if( itr == vertexMap.end( ) )
    {
        cout << startName << " is not a vertex in this graph" << endl;
        return;
    }

    Vertex *start = (*itr).second;
    list<Vertex *> q;
    q.push_back( start ); start->dist = 0;

    while( !q.empty( ) )
    {
        Vertex *v = q.front( );	q.pop_front( );

        for( int i = 0; i < v->adj.size( ); i++ )
        {
            Vertex *w = v->adj[ i ];
            if( w->dist == INFINITY )
            {
                w->dist = v->dist + 1;
                w->path = v;
                q.push_back( w );
            }
        }
    }
}

/**
 * Process a request; return false if end of file.
 */
bool processRequest( istream & in, Graph & g )
{
    string startName;
    string destName;

    cout << "Enter start node: ";
    if( !( in >> startName ) )
        return false;
    cout << "Enter destination node: ";
    if( !( in >> destName ) )
        return false;

    // g.unweighted( startName );
    //  g.printPath( destName );
    cout << "DFS" << endl;
    g.dfs();
    g.printPath( destName );

    return true;
}


/**
 * A simple main that reads the file given by argv[1]
 * and then calls processRequest to compute shortest paths.
 * Skimpy error checking in order to concentrate on the basics.
 */
int main( int argc, char *argv[ ] )
{
    Graph g;

    if( argc != 2 )
    {
        cerr << "Usage: " << argv[ 0 ] << " graphfile" << endl;
        return 1;
    }

    ifstream inFile( argv[ 1 ] );
    if( !inFile )
    {
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }

    string source, dest;

    // Read the vertices; add them to vertexMap
    while( inFile >> source >> dest )
    {
        g.addEdge( source, dest );
    }

    cout << "File read" << endl;
    while( processRequest( cin, g ) )
        ;

    return 0;
}

