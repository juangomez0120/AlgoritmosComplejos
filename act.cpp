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

// Complejidad: O(n)
string manacher(string texto, int &inicio){
	string T = "";
	int n=texto.length();
	for (int i=0; i<n; i++){
		T += "|";
		T += texto[i];
	}
	T += "|";
	n = T.length();
	vector<int> L(n);
	L[0] = 0;
	L[1] = 1;
	int maxLong=0, maxCentro=0;
	bool exp;
	for (int c=1, li=0, ri=2; ri<n; ri++){
		li = c-(ri-c);
		exp = false;
		if (c-maxLong <= ri && ri >= c+maxLong){
			if (L[li] < (c+L[c]-ri)){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) == 2*n){
				L[ri] = L[li];
			}
			else if(L[li] == (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = L[li];
				exp = true;
			}
			else if(L[li] > (c + L[c]) - ri && (c + L[c]) < 2*n){
				L[ri] = (c+L[c]) - ri;
				exp = true;
			}
		}
		else{
			L[ri] = 0;
			exp = true;
		}
		if (exp){
			while((ri + L[ri] < n) && (ri - L[ri] > 0) && (T[ri - L[ri] - 1] == T[ri + L[ri] + 1])){
				L[ri]++;
			}
		}
		c = ri;
		if (L[ri] > maxLong){
	    	maxLong = L[ri];
	    	maxCentro = ri;
	    }
	}
	inicio = (maxCentro-maxLong)/2;
	string salida = "";
	for (int i=inicio; i<(inicio+maxLong); i++){
		salida += texto[i];
	}
	
	return salida;
}

string lcs(int mat[1000][1000], string s1, string s2){
    int len1 = s1.length() >= 1000 ? 1000 : s1.length(), len2 = s2.length() >= 1000 ? 1000 : s2.length(), maxLen = 0, indiceFinal = -1;

    for(int i = 0; i < len1; i++){
        if(s1[i] == s2[0]){
            mat[i][0] = 1;
            maxLen = 1;
        }
        else
            mat[i][0] = 0;
    }

    for(int j = 1; j < len2; j++){
        if(s2[j] == s1[0]){
            mat[0][j] = 1;
            maxLen = 1;
        }
        else
            mat[0][j] = 0;
    }

    for(int i = 1; i < len1; i++){
        for(int j = 1; j < len2; j++){
            if(s1[i] == s2[j]){
                mat[i][j] = mat[i-1][j-1] + 1;
                if(mat[i][j] > maxLen){
                    maxLen = mat[i][j];
                    indiceFinal = i+1;
                }
            }
            else
                mat[i][j] = 0;
        }

    }


    return maxLen == 0 ? "No hay substrings en común." : s1.substr(indiceFinal - maxLen, maxLen);
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

    check << DIVISOR2 << endl;
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

        check << (i < mcodes.size()-1 ? DIVISOR1 : DIVISOR2) << endl;
    }

    // Impresión de datos: Palíndromo más grande
    string palindromo;
    int posicion = 0;
    check << "Palíndromo más grande:" << endl;
    for(int i = 0; i <= 2; i++){
        palindromo = manacher(palindromo, posicion);
        check << "transmission" << i+1 << ".txt ==> ";
        
        if(palindromo != ""){
            check << "Posición: " << posicion << endl;
            check << palindromo << endl;
        }
        else
            check << "No hay palíndromos." << endl;

        check << (i < 2 ? DIVISOR1 : DIVISOR2) << endl;
    }

    // Impresión de datos: Substrings más largos
    int mat[1000][1000];
    check << "Substrings en común más largos:" << endl;
    check << "transmission1.txt & transmission2.txt ==> " << lcs(mat, transmissions[0], transmissions[1]) << endl;
    check << DIVISOR1 << endl;
    check << "transmission1.txt & transmission3.txt ==> " << lcs(mat, transmissions[0], transmissions[2]) << endl;
    check << DIVISOR1 << endl;
    check << "transmission2.txt & transmission3.txt ==> " << lcs(mat, transmissions[1], transmissions[2]) << endl;
    check << DIVISOR2 << endl;
    
    check.close();

    return 0;
}