//
//  game.hpp
//  mlGame
//
//  Created by Alan Yuan on 2018-03-24.
//  Copyright © 2018 Alan Yuan. All rights reserved.
//

#ifndef game_hpp
#define game_hpp
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <stdio.h>
class gameBoard{
public:
    
    static const int gameBoardSize = 10;
    gameBoard(){
        clear();
    }
    
    void render(){
        for (int y = 0; y < gameBoardSize; y++) {
            for (int x = 0; x < gameBoardSize; x++) {
                std::cout<<board[x][y]<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
    
    void clear(){
        for (int y = 0; y < gameBoardSize; y++) {
            for (int x = 0; x < gameBoardSize; x++) {
                board[x][y] = 0;
                
                if(x == 0 || x == gameBoardSize-1){
                    board[x][y] = 1;
                }
                
                if(y == 0 || y == gameBoardSize-1){
                    board[x][y] = 1;
                }
            }
        }
    }
    
    void clear(int i){
        for (int y = 0; y < gameBoardSize; y++) {
            for (int x = 0; x < gameBoardSize; x++) {
                board[x][y] = i;
            }
        }
    }
    
    int getValue(int x, int y){
        return board[x][y];
    }
    
    void setBoardSquare(int x, int y, int value){
        board[x][y] = value;
    }
    
private:
    
    int board[gameBoardSize][gameBoardSize];
    
};

struct block{
    int x;
    int y;
    int value;
    
    block(){
        x = 0;
        y = 0;
        value = 0;
    }
    
    
    block(int x, int y, int value){
        this->x = x;
        this->y = y;
        this->value = value;
    }
};

class game{
protected:
    gameBoard board;
    
    
    int points;
    int amountOfTicks;
    uint randomSeed;
    int x = 0, y = 0;
    int cmd;
    int highestScore;
    int highestTick;
public:
    
    game(){
        randomSeed = time(NULL);
        
        // note do not touch
    }
    
    
    virtual void init(){
        board.clear();
        if(highestScore< points){
            highestScore = points;
        }
        if(highestTick < amountOfTicks){
            highestTick = amountOfTicks;
        }
        amountOfTicks = 0;
        points = 0;
    }
    
    
    // returns wheather the game ends or not. true: continue game, false end game (you lose)
    virtual bool tick(){
        amountOfTicks++;
        return false;
    }
    virtual void render(){
        board.render();
    }
    
    virtual void setCMD(int command){
        cmd = command;
    }
    
    virtual void restart(){
        randomSeed = time(NULL);
        printf("\n\nRESTART: ");
        
        std::cout<<" S: "<<highestScore<<" T: "<<highestTick<<std::endl;
        highestScore = 0;
        highestTick = 0;
    }
    
    gameBoard*getBoard(){
        return&board;
    }
    
    int getX(){
        return x;
    }
    
    int getY(){
        return y;
    }
    
    int getScore(){
        return points;
    }
    
    int getTicks(){
        return  amountOfTicks;
    }
};
#endif /* game_hpp */
