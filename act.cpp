/*
 * TC2038: Análisis y diseño de algoritmos avanzados
 * Actividad Integradora 1
 * 
 * Cristóbal Alberto Escamilla Sada - A00827074
 * Javier Alejandro Banegas Moreno - A00827812
 * Juan Manuel Gómez Ortiz - A00828010
 * 
 * 29 de septiembre del 2021
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define DIVISOR1 "--------------"
#define DIVISOR2 "========================================="

vector<int> ZFunction(string str){
    int n = str.size();
    vector<int> vect(n, 0);
    for(int i = 1, l = 0, r = 0; i < n; i++){
        if(i <= r){
            vect[i] = min(r-1+1, vect[i]);
        }
        while(i+vect[i] < n && str[vect[i]] == str[i+vect[i]]){
            vect[i]++;
        }
        if(i+vect[i]-1 > r){
            l = i;
            r += vect[i]+1;
        }
    }
    return vect;
}

vector<int> PMP(string texto, string patron, int &cont){
    string general = patron + "$" + texto;
    vector<int> vect = ZFunction(general);
    vector<int> sal;
    for(int i = patron.length()+1; i < general.length(); i++){
        if(vect[i] == patron.length()){
            sal.push_back(i-patron.length()-1);
            cont++;
        }
    }
    return sal;
}

int LCS(int mat[1000][1000], string s1, string s2){
    int max;
    for (int i = 0; i < 1000; i++) {
        if (s1[i] == s2[0]) {
            mat[i][0] = 1;
            max = 1;
        }
        else {
            mat[i][0] = 0;
        }
    }
    for (int j = 0; j < 1000; j++) {
        if (s1[0] == s2[j]) {
            mat[0][j] = 1;
            max = 1;
        }
        else {
            mat[0][j] = 0;
        }
    }
    for (int i = 1; i < 1000 - 1; i++) {
        for (int j = 1; j < 1000 - 1; j++) {
            if (s1[i] == s2[j]) {
                mat[i][j] = mat[i - 1][j - 1] + 1;
                if (mat[i][j] > max) {
                    max = mat[i][j];
                }
            }
            else {
                mat[i][j] = 0;
            }
        }
    }
    return max;
}

void readData(vector<string> &mcodes, vector<string> &transmissions){
    string str;
    ifstream file1("transmission1.txt");
    ifstream file2("transmission2.txt");
    ifstream file3("transmission3.txt");
    ifstream mcode("mcode.txt");
    
    while(! mcode.eof()){
        getline(mcode, str);
        mcodes.push_back(str);
    }

    getline(file1, str);
    transmissions.push_back(str);
    getline(file2, str);
    transmissions.push_back(str);
    getline(file3, str);
    transmissions.push_back(str);

    file1.close();
    file2.close();
    file3.close();
    mcode.close();
}

int main(){
    vector<string> mcodes, transmissions;
    ofstream check("checking.txt");
    
    readData(mcodes, transmissions);

    // Impresión de datos: Incidencias de código malicioso
    int cont;
    vector<int> vect;

    for(int i = 0; i < mcodes.size(); i++){
        check << "Código: " << mcodes[i] << endl;
        for(int j = 0; j <= 2; j++){
            cont = 0; 
            vect = PMP(transmissions[j], mcodes[i], cont);
            check << "transmission" << j+1 << ".txt ==> " << cont << " veces" << endl;

            for(int k = 0; k < vect.size(); k++){
                if(k == vect.size()-1){
                    check << vect[k] << endl;
                }else{
                    check << vect[k] << ", ";
                }
            }
        }

        if(i < mcodes.size()-1)
            check << DIVISOR1 << endl;
        else
            check << DIVISOR2 << endl;
    }


    // Impresión de datos: Substrings más largos
    int mat[1000][1000];
    check << "Substrings más largos:" << endl;
    if (LCS(mat, transmissions[0], transmissions[1]))
    check << "transmission1.txt & transmission2.txt ==> " << LCS(mat, transmissions[0], transmissions[1]) << endl;
    check << DIVISOR1 << endl;
    check << "transmission1.txt & transmission3.txt ==> " << LCS(mat, transmissions[0], transmissions[2]) << endl;
    check << DIVISOR1 << endl;
    check << "transmission2.txt & transmission3.txt ==> " << LCS(mat, transmissions[1], transmissions[2]) << endl;
    check << DIVISOR2 << endl;
    
    check.close();

    return 0;
}