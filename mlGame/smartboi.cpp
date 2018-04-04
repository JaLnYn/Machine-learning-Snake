//
//  smartboi.cpp
//  mlGame
//
//  Created by Alan Yuan on 2018-03-24.
//  Copyright © 2018 Alan Yuan. All rights reserved.
//

#include "smartboi.hpp"



Nuerons::Nuerons(int id){
    input_sum =0 ;
    this->id = id;
}

Nuerons::Nuerons(int id,double bias){
    input_sum =0 ;
    this->id = id;
    this->bias = bias;
}

void Nuerons::addInput(double input){
    
    this->input_sum = this->input_sum + input;
    
}

void Nuerons::setBias(double bias){
    this->bias = bias;
}

double Nuerons::getBias(){
    return bias;
}

void Nuerons::addBias(double d){
    bias+=d;
}

int Nuerons::getID(){
    return id;
}

void Nuerons::setID(int id){
    this->id = id;
}

void Nuerons::makeConnection(Nuerons*connectTo){
    
    if(connectTo!=nullptr&&connectTo!=NULL){
        bool doNotConnect = false;
        
        for (int i = 0; i<outputTo.size(); i++) {
            if(outputTo[i] == connectTo->getID()){
                doNotConnect = true;
            }
        }
        
        if(!doNotConnect&&connectTo!=this){
            outputTo.push_back(connectTo->getID());
        }
    }
}

void Nuerons::pushInput(Network*nw){
    double outVal = calcInputs();
    
    for (int i = 0; i < outputTo.size(); i++) {
        nw->getNueronWithID(outputTo[i])->addInput(outVal);
    }
    
}

void Nuerons::clearInputs(){
    input_sum = 0;
    
}

Nuerons*Nuerons::getOutputConnection(int id,Network*nw){
    if(id < outputTo.size() && id >=0){
        return nw->getNueronWithID(outputTo[id]);
    }
    
    std::cout<<"error with outputConnection: "<<id<<std::endl;
    
    exit(10);
    return nullptr;
}

int Nuerons::getOutputConnectionSize(){
    return outputTo.size();
}

void Nuerons::severeConnection(int position){
    if(outputTo.size()>0){
        outputTo.erase(outputTo.begin()+position);
    }else{
        printf("Sevre Fail");
    }
    
}


void Network::genorateNew(){
    id = 0;
    for (int amtOut = 0; amtOut<outputNodes; amtOut++) {
        outputLayer.push_back(Nuerons(id));
        id++;
    }
    
    for (int amtin = 0; amtin<inputNodes; amtin++) {
        inputLayer.push_back(Nuerons(id));
        id++;
        inputLayer[amtin].makeConnection(&outputLayer[rand()%outputLayer.size()]);
        if(rand()%3 < 1){
            inputLayer[amtin].makeConnection(&outputLayer[rand()%outputLayer.size()]);
        }
    }
}

Nuerons*Network::chooseFirstConnect(){
    int position = rand()%(HiddenLayer.size()+inputLayer.size());
    
    if(position < inputLayer.size()){
    
        return &inputLayer[position];
    }else{
        return &HiddenLayer[position-inputLayer.size()];
    }
    
    
}

Nuerons*Network::chooseSecondConnect(){
    int position = rand()%(HiddenLayer.size()+outputLayer.size());
    
    if(position < HiddenLayer.size()){
        
        return &HiddenLayer[position];
    }else{
        return &outputLayer[position-HiddenLayer.size()];
    }
}

void Nuerons::save(std::ofstream &outfile){
    
    outfile<<id<<"\n";
    outfile<<bias<<"\n";
    
    
    
    for (int i = 0; i < outputTo.size(); i++) {
        outfile<<outputTo[i]<<" ";
    }
    
    
    
    outfile<<"en"<<"\n"; // end N ueron
    
}


int Nuerons::load(std::ifstream &infile, std::string input){
    
    if(input != "en"){
        id = std::stoi(input);
        
        infile>>input;
        
        
        bias = std::stod(input);
        
        infile>>input;
        
        while (input != "en"){
            // connectTo
            
            int connectId = -1;
            
            connectId = std::stoi(input);
            infile>>input;
            
            outputTo.push_back(connectId);
        }
        return id;
    }
    
    return -1;
}

double Nuerons::calcInputs(){
    
    double sum;
    
    sum = input_sum+bias;
    
    return tanh(sum);
}

void Nuerons::deleteThisIDFromOutput(int id){
    for (int i = 0; i < outputTo.size(); i++) {
        if(id == outputTo[i]){
            outputTo.erase(outputTo.begin()+i);
            return;
        }
    }
}
//void Nuerons::reconnectAll(Network* nw){
//    std::vector<Nuerons*> newNetwork;
//    
//    for (int i = 0; i < outputTo.size(); i++) {
//        newNetwork.push_back(nw->getNueronWithID(outputTo[i]->getID()));
//        std::cout<<outputTo[i]->getID()<<std::endl;
//    }
//    
//    outputTo.clear();
//    for ( int i = 0; i < newNetwork.size(); i++) {
//        outputTo.push_back(newNetwork[i]);
//    }
//    
//}


int Network::getOutput(){
    
    int output = 0;
    double highestValue = outputLayer[0].calcInputs();
    for (int i = 1; i < outputLayer.size(); i++) {
        double outputValue = outputLayer[i].calcInputs();
        if(outputValue>highestValue){
            output = i;
            highestValue = outputValue;
        }
    }
    return output;
}

void Network::clearOutput(){
    for (int i = 0; i < HiddenLayer.size(); i++) {
        HiddenLayer[i].clearInputs();
    }
    for (int i = 0; i < inputLayer.size(); i++) {
        inputLayer[i].clearInputs();
    }
    for (int i = 0; i < outputLayer.size(); i++) {
        outputLayer[i].clearInputs();
    }
}

void Network::reID(){
    
    id = 0;
    for (int i = outputLayer.size()-1; i >= 0; i--) {
        outputLayer[i].setID(id);
        id+=1;
    }
    
    for (int i = HiddenLayer.size()-1; i >= 0; i--) {
        HiddenLayer[i].setID(id);
        id+=1;
    }
    
    for (int i = inputLayer.size()-1; i >= 0; i--) {
        inputLayer[i].setID(id);
        id+=1;
    }
}

void Network::save(std::ofstream & outfile){
    
    
    // NOTE: we must always save last element to first because the earlier elements can only
    //  connect to later elements
    
    outfile<<score<<std::endl; // save score
    
    // save outer layer
    for (int i = 0; i < outputLayer.size(); i++) {
        outputLayer[i].save(outfile);
    }
    outfile<<"EOL"<<std::endl; // end O ut L ayer
    for (int i = 0; i < HiddenLayer.size(); i++) {
        HiddenLayer[i].save(outfile);
    }
    outfile<<"EHL\n"; // end H idden L ayer
    for (int i = 0; i < inputLayer.size(); i++) {
        inputLayer[i].save(outfile);
    }
    
    outfile<<"EIL\n"; // end I nput L ayer
    
}

void Network::load(std::ifstream & infile, std::string input){
    
    
    // NOTE: we must always save last element to first because the earlier elements can only
    //  connect to later elements
    
    score = std::stod(input); // download score
    
    // save outer layer
    
    infile>>input;
    while (input != "EOL"){
        // connectTo
        outputLayer.push_back(Nuerons(0));
        int highestid = outputLayer.back().load(infile,input);
        if(highestid>id ){
            id = highestid;
        }
        infile>>input;
        
    }
    infile>>input;
    
    while (input != "EHL"){
        
        // connectTo
        HiddenLayer.push_back(Nuerons(0));
        int highestid = HiddenLayer.back().load(infile,input);
        if(highestid>id ){
            id = highestid;
        }
        infile>>input;

    }
    infile>>input;
    
    while (input != "EIL"){
        
        // connectTo
        inputLayer.push_back(Nuerons(0));
        int highestid = inputLayer.back().load(infile,input);
        if(highestid>id ){
            id = highestid;
        }
        infile>>input;
        
    }
    
    id +=1;
}


void Network::mutate(){
    
    
    
    // 10% chance of getting new neuron
    
    if(rand()%10 == 0){
        
        int position = rand()%(HiddenLayer.size()+inputLayer.size());
        
        if(position < inputLayer.size()){
            // inputLayer will get it
            
            HiddenLayer.insert(HiddenLayer.begin(), Nuerons(id,rand()%20-10));
            id++;
            inputLayer[position].makeConnection(&HiddenLayer[0]);
            
            if(rand()%2 == 0||HiddenLayer.size()<2){
                // 50 % chance rebuild existing connection
                int connectToPosition = rand()%inputLayer[position].getOutputConnectionSize();
                
                HiddenLayer[0].makeConnection(inputLayer[position].getOutputConnection(connectToPosition,this));
                
                inputLayer[position].severeConnection(connectToPosition);
            }else{
                //50% chance new connection
                int connectToPosition = rand()%(HiddenLayer.size()-1+0+outputLayer.size())+1+0;
                if(connectToPosition<HiddenLayer.size()){
                    // connect to hidden layer
                    HiddenLayer[0].makeConnection(&HiddenLayer[connectToPosition]);
                    
                }else{
                    //connect to output Layer
                    connectToPosition = connectToPosition - HiddenLayer.size();
                    HiddenLayer[0].makeConnection(&outputLayer[connectToPosition]);
                    
                }
            }
        }else{
            // find random hidden layer that gets this input
            
            position = position-inputLayer.size();
            
            HiddenLayer.insert(HiddenLayer.begin()+position+1, Nuerons(id,rand()%20-10));
            id++;
            HiddenLayer[position].makeConnection(&HiddenLayer[position+1]);
            
            if(position>=HiddenLayer.size()||position<0){
                std::cout<<"inputlayerSize eror";
                
                exit(99);
            }
            
            if((rand()%2 == 0&&HiddenLayer[position].getOutputConnectionSize()>0)||HiddenLayer.size()<2){
                // 50 % chance rebuild existing connection
                int connectToPosition = rand()%HiddenLayer[position].getOutputConnectionSize();
                
                HiddenLayer[position+1].makeConnection(HiddenLayer[position].getOutputConnection(connectToPosition,this));
                
                HiddenLayer[position].severeConnection(connectToPosition);
            }else{
                //50% chance new connection
                int connectToPosition = rand()%(HiddenLayer.size()-1+0+outputLayer.size())+1+0;
                if(connectToPosition<HiddenLayer.size()){
                    // connect to hidden layer
                    HiddenLayer[position+1].makeConnection(&HiddenLayer[connectToPosition]);
                    
                }else{
                    //connect to output Layer
                    connectToPosition = connectToPosition - HiddenLayer.size();
                    HiddenLayer[position+1].makeConnection(&outputLayer[connectToPosition]);
                    
                }
            }
        }
        
    }
    if(rand()%5 <1){
        //10% chance to do random connection
        chooseFirstConnect()->makeConnection(chooseSecondConnect());
    }
    if(rand()%10 <1){
        Nuerons* destroy = chooseFirstConnect();
        if(destroy->getOutputConnectionSize()>0){
            destroy->severeConnection(rand()%(destroy->getOutputConnectionSize()));
        }
        
    }
    if(rand()%20 == 0 && HiddenLayer.size()>0){
        int deletedisid = rand()%(HiddenLayer.size());
        int id = HiddenLayer[deletedisid].getID();
        for (int i = 0; i < inputLayer.size(); i++) {
            inputLayer[i].deleteThisIDFromOutput(id);
        }
        for (int i = 0; i < HiddenLayer.size(); i++) {
            HiddenLayer[i].deleteThisIDFromOutput(id);
        }
        
        HiddenLayer.erase(HiddenLayer.begin()+deletedisid);
    }
    
    
    // change bias of a single Neuron
    if(HiddenLayer.size()+inputLayer.size()>0){
        
        int timesToChange = (rand()%(HiddenLayer.size()+inputLayer.size()))/10+1;
        for(int i = 0; i<timesToChange; i++){
            
            if(rand()%10 < 8){
                // 90% to do a slight change
                chooseFirstConnect()->addBias((rand()%10-5)/(double)(rand()%4+1));
            }else{
                chooseFirstConnect()->setBias(((rand()%20)-10)/(double)(rand()%3+1));
            }
            
        }
    }
    
    
    
}


void Network::pushInput(){
    for (int i = 0; i < inputLayer.size(); i++) {
        inputLayer[i].pushInput(this);
    }
    
    
    for (int i = 0; i < HiddenLayer.size(); i++) {
        HiddenLayer[i].pushInput(this);
    }
    
    
}



void Network::setScore(double sc){
    score = sc;
}

double Network::getScore(){
    return score;
}

Nuerons*Network::getNueronWithID(int id){
    for (int i = 0 ; i < inputLayer.size(); i++) {
        if(inputLayer[i].getID() == id){
            return &(inputLayer[i]);
        }
    }
    for (int i = 0 ; i < HiddenLayer.size(); i++) {
        if(HiddenLayer[i].getID() == id){
            return &(HiddenLayer[i]);
        }
    }
    for (int i = 0 ; i < outputLayer.size(); i++) {
        if(outputLayer[i].getID() == id){
            return &(outputLayer[i]);
        }
    }
    
    std::cout<<"cannot find id: "<< id <<std::endl;
    
    exit(99);
    return 0;
}

//void Network::reconnectAll(){
//    for (int i = 0; i < outputLayer.size(); i++) {
//        outputLayer[i].reconnectAll(this);
//        
//    }
//    
//    for (int i = 0; i < HiddenLayer.size(); i++) {
//        HiddenLayer[i].reconnectAll(this);
//    }
//    
//    for (int i = 0; i < inputLayer.size(); i++) {
//        inputLayer[i].reconnectAll(this);
//    }
//}

Nuerons*Network::getInputLayer(int id){
    return &inputLayer[id];
}
int Network::getInputLayerSize(){
    return inputLayer.size();
}
Nuerons*Network::getHiddenLayer(int id){
    return &HiddenLayer[id];
}
int Network::getHiddenLayerSize(){
    return HiddenLayer.size();
}
Nuerons*Network::getOutputLayer(int id){
    return &outputLayer[id];
}
int Network::getOutputLayerSize(){
    return outputLayer.size();
}

int Network::getNetworkSize(){
    uint size = 0;
    
    
    
    for (int i = 0; i < HiddenLayer.size(); i++) {
        size+=5;
        size+=HiddenLayer[i].getOutputConnectionSize();
    }
    
    
    
    return size;
}

bool smartboi::checkFileValidity(std::ifstream & INPUT){
    if(INPUT.fail()){
        return false;
    }
    
    
    std::string in;
    INPUT>>in;
    while ( !INPUT.eof() ) {
        INPUT>>in;
    }
    
    if(in == "END"){
        INPUT.clear();
        INPUT.seekg(0, std::ios::beg);
        return true; // is valid
    }
    return false;
}



smartboi::smartboi(game * game){
    thisGame = game;
    
}

void smartboi::save(std::ofstream&FILEOUTPUT){
    for (int networkID = 0; networkID<pop.size(); networkID++) {
        pop[networkID].save(FILEOUTPUT);
        // outerlayer back to front
        
    }
    
    FILEOUTPUT<<"\nEND"<<std::endl;
}

void smartboi::load(std::ifstream&FILEINPUT){
    
    std::string in;
    FILEINPUT>>in;
    while (in != "END") {
        pop.push_back(Network());
        pop.back().load(FILEINPUT,in);
        FILEINPUT>>in;
    }
}

void smartboi::init(){
    srand(time(NULL));
    std::ifstream FILEINPUT;
    FILEINPUT.open("projectOverview.txt");
    currentNetwork = 0;
    
    
    if(checkFileValidity(FILEINPUT)){
        
        //file is valid
        load(FILEINPUT);
        
    }else{
        FILEINPUT.open("backup.txt");
        if(checkFileValidity(FILEINPUT)){
            load(FILEINPUT);
        }else{
            
            
            // no backup no real so new start, everything is random
            
            // we start with 8 randomized networks
            for (int i = 0; i < population; i++) {
                pop.push_back(Network());
            }
            for (int i = 0; i < population; i++) {
                pop[i].genorateNew();
                pop[i].mutate();
            }
        }
    }
}

void smartboi::getInput(){
    // read board
    
    
    // dirrections
    // 0 1 2
    // 3   4
    // 5 6 7
    const int xMove[8] = {-1,0,1,-1,1,-1,0,1};
    const int yMove[8] = {-1,-1,-1,0,0,1,1,1};
    
    
    
    for (int dirrection = 0; dirrection < 8; dirrection++) {
        int nx, ny; // new x and new y
        int distance = 0; // one of the inputs
        nx = thisGame->getX()+xMove[dirrection];
        ny = thisGame->getY()+yMove[dirrection];
        while(thisGame->getBoard()->getValue(nx, ny)==0){
            distance++;
            nx = nx+xMove[dirrection];
            ny = ny+yMove[dirrection];
        }
        
        inputs[dirrection*2] = distance;
        inputs[dirrection*2+1] = thisGame->getBoard()->getValue(nx, ny);
    }
    
    
    
}
void smartboi::tick(){
    getInput();
    
    // all inputs are entered into the network
    
    
    for(int i = 0; i<16; i++){
        pop[currentNetwork].getInputLayer(i)->addInput(inputs[i]);
    }
    
    pop[currentNetwork].pushInput();
    
    thisGame->setCMD(pop[currentNetwork].getOutput());
    
    pop[currentNetwork].clearOutput();
    
    
    
}



void smartboi::finishGame(int finalScore, int finalTicks){
    
    struct greater{
         bool operator()(Network a, Network b){
             
             return a.getScore() < b.getScore();
         }
    };
    
    double finalTick2 = (-finalTicks*finalTicks + finalTicks*32 - pop[currentNetwork].getNetworkSize()/320)/200; // encourage for the first 19 steps, and then discourage because we don't want the ai to take too makny steps
    
    
    
    
    
    double fitness = (double)finalScore + tanh((double)finalTick2/2) ;
    
    
    
    pop[currentNetwork].setScore((pop[currentNetwork].getScore()+fitness)/2.0); // retain some previous score so things that did well in one case does not die off imediatly
    
    
    if(currentNetwork < pop.size()-1){
        currentNetwork++;
    }else{
        // new genoration
        //selection here
        
        
        std::sort(pop.begin(), pop.end(), greater()); // low to high scores
        
        for (int i = 0; i < pop.size()/2; i++) {
            pop[i] = pop[i+pop.size()/2];
            
            pop[i].mutate();
        }
        
        
        std::ofstream FILEOUTPUT;
        FILEOUTPUT.open("projectOverview.txt");
        save(FILEOUTPUT);
        FILEOUTPUT.close();
        
        FILEOUTPUT.open("backup.txt");
        save(FILEOUTPUT);
        FILEOUTPUT.close();
        
        thisGame->restart();
        currentNetwork = 0;
    }
}
