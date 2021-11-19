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
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
#define MAX 30
#define INF INT_MAX
#define DIVISOR "-------------------"

// Estructura para representar un nodo
struct Node{
    string name;
    int x;
    int y;
    bool central;
    int idx;

    Node(){
        name = "";
        x = 0;
        y = 0;
        central = false;
        idx = 0;
    }

    Node(string name, int x, int y, int central, int idx){
        this->name = name;
        this->x = x;
        this->y = y;
        this->central = (central == 1);
        this->idx = idx;
    }

    bool operator < (const Node &other) const{
        return this->idx < other.idx;
    }

    double calcDistance(int, int);
};

// Estructura conteniendo los atributos de un nodo al resolver el problema del viajero
struct TSPNode{
    int lev, acumCost, posCost, currVertex;
    bool visited[MAX];
    bool operator<(const TSPNode &other) const{
        return posCost >= other.posCost;
    }
};

// Estructura para representar un grafo de nodos
struct Graph { 
    int V, E;
    int matAdj[MAX][MAX], floydMat[MAX][MAX], floydPath[MAX][MAX];
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
            matAdj[i][i] = floydMat[i][i] = floydPath[i][i] = 0;
            for (int j = i+1; j<V; j++){
                matAdj[i][j] = matAdj[j][i] = floydMat[i][j] = floydMat[j][i] = INF;
                floydPath[i][j] = floydPath[j][i] = -1;
            }
        }
    } 
    
    void addEdge(Node node1, Node node2, int cost) { 
        edges.push_back({cost,{node1, node2}});
        adjList[node1.idx].push_back({node2,cost});
        matAdj[node1.idx][node2.idx] = matAdj[node2.idx][node1.idx] = floydMat[node1.idx][node2.idx] = floydMat[node2.idx][node1.idx] = cost;
    } 
    
    void load();

    Node getCol(int idx){
        return vectorColonias[idx];
    }

    Node getCol(string col){
        return hashColonias[col];
    }

    void optimalConnections(ofstream&);
    void calcPossibleCost(TSPNode&);
    void optimalRoute(ofstream&);
    void getShortestRoute(int, int, ofstream&);
    void shortestRoute(ofstream&);
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

// Función que implementa el algoritmo de Kruskal para encontrar el cableado óptimo de la nueva conexión (punto 1)
// Complejidad: O(E log E)
void Graph::optimalConnections(ofstream &check){ 
    int cost = 0;
    sort(edges.begin(), edges.end());
    DisjointSets ds(V);

    
    for(auto it:edges){
        int u = it.second.first.idx;
        int v = it.second.second.idx;
        int set_u = ds.find(u);
        int set_v = ds.find(v);
        if(set_u != set_v){
            ds.merge(u, v);
            cost += it.first;
            check << getCol(u).name << " - " << getCol(v).name << " " << it.first << endl;
        }
    }
    check << endl << "Costo Total: " << cost << endl;

} 

// Función para calcular el menor costo posible tomando la ruta desde el nodo origen hasta el nodo actual
// Complejidad: O(n)
void Graph::calcPossibleCost(TSPNode &currNode){
    currNode.posCost = currNode.acumCost;
    int obtCost;
    for(int i = 0; i < V; i++){
        obtCost = INF;
        if(!currNode.visited[i] || i == currNode.currVertex){
            if(!currNode.visited[i]){
                for(int j = 0; j < V; j++){
                    if(i != j && (!currNode.visited[j] || j == 0)){
                        obtCost = min(obtCost, matAdj[i][j]);
                    }
                }
            }
            else{
                for(int j = 0; j < V; j++){
                    if(!currNode.visited[j]){
                        obtCost = min(obtCost, matAdj[i][j]);
                    }
                }
            }
            currNode.posCost += obtCost;
        }
    }
}

// Función que implementa el problema del viajero para encontrar la ruta óptima que pase por todas las colonias no centrales (punto 2)
// Complejidad: O(2^n)
void Graph::optimalRoute(ofstream &check){
    int optimalCost = INF, iter = 0;
    TSPNode root;
    root.lev = 0;
    root.acumCost = 0;
    root.currVertex = 0;
    for(int i = 0; i < V; i++){
        root.visited[i] = false;
    }
    root.visited[0] = true;
    calcPossibleCost(root);
    
    priority_queue<TSPNode> pq;
    pq.push(root);
    while(!pq.empty()){
        root = pq.top();
        pq.pop();
        if(root.posCost < optimalCost){
            for(int i = 0; i < V; i++){
                if(!root.visited[i] && matAdj[root.currVertex][i] != INF){
                    TSPNode connection = root;
                    connection.lev = root.lev + 1;
                    connection.acumCost = root.acumCost + matAdj[root.currVertex][i];
                    connection.currVertex = i;
                    connection.visited[i] = true;

                    if(connection.lev == V-1){
                        if(matAdj[connection.currVertex][0] != INF && connection.acumCost + matAdj[connection.currVertex][0] < optimalCost)
                            optimalCost = connection.acumCost + matAdj[connection.currVertex][0];
                    }
                    else{
                        calcPossibleCost(connection);
                        if(connection.posCost < optimalCost)
                            pq.push(connection);
                    }
                }
            }
        }
    }

    check << "La Ruta Óptima tiene un costo total de: " << optimalCost << endl;
    check << endl << DIVISOR << endl;
}

// Función auxiliar a shortestRoute encargada de hacer las consultas y desplegar las rutas entre colonias centrales
// Complejidad: O(n)
void Graph::getShortestRoute(int a, int b, ofstream& check){
    if(floydPath[a][b] != -1){
        getShortestRoute(a, floydPath[a][b], check);
        check << getCol(floydPath[a][b]).name << " - ";
        getShortestRoute(floydPath[a][b], b, check);
    }
}

// Función que implementa el algoritmo de floyd-warshall para encontrar la ruta óptima entre las colonias centrales (punto 3)
// Complejidad: O(n^3)
void Graph::shortestRoute(ofstream& check){
    for(int k = 0; k < V; k++){
        for(int i = 0; i < V; i ++){
            for(int j = 0; j < V; j++){
                if(floydMat[i][k] != INF && floydMat[k][j] != INF && floydMat[i][k]+floydMat[k][j] < floydMat[i][j]){
                    floydMat[i][j] = floydMat[i][k] + floydMat[k][j];
                    floydPath[i][j] = k;
                }
            }
        }
    }

    for(int i = 0; i < V; i++){
        if(!getCol(i).central)
            i++;
        else{
            for(int j = i+1; j < V; j++){
                if(getCol(j).central){
                    if(floydMat[i][j] == INF)
                        check << "No existe un camino de " << getCol(i).name << " a " << getCol(j).name << endl;
                    else{
                        check << getCol(i).name << " - ";
                        getShortestRoute(i, j, check);
                        check << getCol(j).name << " (" << floydMat[i][j] << ")" << endl;
                    }
                }
            }
        }
    }
} 

// Función para calcular la distancia entre dos puntos cartecianos
// Complejidad: O(1)
double Node::calcDistance(int x, int y){
    return sqrt((this->x-x) * (this->x-x) + (this->y-y) * (this->y-y));
}

// Función para determinar en dónde se planea conectar nuevas colonias (punto 4)
// Complejidad: O(nq)
void Graph::connectNewColonies(int q, ofstream &check){
    string colonia;
    int x, y;
    double minDist = DBL_MAX, distance;
    Node conexion;
    
    for(int i = 1; i <= q; i++){
        cin >> colonia >> x >> y;
        for(int j = 0; j < vectorColonias.size(); j++){
            distance = getCol(j).calcDistance(x, y);
            if(distance < minDist){
                minDist = distance;
                conexion = getCol(j);
            }
        }
        check << colonia << " debe conectarse con " << conexion.name << endl;
        minDist = DBL_MAX;
    }
}

// Función principal encargada de ejecutar el programa
int main(){
    ofstream check("checking2.txt");
    int n, m, q;
    
    
    cin >> n >> m >> q;
    Graph g(n, m);
    g.load();

    check << DIVISOR << endl;
    check << "1 - Cableado óptimo de la nueva conexión." << endl << endl;
    g.optimalConnections(check);
    
    check << endl << DIVISOR << endl;
    check << "2 - La ruta óptima." << endl << endl;
    g.optimalRoute(check);
    
    check << endl << DIVISOR << endl;
    check << "3 - Caminos más cortos entre centrales." << endl << endl;
    g.shortestRoute(check);
    
    check << endl << DIVISOR << endl;
    check << "4 - Conexión de nuevas colonias." << endl << endl;
    g.connectNewColonies(q, check);
    check << endl << DIVISOR << endl;
    check.close();

    cout << "\nLos resultados han sido almacenados en el archivo \'checking2.txt\', dentro del mismo directorio.\n" << endl;
    
    return 0;
}