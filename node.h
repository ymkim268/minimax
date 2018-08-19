//
//  node.h
//  fruitrage
//
//  Created by Young Kim on 10/5/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef node_h
#define node_h

#include "board.h"

using namespace std;

class Node {

public:
    
    int v;
    int actionLoc;
    
    int nid;
    int depth;
    int cost;
    
    int cut;
    int terminal;
    int touched;
    
    Board *myBoard;
    Node *parent;
    
    vector<Node*> *children;
    
    // Constructor
    Node(int nid, int depth, int cost, Board *myBoard, Node *parent);
    // Destructor
    ~Node();
    
    int expand(int nodeType);
    
    int evalTest(int nodeType);
    bool cutOffTest(int nodeType);
    
    
private:
    
};

Node::Node(int nid, int depth, int cost, Board *myBoard, Node *parent) {
     this->v = -2147483648;
     this->actionLoc = -1;
     this->cut = -1;
     this->terminal = -1;
     this->touched = -1;
     
     
    this->nid = nid;
    this->depth = depth;
    this->cost = cost;
    this->myBoard = new Board(myBoard->n, myBoard->p, myBoard->board);
    this->parent = parent;
    
    this->children = new vector<Node*>();
};

Node::~Node() {
    delete myBoard;
    
    for(int i = 0; i < this->children->size(); i++) {
        delete (*this->children)[i];
    }
    delete this->children;
};

int Node::expand(int nodeType) {
map<int, set<int>*> *moveMap = myBoard->getMoveMap();
    
    /*
    int c = 1;
    for(int f = 0; f < myBoard->p; f++) {
        // choose best move for each fruit type
        set<int> *moveSet = myBoard->getBestMove(f);
        
        if(moveSet->size() != 0) {
            set<int>::iterator it = moveSet->begin();
            int moveLoc = *it;
            
            Board *nextBoard = new Board(myBoard->n, myBoard->p, myBoard->board);
            int moveVal = nextBoard->makeMoveAt(moveLoc);
            
            int offset = 0;
            if(parent != NULL) {
                 offset = (int) parent->children->size() - 1;
            }
            
            Node *nextNode = new Node(nid + offset + c, depth + 1, cost + (nodeType)*moveVal, nextBoard, this);
            nextNode->actionLoc = moveLoc;
            //cout << "move val = " << moveVal << endl;
            children->push_back(nextNode);
            
            c++;
            delete nextBoard;
        }
        delete moveSet;
    }
     */
    
    int c = 1;
    for(map<int, set<int>*>::iterator it = moveMap->begin(); it != moveMap->end(); ++it) {
        int moveLoc = it->first;
        set<int> *moveSet = it->second;
        
        if(moveSet->size() != 0) {
            Board *nextBoard = new Board(myBoard->n, myBoard->p, myBoard->board);
            int moveVal = nextBoard->makeMoveAt(moveLoc);
            
            int offset = 0;
            if(parent != NULL) {
                offset = (int) parent->children->size() - 1;
            }
            
            Node *nextNode = new Node(nid + offset + c, depth + 1, cost + (nodeType)*moveVal, nextBoard, this);
            nextNode->actionLoc = moveLoc;
            children->push_back(nextNode);
            c++;
            delete nextBoard;
        }
    }
    
    myBoard->clearMoveMap(moveMap);
    delete moveMap;
    
    //cout << "child size = " << children->size() << endl;
    //cout << "depth = " << this->depth << endl;
    
    return (int) children->size();
};

bool Node::cutOffTest(int nodeType) {
    
    if(this->parent == NULL) {
        return false;
    }
     
    if(this->myBoard->getAvailableFruit() == 0) {
        return true;
    }
     
    if(this->myBoard->getAvailableFruit() <= ((this->myBoard->n * this->myBoard->n) / 2)) {
        return true;
    }
    
    
    if(nodeType == -1) {
        int bestMoveVal = (int) this->myBoard->getBestMove()->size(); // num of fruit removed for best move
        if((nodeType*bestMoveVal*bestMoveVal) + this->cost < 0) {
            return true;
        }
    }
    
    if(nodeType == 1) {
        int nextVal = (int) this->myBoard->getBestMove()->size();
        if((nodeType*nextVal*nextVal) + this->cost > 0) {
            return true;
        }
    }
    
    return depth == 3;
};

int Node::evalTest(int nodeType) {
     
    int nextVal = (int) this->myBoard->getBestMove()->size();
    return (nodeType*nextVal*nextVal) + this->cost;
     
    // return this->cost;
}

#endif /* node_h */
