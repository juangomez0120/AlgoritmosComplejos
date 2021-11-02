#include <iostream>
#include <string>
#include <unordered_map>
#include <climits>

using namespace std;
#define MAX 21
#define INF INT_MAX

struct Nodo{
    string nombre;
    int x;
    int y;
    int id;
    int num;
};

void printMat(int mat[MAX][MAX], int m){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < m; j++){
            cout << mat[i][j] << "\t";
        }
        cout << endl;
    }
}

int main(){
    // n = cantidad de colonias
    // m = numero de conexiones entre colonias
    // q = futuras nuevas colonias que se desean conectar

    // Crear un diccionario para las primeras lineas, que son las que leen los nodos. 
    // El diccionario se usará para obtener los numeros de los nodos ya que se ingresan nombres.



    int n, m, q, x, y, id, costo;
    string colonia, colonia_a, colonia_b;
    unordered_map<string, Nodo> nodos;
    int matAdj[MAX][MAX];
    cin >> n >> m >> q;

    for (int i = 0; i < n; i++){
        cin >> colonia >> x >> y >> id;
        Nodo nodo;
        nodo.nombre = colonia;
        nodo.x = x;
        nodo.y = y;
        nodo.id = id;
        nodo.num = i;
        nodos[colonia] = nodo;
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
        matAdj[nodos[colonia_a].num][nodos[colonia_b].num] = matAdj[nodos[colonia_b].num][nodos[colonia_a].num] = costo;
    }
    printMat(matAdj, n);
    return 0;
}