#ifndef COUNT_H
#define COUNT_H

#endif // COUNT_H
#include<iostream>
#include<fstream>
#include<QFile>
using namespace std;

int rowCount(string f){
    int count = 0;
    string line;

    ifstream file(f);
    while (getline(file, line) && count < 10000)
        count++;

    return count;
}

int colCount(string s){
    string f = "";
    ifstream infile(s);
    if (infile.good()){
        string sLine;
        getline(infile, sLine);
        f = sLine;
    }

    int n = f.length();
    int c = 0;
    bool a = false;
    for (int i=0; i< n; i++){
        if (f[i] == '"'){
            a = !a;
        }
        if (f[i] == ',' && a == false){
            c++;
        }
    }
    c=c+1;
    return c;
}
