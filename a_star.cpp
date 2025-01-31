/*******************************************************
 * A* Algorithm Example in C++ for Grid Map Routing
 *
 * This program reads a grid from "map.txt" where
 *   0 = walkable cell
 *   1 = obstacle cell
 * Then it runs A* to find a path between a start
 * and goal position and prints the path if found.
 *******************************************************/

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>
#include <stdexcept>

// A small structure to hold basic node information.
struct Node {
    int row, col;
    float gCost;  // Cost from start to current node
    float hCost;  // Heuristic cost to goal
    float fCost;  // gCost + hCost
    Node* parent;
    
    Node(int r, int c) : row(r), col(c), gCost(0.0f), hCost(0.0f), fCost(0.0f), parent(nullptr) {}
};

// Comparator for the priority queue (min-heap based on fCost).
struct CompareFCost {
    bool operator()(const Node* a, const Node* b) {
        return a->fCost > b->fCost;
    }
};

// Heuristic function - using Manhattan distance for a grid.
float heuristicManhattan(int row1, int col1, int row2, int col2) {
    return static_cast<float>(std::abs(row1 - row2) + std::abs(col1 - col2));
}

// Check if a cell is within the map boundaries
bool isValid(int row, int col, int rows, int cols) {
    return (row >= 0 && row < rows && col >= 0 && col < cols);
}

// A* Search function
std::vector<std::pair<int,int>> aStarSearch(const std::vector<std::vector<int>>& grid,
                                           int startRow, int startCol,
                                           int goalRow, int goalCol)
{
    int rows = grid.size();
    int cols = grid[0].size();

    // Create a 2D array of Node pointers
    std::vector<std::vector<Node*>> allNodes(rows, std::vector<Node*>(cols, nullptr));
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            allNodes[r][c] = new Node(r, c);
        }
    }

    // Keep track of visited cells
    std::vector<std::vector<bool>> closedSet(rows, std::vector<bool>(cols, false));

    // Priority queue for open set
    std::priority_queue<Node*, std::vector<Node*>, CompareFCost> openSet;

    // Initialize the start node
    Node* startNode = allNodes[startRow][startCol];
    startNode->gCost = 0.0f;
    startNode->hCost = heuristicManhattan(startRow, startCol, goalRow, goalCol);
    startNode->fCost = startNode->gCost + startNode->hCost;
    openSet.push(startNode);

    // Directions for 4-neighborhood (up, down, left, right)
    int dRow[4] = {-1, 1, 0, 0};
    int dCol[4] = {0, 0, -1, 1};

    Node* goalNode = allNodes[goalRow][goalCol];

    while(!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        // If this node is already closed, skip
        if(closedSet[current->row][current->col])
            continue;

        // Mark current node as visited
        closedSet[current->row][current->col] = true;

        // Check if we reached the goal
        if(current == goalNode) {
            // Reconstruct path
            std::vector<std::pair<int,int>> path;
            Node* temp = goalNode;
            while(temp != nullptr) {
                path.push_back({temp->row, temp->col});
                temp = temp->parent;
            }
            // Reverse path to get start -> goal
            std::reverse(path.begin(), path.end());

            // Clean up allocated memory
            for(int r = 0; r < rows; r++) {
                for(int c = 0; c < cols; c++) {
                    delete allNodes[r][c];
                }
            }
            return path;
        }

        // Explore neighbors
        for(int i = 0; i < 4; i++) {
            int nr = current->row + dRow[i];
            int nc = current->col + dCol[i];

            if(!isValid(nr, nc, rows, cols)) continue;     // out of bounds
            if(grid[nr][nc] == 1) continue;                // obstacle
            if(closedSet[nr][nc]) continue;                // already in closed set

            Node* neighbor = allNodes[nr][nc];

            float tentativeGCost = current->gCost + 1.0f; // Cost from current to neighbor
            if(tentativeGCost < neighbor->gCost || neighbor->parent == nullptr) {
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = heuristicManhattan(nr, nc, goalRow, goalCol);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = current;
                openSet.push(neighbor);
            }
        }
    }

    // If we exit the loop, no path was found
    // Clean up allocated memory
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            delete allNodes[r][c];
        }
    }

    return {}; // Return empty path to indicate failure
}

int main() {
    // Read the map from a file
    std::ifstream fin("map.txt");
    if(!fin.is_open()) {
        std::cerr << "Error: Could not open map.txt\n";
        return 1;
    }

    int rows, cols;
    fin >> rows >> cols;
    if(rows <= 0 || cols <= 0) {
        std::cerr << "Invalid map dimensions.\n";
        return 1;
    }

    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols));

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            fin >> grid[r][c];
        }
    }
    fin.close();

    // Define start and goal (change as needed)
    int startRow = 0, startCol = 0;
    int goalRow = rows - 1, goalCol = cols - 1;

    // Make sure start and goal are valid
    if(grid[startRow][startCol] == 1 || grid[goalRow][goalCol] == 1) {
        std::cerr << "Start or goal is on an obstacle. Exiting.\n";
        return 1;
    }

    // Run A*
    auto path = aStarSearch(grid, startRow, startCol, goalRow, goalCol);

    // Check result
    if(path.empty()) {
        std::cout << "No path found.\n";
    } else {
        // Print path coordinates
        std::cout << "Path found (" << path.size() << " steps):\n";
        for(auto &p : path) {
            std::cout << "(" << p.first << ", " << p.second << ") ";
        }
        std::cout << "\n";

        // Mark path on the grid for visualization
        for(auto &p : path) {
            // Mark with 'P'
            if(!(p.first == startRow && p.second == startCol) && 
               !(p.first == goalRow && p.second == goalCol))
            {
                grid[p.first][p.second] = 2; 
            }
        }

        // Print grid with path
        // 0 = open, 1 = obstacle, 2 = path, S = start, G = goal
        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                if(r == startRow && c == startCol) {
                    std::cout << "S ";
                } else if(r == goalRow && c == goalCol) {
                    std::cout << "G ";
                } else if(grid[r][c] == 0) {
                    std::cout << ". ";
                } else if(grid[r][c] == 1) {
                    std::cout << "# ";
                } else if(grid[r][c] == 2) {
                    std::cout << "P ";
                }
            }
            std::cout << "\n";
        }
    }

    return 0;
}
