#include <random>
#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Stack.h"
#include <fstream>
#include <sstream>
#include <set>   // Include this header for std::set
#include <utility>
using namespace std;

const int MAX_M = INT_MAX; // Maximum maze size
const int MAX_N = INT_MAX;


struct Cell { //create a cell struct representing a single cell
    bool solve_visited;
    bool visited;
    int row, col;
    bool isPath;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;
    bool top_wall = true; //assume that there is a wall up, down, right and left for a single cell.
    bool bottom_wall = true;
    bool left_wall = true;
    bool right_wall = true;
    Cell(){} //default constructor
    Cell(int r, int c) : row(r), col(c), visited(false) , solve_visited(false), isPath(false) {} //first parameter is row number and the second one is column number for corresponding cell, and of
    // ofcourse it is unintialised.
};

struct Maze { //create a maze struct representing the whole maze.
    int M, N; //M is the row number, N is the columns number
    Cell* cells; // in order to access the members of Cell Struct we need to declare a pointer object of Cell struct
    int count;

};

void InitializeMaze(Maze& maze) {
    // Initialize the stack
    
    maze.cells = new Cell[maze.M * maze.N]; //declare the cells
    for (int i = 0; i < maze.M; i++) {

        for (int j = 0; j < maze.N; j++) {

            Cell& cell = maze.cells[i * maze.N + j]; // start intialising all cells one by one with a referenced cell object of Cell struct.
            cell.visited = false;
            cell.solve_visited = false;
            cell.isPath = false;
            cell.row = i;
            cell.col = j;
            if (i > 0) {
                cell.up = &maze.cells[(i - 1) * maze.N + j]; //initialise their up, down, right, and left cells as well.
            }
            else {
                cell.up = nullptr;
            }
            if (i < maze.M - 1) {
                cell.down = &maze.cells[(i + 1) * maze.N + j];
            }
            else {
                cell.down = nullptr;
            }
            if (j > 0) {
                cell.left = &maze.cells[i * maze.N + (j - 1)];
            }
            else {
                cell.left = nullptr;
            }
            if (j < maze.N - 1) {
                cell.right = &maze.cells[i * maze.N + (j + 1)];
            }
            else {
                cell.right = nullptr;
            }
        }

    }
}

vector<Cell*> getUnvisitedNeighbors(Cell* cell, Maze& maze) { //a vector function with Cell pointer type returning.

    vector<Cell*> neighbors; //initialise a vector with Cell pointer to return at the end.
    int i = cell->row;
    int j = cell->col;

    if (i > 0 && !(maze.cells[(i - 1) * maze.N + j].visited)) { //check if we are not on the uppest row and the upper row is not visited.

        neighbors.push_back(&maze.cells[(i - 1) * maze.N + j]); //if so, push it into the vector ve created.
    }
    if (i < maze.M - 1 && !(maze.cells[(i + 1) * maze.N + j].visited)) { //check if we are not on the downest row and the below row is not visited.
        neighbors.push_back(&maze.cells[(i + 1) * maze.N + j]);
    }
    if (j > 0 && !(maze.cells[i * maze.N + (j - 1)].visited)) { //check if we are not on the leftest column and the column on the left is unvisted.
        neighbors.push_back(&maze.cells[i * maze.N + (j - 1)]);
    }
    if (j < maze.N - 1 && !(maze.cells[i * maze.N + (j + 1)].visited)) { //check if we are not on the rightest column and the column on the right is unvisited.
        neighbors.push_back(&maze.cells[i * maze.N + (j + 1)]);
    }

    return neighbors; //return the vector with pointers of Cell struct.
}


void RemoveWall(Cell* currentCell, Cell* nextCell) { 
    // Determine the position of the next cell relative to the current cell
    int dx = currentCell->col - nextCell->col; 
    int dy = currentCell->row - nextCell->row;

    // Remove the appropriate walls
    if (dx == 1) {
        currentCell->left_wall = false;
        nextCell->right_wall = false;
    }
    else if (dx == -1) {
        currentCell->right_wall = false;
        nextCell->left_wall = false;
    }

    if (dy == 1) {
        currentCell->top_wall = false;
        nextCell->bottom_wall = false;
    }
    else if (dy == -1) {
        currentCell->bottom_wall = false;
        nextCell->top_wall = false;
    }
}
void GenerateMazeIteratively(Maze& maze) {
    std::stack<Cell*> s;
    Cell* currentCell = &maze.cells[0];  // Starting from the first cell
    currentCell->visited = true;
    s.push(currentCell);

    while (!s.empty()) {
        currentCell = s.top();
        vector<Cell*> unvisitedNeighbors = getUnvisitedNeighbors(currentCell, maze);

        if (!unvisitedNeighbors.empty()) {
            // Randomly select an unvisited neighbor
            Cell* nextCell = unvisitedNeighbors[rand() % unvisitedNeighbors.size()];
            RemoveWall(currentCell, nextCell);
            nextCell->visited = true;
            s.push(nextCell);
        }
        else {
            // Backtrack
            s.pop();
        }
    }
}

/*
void GenerateMazeRecursively(Cell* currentCell, Maze& maze, Stack<Cell*>& s) { //a recursive function to generate maze.
    //parameters; a pointer to a Cell struct, a maze object and a stack having a pointers to Cell struct with a reference parameter
    if (currentCell != nullptr) { 
        currentCell->visited = true;
    }
    else {
        return;
    }

    s.push(currentCell); //push the first pointer into the stack
    vector<Cell*> unvisitedNeighbors = getUnvisitedNeighbors(currentCell, maze); // get the unvisited neighbours of that pointer cell.

    while (!unvisitedNeighbors.empty()) { //as long as the vector is not empty.
        // Randomly select an unvisited neighbor
        Cell* nextCell = unvisitedNeighbors[rand() % unvisitedNeighbors.size()];
        RemoveWall(currentCell, nextCell);
        //after removing the walls on each side of the corresponding cell, call our recursive function again but this time the next available cell we picked.
        GenerateMazeRecursively(nextCell, maze, s); // continue with this process untill we come across with a cell having no any unvisited neighbours.
        unvisitedNeighbors = getUnvisitedNeighbors(currentCell, maze);
    }

    // Backtrack if there are no unvisited neighbors
    while (unvisitedNeighbors.empty() && !s.isEmpty()) { //if the top cell on the stack has no neighbour again, repeat the process.
        currentCell = s.topAndPop(); //use the stack to back track.
        unvisitedNeighbors = getUnvisitedNeighbors(currentCell, maze);
        if (!unvisitedNeighbors.empty()) { //if we find a stack element with a at least one unvisited neighbour. we call our maze generation functiuon again.
            // Found a cell with unvisited neighbors, resume maze generation from this cell
            GenerateMazeRecursively(currentCell, maze, s); // after visiting all the cells and the stack if finally empty. The loop will not work and recursive will end.
            return; // Important to return here to avoid repeating the same cell
        }
    }

    // If the stack is empty, then we have finished the maze generation
}*/

// Function to get a random cell from a maze with M rows and N columns
Cell* getRandomCell(Maze& maze) {

    int row = rand() % maze.M;    // Random row
    int col = rand() % maze.N; // Random column
    return &(maze.cells[row * maze.N + col]);
}

void printCoordinates(Maze& maze, const string & filename) { //a function to print out the coordinates those have been created by the previous functions.
    ofstream file(filename); //create a ofstream object

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < maze.M; i++) {

        for (int j = 0; j < maze.N; j++) {

            Cell * currentCell = &maze.cells[(i)*maze.N + j]; //intialise a new currentCell pointer keeping the address of each cell that was generated.
            //print out the following informations on to .txt file.
            file << "x=" << currentCell->col << " " << "y=" << currentCell->row << " " << "l=" << currentCell->left_wall << " " << "r=" << currentCell->right_wall << " " << "u=" <<  currentCell->top_wall << " " << "d=" << currentCell->bottom_wall << endl;
      
        }
    }
    file.close(); //close the file

}
void printMazeToFile(const Maze& maze, const std::string& filename) { //this time for visualisation purposes.
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Print the top row of walls
    for (int j = 0; j < maze.N; ++j) {
        Cell* currentCell = &maze.cells[j]; // Top row cells
        if (currentCell->top_wall) {
            if (j == 0) {
                
                file << "    ";
            }
            file << "___";
        }
        
    }
    file << "\n";

    for (int i = 0; i < maze.M; ++i) {
        for (int j = 0; j < maze.N; ++j) {
            Cell* currentCell = &maze.cells[i * maze.N + j];

            // Print left wall for each cell
            if (currentCell->left_wall) {
                if (j == 0) {
                    file << (maze.M - i -1);
                    if ((maze.M - i - 1) >= 10) { //we need to push one less space than one digit number of rows because more than one digit pushes more than a single digit.
                        file << " ";
                    }
                    else if ((maze.M - i - 1) >= 100) { // same logic.
                        file << "";
                    }
                    else { //if this is a single digit row.
                        file << "  ";
                    }
                    
                }
                
                file << "|";
            }
            else {
                
                file << "_";
            }

            // Print bottom wall or space
            if (i == maze.M - 1) { // For the bottom-most cells
                
                file << "__";
            }
            else { // For nonbottom cells, check the bottom neighbor
                Cell* bottomNeighbor = &maze.cells[(i + 1) * maze.N + j];
                if (currentCell->bottom_wall || bottomNeighbor->top_wall) {
                    file << "__";
                }
                else {
                    file << "  ";
                }
            }
        }

        // Print right wall of the last column
        file << "|\n";
    }

    file.close();
}

bool vectorFind(vector<Cell*> obj, Cell * cll) {
    for (const auto& x : obj) {
        if (cll->row == x->row && cll->col == x->col) {
            return true;
        }
    }
    return false;
}

void vectorRemove(vector<Cell*> obj, Cell* cll) {
    for (int i = 0; i < obj.size(); i++) {
        if (cll->row == obj[i]->row && cll->col == obj[i]->col) {
            obj[i] = NULL;
        }
    }
}

void isPathAdjuster(vector<Cell*> path, Maze & maze) {
    
    for (int i = 0; i < maze.M; i++) {
        for (int j = 0; j < maze.N; j++) {
            Cell * ptr = &maze.cells[i * maze.N + j];
            if (vectorFind(path, ptr)) {
                ptr->isPath = true;
                vectorRemove(path, ptr);
            }
        }
    }
}




void printMazeSolutionToFile(Maze& maze, const std::string& filename, const std::vector<Cell*>& path) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    // Print the top row of walls
    for (int j = 0; j < maze.N; ++j) {
        Cell* currentCell = &maze.cells[j];
        //isPathAdjuster(path, currentCell); // Top row cells
        if (currentCell->top_wall) {
            if (j == 0) {

                file << "    ";
            }
            file << "___ ";
        }

    }
    
    file << "\n";
    // Iterate through the maze and build the string representation
    for (int i = 0; i < maze.M; ++i) {
        for (int j = 0; j < maze.N; ++j) {
            Cell* currentCell = &maze.cells[i * maze.N + j];

            // Print left wall for each cell
            if (j == 0) {
                file << (maze.M - i - 1);
                if ((maze.M - i - 1) >= 10) {
                    file << " ";
                }
                else if ((maze.M - i - 1) >= 100) { // same logic.
                    file << "";
                }
                else {
                    file << "  ";
                }
            }
            file << (currentCell->left_wall ? "|" : "_");
            //file << (currentCell->right_wall ? "|" : "_");

            // Print the cell space or path
            if (std::find(path.begin(), path.end(), currentCell) != path.end()) {
                if (currentCell == path[path.size() - 1]) {
                    file << "E";
                }
                else if (currentCell == path[0]) {
                    file << "E";
                }
                else {
                    file << "*";
                }
                
            
            }
            else {
                file << " ";
            }

            // Print bottom wall or space
            if (i == maze.M - 1 || currentCell->bottom_wall) {
                file << "__";
            }
            else {
                file << "  ";
            }
            
        }

        // Print right wall of the last column
        file << "|\n";
    }

    file.close();
}

bool findPathDFS(Cell* currentCell, Cell* endCell, Maze& maze, vector<Cell*>& path) {
    
    if (currentCell == nullptr || currentCell->solve_visited) {
        return false;
    }

    // Mark the current cell as visited and add it to the path
    currentCell->solve_visited = true;
    path.push_back(currentCell); //store the current path into a Cell* type vector preserving its order.

    // Check if the current cell is the end cell
    if (currentCell == endCell) {
        return true;
    }

    // Try to find a path from each neighbor
    if ((currentCell->up && !(currentCell->top_wall) && findPathDFS(currentCell->up, endCell, maze, path)) || //in this if statement, check if neighbour cell exist. If so, it wil return true. As long as they are not nullptr, pointer is return at the if statement.
        (currentCell->down && !(currentCell->bottom_wall) && findPathDFS(currentCell->down, endCell, maze, path)) || //call the recursive function again. Apply the same logic.
        (currentCell->left && !(currentCell->left_wall) && findPathDFS(currentCell->left, endCell, maze, path)) ||
        (currentCell->right && !(currentCell->right_wall) && findPathDFS(currentCell->right, endCell, maze, path))) {
        return true;
    }

    // No path found from this cell, backtrack
    path.pop_back();
    return false;
}






int main() {
    int mazenum;
    int rows, cols;
    static Stack<Cell*> j;
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << "Enter the number of mazes: ";
    cin >> mazenum;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rows >> cols;
    cout << "All mazes are generated." << endl;

    for (int k = 0; k < mazenum; k++) {
        Maze maze;
        maze.M = rows;
        maze.N = cols;
        InitializeMaze(maze);
        Cell* startCell = getRandomCell(maze);
        GenerateMazeIteratively(maze);
        //GenerateMazeRecursively(startCell, maze, j);
        cout << endl;

        stringstream filenameStream;
        filenameStream << "maze" << (k + 1) << ".txt";
        string filename = filenameStream.str();
        
        stringstream mazeDrawn;
        mazeDrawn << "mazeDrawn" << (k + 1) << ".txt";
        string drawnFilename = mazeDrawn.str();

        printCoordinates(maze, filename);
        printMazeToFile(maze, drawnFilename);
        //Cell* start_Cell = getRandomUnvisitedCell(maze);
        
        Cell* rand_start_Cell = getRandomCell(maze); // Pick a random starting cell from the maze
        Cell* endCell;
        
        do {
            endCell = getRandomCell(maze); // Pick a random ending cell from the maze
        } while (endCell == rand_start_Cell); // Continue picking until a different cell is found 
        
        vector<Cell*> path; //declare a vector with Cell* type. Thus, pointers can be stored in order.
        

        if (findPathDFS(rand_start_Cell, endCell, maze, path)) {
            cout << "Path found from (" << rand_start_Cell->row << ", " << rand_start_Cell->col << ") to (" << endCell->row << ", " << endCell->col << ")" << endl;
            for (const auto& x : path) {
                cout << "ROW: " << x->row << "  " << "COLUMNS: " << x->col << endl;
            }
            stringstream pathFilename;
            pathFilename << "mazePath" << (k + 1) << ".txt";
            string ebe = pathFilename.str();
            printMazeSolutionToFile(maze, ebe,path);
        }
        else {
            cout << "No path found" << endl;
        }
        
        

    }



    return 0;
}
