#include<fstream>
#include<iostream>
using namespace std;

void map_extractor(int rows, int columns, int * contour){
    char ch;

    fstream fin("C:\\Users\\codeNamuna\\Desktop\\glut\\bruh\\script\\contour_map.txt", fstream::in);
    for(int i=0;i<rows*columns;i++)
    {
        fin>>ch;
        contour[i]=int(ch-48);
    }

    fin.close();

}
