#include<fstream>
#include<iostream>
using namespace std;

void movement_extractor(int rows, int columns, int * moveout){
    char ch;
    int i=0;

    fstream fin("C:\\Users\\codeNamuna\\Desktop\\glut\\bruh\\script\\movement_map.txt", fstream::in);

    while(fin>>ch){
        moveout[i]=int(ch-48);
        i++;
    }

    fin.close();

}

