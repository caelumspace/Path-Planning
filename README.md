# A* Pathfinding Example in C++

This project contains a simple demonstration of the A* (A-star) algorithm for pathfinding on a 2D grid map. By default, it reads a map from `map.txt`, which specifies the size of the grid (rows and columns) followed by the grid cells. Each cell can be `0` (walkable) or `1` (obstacle).

## Getting Started

1. **Clone or download this repository** to your local machine.
2. **Ensure you have a C++ compiler** such as `g++` installed.
3. **Copy** the `map.txt` file into the same directory as the C++ source.
  
  ### Example `map.txt`
  
  An example `map.txt` might look like this:
  5 5 0 0 1 1 0 0 1 1 1 0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0
  This represents a 5x5 grid.
  
  ## Compilation
  
  To compile with `g++`, run:
  
  ```
  g++ main.cpp -o a_star
  ```
  
  This will produce an executable named a_star.
  Usage
  After compiling, run:

   ```bash
   ./a_star
   ```

The program will:
Read the grid dimensions and data from map.txt.
Execute the A algorithm* from the top-left corner of the grid (0,0) to the bottom-right corner (rows-1, cols-1).
Output the path coordinates, if found.
Print the path overlaid on the grid using the following symbols:
S = Start
G = Goal
. = Walkable

# = Obstacle

P = Path
Modifications
Change the start and goal by modifying:
int startRow = 0, startCol = 0;
int goalRow = rows - 1, goalCol = cols - 1;
in main.cpp.
Switch between different heuristics (Manhattan, Euclidean, etc.) by editing the heuristicManhattan function in main.cpp.
License
This project is for demonstration and educational purposes. Feel free to modify and use it in your own applications. No warranty provided.

---

### How it Works

1. **Reading the Map**  
  The program reads two integers (rows and columns) from `map.txt`. Then it reads each row of the grid, with `0` or `1` indicating whether the cell is walkable or an obstacle.
2. **A* Algorithm**
  - Each grid cell is represented by a `Node` that keeps track of:
    - `gCost`: the cost so far (distance from the start node).
    - `hCost`: the heuristic estimate to the goal.
    - `fCost`: `gCost + hCost`.
    - `parent`: a pointer to the previous node in the path for reconstruction.
  - The open set is managed by a `std::priority_queue` that always expands the node with the smallest `fCost`.
  - Closed set tracking is done via a 2D boolean array that indicates nodes that have been processed.
  - The **Manhattan** distance (|x1 - x2| + |y1 - y2|) is used as the heuristic in this example.
3. **Output**  
  The path (if found) is printed to the console, along with the map containing the path marked with `P`. If no path is found, the program reports “No path found.”

---

## Conclusion

This sample provides a foundation for using A* for map routing in C++. You can extend the program to support:

- Dynamic obstacles.
- Larger or 3D maps.
- Other heuristics or tie-breaking strategies.
- Different cost values for different terrains.
  Feel free to adapt the code to your own applications and enjoy the power of A* in your projects!
