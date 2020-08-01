#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main(){
    char c;
    ifstream readFile;
    readFile.open("CODE_simulator_2D_XO_KINADV_BornHeun_tINDEP.cpp");
    while(readFile.peek()!=EOF){
        if (readFile.peek()=='\n') cout << "\\n";
        if(readFile.peek()==' ') cout <<" ";
        readFile >> c;
        if(c=='"') cout <<"\\\"";
        else cout << c;
        
    }
    readFile.close();
    return 0;
}
