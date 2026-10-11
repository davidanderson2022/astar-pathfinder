#include <iostream>
#include <vector>
#include <string>
using namespace std;

const int WIDTH = 10;
const int HEIGHT = 7;

// '#' wall, '.' open, 'S' start, 'E' end
vector<string> grid = {
    "##########",
    "#S...#...#",
    "###..#.E.#",
    "#....#...#",
    "#.######.#",
    "#........#",
    "##########",
};
struct Node {
    int x, y;             // position on the grid
    int g;                // cost from the start
    int h;                // heuristic: estimated cost left
    int f;                // g + h (what A* decides on)
    int parentX, parentY; // where we came from
};

const int START_X = 1, START_Y = 1;
const int END_X = 7, END_Y = 2;

// Manhattan distance: how far in straight lines
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
bool findPath() {
    vector<Node> open;   // to explore
    vector<Node> closed; // already explored

    Node start;
    start.x = START_X; start.y = START_Y;
    start.g = 0;
    start.h = heuristic(START_X, START_Y, END_X, END_Y);
    start.f = start.g + start.h;
    start.parentX = -1; start.parentY = -1;
    open.push_back(start);

    int dx[4] = { 0, 0, -1, 1 };
    int dy[4] = { -1, 1, 0, 0 };

    while (!open.empty()) {
        // grab the lowest-f node
        int best = 0;
        for (int i = 1; i < open.size(); i++)
            if (open[i].f < open[best].f) best = i;

        Node current = open[best];
        open.erase(open.begin() + best);
        closed.push_back(current);

        // reached the end? trace parents back, stamp '*'
        if (current.x == END_X && current.y == END_Y) {
            int x = current.parentX, y = current.parentY;
            while (!(x == START_X && y == START_Y)) {
                grid[y][x] = '*';
                for (Node n : closed)
                    if (n.x == x && n.y == y) { x = n.parentX; y = n.parentY; break; }
            }
            return true;
        }

        // check 4 neighbors
        for (int d = 0; d < 4; d++) {
            int nx = current.x + dx[d];
            int ny = current.y + dy[d];
            if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
            if (grid[ny][nx] == '#') continue;

            bool seen = false;
            for (Node n : closed) if (n.x == nx && n.y == ny) { seen = true; break; }
            if (seen) continue;
            for (Node n : open) if (n.x == nx && n.y == ny) { seen = true; break; }
            if (seen) continue;

            Node nb;
            nb.x = nx; nb.y = ny;
            nb.g = current.g + 1;
            nb.h = heuristic(nx, ny, END_X, END_Y);
            nb.f = nb.g + nb.h;
            nb.parentX = current.x; nb.parentY = current.y;
            open.push_back(nb);
        }
    }
    return false;
}



void printGrid() {
    for (string row : grid)
        cout << row << endl;
}

int main() {
    if (findPath()) {
        cout << "Path found!" << endl;
        printGrid();
    }
    else {
        cout << "No path!" << endl;
    }
    return 0;
}
