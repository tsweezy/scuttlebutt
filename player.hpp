#ifndef     __HPP_PLAYER__
#define     __HPP_PLAYER__

#include <string>
#include "ship.hpp"
#include "board.hpp"
#include "tile.hpp"

class Player {
    public:
        Player();

        /**
         * @brief Set the radar Board to the opposing player's board.
         * 
         * @param radar a reference to the opposing player's board.
         */
        void setRadar(Board& radar);
        
        /**
         * @brief Place a ship at a coordinate on the board.
         * If vertical is false, the coordinates represent the left-most tile of the ship.
         * If vertical is true, the coordinates represent the top-most tile of the ship.
         * 
         * @param ship The ship to be placed.
         * @param x The x-coordinate of the placement.
         * @param y  The y-coordinate of the placement
         * @param vertical If true, the ship is placed vertically.
         * @throws out_of_range If ship placement would exceed the boundaries of the board.
         */
        Ship& place(Ship& ship, int x, int y, bool vertical);

        /**
         * @brief Strike at a given coordinate on the opposing player's board.
         * 
         * @param x The x-coordinate of the strike.
         * @param y The y-coordinate of the strike.
         */
        TileStatus strike(int x, int y);

        /**
         * @brief Get a reference to this player's Board.
         * 
         * @return Board& - a reference to this player's board.
         */
        Board& getBoard();

        /**
         * @brief Get the Radar object
         * 
         * @return Board* A pointer to the enemy's board.
         */
        Board* getRadar();

        /**
         * @brief Update the status of all ships based on the data
         * stored within the board..
         * 
         * @param isHuman 
         * @return int Sum of the player's ship's remaining health.
         */
        int updateShips(bool isHuman);

        std::array<Ship, 5>& getShips();
        
    private:
        Board board;                // This player's board
        Board* radar;               // The opposing player's board
        std::array<Ship, 5> ships;  // This player's fleet of ships
};

#endif