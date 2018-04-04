//
//  smartboi.hpp
//  mlGame
//
//  Created by Alan Yuan on 2018-03-24.
//  Copyright © 2018 Alan Yuan. All rights reserved.
//

#ifndef smartboi_hpp
#define smartboi_hpp

#include <stdio.h>
#include "game.hpp"
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
class Network;

class Nuerons{
protected:
    
    
    double bias = 0;
    std::vector<int> outputTo; // neurons connected to this (forwards)
    double input_sum = 0; // all inputs are added to thsi
    double finalValue;
    int id = 0;
public:
    
    
    
    Nuerons(int id);
    Nuerons(int id,double bias);
    void setBias(double bias);
    double getBias();
    void addBias(double d);
    void makeConnection(Nuerons*connectTo);
    void pushInput(Network*);
    void clearInputs();
    
    void deleteThisIDFromOutput(int id);
    void reconnectAll(Network*);
    
    Nuerons*getOutputConnection(int position,Network*nw);
    void severeConnection(int position);
    int getOutputConnectionSize();
    int getID();
    void setID(int id);
    
    void save(std::ofstream & outfile);
    int load(std::ifstream & infile, std::string input);
    void addInput(double input);
    
    double calcInputs();
};



class Network{
private:
    uint id = 0;
    const static int inputNodes = 16;
    const static int outputNodes = 4;
    double score = 0;
    std::vector<Nuerons> inputLayer;
    std::vector<Nuerons> outputLayer;
    std::vector<Nuerons> HiddenLayer;
    
    
    
public:
    
    void save(std::ofstream & outfile);
    void load(std::ifstream & infile,std::string input);
    void genorateNew();
    
    void clone(Network*);
    
    void mutate();
    void setInputs();
    void pushInput();
    
    int getNetworkSize();
    
    Nuerons*getNueronWithID(int id);
    int getOutput();
    
    //void reconnectAll();
    void reID(); // IDs all neurons for saving/copying
    
    void clearOutput();
    
    void setScore(double socre);
    double getScore();
    
    Nuerons*chooseFirstConnect();
    Nuerons*chooseSecondConnect();
    
    Nuerons*getInputLayer(int id);
    int getInputLayerSize();
    Nuerons*getHiddenLayer(int id);
    int getHiddenLayerSize();
    Nuerons*getOutputLayer(int id);
    int getOutputLayerSize();
    
};

class smartboi{
    
    // this NN will have 4 layers;
    // l1 input (16 Ns) l2 hidden (12 Ns) l3 hidden (8 Ns) l4 output (4)
    // 0 is input layer 3 is output
    
private:
    // dirrections
    // 0 1 2
    // 3   4
    // 5 6 7
    
    const int population = 8;
    
    int inputs[16]; // distancce and block type in 8 directions
    game * thisGame;
    
    
    
    std::vector<Network> pop;
    int currentNetwork;
    
    
public:
    smartboi(game*game);
    bool checkFileValidity(std::ifstream&FILEINPUT);
    virtual void getInput();
    void init();
    void save(std::ofstream&FILEOUTPUT);
    void load(std::ifstream&FILEINPUT);
    void tick();
    
    
    void finishGame(int finalScore, int finalTicks);
    
};

#endif /* smartboi_hpp */
