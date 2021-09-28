#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<algorithm>
using namespace std;

#define MAX 1000

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

int LCS(int mat[MAX][MAX], string s1, string s2){
    int max;
    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] == s2[0]) {
            mat[i][0] = 1;
            max = 1;
        }
        else {
            mat[i][0] = 0;
        }
    }
    for (int j = 0; j < s2.length(); j++) {
        if (s1[0] == s2[j]) {
            mat[0][j] = 1;
            max = 1;
        }
        else {
            mat[0][j] = 0;
        }
    }
    for (int i = 1; i < s1.length() - 1; i++) {
        for (int j = 1; j < s2.length() - 1; j++) {
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

int main(){
    // Definición de variables
    vector<string> codes; // Codigos en mconde
    vector<string> transmissions; // Strings de cada archivo transmission
    string tr, code, str, pat;  //tr => transmision
                                //code => codigo
    int cont;
    ifstream file1("transmission1.txt");
    ifstream file2("transmission2.txt");
    ifstream file3("transmission3.txt");
    ifstream mcode("mcode.txt");
    ofstream sol("solucion.txt");

    // Lectrua de datos
    while(! mcode.eof()){
        getline(mcode, code);
        codes.push_back(code);
    }
    getline(file1, tr);
    transmissions.push_back(tr);
    getline(file2, tr);
    transmissions.push_back(tr);
    getline(file3, tr);
    transmissions.push_back(tr);

    // Impresion de datos / ya con la busqueda ZFunction
    for(int i = 0; i<codes.size(); i++){
        sol<<"Codigo: "<<codes[i]<<endl;
        vector<int> vect = PMP(transmissions[0], codes[i], cont);
        sol<<"transmission1.txt ==> "<<cont<<" veces"<<endl;
        for(int i =0; i<vect.size(); i++){
            if(i == vect.size()-1){
                sol<<vect[i]<<endl;
            }else{
                sol<<vect[i]<<", ";
            }
        }
        vect.clear();
        cont = 0;
        vect = PMP(transmissions[1], codes[i], cont);
        sol<<"transmission2.txt ==> "<<cont<<" veces"<<endl;
        for(int i =0; i<vect.size(); i++){
            if(i == vect.size()-1){
                sol<<vect[i]<<endl;
            }else{
                sol<<vect[i]<<", ";
            }
        }
        vect.clear();
        cont =0;
        vect = PMP(transmissions[2], codes[i], cont);
        sol<<"transmission3.txt ==> "<<cont<<" veces"<<endl;
        for(int i =0; i<vect.size(); i++){
            if(i == vect.size()-1){
                sol<<vect[i]<<endl;
            }else{
                sol<<vect[i]<<", ";
            }
        }
        vect.clear();
        cont =0;
        sol<<"--------------"<<endl;
    }

    // Comparaciones entre strings
    // Comparacion t1-t2
    int mat[MAX][MAX];
    cout << "hola" << endl;
    cout << LCS(mat, transmissions[0], transmissions[1]) << endl;
    cout << "hola" << endl;
    sol<<"=============="<<endl;
    return 0;
}