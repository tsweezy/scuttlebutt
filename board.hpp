#ifndef     __HPP_BOARD__
#define     __HPP_BOARD__

#include <utility>
#include <array>
#include "tile.hpp"

#define WIDTH 10    // The width of the board
#define HEIGHT 10   // The height of the board

class Board {
    public:
        Board();
        
        /* getters */
        std::array<std::array<Tile, HEIGHT>, WIDTH> getBoard();
        std::pair<int, int> getDimensions();
        int size();

        /* operator overloads */
        std::array<Tile, HEIGHT>& operator[](size_t index);
        friend std::ostream& operator<<(std::ostream& os, const Board& b);
        friend std::ostream& operator<<(std::ostream& os, const Board* b);
    protected:
        std::array<std::array<Tile, HEIGHT>, WIDTH> board;
};

#endif