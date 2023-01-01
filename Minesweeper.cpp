#include <iostream>
#include <string>
#include <vector>

#include <cmath>
#include <stdio.h>
using namespace std;


const enum CommandModes {Open, Mark, Unmark, Question, Help, Error};

struct Command{
    CommandModes mode;
    int x;
    int y;
};

const enum Errors{
    InvalidCommand = 0,
    DefaultError = -1,
};

int BoardSize = 3;

bool isNumber(const string& s)
{
    for (char const &ch : s) {
        if (isdigit(ch) == 0)
            return false;
    }
    return true;
}

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


vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}



void showHelp(){
    cout<<"The object of the game is to find and mark all mines"<<endl;
    cout<<"The numbers on the open squares show you how many adjacent mines does it have"<<endl<<endl;

    cout<<"You can use the following commands:"<<endl;
    cout<<"open X Y - open a field"<<endl;
    cout<<"mark X Y - mark a field as mine"<<endl;
    cout<<"unmark X Y - unmark field that you have marked"<<endl;
    cout<<"question X Y - mark a field with ?. Use when you are not sure what to do in this field"<<endl<<"(to use other command on this field you need to do it two times)"<<endl;
    cout<<"help - shows you this help menu"<<endl;
    cout<<"Press any key and enter to continue"<<endl;
    string buffer;
    getline(cin, buffer);
}

void throwError(Errors err){
    switch (err) {
        case InvalidCommand:
            cout << "Invalid command. Please try again." << endl;
            cout << "If you need help try typing \"help\"" << endl;
            return;
        default:
            cout<<"Something went wrong!";
    }


   }

CommandModes getMode(string mode){
    if(mode.compare("open") == 0){

        return Open;
    }
    if(mode.compare("mark") == 0){
        return Mark;
    }
    if(mode.compare("unmark") == 0){
        return Unmark;
    }
    if(mode.compare("question") == 0){
        return Question;
    }
    if(mode.compare("help") == 0){
        return Help;
    }
    return Error;
}
int getCoordinate(string input){
    int coord= -1;
    if(isNumber(input)){
        coord = stoi(input);
    }
    if(coord < 1 || coord>BoardSize) {
        coord = -1;
    }
    return  coord;
}
Command getCommand(){
    string input;
    cout<<"> ";
    getline(cin, input);

    vector<string> commands = split (input, " ");

    Command command;

    command.mode = getMode(commands[0]);
    switch (command.mode) {
        case Open:
        case Mark:
        case Unmark:
        case Question:
            if(commands.size()!=3) {
                throwError(InvalidCommand);
                return getCommand();
            }
            command.x = getCoordinate(commands[1]);
            command.y = getCoordinate(commands[2]);
            break;

        case Help:
            if(commands.size()!=1) {
                throwError(InvalidCommand);
                return getCommand();
            }
            command.x = -1;
            command.y = -1;
            break;
        default:
            throwError(InvalidCommand);
            return getCommand();
    }

    if(command.mode!=Help && (command.x == -1 || command.y == -1)) {
        throwError(InvalidCommand);
        return getCommand();
    }
    return command;
}

int main(){

    int state = 0;
    int mines = 1;
    //add char consts * ? ! empty
    char values [9] = {' ', '?', '*', ' ', '8', ' ', ' ', ' ', '!'};

    renderMinesweeper(BoardSize, state, mines, values);


    getCommand();


    return 0;
}