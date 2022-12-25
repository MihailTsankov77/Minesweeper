#include <iostream>
#include <cmath>
using namespace std;


void renderStatsDisplay(int columns, int mines, int state){
    cout<<mines;
    int spaces = (columns/2)*4 - 1;
    if(columns%2==0){
        spaces-=2;
    }
    const int start = log10(mines) + 1;

    for(int i=start; i<spaces; i++){
        cout<<" ";
    }
    switch(state){
        case 0:
            cout<<"(*-*)";
            break;
        case 1:
            cout<<"(^-^)";
            break;
        case 2:
            cout<<"(O_o)";
            break;
    }
    cout<<endl<<endl;
}

void renderRowSeperator(int columns){
    for(int i=0; i<columns-1; i++){
        cout<<"___|";
    }
    cout<<"___";
    cout<<endl;
}

void renderLastRowSeperator(int columns){
    for(int i=0; i<columns-1; i++){
        cout<<"   |";
    }
    cout<<endl;
}


void renderRow(int columns, char * rowValues){
    cout<<" ";
    for(int i=0; i<columns-1; i++){
        cout<<rowValues[i];
        cout<<" | ";
    }
    cout<<rowValues[columns-1];
    cout<<endl;
}

void renderMinesweeper(int columns, int state, int mines, char * values){

    renderStatsDisplay(columns, mines, state);
    for(int i = 0; i<columns; i++){
        renderRow(columns, values + i*columns);
        if(i<columns-1){
            renderRowSeperator(columns);
        }else{
            renderLastRowSeperator(columns);
        }
    }
    cout<<endl;
}

int main(){

    int n = 3;
    int state = 0;
    int mines = 1;
    //add char consts * ? ! empty
    char values [9] = {' ', '?', '*', ' ', '8', ' ', ' ', ' ', '!'};


    renderMinesweeper(n, state, mines, values);

    return 0;
}
