//
//  main.cpp
//  mlGame
//
//  Created by Alan Yuan on 2018-03-24.
//  Copyright © 2018 Alan Yuan. All rights reserved.
//

#include <iostream>
#include <vector>
#include "game.hpp"
#include "smartboi.hpp"

class snakeGame: public game{
    
private:
    const int CLEARBLOCK = 0;
    const int WALL = 1;
    const int SNAKEHEAD = 2;
    const int SNAKEBODY = 3;
    const int FOOOOOD = 4;
    
    const int maxTick = 100;
    
    std::vector<block> snake;
    int dir = 0; // 0 = right, 1 = down, 2 = left, 3 = up
    block food;
public:
    
    // rules for this game;
    // 0 = clear block;
    // 1 = wall;
    // 2 = snake head
    // 3 = snake body;
    // 4 = food block
    
    // starts up the game
    virtual void init(){
        game::init();
        snake.clear();
        int xOffSet = rand()%2;
        int yOffSet = rand()%2;
        snake.push_back(block(board.gameBoardSize/2-1-xOffSet, board.gameBoardSize/2-yOffSet, SNAKEBODY));
        snake.push_back(block(board.gameBoardSize/2-xOffSet, board.gameBoardSize/2-yOffSet, SNAKEBODY));
        snake.push_back(block(board.gameBoardSize/2+1-xOffSet, board.gameBoardSize/2-yOffSet, SNAKEHEAD));
        dir = 0;
        
        board.clear();
        
        for (int i = 0; i < snake.size(); i++) {
            board.setBoardSquare(snake[i].x, snake[i].y, snake[i].value);
        }
        
        food.x = rand()%board.gameBoardSize;
        food.y = rand()%board.gameBoardSize;
        
        while (board.getValue(food.x, food.y) != 0) {
            food.x = rand()%board.gameBoardSize;
            food.y = rand()%board.gameBoardSize;
        }
        
        food.value = FOOOOOD;
        
        board.setBoardSquare(food.x, food.y, food.value);
        
        x = board.gameBoardSize/2+1;
        y = board.gameBoardSize/2;
        
    }
    
    
    // returns if the if you can continue playing (true if nothing, false if lose)
    virtual bool move(int mx, int my){
        if(mx >= board.gameBoardSize || mx < 0){
            return false; //out of bounds
        }
        
        // if move right 1 is snake then lose
        if(board.getValue(mx, my) == SNAKEBODY||board.getValue(mx, my) == WALL){
            return false;
        }else if (board.getValue(mx,my) == FOOOOOD){
            points++;
            
            if(points == 64){
                return false;
            }
            
            for (int i = 0; i < snake.size(); i++) {
                snake[i].value = SNAKEBODY;
            }
            snake.push_back(block(mx, my, SNAKEHEAD));
            food.x = rand()%board.gameBoardSize;
            food.y = rand()%board.gameBoardSize;
            
            while (board.getValue(food.x, food.y) != 0) {
                food.x = rand()%board.gameBoardSize;
                food.y = rand()%board.gameBoardSize;
            }
        }else{
            block b = snake[0];
            snake.erase(snake.begin());
            b.x = mx;
            b.y = my;
            snake.push_back(b);
            for (int i = 0; i < snake.size()-1; i++) {
                snake[i].value = SNAKEBODY;
            }
            snake.back().value = SNAKEHEAD;
        }
        
        
        x = mx;
        y = my;
        
        return  true;
    }
    
    virtual bool tick(){
        game::tick();
        
        dir = cmd;
        
        if(getTicks() > maxTick){
            amountOfTicks = 0;
            return false;
        }
        
        if(dir == 0){
            
            if(!move(snake.back().x+1, snake.back().y)){
                return false;
            }
        }else if(dir == 1){
            if(!move(snake.back().x, snake.back().y+1)){
                return false;
            }
        }else if(dir == 2){
            if(!move(snake.back().x-1, snake.back().y)){
                return false;
            }
        }else if(dir == 3){
            if(!move(snake.back().x, snake.back().y-1)){
                return false;
            }
        }
        board.clear();
        
        for (int i = 0; i < snake.size(); i++) {
            board.setBoardSquare(snake[i].x, snake[i].y, snake[i].value);
        }
        
        board.setBoardSquare(food.x, food.y, food.value);
        
        
        
        
        
        return true;
    }
};

int main(int argc, const char * argv[]) {
    bool gameRunning = true;
    
    game * game = new snakeGame();
    std::string input;
    
    smartboi sb(game);
    
    while (input != "show" && input != "learn") {
        std::cout<<"learn or show"<<std::endl;
        std::cin>>input;
    }
    
    
    game->init();
    
    sb.init();
    
    if(input == "learn"){
        while (gameRunning) {
            sb.tick();
            if(!game->tick()){ // we lost
                //std::cout<<game->getScore()<<std::endl;
                sb.finishGame(game->getScore(), game->getTicks());
                game->init();
                
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }else{
        while (gameRunning) {
            sb.tick();
            if(!game->tick()){ // we lost
                sb.finishGame(game->getScore(), game->getTicks());
                game->init();
            }
            game->render();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
    
    
    
    return 0;
}
