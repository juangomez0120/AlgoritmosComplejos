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

// COMPILAR USANDO: "g++ -std=c++14 evidencia2.cpp -o evidencia2"

#include <cfloat>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
#define MAX 21
#define INF INT_MAX
#define DIVISOR "-------------------"

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

void printMat(int mat[MAX][MAX], int m){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < m; j++){
            cout << (mat[i][j] != INF ? to_string(mat[i][j]) : "INF") << "\t";
        }
        cout << endl;
    }
}

// Función para calcular la distancia entre dos puntos cartecianos
// Complejidad: O(1)
double calcDistance(Node col1, Node col2){
    return sqrt((col1.x-col2.x) * (col1.x-col2.x) + (col1.y-col2.y) * (col1.y-col2.y));
}

// Función para determinar en dónde se planea conectar nuevas colonias (punto 5)
// Complejidad: O(nq)
void connectNewColonies(unordered_map<string, Node> colonias, unordered_map<string, Node> coloniasNuevas, ofstream &check){
    Node conexion;
    double minDist = DBL_MAX, distance;
    check << "4 - Conexión de nuevas colonias." << endl << endl;
    for(pair<string, Node> colNueva : coloniasNuevas){
        for(pair<string, Node> col : colonias){
            distance = calcDistance(col.second, colNueva.second);
            if(distance < minDist){
                minDist = distance;
                conexion = col.second;
            }
        }
        check << colNueva.first << " debe conectarse con " << conexion.nombre << endl;
        minDist = DBL_MAX;
    }
    check << endl << DIVISOR;
}

// Función principal encargada de ejecutar el programa
int main(){
    // n = cantidad de colonias
    // m = número de conexiones entre colonias
    // q = futuras nuevas colonias que se desean conectar

    // Crear un diccionario para las primeras lineas, que son las que leen los nodos. 
    // El diccionario se usará para obtener los numéros de los nodos ya que se ingresan nombres.

    ofstream check("checking2.txt");
    int n, m, q, x, y, central, costo;
    string colonia, colonia_a, colonia_b;
    unordered_map<string, Node> colonias;
    unordered_map<string, Node> coloniasNuevas;
    int matAdj[MAX][MAX];
    
    check << DIVISOR << endl;
    cin >> n >> m >> q;

    for (int i = 0; i < n; i++){
        cin >> colonia >> x >> y >> central;
        colonias[colonia] = Node(colonia, x, y, central, i);
    }
    for (int i = 0; i<m; i++){
        for (int j = 0; j<m; j++){
            if (j == i){
                matAdj[i][j] = INF;
            }
            else{
                matAdj[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < m; i++){
        cin >> colonia_a >> colonia_b >> costo;
        matAdj[colonias[colonia_a].idx][colonias[colonia_b].idx] = matAdj[colonias[colonia_b].idx][colonias[colonia_a].idx] = costo;
    }

    for(int i = n; i < n+q; i++){
        cin >> colonia >> x >> y;
        coloniasNuevas[colonia] = Node(colonia, x, y, 0, i);
    }

    printMat(matAdj, n);
    connectNewColonies(colonias, coloniasNuevas, check);

    check.close();
    
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