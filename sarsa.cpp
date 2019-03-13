/*
    Tony Misic 500759917
    Daniel Bellissimo 500749419
    CPS841 Assignment 3 Question 2 SARSA Implementation
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <array>
#include <random>
bool left_of_wall(int x, int y);
bool right_of_wall(int x, int y);
bool above_wall(int x, int y);
bool below_wall(int x, int y);
int epsilon_greedy(std::array<double, 4> policy,float epsilon);
std::array< std::array<std::array<double, 4>, 10>, 10> sarsa(std::array< std::array<std::array<double, 4>, 10>, 10> policy,std::array< std::array<std::array<double, 4>, 10>, 10> q,float p1, float p2,char** board, double alpha, double gamma,double epsilon);


char** build_board(){
    char** board = new char*[10];
    for (int i = 0; i < 10; i++){
        board[i] = new char[10];
    } 
    for (int i = 0; i < 10; i++){
        for(int k = 0 ; k < 10; k++){
            board[k][i] = ' ';
        }
    }
    return board;
}
int select_action(double* policy){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double rando = dist(mt);
    double upper_bound = 0.0;
    double lower_bound = 0.0;
    for (int i = 0; i < 4; i++){
        upper_bound += policy[i];
        if (lower_bound < rando && rando <= upper_bound){
            return i;
        }
        lower_bound += policy[i];
    }
}
std::array< std::array<std::array<double, 4>, 10>, 10> initpolicy(){
    std::array< std::array<std::array<double, 4>, 10>, 10> policy;
    for(int i = 0; i < 10; i++){
        for(int k = 0; i < 10; k++){
            for(int a = 0; a < 4; a++){
                policy[k][i][a] = 0.25;
            }
        }
    }
    return policy;

}
std::array< std::array<std::array<double, 4>, 10>, 10> initq(){
    std::array< std::array<std::array<double, 4>, 10>, 10> Q;
    return Q;
}

std::array< std::array<std::array<double, 4>, 10>, 10> sarsa(std::array< std::array<std::array<double, 4>, 10>, 10> policy,std::array< std::array<std::array<double, 4>, 10>, 10> q,float p1, float p2,char** board, double alpha, double gamma,double epsilon){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 9);
    
    int x = dist(mt);
    int y = dist(mt);
    int A = epsilon_greedy(policy[y][x],epsilon);
    board[y][x] = 8;
    
    if (x != 9 && y != 0){
        //printf("[%d,%d]:%d\n",x,y,A);

        while (true) {
            double transitionP[4] = {0,0,0,0};
            int next_state;
            int x1 = x;
            int y1 = y;
            int a1 = A;
            int R = -1;
            int x2,y2,a2;

            if (A == 0){ //up
                if (below_wall(x,y)){
                    transitionP[0] = (1.0-p1-p2)/2.0; //left adj
                    transitionP[1] = (p1+p2); //same
                    transitionP[2] = (1.0-p1-p2)/2; //bottom adj
                    next_state = select_action(transitionP); //using select action function to select transition using transition probabilities
                    if (below_wall(x,y) && right_of_wall(x,y)){ //right corner below wall
                        if (next_state == 0){x -= 1;}
                        else if (next_state == 2){y += 1;}}
                        //else do nothing ...
                    else if (below_wall(x,y) && left_of_wall(x,y)){ //left corner below wall
                        if (next_state == 0){x -= 1;}
                        else if (next_state == 2){y += 1;}}
                    
                    else if (below_wall(x,y)){//just below wall
                        if (next_state == 0){x-=1;}
                        else if (next_state == 2){x+=1;}}
                }
                else if (left_of_wall(x,y)){//just left of wall
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y-=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y-=1;x-=1;}
                    
                }
                else if (right_of_wall(x,y)){//just right of wall
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y-=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y-=1;x+=1;}
                } 
                else{
                    transitionP[0] = p1; //desired state
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left adj
                    transitionP[3] = (1.0-p1-p2)/2.0; //right adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y-=1;}
                    //else if (next_state == 1){do nothing}
                    else if (next_state == 2){y-=1;x-=1;}
                    else if (next_state == 3){y-=1;x+=1;}
                }
            }
            if (A == 1){ //right
                if (left_of_wall(x,y)){
                    transitionP[0] = (1.0-p1-p2)/2.0; //up adj
                    transitionP[1] = (p1+p2); //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //down adj
                    next_state = select_action(transitionP);
                    if (above_wall(x,y)){ 
                        if(next_state == 0){y -= 1;}
                        else if (next_state == 2){ x -= 1;}
                    }
                    else if (below_wall(x,y)){
                        if (next_state == 0){y -= 1;}
                        else if (next_state == 2){x -= 1;}}
                        //else do nothing ...
                    
                    else {
                        if(next_state == 0){y-= 1;}
                        else if(next_state == 2){y+=1;}
                    }
                }
                else if (above_wall(x,y)){
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){x+=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y-=1;x+=1;}
                }
                else if (below_wall(x,y)){
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){x+=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y+=1;x+=1;}
                }
                else{
                    //std::cout << x <<"," << y << "action= " << A <<"right" << "\n";
                    transitionP[0] = p1; //desired state
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //top adj
                    transitionP[3] = (1.0-p1-p2)/2.0; //bottom adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){x+=1;}
                    //else if (next_state == 1){do nothing}
                    else if (next_state == 2){y-=1;x+=1;}
                    else if (next_state == 3){y+=1;x+=1;}
                }
            }
            if (A == 2){ //down
                if (above_wall(x,y)){
                        transitionP[0] = (1.0-p1-p2)/2.0; //left adj
                        transitionP[1] = (p1+p2); //same
                        transitionP[2] = (1.0-p1-p2)/2.0; //top adj
                        next_state = select_action(transitionP); //using select action function to select transition using transition probabilities
                        if (above_wall(x,y) && right_of_wall(x,y)){ //right corner above wall
                            if (next_state == 0){x -= 1;}
                            else if (next_state == 2){y -= 1;}}
                            //else do nothing ...
                        else if (above_wall(x,y) && left_of_wall(x,y)){ //left corner above wall
                            if (next_state == 0){x -= 1;}
                            else if (next_state == 2){y -= 1;}}
                        
                        else if (above_wall(x,y)){//just above wall
                            if (next_state == 0){x-=1;}
                            else if (next_state == 2){x+=1;}}
                    }
                else if (left_of_wall(x,y)){//just left of wall
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y+=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y+=1;x-=1;}
                    
                }
                else if (right_of_wall(x,y)){//just left of wall
                    transitionP[0] = p1; //desired state //left adj
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y+=1;}
                    //else if(next_state == 1){do nothing}
                    else if(next_state == 2){y+=1;x+=1;}
                } 
                else{
                    transitionP[0] = p1; //desired state
                    transitionP[1] = p2; //same
                    transitionP[2] = (1.0-p1-p2)/2.0; //left adj
                    transitionP[3] = (1.0-p1-p2)/2.0; //right adj
                    next_state = select_action(transitionP);
                    if (next_state == 0){y+=1;}
                    //else if (next_state == 1){do nothing}
                    else if (next_state == 2){y+=1;x-=1;}
                    else if (next_state == 3){y+=1;x+=1;}
                }
            }
            if (A == 3){//left
                if (right_of_wall(x,y)){
                        transitionP[0] = (1.0-p1-p2)/2.0; //up adj
                        transitionP[1] = (p1+p2); //same
                        transitionP[2] = (1.0-p1-p2)/2.0; //down adj
                        next_state = select_action(transitionP);
                        if (above_wall(x,y)){ 
                            if(next_state == 0){y -= 1;}
                            else if (next_state == 2){ x += 1;}
                        }
                        else if (below_wall(x,y)){
                            if (next_state == 0){y -= 1;}
                            else if (next_state == 2){x += 1;}}
                            //else do nothing ...
                        
                        else {
                            if(next_state == 0){y-= 1;}
                            else if(next_state == 2){y+=1;}
                        }
                    }
                    else if (above_wall(x,y)){
                        transitionP[0] = p1; //desired state 
                        transitionP[1] = p2; //same
                        transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                        next_state = select_action(transitionP);
                        if (next_state == 0){x-=1;}
                        //else if(next_state == 1){do nothing}
                        else if(next_state == 2){y-=1;x-=1;}
                    }
                    else if (below_wall(x,y)){
                        transitionP[0] = p1; //desired state //left adj
                        transitionP[1] = p2; //same
                        transitionP[2] = (1.0-p1-p2)/2.0; //left up adj
                        next_state = select_action(transitionP);
                        if (next_state == 0){x-=1;}
                        //else if(next_state == 1){do nothing}
                        else if(next_state == 2){y+=1;x-=1;}
                    }
                    else{
                        transitionP[0] = p1; //desired state
                        transitionP[1] = p2; //same
                        transitionP[2] = (1.0-p1-p2)/2.0; //top adj
                        transitionP[3] = (1.0-p1-p2)/2.0; //bottom adj
                        next_state = select_action(transitionP);
                        if (next_state == 0){x-=1;}
                        //else if (next_state == 1){do nothing}
                        else if (next_state == 2){y-=1;x-=1;}
                        else if (next_state == 3){y+=1;x-=1;}
                    }
            }
            if (x == 9 && y == 0){R=100;}
            x2 = x;
            y2 = y;
            a2 = epsilon_greedy(q[y1][x1],epsilon);
            q[y1][x1][a1] = q[y1][x1][a1] + alpha*(R + gamma*q[y2][x2][a2] - q[y1][x1][a1]);
            if (x == 9 && y == 0){break;}

            
            //std::printf("[%d,%d]:%d\n",x,y,A);
            /*
            char c ;
            if (A == 0){c = '^';}
            else if (A == 1){c = '>';}
            else if (A == 2){c = 'v';}
            else if (A==3){c = '<';}
            board[y][x] = c;*/
            }
        }
    else{
        return sarsa(policy,q,p1,p2,board,alpha,gamma,epsilon);
    }
}

int epsilon_greedy(std::array<double, 4> policy,float epsilon){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double rando = dist(mt);
    if (rando <= 0.3){
        std::random_device rd2;
        std::mt19937 mt2(rd2());
        std::uniform_int_distribution<int> dist(0,3);
        int rando = dist(mt2);
        return rando;
        }
    else{return std::distance(policy,std::max_element(policy.begin(),policy.end()));}
}
int maxIndex(std::array<double, 4> arr){

}
bool left_of_wall(int x, int y){
    if (x == 9){return true;}
    if (x == 4 && y != 2 && y != 7){return true;}
    return false;
}
bool right_of_wall(int x, int y){
    if (x == 0){return true;}
    if (x == 5 && y != 2 && y != 7){return true;}
    return false;
}
bool above_wall(int x, int y){
    if (y == 9){return true;}
    if (y == 4 && x != 2 && x != 7){return true;}
    return false;
}
bool below_wall(int x, int y){
    if (y == 0){return true;}
    if (y == 5 && x != 2 && x != 7){return true;}
    return false;
}

int main(int argc, char *argv[]) {
    std::array< std::array<std::array<double, 4>, 10>, 10>  policy = initpolicy();
    std::array< std::array<std::array<double, 4>, 10>, 10>  new_policy;

    char** board = build_board();

    printf("\nKek");
}