#include <algorithm>
#include <iostream>
#include "player.hpp"

Player::Player() {
    board = Board();
    ships = {
        Ship("Carrier", 5),
        Ship("Battleship", 4),
        Ship("Destroyer", 3),
        Ship("Submarine", 3),
        Ship("Patrol Boat", 2)
    };
}

Board& Player::getBoard() {
    return board;
}

Board* Player::getRadar() {
    return radar;
}

std::array<Ship, 5>& Player::getShips() {
    return ships;
}

int Player::updateShips(bool isHuman) {
    int sumShipHealth = 0;
    int newShipHealth;
    for (int i = 0; i < ships.size(); ++i) {
        Ship* currentShip = &ships[i];

        if (currentShip->isDestroyed()) { continue; }

        newShipHealth = currentShip->getSize();
        std::pair<int, int> shipLocation = currentShip->getLocation();
        for (int j = 0; j < currentShip->getSize(); ++j) {

            if (currentShip->getVertical()) {
                if (board[shipLocation.first][shipLocation.second + j].status == FIRE)
                    newShipHealth--;
            } else {
                if (board[shipLocation.first + j][shipLocation.second].status == FIRE)
                    newShipHealth--;
            }
        }

        currentShip->setHealth(newShipHealth);
        sumShipHealth += currentShip->getHealth();

        if ((currentShip->getHealth() <= 0) && !(currentShip->isDestroyed())) {
            if (isHuman) {
                std::cout << "\033[1;31mOur " << *currentShip << " has been sunk!\033[0m\n" << std::endl;
            } else {
                std::cout << "\033[1;32mWe sunk their " << *currentShip << "!\033[0m\n" << std::endl;
            }
            currentShip->setDestroyed(true);
        }
    }
    return sumShipHealth;
}

void Player::setRadar(Board& radar) {
    this->radar = &radar;
}

Ship& Player::place(Ship& ship, int x, int y, bool vertical) {
    std::pair<int, int> bd = board.getDimensions();

    if ((x >= 0 && x < bd.first) && (y >= 0 && y < bd.second)) {
        // Holds a coordinate marking the last tile of the ship
        int last = vertical ? y + (ship.getSize() - 1) : x + (ship.getSize() - 1);

        // Check for boundaries/collisions and place
        if (vertical && last >= 0 && last < bd.second) {
            for (int i = 0; i < ship.getSize(); ++i) {
                if (board[x][y + i].status == SHIP)
                    throw std::out_of_range("The ship would collide with another vessel!");
            }
            for (int i = 0; i < ship.getSize(); ++i) {
                board[x][y + i].status = SHIP;
            }
            ship.setLocation(x, y);
            ship.setVertical(vertical);
            return ship;
        } else if (!vertical && last >= 0 && last < bd.first) {
            for (int i = 0; i < ship.getSize(); ++i) {
                if (board[x + i][y].status == SHIP)
                    throw std::out_of_range("The ship would collide with another vessel!");
            }
            for (int i = 0; i < ship.getSize(); ++i) {
                board[x + i][y].status = SHIP;
            }
            ship.setLocation(x, y);
            ship.setVertical(vertical);
            return ship; 
        }
    }

    throw std::out_of_range("The ship would lie outside the board's boundaries.");
}

TileStatus Player::strike(int x, int y) {
    switch ((*radar)[x][y].status) {
        case WATER:
            // strike, no hit
            (*radar)[x][y].status = SPLASH;
            return SPLASH;
        case SHIP:
            // strike, hit
            (*radar)[x][y].status = FIRE;
            return FIRE;

        case SPLASH:
        case FIRE:
            // already struck, try another location
            throw std::invalid_argument("This location has already been fired upon!");
    }
}