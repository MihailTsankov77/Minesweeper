#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

enum CommandModes {
    Open,
    Mark,
    Unmark,
    Question,
    Help,
    Error
};

struct Command {
    CommandModes mode;
    int x;
    int y;
};

struct GameParams {
    int boardSize;
    int minesCount;
};

enum Errors {
    InvalidCommand,
    InvalidGameParams,
    CellAlreadyOpen,
    CellIsInThisState,
    CellIsMarked,
    DefaultError,
};

enum GameState {
    InProgress,
    Win,
    Loss
};

enum CellValue {
    Empty = ' ',
    Mine = '*',
    QuestionMark = '?',
    PossibleMine = '!'
};

char* generateArray (const int size, const char defaultValue){
    char *arr = new char[size];
    for (int j = 0; j < size; ++j) {
        arr[j] = defaultValue;
    }

    return arr;
}

char** generate2DArray (const int size, const char defaultValue){
    char **arr = new char* [size];
    for (int i = 0; i <size; ++i) {
        arr[i] = generateArray(size, defaultValue);
    }
    return arr;
}

void delete2DArray(char **arr, int size){
    for (int i = 0; i < size; ++i) {
        delete [] arr[i];
    }
    delete []arr;
}

bool isNumber(const char* arr, int size) {
    if(arr[0]=='0' && arr[1]!='\0'){
        return false;
    }
    for (int i = 0; arr[i]!='\0' && i<size; i++) {
        if (arr[i]<'0' || arr[i]>'9'){
            return false;
        }
    }
    return true;
}

char * resizeArr(char* arr, const int size){
    char* newArr = new char[size*2];
    for (int i = 0; i < size; ++i) {
        newArr[i] = arr[i];
    }
    delete [] arr;
    return newArr;
}

int split(const char* s, int size, const char delimiter, char** const arr) {

    int arrSize = 5;
    int inArr = 0;
    int subStringsTrack = 0;

    arr[subStringsTrack] = new char[5];

    for (int i = 0; s[i]!='\0' && i < size; ++i) {
        if(s[i]!=delimiter){
            if(inArr>=arrSize){
                arr [subStringsTrack] =resizeArr(arr[subStringsTrack], arrSize);
                arrSize*=2;
            }
            arr[subStringsTrack][inArr] = s[i];
            inArr++;
        }else{
            if(subStringsTrack<3) {
                subStringsTrack++;
                arr[subStringsTrack] = new char[5];
                inArr = 0;
                arrSize = 5;
            }
        }
    }

    for (int i = subStringsTrack+1; i < 4; ++i) {
        arr[i] = new char[5];
    }

    return subStringsTrack +1;
}

bool compareCharArr(const char * arr1, const char * arr2){
    for (int i = 0; arr1[i]!='\0' || arr2[i]!='\0'; ++i) {
        if(arr1[i]!=arr2[i]){
            return false;
        }
    }
    return true;
}

CommandModes getMode(const char * mode) {
    if (compareCharArr(mode, "open")) {
        return Open;
    }
    if (compareCharArr(mode, "mark")) {
        return Mark;
    }
    if (compareCharArr(mode, "unmark")) {
        return Unmark;
    }
    if (compareCharArr(mode, "question")) {
        return Question;
    }
    if (compareCharArr(mode, "help")) {
        return Help;
    }
    return Error;
}

int convertCharToInt(const char * arr){
    int num = 0;
    for(int i=0; arr[i]!='\0';i++){
        num *=10;
        num += (arr[i]-'0');
    }
    return num;
}

int getCoordinate(const char* input, const int boardSize) {
    int coord = -1;
    if (isNumber(input, 3)) {
        coord = convertCharToInt(input);
    }
    if (coord < 1 || coord > boardSize) {
        coord = -1;
    }
    return coord - 1;
}

GameState checkBoardState( char** const board,  char** const ValueBoard, const int boardSize) {
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            if ((ValueBoard[x][y] == Mine && board[x][y] != PossibleMine) ||
                (ValueBoard[x][y] != Mine && board[x][y] == PossibleMine)) {
                return InProgress;
            }
        }
    }
    return Win;
}

void generateBoard(char** ValueBoard, const int boardSize, const int minesCount) {
    int minesLeft = minesCount;
    srand(time(nullptr));
    while (minesLeft != 0) {
        int x = rand() % boardSize - 1;
        int y = rand() % boardSize - 1;
        if (x < 0 || x >= boardSize || y < 0 || y >= boardSize || ValueBoard[x][y] == Mine) {
            continue;
        }
        ValueBoard[x][y] = Mine;
        //Horizontal and Vertical
        if (x - 1 >= 0 && ValueBoard[x - 1][y] != Mine) {
            ValueBoard[x - 1][y] += 1;
        }
        if (x + 1 < boardSize && ValueBoard[x + 1][y] != Mine) {
            ValueBoard[x + 1][y] += 1;
        }
        if (y - 1 >= 0 && ValueBoard[x][y - 1] != Mine) {
            ValueBoard[x][y - 1] += 1;
        }
        if (y + 1 < boardSize && ValueBoard[x][y + 1] != Mine) {
            ValueBoard[x][y + 1] += 1;
        }
        //Diagonals
        if (x - 1 >= 0) {
            if (y - 1 >= 0 && ValueBoard[x - 1][y - 1] != Mine) {
                ValueBoard[x - 1][y - 1] += 1;
            }
            if (y + 1 < boardSize && ValueBoard[x - 1][y + 1] != Mine) {
                ValueBoard[x - 1][y + 1] += 1;
            }
        }
        if (x + 1 < boardSize) {
            if (y - 1 >= 0 && ValueBoard[x + 1][y - 1] != Mine) {
                ValueBoard[x + 1][y - 1] += 1;
            }
            if (y + 1 < boardSize && ValueBoard[x + 1][y + 1] != Mine) {
                ValueBoard[x + 1][y + 1] += 1;
            }
        }
        minesLeft--;
    }
}

void renderStatsDisplay(int mines, GameState state, const int boardSize) {

    int spaces = (boardSize / 2) * 4 - 1;
    if (boardSize % 2 == 0) {
        spaces -= 2;
    }
    const int start = log10(mines == 0 ? 1 : mines) + 1;

    cout << mines;
    for (int i = start; i < spaces; i++) {
        cout << " ";
    }
    switch (state) {
        case InProgress:
            cout << "(*-*)";
            break;
        case Win:
            cout << "(^-^)";
            break;
        case Loss:
            cout << "(O_o)";
            break;
    }
    cout << endl << endl;
}

void renderRowSeperator(const int boardSize) {
    for (int i = 0; i < boardSize - 1; i++) {
        cout << "___|";
    }
    cout << "___";
    cout << endl;
}

void renderLastRowSeperator(const int boardSize) {
    for (int i = 0; i < boardSize - 1; i++) {
        cout << "   |";
    }
    cout << endl;
}


void renderRow(const char* rowValues, const int boardSize) {
    cout << " ";
    for (int i = 0; i < boardSize - 1; i++) {
        cout << rowValues[i];
        cout << " | ";
    }
    cout << rowValues[boardSize - 1];
    cout << endl;
}

void renderBoard(const GameState state, const int mines, char** const board, const int boardSize) {

    renderStatsDisplay(mines, state, boardSize);
    for (int i = 0; i < boardSize; i++) {
        renderRow(board[i], boardSize);
        if (i < boardSize - 1) {
            renderRowSeperator(boardSize);
        } else {
            renderLastRowSeperator(boardSize);
        }
    }
    cout << endl;
}

void throwError(Errors err) {
    switch (err) {
        case InvalidCommand:
            cout << "Invalid command. Please try again." << endl;
            cout << "If you need help try typing \"help\"" << endl;
            return;
        case InvalidGameParams:
            cout << "Invalid game params. Please input params that satisfy the instructions" << endl;
            return;
        case CellAlreadyOpen:
            cout << "You can not take actions on a open cell." << endl;
            return;
        case CellIsInThisState:
            cout << "Cell is already in this state." << endl;
            return;
        case CellIsMarked:
            cout << "Cell is already marked if you want to take actions, first unmark it." << endl;
            return;
        default:
            cout << "Something went wrong!" << endl;;
    }
}

bool continueGame() {
    cout << "Y/N" << endl;
    string answer;
    cin >> answer;
    if (answer.compare("Y") == 0 || answer.compare("y") == 0) {
        return true;
    }
    if (answer.compare("N") == 0 || answer.compare("n") == 0) {
        return false;
    }
    throwError(InvalidCommand);
    return continueGame();
}

GameParams getGameParams() {
    cout << "Select board size." << endl;
    cout << "Your options are between 3 and 10" << endl;
    char boardSizeInput[5];
    cin >> boardSizeInput;
    int boardSize = -1;
    if (!isNumber(boardSizeInput, 5)) {
        throwError(InvalidGameParams);
        return getGameParams();
    } else {
        const int boardSizeBuffer = convertCharToInt(boardSizeInput);
        if (boardSizeBuffer < 3 || boardSizeBuffer > 10) {
            throwError(InvalidGameParams);
            return getGameParams();
        }
        boardSize = boardSizeBuffer;
    }

    const int maxMines = 3 * boardSize;
    cout << "Select mine count." << endl;
    cout << "Your options are between 1 and " << maxMines << endl;
    char minesInput[4];
    cin >> minesInput;
    int minesCount = -1;
    if (!isNumber(minesInput, 4)) {
        throwError(InvalidGameParams);
        return getGameParams();
    } else {
        const int minesBuffer = stoi(minesInput);
        if (minesBuffer < 1 || minesBuffer > maxMines) {
            throwError(InvalidGameParams);
            return getGameParams();
        }
        minesCount = minesBuffer;
    }
    cin.ignore();
    return {
        boardSize: boardSize,
        minesCount: minesCount,
    };
}

void setCell(char &cell, const CommandModes mode, int* minesCount) {
    if (cell >= '0' && cell <= '9') {
        throwError(CellAlreadyOpen);
        return;
    }

    switch (mode) {
        case Question:
            cell = QuestionMark;
            return;
        case Unmark:
            if (cell == Empty) {
                throwError(CellIsInThisState);
                return;
            }
            if (cell == PossibleMine) {
                *minesCount += 1;
            }
            cell = Empty;
            return;
        case Mark:
            if (cell == PossibleMine) {
                throwError(CellIsInThisState);
                return;
            }
            if (cell == QuestionMark) {
                cell = Empty;
                return;
            }
            cell = PossibleMine;
            *minesCount -= 1;
            return;
        default:
            throwError(DefaultError);
    }
}

void openCell(const int x, const int y, char** board,  char**const ValueBord, const int boardSize, const bool showErrors = true) {
    if (x < 0 || x >= boardSize || y < 0 || y >= boardSize) {
        if (showErrors) {
            throwError(DefaultError);
        }
        return;
    }

    char *cell = &board[x][y];
    const char value = ValueBord[x][y];
    if (*cell >= '0' && *cell <= '9') {
        if (showErrors) {
            throwError(CellAlreadyOpen);
        }
        return;
    }
    switch (*cell) {
        case Empty:
            *cell = value;
            if (value == '0') {
                openCell(x - 1, y, board, ValueBord, boardSize, false);
                openCell(x - 1, y - 1, board, ValueBord, boardSize, false);
                openCell(x - 1, y + 1, board, ValueBord, boardSize, false);
                openCell(x + 1, y, board, ValueBord, boardSize, false);
                openCell(x + 1, y - 1, board, ValueBord, boardSize, false);
                openCell(x + 1, y + 1, board, ValueBord, boardSize, false);
                openCell(x, y + 1, board, ValueBord, boardSize, false);
                openCell(x, y - 1, board, ValueBord, boardSize, false);
            }
            break;
        case PossibleMine:
            if(showErrors){
                throwError(CellIsMarked);
            }
            break;
        case QuestionMark:
            *cell = Empty;
            break;
        default:
            throwError(DefaultError);
    }
}

void showHelp() {
    cout << "The object of the game is to find and mark all mines" << endl;
    cout << "The numbers on the open squares show you how many adjacent mines does it have" << endl << endl;

    cout << "You can use the following commands:" << endl;
    cout << "open X Y - open a field" << endl;
    cout << "mark X Y - mark a field as mine" << endl;
    cout << "unmark X Y - unmark field that you have marked" << endl;
    cout << "question X Y - mark a field with ?. Use when you are not sure what to do in this field" << endl
         << "(to use other command on this field you need to do it two times)" << endl;
    cout << "help - shows you this help menu" << endl;
    cout << "Press any key and enter to continue" << endl;
    char buffer[2];

    cin.getline(buffer, 2);
}

Command getCommand(const int boardSize) {
    cout << "> ";
    char input [20];
    cin.getline(input, 20);

    char** commands = new char*[4];
    int commandArrSize = split(input, 20, ' ', commands);
    Command command;

    command.mode = getMode(commands[0]);
    switch (command.mode) {
        case Open:
        case Mark:
        case Unmark:
        case Question:
            if (commandArrSize != 3) {
                throwError(InvalidCommand);
                delete2DArray(commands, 4);
                return getCommand(boardSize);
            }
            command.x = getCoordinate(commands[1], boardSize);
            command.y = getCoordinate(commands[2], boardSize);
            break;

        case Help:
            if (commandArrSize != 1) {
                throwError(InvalidCommand);
                delete2DArray(commands, 4);
                return getCommand(boardSize);
            }
            command.x = -1;
            command.y = -1;
            break;
        default:
            throwError(InvalidCommand);
            delete2DArray(commands, 4);
            return getCommand(boardSize);
    }
    if (command.mode != Help && (command.x < 0 || command.y < 0)) {
        throwError(InvalidCommand);
        delete2DArray(commands, 4);
        return getCommand(boardSize);
    }
    delete2DArray(commands, 4);
    return command;
}

GameState
handleUserCommand(const Command command, char** board, char** const ValueBord, const int boardSize, int * minesCount) {
    switch (command.mode) {
        case Open:
            if (ValueBord[command.x][command.y] == Mine) {
                return Loss;
            }
            openCell(command.x, command.y, board, ValueBord, boardSize);
            break;
        case Question:
        case Mark:
        case Unmark:
            setCell(board[command.x][command.y], command.mode, minesCount);
            break;
        case Help:
            showHelp();
            break;
        default:
            throwError(DefaultError);
            return Loss;
    }

    return InProgress;
}

void initGame() {
    GameParams params = getGameParams();
    char **board = generate2DArray(params.boardSize, Empty);
    char **valueBoard = generate2DArray(params.boardSize, '0');

    generateBoard(valueBoard, params.boardSize, params.minesCount);
    GameState state = InProgress;

    while (state == InProgress) {
        renderBoard(InProgress, params.minesCount, board, params.boardSize);
        Command command = getCommand(params.boardSize);
        state = handleUserCommand(command, board, valueBoard, params.boardSize, &params.minesCount);
        if (params.minesCount == 0) {
            state = checkBoardState(board, valueBoard, params.boardSize);
        }
    }

    if (state == Loss) {
        renderBoard(state, params.minesCount, valueBoard, params.boardSize);
        cout << "Ops you hit a mine. Loser. Try again!" << endl;
    } else if (state == Win) {
        renderBoard(state, params.minesCount, board, params.boardSize);
        cout << "You win. Loser. Try again?" << endl;
    }

    delete2DArray(board, params.boardSize);
    delete2DArray(valueBoard, params.boardSize);
}

int main() {
    do {
        initGame();
    } while (continueGame());

    cout << endl<< "Nice knowing you!";
    return 0;
}
