#include<iostream>
#include<vector>
#include<fstream>
#include<string>
using namespace std;

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

int main(){
    vector<string> codes;
    vector<string> transmissions;
    string tr, code, str, pat;
    int cont;
    ifstream file1("transmission1.txt");
    ifstream file2("transmission2.txt");
    ifstream file3("transmission3.txt");
    ifstream mcode("mcode.txt");
    ofstream sol("solucion.txt");
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
    sol<<"=============="<<endl;
   
    return 0;
}