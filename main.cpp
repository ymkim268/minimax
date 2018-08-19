//
//  main.cpp
//  fruitrage
//
//  Created by Young Kim on 10/2/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#include <iostream>
#include <algorithm>    // std::max
#include <cstdint> //or <stdint.h>
 #include <limits>


#include "input.h"
#include "board.h"
#include "node.h"

using namespace std;


void printSet(set<int> *s) {
    set<int>::iterator it;
    cout << "set[ ";
    
    for(it = s->begin(); it != s->end(); ++it) {
        cout << (*it) << " ";
    }
    cout << "]"<< endl;
};

string setToString(set<int> *s) {
    string temp = "set[ ";
    set<int>::iterator it;
    for(it = s->begin(); it != s->end(); ++it) {
       temp +=  (char)(*it) + " ";
    }
    temp += "]";
    return temp;
};

void moveMapClean(map<int, set<int>*> *m) {
    for(map<int, set<int>*>::iterator it = m->begin(); it != m->end(); ++it) {
        delete (it)->second;
    }
    m->clear();
};



bool isMoveEqual(set<int> *s1, set<int> *s2) {
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


string idxToColRow(int i, int n) {
    string colString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    string returnString = "";
    
    int col = i % n;
    int row = i / n;
    
    cout << col << ", " << row << endl;
    cout << colString.at(col) << ", " << to_string(row + 1) << endl;
    
    returnString += colString.at(col) + to_string(row + 1);
    return returnString;
};


int max_val(Node *n, int alpha, int beta);
int min_val(Node *n, int alpha, int beta);


int max_val(Node *n, int alpha, int beta) {
    int v = -2147483647;
    n->touched = 1;
    
    if(n->cutOffTest(1)) {
        n->v = n->evalTest(1);
        n->terminal = 1;
        return n->v;
    }
    
    int len = n->expand(1);
    vector<Node*> *successors = n->children;
    for(int i = 0; i < len; i++) {
        v = max(v, min_val((*successors)[i], alpha, beta));

        if(v >= beta) {
            n->cut = 1;
            n->v = v;
            return v;
        }
        alpha = max(alpha, v);
    }
    
    n->v = v;
    return v;
};

int min_val(Node *n, int alpha, int beta) {
    int v = 2147483647;
    n->touched = 1;
    
    if(n->cutOffTest(-1)) {
        n->v = n->evalTest(-1);
        n->terminal = 1;
        return n->v;
    }
    
    int len = n->expand(-1);
    vector<Node*> *successors = n->children;
    for(int i = 0; i < len; i++) {
        v = min(v, max_val((*successors)[i], alpha, beta));

        if(v <= alpha) {
            n->cut = 1;
            n->v = v;
            return v;
        }
        
        beta = min(beta, v);
    }
    
    n->v = v;
    return v;
};

int alphaBetaSearch(Node *n) {
    int alpha = -2147483647;
    int beta = 2147483647;
    
    cout << "==== testing alpha beta ==== " << endl;
    int v = max_val(n, alpha, beta);
    cout << "root = " << v << endl;
    
    vector<Node*> *actions = n->children;
    vector<int> possibleLoc;
    
    cout << "finding possible actions" << endl;
    for(int i = 0; i < actions->size(); i++) {
        if(v == (*actions)[i]->v) {
            possibleLoc.push_back((*actions)[i]->actionLoc);
            
            /*
            cout << "v = " << (*actions)[i]->v << endl;
            cout << "action loc = " << (*actions)[i]->actionLoc << endl;
            cout << "cost = " << (*actions)[i]->cost << endl;
            */
        }
    }
    
    if(possibleLoc.size() == 1) {
        return possibleLoc[0];
    } else {
        cout << "narrow down loc" << endl;
        vector<int> limitPossibleLoc;
        
        int maxFruitRemoved = 0;
        map<int, set<int>*> *moveMap = n->myBoard->getMoveMap();
        for(int i = 0; i < possibleLoc.size(); i++) {
            int loc = possibleLoc[i];
            
            int numFruitRemoved = (int) (*moveMap)[loc]->size();
            if(numFruitRemoved > maxFruitRemoved) {
                maxFruitRemoved = numFruitRemoved;
                limitPossibleLoc.push_back(loc);
            }
        }
        n->myBoard->clearMoveMap(moveMap);
        delete moveMap;
        
        if(limitPossibleLoc.size() != 1) {
            cout << "limited possible loc size = " << limitPossibleLoc.size() << endl;
        }
        return limitPossibleLoc[0];
    }
};



int main(int argc, const char * argv[]) {
    cout << "=== starting ===" << endl;
    
    Input input2;
    readFile("input.txt", input2);
    Board *test = new Board(input2.n, input2.p, &(input2.stateSpace));
    // Board *temp = new Board(input2.n, input2.p, &(input2.stateSpace)); // greedy approach
 
 	// cout << "print input!" << endl;
 	// test->printBoard();
 
 /*
 	cout << "All Possible choices!" << endl;
    map<int, set<int>*> *moveMap = test->getMoveMap();
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        cout << it->first << " fruit = " << test->getFruitAt(it->first) << " => " << it->second->size() << endl;
    }
*/
    
    Node *parent = new Node(0, 0, 0, test, NULL);
    int loc = alphaBetaSearch(parent);
    
    int val = parent->myBoard->makeMoveAt(loc);
    // parent->myBoard->printBoard();
    cout << "alpha beta approach loc = " << loc << ", s = " << val << endl;
    
    string retLoc = idxToColRow(loc, input2.n);
    cout << "retLoc = " << retLoc << endl;
    
    string outputString = retLoc + '\n' + parent->myBoard->toString();
    writeFile("output.txt", outputString);
    
    /*
 	int greedySize = (int) temp->getBestMove()->size();
    set<int>::iterator itGreedy = temp->getBestMove()->begin();
    cout << "greedy approach loc = " << *itGreedy << ", s = " << greedySize*greedySize << endl;
    temp->makeMoveAt(*itGreedy);
    temp->printBoard();
    */
 
    
 	cout << "=== end ===" << endl;
    
    // delete temp;
    delete test;
    delete parent;
    
    return 0;
}
