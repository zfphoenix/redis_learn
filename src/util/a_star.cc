#include <iostream>
#include <stdint.h>
#include <vector>
#include <queue>
#include <string>

struct Coordinate {
    Coordinate() {}
    Coordinate(int32_t cx, int32_t cy):x(cx),y(cy) {
    }
    int32_t x;
    int32_t y;
};


struct Grid {
    Grid() {}
    explicit Grid(uint32_t x, uint32_t y) {
        coor.x = x; coor.y = y;
        is_walkable = true;
        parent = NULL;
        gn = 0;
        fn = 0;
    }
    Coordinate  coor; // coordinate for Grid
    bool        is_walkable; // can pass or not
    Grid        *parent; // parent node, passed-from
    uint32_t    gn; //  the cost of [start , n]
    uint32_t    fn; //  the cost of gn + hn; hn is the assessment
                    //  cost of [n, end]
};

struct FnCmp {
    bool operator() (const Grid *left, const Grid *right) {
        return left->fn > right->fn;
    }
};

//
//type definition
typedef std::vector< Grid > GridVect;
typedef std::vector< Grid* > GridPointerVect;
typedef std::priority_queue< Grid*, GridPointerVect, FnCmp > GridPointerHeap;

//for test 
//
Grid gmap[5][5];
Coordinate begin(2, 1);
Coordinate goal(2, 3);
GridPointerHeap open_heap;

inline void print_map() {
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            std::cout << gmap[i][j].is_walkable << ' ';
        }
        std::cout << std::endl;
    }
}

inline uint32_t Abs(int32_t x) {
    return x > 0 ? x : x*(-1); 
}

inline uint32_t distance(const Coordinate &coor, const Coordinate &goal, uint32_t cost) {
    return (Abs(coor.x - goal.x) + Abs(coor.y - goal.y)) * cost;
}

void tryAdd(const Coordinate &coor, Grid *parent, uint32_t cost) {
    if (coor.x < 0 || coor.y < 0 || coor.x > 4 || coor.y > 4) {
        return ;
    }
    Grid *temp = &gmap[coor.x][coor.y];
    if (temp->fn || !temp->is_walkable) {
        return ;
    }
    temp->gn = parent->gn + cost;
    temp->fn = temp->gn + distance(coor, goal, cost);
    temp->parent = parent;
    open_heap.push(temp);
}

void findPath(const Coordinate &start, const Coordinate &end) {
    Grid *start_grid = &gmap[start.x][start.y];
    Grid *end_grid = &gmap[end.x][end.y];
    start_grid->parent = start_grid;
    if (start_grid == end_grid) {
        std::cout << "start point equal end point\n";
        return;
    }
    open_heap.push(start_grid);
    while (!open_heap.empty()) {
        Grid *cur = open_heap.top();
        open_heap.pop();
        if (cur == end_grid) {
            break;
        }
        tryAdd(Coordinate(cur->coor.x, cur->coor.y - 1), cur, 10);
        tryAdd(Coordinate(cur->coor.x, cur->coor.y + 1), cur, 10);
        tryAdd(Coordinate(cur->coor.x + 1, cur->coor.y), cur, 10);
        tryAdd(Coordinate(cur->coor.x - 1, cur->coor.y), cur, 10);
        tryAdd(Coordinate(cur->coor.x - 1, cur->coor.y - 1), cur, 14);
        tryAdd(Coordinate(cur->coor.x + 1, cur->coor.y - 1), cur, 14);
        tryAdd(Coordinate(cur->coor.x + 1, cur->coor.y + 1), cur, 14);
        tryAdd(Coordinate(cur->coor.x - 1, cur->coor.y + 1), cur, 14);
    }
    if (end_grid->parent) {
        Grid *temp = end_grid;
        while (temp != start_grid) {
            std::cout << "x:y" << "(" << temp->coor.x << "," << temp->coor.y << ")\n";
            temp = temp->parent;
        }
        std::cout << "x:y" << "(" << temp->coor.x << "," << temp->coor.y << ")\n";
       std::cout << "found\n"; 
    } else {
        std::cout << "not found\n";
    }
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            gmap[i][j] = Grid(i,j);
        }
    }
    gmap[1][2].is_walkable = false;
    gmap[2][2].is_walkable = false;
    gmap[3][2].is_walkable = false;
    print_map();
    findPath(begin, goal);
	return 0;
}
