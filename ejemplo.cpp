#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){
    string s1 = "AABAABAABAABAAB";
    vector<string> vect;
    for (int i=0; i<s1.length(); i+=3){
        string str_temp = s1.substr(i, 3);
        vect.push_back(str_temp);
    }
    for (int i=0; i<vect.size(); i++){
        cout << vect[i] << endl;
    }
    return 0;
}