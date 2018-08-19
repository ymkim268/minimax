//
//  input.h
//  fruitrage
//
//  Created by Young Kim on 10/3/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef input_h
#define input_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h> // atoi

using namespace std;

struct Input {
    int n; // n x n board
    int p; // num of different types of fruit
    double dur; // remaining time duration sec
    vector<int> stateSpace; // board
};

void readFile(std::string fileName, Input &input) {
    // open input file to read
    ifstream infile;
    infile.open(fileName.c_str(), fstream::in);
    
    // parse input file
    if(infile.is_open()) {
        string n, p, dur;
        
        getline(infile, n);
        getline(infile, p);
        getline(infile, dur);

        
        input.n = atoi(n.c_str()); // stoi(n) not working in vocareum
        input.p = atoi(p.c_str());
        input.dur = atof(dur.c_str());
        
        
        // build state space
        for(int i = 0; i < input.n; i++) {
            string line;
            getline(infile, line);

            for(int j = 0; j < input.n; j++) {

                if(line[j] == '*') {
                    input.stateSpace.push_back(-1);
                } else {
                    input.stateSpace.push_back((int) line[j] - 48);
                }

            }
        }

    } else {
        cout << "ERROR: Failed to open file!" << endl;
    }
    infile.close();
};


void writeFile(std::string fileName, std::string &output) {
    
    // open output file to write
    ofstream outfile(fileName.c_str());
    if(outfile.is_open()) {
        outfile << output;
    } else {
        cout << "ERROR: Failed to write file!" << endl;
    }
    outfile.close();
    
};


#endif /* input_h */
