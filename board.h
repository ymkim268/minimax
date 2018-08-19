//
//  board.h
//  fruitrage
//
//  Created by Young Kim on 10/3/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef board_h
#define board_h

#include <vector>
#include <set>
#include <list>
#include <map>

using namespace std;

class Board {
    
public:
    
    // public var
    int n; // n x n board
    int p; // num of different types of fruit
    vector<int> *board; // board
    
    // Constructor
    Board(int n, int p, vector<int> *stateSpace);
    // Destructor
    ~Board();
    
    void printBoard();
    
    // get fruit type at index i or row r, col c
    int getFruitAt(int i);
    int getFruitAt(int r, int c);
    
    // num of fruit of type f
    int getNumFruitOf(int f);
    
    // make move at i
    // call chooseFruitAt() to remove all fruit of same type
    // call dropAllFruit() -> dropTopFruitAt() to drop top fruit into empty space
    // return value of move = num of fruits remove ^ 2
    int makeMoveAt(int i);
    void dropAllFruit();
    void dropTopFruitAt(int c);
    
    // find all fruit of same type connected vertically, horizontally
    // return set of index loc of all fruit of same type including i
    set<int> *chooseFruitAt(int i);
    int lookEast(int i);
    int lookWest(int i);
    int lookSouth(int i);
    int lookNorth(int i);
    
    
    // return num of fruit available on board
    int getAvailableFruit();
    
    // get best move set for this->board
    // return move set of index loc that are of same fruit type
    // call getMoveMap() to get map of loc i, set of loc fruits to remove
    set<int> *getBestMove();
    map<int, set<int>*> *getMoveMap(); // possible moves (children)
    void clearMoveMap(map<int, set<int>*> *m);
    
    set<int> *getBestMove(int f);
    map<int, vector<set<int>*>*> *getMoveMapByFruit();
    void clearMoveMapByFruit(map<int, vector<set<int>*>*> *m);
    
    // return true if board has 1 move left
    // call getMoveMap() to check num of moves
    bool isTerminalState();
    
    
    string toString();

private:
    // Helper Functions
    int idxToCol(int i, int n);
    int idxToRow(int i, int n);
    int coordToIdx(int row, int col);
    
    // check out of bounds on board
    bool isValid(int i);
    
    // helper function for getMoveMap()
    bool isSetEqual(set<int> *s1, set<int> *s2);
    bool isMapValueUnique(map<int, set<int>*> *moveMap, set<int> *moveSet);
    
};


Board::Board(int n, int p, vector<int> *stateSpace) {
    this->n = n;
    this->p = p;
    this->board = new vector<int>(*stateSpace);
};

Board::~Board() {
    delete this->board;
};

// Print Board
void Board::printBoard() {
    cout << "=== Print Board ===" << endl;
    int c = 0;
    for(int i = 0; i < board->size(); i++) {
        if((*board)[i] == -1) {
            cout << "*";
        } else {
            cout << (*board)[i] << "";
        }
        
        
        c++;
        if(c == n) {
            cout << endl;
            c = 0;
        }
    }
    cout << "=== Print Done! ===" << endl;
};

string Board::toString() {
    string returnString = "";
    
    int c = 0;
    for(int i = 0; i < board->size(); i++) {
        if((*board)[i] == -1) {
            returnString += "*";
        } else {
            returnString += to_string((*board)[i]);
        }
        
        c++;
        if(c == n) {
            returnString += '\n';
            c = 0;
        }
    }
    return returnString;
}

// Return fruit type at index i
int Board::getFruitAt(int i) {
    return (*board)[i];
};

// Return fruit type at row r, col c
int Board::getFruitAt(int r, int c) {
    int i = coordToIdx(r, c);
    return (*board)[i];
};

// Return num of available fruit left on board
int Board::getAvailableFruit() {
    int c = 0;
    for(int i = 0; i < board->size(); i++) {
        if(getFruitAt(i) != -1) {
            c++;
        }
    }
    return c;
};

int Board::getNumFruitOf(int f) {
    int c = 0;
    for(int i = 0; i < board->size(); i++) {
        if(getFruitAt(i) == f) {
            c++;
        }
    }
    return c;
};

bool Board::isTerminalState() {
    map<int, set<int>*> *moveMap = this->getMoveMap();
    
    bool currState = (moveMap->size() == 1);
    
    set<int> movePoolSize;
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        movePoolSize.insert((int) it->second->size());
    }
    
    bool isAllMoveSameSize = (movePoolSize.size() == 1);
    
    bool boardHalfGone = ((n*n) / 2) >= this->getAvailableFruit();
    
    
    this->clearMoveMap(moveMap);
    delete moveMap;
    
    return currState || isAllMoveSameSize || boardHalfGone;
};

bool Board::isSetEqual(set<int> *s1, set<int> *s2) {
    if(s1->size() != s2->size()) {
        return false;
    } else {
        for(set<int>::iterator it = s1->begin(); it != s1->end(); ++it) {
            if(s2->find((*it)) == s2->end()) {
                return false;
            }
        }
        return true;
    }
};

bool Board::isMapValueUnique(map<int, set<int>*> *moveMap, set<int> *moveSet) {
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        if(isSetEqual(moveSet, (it)->second)) {
            return false;
        }
    }
    return true;
};

set<int> *Board::getBestMove() {
    set<int> *moveSet;
    map<int, set<int>*> *moveMap = this->getMoveMap();
    
    int max = 0;
    int maxIndex = -1;
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        if((it->second)->size() > max) {
            max = (int) (it->second)->size();
            maxIndex = it->first;
        }
    }
    
    if(moveMap->size() == 0) { // moveMap is empty
        moveSet = new set<int>();
    } else {
        moveSet = new set<int>((*moveMap)[maxIndex]->begin(),(*moveMap)[maxIndex]->end());
    }
    
    this->clearMoveMap(moveMap);
    delete moveMap;
    
    return moveSet;
};

set<int> *Board::getBestMove(int f) {
    set<int> *moveSet;
    
    map<int, vector<set<int>*>*> *moveMapByFruit = this->getMoveMapByFruit();
    
    vector<set<int>*>* moveList = (*moveMapByFruit)[f];
    
    int max = 0;
    int maxIndex = -1;
    for(int i = 0; i < moveList->size(); i++) {
        if((*moveList)[i]->size() > max) {
            max = (int) (*moveList)[i]->size();
            maxIndex = i;
        }
    }
    
    if(moveList->size() == 0) {
        moveSet = new set<int>();
    } else {
        moveSet = new set<int>((*moveList)[maxIndex]->begin(), (*moveList)[maxIndex]->end());
    }
    
    this->clearMoveMapByFruit(moveMapByFruit);
    delete moveMapByFruit;
    
    return moveSet;
};

map<int, vector<set<int>*>*> *Board::getMoveMapByFruit() {
    map<int, vector<set<int>*>*> *moveMapByFruit = new map<int, vector<set<int>*>*>();
    for(int f = 0; f < p; f++) {
        (*moveMapByFruit)[f] = new vector<set<int>*>();
    }
    
    map<int, set<int>*> *moveMap = this->getMoveMap();
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        
        int fruitType = getFruitAt(it->first);
        (*moveMapByFruit)[fruitType]->push_back(new set<int>((it->second)->begin(), (it->second)->end()));
    }
    
    this->clearMoveMap(moveMap);
    delete moveMap;
    
    return moveMapByFruit;
};

void Board::clearMoveMapByFruit(map<int, vector<set<int>*>*> *m) {
    for(map<int, vector<set<int>*>*>::iterator it = m->begin(); it != m->end(); ++it) {
        for(int i = 0; i < (it->second)->size(); i++) {
            delete (*it->second)[i];
        }
        delete it->second;
    }
    m->clear();
}

void Board::clearMoveMap(map<int, set<int>*> *m) {
    for(map<int, set<int>*>::iterator it = m->begin(); it != m->end(); ++it) {
        delete (it)->second;
    }
    m->clear();
};

map<int, set<int>*> *Board::getMoveMap() {
    map<int, set<int>*> *moveMap = new map<int, set<int>*>();
    
    for(int i = 0; i < n*n; i++) {
        set<int> *moveSet = this->chooseFruitAt(i);
        if(moveSet->size() != 0) {
            if(moveMap->size() == 0) {
                (*moveMap)[i] = new set<int>(*moveSet);
            } else { // check for repeated moveSet
                if(isMapValueUnique(moveMap, moveSet)) {
                    (*moveMap)[i] = new set<int>(*moveSet);
                }
            }
        }
        delete moveSet;
    }
    return moveMap;
};

int Board::makeMoveAt(int i) {
    
    if(getFruitAt(i) == -1) {
        cout << "ERROR: made move at empty fruit loc!" << endl;
        return -1;
    }
    
    set<int> *fruitSet = chooseFruitAt(i);
    int moveValue = (int) fruitSet->size();
    
    set<int>::iterator it;
    for(it = fruitSet->begin(); it != fruitSet->end(); ++it) {
        (*board)[(*it)] = -1;
    }
    delete fruitSet;
    
    (void) dropAllFruit();
    return moveValue*moveValue;
}

set<int> *Board::chooseFruitAt(int i) {
    
    if(getFruitAt(i) == -1) {
        return new set<int>();
    }
    
    list<int> openQueue;
    set<int> closedQueue;
    
    openQueue.push_back(i);
    
    int flag = 0;
    while(flag != 1) {
        if(openQueue.empty()) {
            flag = 1;
            // cout << "open queue is empty!" << endl;
        } else {
            
            int idx = openQueue.front();
            openQueue.pop_front();
            closedQueue.insert(idx);
            
            int eastIdx = lookEast(idx);
            int westIdx = lookWest(idx);
            int northIdx = lookNorth(idx);
            int southIdx = lookSouth(idx);
            
            int search[4] = {eastIdx, westIdx, northIdx, southIdx};
            for(int i = 0; i < 4; i++) {
                if(search[i] != -1 && (closedQueue.find(search[i]) == closedQueue.end())) {
                    openQueue.push_back(search[i]);
                }
            }
        }
    }
    
    /* set<int>::iterator it;
    cout << "set[ ";
    for(it = closedQueue.begin(); it != closedQueue.end(); ++it) {
        cout << (*it) << " ";
    }
    cout << "]"<< endl;
     */
    
    return new set<int>(closedQueue.begin(), closedQueue.end());
};


void Board::dropAllFruit() {
    for(int i = 0; i < this->n; i++) {
        dropTopFruitAt(i);
    }
};

void Board::dropTopFruitAt(int c) {
    
    list<int> yesFruit;
    list<int> noFruit;
    
    for(int r = 0; r < n; r++) {
        int idx = coordToIdx(r, c);
        if(getFruitAt(idx) == -1) {
            noFruit.push_front(idx);
        } else {
            yesFruit.push_front(idx);
        }
    }
    noFruit.sort();
    
    // cout << yesFruit.size() << " " << noFruit.size() << endl;
    
    
    if(yesFruit.size() == 0 && noFruit.size() == n) {
        // cout << "no fruit to drop!" << endl;
    } else if(noFruit.size() == 0) {
        // cout << "all fruit is full!" << endl;
    } else {
        
        while(yesFruit.size() != 0) {
            int fruitIdx = yesFruit.front();
            int emptyIdx = noFruit.back();
            
            // cout << emptyIdx << "->" << fruitIdx << endl;
            
            if(fruitIdx > emptyIdx) {
                yesFruit.pop_front();
            } else {
                
                (*this->board)[emptyIdx] = getFruitAt(fruitIdx);
                (*this->board)[fruitIdx] = -1;
                
                yesFruit.pop_front();
                noFruit.pop_back();
                
                noFruit.push_back(fruitIdx);
                // cout << fruitIdx << endl;
                noFruit.sort();
                
                //for (std::list<int>::iterator it=noFruit.begin(); it!=noFruit.end(); ++it) {
                //    std::cout << ' ' << *it;
                //}
                //std::cout << '\n';
                
                //cout << noFruit.front() << endl;
                
            }
        }
    }
};

int Board::lookEast(int i) {
    if(!isValid(i)) {
        cout << "ERROR: invalid index loc!" << endl;
        return -1;
    }
    
    int w = this->n - 1; // max width
    int col = idxToCol(i, this->n);
    
    if(col < w && col >= 0) {
        if(getFruitAt(i) == getFruitAt(i + 1)) {
            return i + 1;
        } else {
            // cout << "not same fruit type. stop!" << endl;
            return -1; // not same fruit type -> stop
        }
    } else {
        // cout << "at max width. cannot move!" << endl;
        return -1; // at max width -> cannot move
    }
};

int Board::lookWest(int i) {
    if(!isValid(i)) {
        cout << "ERROR: invalid index loc!" << endl;
        return -1;
    }
    
    int w = this->n - 1; // max width
    int col = idxToCol(i, this->n);
    
    if(col <= w && col > 0) {
        if(getFruitAt(i) == getFruitAt(i-1)) {
            return i - 1;
        } else {
            // cout << "not same fruit type. stop!" << endl;
            return -1; // not same fruit type -> stop
        }
    } else {
        // cout << "at max width. cannot move!" << endl;
        return -1; // at max width -> cannot move
    }
};

int Board::lookNorth(int i) {
    if(!isValid(i)) {
        cout << "ERROR: invalid index loc!" << endl;
        return -1;
    }
    
    int w = this->n - 1; // max width
    int row = idxToRow(i, this->n);
    
    if(row <= w && row > 0) {
        if(getFruitAt(i) == getFruitAt(i - this->n)) {
            return i - this->n;
        } else {
            // cout << "not same fruit type. stop!" << endl;
            return -1; // not same fruit type -> stop
        }
    } else {
        // cout << "at max width. cannot move!" << endl;
        return -1; // at max width -> cannot move
    }
};

int Board::lookSouth(int i) {
    if(!isValid(i)) {
        cout << "ERROR: invalid index loc!" << endl;
        return -1;
    }
    
    int w = this->n - 1; // max width
    int row = idxToRow(i, this->n);
    
    if(row < w && row >= 0) {
        if(getFruitAt(i) == getFruitAt(i + this->n)) {
            return i + this->n;
        } else {
            // cout << "not same fruit type. stop!" << endl;
            return -1; // not same fruit type -> stop
        }
    } else {
        // cout << "at max width. cannot move!" << endl;
        return -1; // at max width -> cannot move
    }
};


bool Board::isValid(int i) {
    return (i >= 0 && i < (this->n * this->n));
};

int Board::idxToCol(int i, int n) {
    return i % n;
};

int Board::idxToRow(int i, int n) {
    return i / n;
};

int Board::coordToIdx(int row, int col) {
    return ((row * n) + col);
};


#endif /* board_h */
