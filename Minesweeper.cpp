#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
using namespace std;

//add restart
enum CommandModes {Open, Mark, Unmark, Question, Help, Error};

struct Command{
    CommandModes mode;
    int x;
    int y;
};

enum Errors{
    InvalidCommand,
    InvalidGameParams,
    CellAlreadyOpen,
    CellIsInThisState,
    CellIsMarked,
    DefaultError,
};

enum GameState{
    InProgress,
    Win,
    Lose
};

enum CellValue{
    Empty = ' ',
    Mine = '*',
    QuestionMark = '?',
    PossibleMine = '!'
};


int BoardSize;
int MinesCount;

bool isNumber(const string& s)
{
    for (char const &ch : s) {
        if (isdigit(ch) == 0)
            return false;
    }
    return true;
}

void renderStatsDisplay(int mines, GameState state){

    cout<<mines;
    int spaces = (BoardSize/2)*4 - 1;
    if(BoardSize%2==0){
        spaces-=2;
    }
    const int start = log10(mines) + 1;

    for(int i=start; i<spaces; i++){
        cout<<" ";
    }
    switch(state){
        case InProgress:
            cout<<"(*-*)";
            break;
        case Win:
            cout<<"(^-^)";
            break;
        case Lose:
            cout<<"(O_o)";
            break;
    }
    cout<<endl<<endl;
}

void renderRowSeperator(){
    for(int i=0; i<BoardSize-1; i++){
        cout<<"___|";
    }
    cout<<"___";
    cout<<endl;
}

void renderLastRowSeperator(){
    for(int i=0; i<BoardSize-1; i++){
        cout<<"   |";
    }
    cout<<endl;
}


void renderRow(const vector<char> rowValues){
    cout<<" ";
    for(int i=0; i<BoardSize-1; i++){
        cout<<rowValues[i];
        cout<<" | ";
    }
    cout<<rowValues[BoardSize-1];
    cout<<endl;
}

void renderBoard(const GameState state, const int mines, const vector<vector<char> > board){

    renderStatsDisplay(mines, state);
    for(int i = 0; i<BoardSize; i++){
        renderRow(board[i]);
        if(i<BoardSize-1){
            renderRowSeperator();
        }else{
            renderLastRowSeperator();
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
        case InvalidGameParams:
            cout<<"Invalid game params. Please input params that satisfy the instructions"<< endl;
            return;
        case CellAlreadyOpen:
            cout<<"You can not take actions on a open cell."<<endl;
            return;
        case CellIsInThisState:
            cout<<"Cell is already in this state."<<endl;
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
    if(coord < 0 || coord>=BoardSize) {
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

void getGameParams(){
    cout<<"Select board size."<<endl;
    cout<<"Your options are between 3 and 10"<<endl;

    cin>>BoardSize;
    if(cin.bad() || BoardSize<3 || BoardSize>10){
        cin.clear();
        cin.ignore(); //TODO: fix
        throwError(InvalidGameParams);
        return getGameParams();
    }
    int maxMines = 3*BoardSize;
    cout<<"Select mine count."<<endl;
    cout<<"Your options are between 1 and "<<maxMines<<endl;
    cin>>MinesCount;
    if(cin.bad()||MinesCount<1 || MinesCount>maxMines){
        cin.clear();
        cin.ignore();
        throwError(InvalidGameParams);
        return getGameParams();
    }
    cin.ignore();
}

void setCell(char& cell, const CommandModes mode ){
    if(cell >= '0' && cell <= '9'){
        throwError(CellAlreadyOpen);
        return;
    }

    switch (mode) {
        case Question:
            cell = QuestionMark;
            return;
        case Unmark:
            if(cell == Empty){
                throwError(CellIsInThisState);
                return;
            }
            //Mine count goes up
            cell = Empty;
        case Mark:
            if(cell == Mark){
                throwError(CellIsInThisState);
                return;
            }
            if(cell == Question){
                cell = Empty;
                return;
            }
            //Mine count goes down
            cell = PossibleMine;
        default:
            throwError(DefaultError);
    }
}

void openCell(char& cell, const char value){
    //TODO: when open 0 open neighbor cells
    if(cell >= '0' && cell <= '9'){
        throwError(CellAlreadyOpen);
        return;
    }

    switch (cell) {
        case Empty:
            cell = value;
            break;
        case Mark:
            throwError(CellIsMarked);
            break;
        case QuestionMark:
            cell = Empty;
            break;
        default:
            throwError(DefaultError);
    }
}

GameState handleUserCommand(const Command command, vector<vector<char> >& board, const vector<vector<char> > & ValueBord){
    switch (command.mode) {
        case Open:
            if(ValueBord[command.x][command.y]==Mine){
                return Lose;
            }
            openCell(board[command.x][command.y], ValueBord[command.x][command.y]);
        case Question:
        case Mark:
        case Unmark:
            setCell(board[command.x][command.y], command.mode);
            break;
        case Help:
            showHelp();
            break;
        default:
            throwError(DefaultError);
            return Lose;
    }

    return InProgress;
}

void generateBoard(vector<vector<char> >& ValueBoard){
    int minesLeft = MinesCount;
    //TODO: change to c++11 random
    srand(time(nullptr));
    while( minesLeft!=0){
        int x = rand()%BoardSize-1;
        int y = rand()%BoardSize-1;
        if(x<0 || x>=BoardSize || y<0 || y>=BoardSize || ValueBoard[x][y]==Mine){
            continue;
        }
        ValueBoard[x][y]= Mine;
        //Horizontal and Vertical
        if(x-1>=0 && ValueBoard[x-1][y]!=Mine){
            ValueBoard[x-1][y] += 1;
        }
        if(x+1<BoardSize && ValueBoard[x+1][y]!=Mine){
            ValueBoard[x+1][y] += 1;
        }
        if(y-1>=0 && ValueBoard[x][y-1]!=Mine){
            ValueBoard[x][y-1] += 1;
        }
        if(y+1<BoardSize && ValueBoard[x][y+1]!=Mine){
            ValueBoard[x][y+1] += 1;
        }
        //Diagonals
        if(x-1>=0){
            if(y-1>=0 && ValueBoard[x-1][y-1]!=Mine){
                ValueBoard[x-1][y-1] += 1;
            }
            if(y+1<BoardSize && ValueBoard[x-1][y+1]!=Mine){
                ValueBoard[x-1][y+1] += 1;
            }
        }
        if(x+1<BoardSize){
            if(y-1>=0 && ValueBoard[x+1][y-1]!=Mine){
                ValueBoard[x+1][y-1] += 1;
            }
            if(y+1<BoardSize && ValueBoard[x+1][y+1]!=Mine){
                ValueBoard[x+1][y+1] += 1;
            }
        }
        minesLeft--;
    }
}

int main(){
    getGameParams();
    int mines = 1;
    vector<vector<char> > board ( BoardSize , vector<char> (BoardSize, Empty));
    vector<vector<char> > ValueBoard ( BoardSize , vector<char> (BoardSize, '0'));


    generateBoard(ValueBoard);

    renderBoard(InProgress, mines, board);

    handleUserCommand(getCommand(), board, ValueBoard);


    renderBoard(InProgress, mines, board);


    return 0;
}