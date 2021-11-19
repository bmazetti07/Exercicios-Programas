#include <bits/stdc++.h>
using namespace std;

int main (int argc, char **argv) {
    fstream sistemaArq; string nomeArquivo = "anything";

    sistemaArq.open (nomeArquivo, ios::out);

    for (int i = 0; i < 50000000; i ++)
        sistemaArq << "a";
    /* string dir = "/";
    int contador = 0;
    for (int i = 0; i < 30; i ++) {
        sistemaArq << "mkdir ";
        dir = dir + "dir" + to_string (i) + "/";
        sistemaArq << dir << endl;

        string arq = dir;
        for (int j = 0; j < 30; j ++) {
            sistemaArq << "cp /t " << dir << "arq" << contador << endl;
            contador ++;
        }
    } */
    
    sistemaArq.close ();

    return 0;
}