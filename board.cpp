#include <iostream>
#include "board.hpp"

Board::Board() {
    board.fill( std::array<Tile, HEIGHT> { Tile() } );
}

std::array<std::array<Tile, HEIGHT>, WIDTH> Board::getBoard() { return board; }

int Board::size() {
    return board.size();
}

std::pair<int, int> Board::getDimensions() {
    return std::pair<int, int> (WIDTH, HEIGHT);
}

std::array<Tile, HEIGHT>& Board::operator[](size_t index) { return board[index]; }

std::ostream& operator<<(std::ostream& os, const Board& b) {
    std::cout   << "\n     0 1 2 3 4 5 6 7 8 9 "
                << "\n   ┌─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┐" << std::endl;
    for (int i = 0; i < b.board.size(); ++i) {
        std::cout << i << " ─┤";
        for (int j = 0; j < b.board[i].size(); ++j) {
            switch (b.board[j][i].status) {
                case WATER:
                    os << " •";
                    break;
                case SHIP:
                    os << " ■";
                    break;
                case SPLASH:
                    os << " ○";
                    break;
                case FIRE:
                    os << "\033[1;31m X\033[0m";
                    break;
            }
        }
        std::cout << " │" << std::endl;
    }
    std::cout << "   └─────────────────────┘\n" << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Board* b) {
    std::cout   << "\n     0 1 2 3 4 5 6 7 8 9 "
                << "\n   ┌─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┐" << std::endl;
    for (int i = 0; i < b->board.size(); ++i) {
        std::cout << i << " ─┤";
        for (int j = 0; j < b->board[i].size(); ++j) {
            switch (b->board[j][i].status) {
                case WATER:
                case SHIP:
                    os << " •";
                    break;
                case SPLASH:
                    os << " ○";
                    break;
                case FIRE:
                    os << "\033[1;31m X\033[0m";
                    break;
            }
        }
        std::cout << " │" << std::endl;
    }
    std::cout << "   └─────────────────────┘\n" << std::endl;
    return os;
}