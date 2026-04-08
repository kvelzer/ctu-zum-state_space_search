#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <functional>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <compare>
#include <ranges>
#include <optional>
#include <variant>
#include <fstream> 
#include <chrono>
#include <thread>
#include <bits/stdc++.h>




struct Point {
        size_t x;
        size_t y;
};

Point globalEnd;
std::map<std::pair<size_t,size_t>, int> gScoreGlobal;

double heuristic(const Point & p) {
    return std::abs((double)p.x - (double)globalEnd.x) + std::abs((double)p.y - (double)globalEnd.y);
}

struct CmpGreedy {
    bool operator()(const Point & a, const Point & b) {
        return heuristic(a) > heuristic(b);
    }
};

struct CmpAstar {
    bool operator()(const Point & a, const Point & b) {
        double fA = gScoreGlobal[{a.x, a.y}] + heuristic(a);
        double fB = gScoreGlobal[{b.x, b.y}] + heuristic(b);
        return fA > fB;
    }
};

void printTemp (const std::vector<std::string> & input) {
    for (size_t i = 0; i < input.size();i++) {
        std::cout << input[i] << std::endl;
    }
    return;
}

void printSummary (const size_t & nodesExpanded, const size_t & pathLength) {
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "S Start" << std::endl;
    std::cout << "E End" << std::endl;
    std::cout << "# Opened node" << std::endl;
    std::cout << "o Path" << std::endl;
    std::cout << "X Wall" << std::endl;
    std::cout << "space Fresh node" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Nodes expanded: " << nodesExpanded << std::endl;
    std::cout << "Path length: " << pathLength << std::endl;
}

void mazeInput(const std::string & soubor, std::vector<std::string> & input, Point & start, Point & end) {

    std::ifstream in(soubor, std::ios_base::in);
    std::string tmpRadek;
    while (std::getline(in,tmpRadek)) {
        if (tmpRadek[0] == 's') break;
        input.push_back(tmpRadek);
    }
    std::istringstream startRadek(tmpRadek);
    std::string slovoStart;
    char carka;
    startRadek >> slovoStart >> start.x >> carka >> start.y;
    input[start.y][start.x] = 'S';

    std::getline(in,tmpRadek);
    std::istringstream endRadek(tmpRadek);
    std::string slovoEnd;
    endRadek >> slovoEnd >> end.x >> carka >> end.y;
    input[end.y][end.x] = 'E';
}

void randomSearch(const std::string & soubor, const int & miliseconds) {
    srand(time(NULL));
    std::vector<std::string> input;
    Point start, end;
    mazeInput(soubor, input, start,end);
    std::vector<Point> q;
    std::map<std::pair<size_t,size_t>, Point> pred;
    std::set<std::pair<size_t,size_t>> visited;
    visited.emplace(std::make_pair(start.x,start.y));
    q.push_back(start);
    Point tmp;
    size_t nodesExpanded = 0;
    size_t pathLength = 1;
    while(!q.empty()) {
        int index = rand() % q.size();
        tmp = q[index];
        q.erase(q.begin() + index);
        nodesExpanded++;
        if (tmp.x == end.x && tmp.y == end.y) {
            input[end.y][end.x] = 'E';
            break;
        }
        if (tmp.x != start.x || tmp.y != start.y) input[tmp.y][tmp.x] = '#';

         
        if (input[tmp.y+1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y+1))) {
            pred[{tmp.x,tmp.y+1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y+1));
            q.push_back({tmp.x,tmp.y+1});
            
        }
        if (input[tmp.y-1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y-1))) {
            pred[{tmp.x,tmp.y-1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y-1));
            q.push_back({tmp.x,tmp.y-1});
            
        }
        if (input[tmp.y][tmp.x-1] != 'X' && !visited.count(std::make_pair(tmp.x-1,tmp.y))) {
            pred[{tmp.x-1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x-1,tmp.y));
            q.push_back({tmp.x-1,tmp.y});
         
        }
        if (input[tmp.y][tmp.x+1] != 'X' && !visited.count(std::make_pair(tmp.x+1,tmp.y))) {
            pred[{tmp.x+1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x+1,tmp.y));
            q.push_back({tmp.x+1,tmp.y});
          
        }
        
        printTemp(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
        system ("clear");

        

    }

    Point tmpTmp = {end.x,end.y};
    if (end.x == start.x && end.y == start.y) {
        nodesExpanded = 1;
        pathLength =0;

    }
    else {
        while (true) {
        tmpTmp = pred[{tmpTmp.x,tmpTmp.y}];
        if (tmpTmp.y == start.y && tmpTmp.x == start.x) break;
        input[tmpTmp.y][tmpTmp.x] = 'o';
        pathLength++;
    }
    }
    

    printTemp(input);
    printSummary(nodesExpanded,pathLength);

}

void bfs(const std::string & soubor, const int & miliseconds) {
    std::vector<std::string> input;
    Point start, end;
    mazeInput(soubor, input, start,end);
    std::queue<Point> q;
    std::map<std::pair<size_t,size_t>, Point> pred;
    std::set<std::pair<size_t,size_t>> visited;
    visited.emplace(std::make_pair(start.x,start.y));
    q.push(start);
    Point tmp;
    size_t nodesExpanded = 0;
    size_t pathLength = 1;
    while(!q.empty()) {
        tmp = q.front();
        q.pop();
        nodesExpanded++;
        if (tmp.x == end.x && tmp.y == end.y) {
            input[end.y][end.x] = 'E';
            break;
        }
        if (tmp.x != start.x || tmp.y != start.y) input[tmp.y][tmp.x] = '#';

            
        // Point vpravo = {tmp.x,tmp.y+1};
        // Point vlevo = {tmp.x,tmp.y-1};
        // Point nahoru = {tmp.x-1,tmp.y};
        // Point dolu = {tmp.x+1,tmp.y};

        if (input[tmp.y+1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y+1))) {
            pred[{tmp.x,tmp.y+1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y+1));
            q.push({tmp.x,tmp.y+1});
            
        }
        if (input[tmp.y-1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y-1))) {
            pred[{tmp.x,tmp.y-1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y-1));
            q.push({tmp.x,tmp.y-1});
            
        }
        if (input[tmp.y][tmp.x-1] != 'X' && !visited.count(std::make_pair(tmp.x-1,tmp.y))) {
            pred[{tmp.x-1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x-1,tmp.y));
            q.push({tmp.x-1,tmp.y});
            
        }
        if (input[tmp.y][tmp.x+1] != 'X' && !visited.count(std::make_pair(tmp.x+1,tmp.y))) {
            pred[{tmp.x+1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x+1,tmp.y));
            q.push({tmp.x+1,tmp.y});
            
        }
        
        printTemp(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
        system ("clear");

        

    }

    Point tmpTmp = {end.x,end.y};
    if (end.x == start.x && end.y == start.y) {
        nodesExpanded = 1;
        pathLength =0;

    }
    else {
        while (true) {
        tmpTmp = pred[{tmpTmp.x,tmpTmp.y}];
        if (tmpTmp.y == start.y && tmpTmp.x == start.x) break;
        input[tmpTmp.y][tmpTmp.x] = 'o';
        pathLength++;
    }
    }
    

    printTemp(input);
    printSummary(nodesExpanded,pathLength);

}

//dfs
void dfs(const std::string & soubor,const int & miliseconds) {
    std::vector<std::string> input;
    Point start, end;
    mazeInput(soubor, input, start,end);
    std::stack<Point> q;
    std::map<std::pair<size_t,size_t>, Point> pred;
    std::set<std::pair<size_t,size_t>> visited;
    visited.emplace(std::make_pair(start.x,start.y));
    q.push(start);
    Point tmp;
    size_t nodesExpanded = 0;
    size_t pathLength = 1;
    while(!q.empty()) {
        tmp = q.top();
        q.pop();
        nodesExpanded++;
        if (tmp.x == end.x && tmp.y == end.y) {
            input[end.y][end.x] = 'E';
            break;
        }
        if (tmp.x != start.x || tmp.y != start.y) input[tmp.y][tmp.x] = '#';

            
        // Point vpravo = {tmp.x,tmp.y+1};
        // Point vlevo = {tmp.x,tmp.y-1};
        // Point nahoru = {tmp.x-1,tmp.y};
        // Point dolu = {tmp.x+1,tmp.y};

        if (input[tmp.y+1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y+1))) {
            pred[{tmp.x,tmp.y+1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y+1));
            q.push({tmp.x,tmp.y+1});
        
        }
        if (input[tmp.y-1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y-1))) {
            pred[{tmp.x,tmp.y-1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y-1));
            q.push({tmp.x,tmp.y-1});
        
        }
        if (input[tmp.y][tmp.x-1] != 'X' && !visited.count(std::make_pair(tmp.x-1,tmp.y))) {
            pred[{tmp.x-1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x-1,tmp.y));
            q.push({tmp.x-1,tmp.y});
        
        }
        if (input[tmp.y][tmp.x+1] != 'X' && !visited.count(std::make_pair(tmp.x+1,tmp.y))) {
            pred[{tmp.x+1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x+1,tmp.y));
            q.push({tmp.x+1,tmp.y});
        
        }
        
        printTemp(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
        system ("clear");
    }

    Point tmpTmp = {end.x,end.y};
    if (end.x == start.x && end.y == start.y) {
        nodesExpanded = 1;
        pathLength =0;

    }
    else {
        while (true) {
        tmpTmp = pred[{tmpTmp.x,tmpTmp.y}];
        if (tmpTmp.y == start.y && tmpTmp.x == start.x) break;
        input[tmpTmp.y][tmpTmp.x] = 'o';
        pathLength++;
    }
    }
    

    printTemp(input);
    printSummary(nodesExpanded,pathLength);
}

void greedy(const std::string & soubor,const int & miliseconds) {
    std::vector<std::string> input;
    Point start, end;
    mazeInput(soubor, input, start,end);
    globalEnd = end;
    std::priority_queue<Point, std::vector<Point>, CmpGreedy> q;

    std::map<std::pair<size_t,size_t>, Point> pred;
    std::set<std::pair<size_t,size_t>> visited;
    visited.emplace(std::make_pair(start.x,start.y));
    q.push(start);
    Point tmp;
    size_t nodesExpanded = 0;
    size_t pathLength = 1;
    while(!q.empty()) {
        tmp = q.top();
        q.pop();
        nodesExpanded++;

        if (tmp.x == end.x && tmp.y == end.y) {
            input[end.y][end.x] = 'E';
            break;
        }
        if (tmp.x != start.x || tmp.y != start.y) input[tmp.y][tmp.x] = '#';

            
        // Point vpravo = {tmp.x,tmp.y+1};
        // Point vlevo = {tmp.x,tmp.y-1};
        // Point nahoru = {tmp.x-1,tmp.y};
        // Point dolu = {tmp.x+1,tmp.y};

        if (input[tmp.y+1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y+1))) {
            pred[{tmp.x,tmp.y+1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y+1));
            q.push({tmp.x,tmp.y+1});
        }
        if (input[tmp.y-1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y-1))) {
            pred[{tmp.x,tmp.y-1}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x,tmp.y-1));
            q.push({tmp.x,tmp.y-1});
            
        }
        if (input[tmp.y][tmp.x-1] != 'X' && !visited.count(std::make_pair(tmp.x-1,tmp.y))) {
            pred[{tmp.x-1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x-1,tmp.y));
            q.push({tmp.x-1,tmp.y});
            
        }
        if (input[tmp.y][tmp.x+1] != 'X' && !visited.count(std::make_pair(tmp.x+1,tmp.y))) {
            pred[{tmp.x+1,tmp.y}] = {tmp.x,tmp.y};
            visited.emplace(std::make_pair(tmp.x+1,tmp.y));
            q.push({tmp.x+1,tmp.y});
            
        }
        
        printTemp(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
        system ("clear");

        

    }

    Point tmpTmp = {end.x,end.y};
    if (end.x == start.x && end.y == start.y) {
        nodesExpanded = 1;
        pathLength =0;

    }
    else {
        while (true) {
        tmpTmp = pred[{tmpTmp.x,tmpTmp.y}];
        if (tmpTmp.y == start.y && tmpTmp.x == start.x) break;
        input[tmpTmp.y][tmpTmp.x] = 'o';
        pathLength++;
    }
    }
    

    printTemp(input);
    printSummary(nodesExpanded,pathLength);
}

void astar(const std::string & soubor,const int & miliseconds) {
    std::vector<std::string> input;
    Point start, end;
    mazeInput(soubor, input, start,end);
    
    globalEnd = end;
    std::priority_queue<Point, std::vector<Point>, CmpAstar> q;

    std::map<std::pair<size_t,size_t>, Point> pred;
    std::set<std::pair<size_t,size_t>> visited;
    gScoreGlobal.emplace(std::make_pair(start.x,start.y),0);
    q.push(start);
    Point tmp;
    size_t nodesExpanded = 0;
    size_t pathLength = 1;
    while(!q.empty()) {
        tmp = q.top();
        q.pop();

        if (visited.count(std::make_pair(tmp.x, tmp.y))) {
            continue;
        }
        nodesExpanded++;

        visited.emplace(std::make_pair(tmp.x,tmp.y));

        if (tmp.x == end.x && tmp.y == end.y) {
            input[end.y][end.x] = 'E';
            break;
        }
        if (tmp.x != start.x || tmp.y != start.y) input[tmp.y][tmp.x] = '#';

            
        // Point vpravo = {tmp.x,tmp.y+1};
        // Point vlevo = {tmp.x,tmp.y-1};
        // Point nahoru = {tmp.x-1,tmp.y};
        // Point dolu = {tmp.x+1,tmp.y};

        
        if (input[tmp.y+1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y+1))) {
            if ( !gScoreGlobal.count(std::make_pair(tmp.x,tmp.y+1)) || gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1 < gScoreGlobal[std::make_pair(tmp.x,tmp.y+1)]){
                gScoreGlobal[std::make_pair(tmp.x,tmp.y+1)] = gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1;
                q.push({tmp.x,tmp.y+1});
                pred[{tmp.x,tmp.y+1}] = {tmp.x,tmp.y};
            }
            
        }
        if (input[tmp.y-1][tmp.x] != 'X' && !visited.count(std::make_pair(tmp.x,tmp.y-1))) {
            if (!gScoreGlobal.count(std::make_pair(tmp.x,tmp.y-1))  || (gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1 < gScoreGlobal[std::make_pair(tmp.x,tmp.y-1)])){
                gScoreGlobal[std::make_pair(tmp.x,tmp.y-1)] = gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1;
                q.push({tmp.x,tmp.y-1});
                pred[{tmp.x,tmp.y-1}] = {tmp.x,tmp.y};

            }
            
        }
        if (input[tmp.y][tmp.x-1] != 'X' && !visited.count(std::make_pair(tmp.x-1,tmp.y))) {
            if (!gScoreGlobal.count(std::make_pair(tmp.x-1,tmp.y)) || gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1 < gScoreGlobal[std::make_pair(tmp.x-1,tmp.y)]){
                gScoreGlobal[std::make_pair(tmp.x-1,tmp.y)] = gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1;
                q.push({tmp.x-1,tmp.y});
                pred[{tmp.x-1,tmp.y}] = {tmp.x,tmp.y};
            }
            
            
        }
        if (input[tmp.y][tmp.x+1] != 'X' && !visited.count(std::make_pair(tmp.x+1,tmp.y))) {
            if (!gScoreGlobal.count(std::make_pair(tmp.x+1,tmp.y)) || gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1 < gScoreGlobal[std::make_pair(tmp.x+1,tmp.y)]) {
                gScoreGlobal[std::make_pair(tmp.x+1,tmp.y)] = gScoreGlobal[std::make_pair(tmp.x,tmp.y)]+1;
                q.push({tmp.x+1,tmp.y});
                pred[{tmp.x+1,tmp.y}] = {tmp.x,tmp.y};

            }

            
        }
        
        printTemp(input);
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
        system ("clear");

    }

    Point tmpTmp = {end.x,end.y};
    if (end.x == start.x && end.y == start.y) {
        nodesExpanded = 1;
        pathLength =0;

    }
    else {
        while (true) {
        tmpTmp = pred[{tmpTmp.x,tmpTmp.y}];
        if (tmpTmp.y == start.y && tmpTmp.x == start.x) break;
        input[tmpTmp.y][tmpTmp.x] = 'o';
        pathLength++;
    }
    }
    

    printTemp(input);
    printSummary(nodesExpanded,pathLength);
}

int main(int argc, char * argv[]) {
    
    system ("clear");

    std::string soubor = "dataset/0.txt";
    std::string algoritmus = "bfs";
    int miliseconds = 500;

    if (argc >= 2) {
        algoritmus = argv[1];
    }
    if (argc >= 3) {
        soubor = argv[2];
    }
    if (argc >= 4) {
        miliseconds = atoi(argv[3]);
    }


    if (algoritmus == "bfs") {
        bfs(soubor,miliseconds);
    } else if (algoritmus == "dfs") {
         dfs(soubor,miliseconds);
    } else if (algoritmus == "random") {
        randomSearch(soubor,miliseconds);
    } else if (algoritmus == "greedy") {
         greedy(soubor,miliseconds);
    } else if (algoritmus == "astar") {
         astar(soubor,miliseconds);
    } else {
        std::cout << "PRomblemos " << std::endl;
        return 1;
    }
    
    return 0;
}