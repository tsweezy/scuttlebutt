#include <string>
#include <utility>
#include <iostream>

#include "ship.hpp"

Ship::Ship() {
    this->name = "Life Raft";
    this->size = 0;
    this->health = 0;
}

Ship::Ship(std::string name, int size) {
    this->name = name;
    this->size = size;
    this->health = size;
    this->destroyed = false;
}

void Ship::setVertical(bool vertical) { this->vertical = vertical; }

void Ship::setHealth(int health) {
    this->health = health;
}

void Ship::setLocation(int x, int y) {
    this->location = std::pair<int, int>(x, y);
}

void Ship::setDestroyed(bool destroyed) {
    this->destroyed = destroyed;
}

bool Ship::isDestroyed() {
    return this->destroyed;
}

int Ship::getSize() { return size; }

std::pair<int, int> Ship::getLocation() {
    return location;
}

bool Ship::getVertical() {
    return vertical;
}

int Ship::getHealth() {
    return health;
}

std::ostream& operator<<(std::ostream& os, const Ship& ship) {
    os << ship.name << " [" << ship.size << " spaces]";
    return os;
}