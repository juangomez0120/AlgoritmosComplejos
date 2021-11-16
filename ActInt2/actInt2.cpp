/*
 * TC2038: Análisis y diseño de algoritmos avanzados
 * Actividad Integradora 2
 * 
 * Cristóbal Alberto Escamilla Sada - A00827074
 * Javier Alejandro Banegas Moreno - A00827812
 * Juan Manuel Gómez Ortiz - A00828010
 * 
 * 21 de noviembre del 2021
 */

// COMPILAR USANDO: "g++ -std=c++14 actInt2.cpp -o actInt2"

#include <cfloat>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
#define MAX 31
#define INF INT_MAX
#define DIVISOR "-------------------"

// Estructura para representar un nodo
struct Node{
    string nombre;
    int x;
    int y;
    bool central;
    int idx;

    Node(){
        nombre = "";
        x = 0;
        y = 0;
        central = false;
        idx = 0;
    }

    Node(string nombre, int x, int y, int central, int idx){
        this->nombre = nombre;
        this->x = x;
        this->y = y;
        this->central = (central == 1);
        this->idx = idx;
    }
};

// Estructura para representar un grafo
struct Graph { 
    int V, E;
    int matAdj[MAX][MAX];
    vector<Node> vectorColonias;
    vector< pair< int, pair<Node, Node> > > edges; 
    vector< vector< pair<Node, int> > > adjList;
    unordered_map<string, Node> hashColonias;
  
    Graph(int V, int E){ 
        this->V = V; 
        this->E = E;
        adjList.resize(V);
        vectorColonias.resize(V);

        for (int i = 0; i<V; i++){
            for (int j = 0; j<V; j++){
                if (j == i){
                    matAdj[i][j] = INF;
                }
                else{
                    matAdj[i][j] = 0;
                }
            }
        }
    } 
    
    void addEdge(Node node1, Node node2, int cost) { 
        edges.push_back({cost,{node1, node2}});
        adjList[node1.idx].push_back({node2,cost});
        matAdj[node1.idx][node2.idx] = matAdj[node2.idx][node1.idx] = cost;
    } 
    
    void load();

    Node getCol(int idx){
        return vectorColonias[idx];
    }

    Node getCol(string col){
        return hashColonias[col];
    }

    double calcDistance(int, int, Node);
    void connectNewColonies(int, ofstream&);

}; 
  
// Estructura para representar disjoint sets
struct DisjointSets 
{ 
    int *parent, *rnk; 
    int n; 
  
    DisjointSets(int n){ 
        this->n = n; 
        parent = new int[n+1]; 
        rnk = new int[n+1]; 
        for (int i = 0; i <= n; i++){ 
            rnk[i] = 0; 
            parent[i] = i; 
        } 
    } 

    int find(int u) 
    { 
        if (u != parent[u]) 
            parent[u] = find(parent[u]); 
        return parent[u]; 
    } 
  
    void merge(int x, int y) 
    { 
        x = find(x), y = find(y); 
  
        if (rnk[x] > rnk[y]) 
            parent[y] = x; 
        else
            parent[x] = y; 
  
        if (rnk[x] == rnk[y]) 
            rnk[y]++; 
    } 
};

// Función para crear el grafo
// Complejidad: O(m)
void Graph::load(){
    string col1, col2;
    Node nodo;
    int x, y, central, cost;

    for (int i = 0; i < V; i++){
        cin >> col1 >> x >> y >> central;
        nodo = Node(col1, x, y, central, i);
        hashColonias[col1] = nodo;
        vectorColonias[i] = nodo;
    }

    for(int i = 0; i < E; i++){
        cin >> col1 >> col2 >> cost;
        addEdge(getCol(col1), getCol(col2), cost);
    }
}

// Función para calcular la distancia entre dos puntos cartecianos
// Complejidad: O(1)
double Graph::calcDistance(int x, int y, Node col){
    return sqrt((x-col.x) * (x-col.x) + (y-col.y) * (y-col.y));
}

// Función para determinar en dónde se planea conectar nuevas colonias (punto 5)
// Complejidad: O(nq)
void Graph::connectNewColonies(int q, ofstream &check){
    string colonia;
    int x, y;
    double minDist = DBL_MAX, distance;
    Node conexion;
    
    check << "4 - Conexión de nuevas colonias." << endl << endl;
    for(int i = 1; i <= q; i++){
        cin >> colonia >> x >> y;
        for(int j = 0; j < vectorColonias.size(); j++){
            distance = calcDistance(x, y, getCol(j));
            if(distance < minDist){
                minDist = distance;
                conexion = getCol(j);
            }
        }
        check << colonia << " debe conectarse con " << conexion.nombre << endl;
        minDist = DBL_MAX;
    }

    check << endl << DIVISOR;
}

// Función principal encargada de ejecutar el programa
int main(){
    ofstream check("checking2.txt");
    int n, m, q;
    
    check << DIVISOR << endl;
    cin >> n >> m >> q;
    Graph g(n, m);

    g.load();
    g.connectNewColonies(q, check);
    check.close();

    cout << "\nLos datos han sido almacenados en el archivo \'checking2.txt\', dentro del mismo directorio.\n" << endl;
    
    return 0;
}


/*
CASO DE PRUEBA
5 8 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Independencia 180 -15
Roble 45 68
*/