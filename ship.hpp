#ifndef     __HPP_SHIP__
#define     __HPP_SHIP__

#include <utility>
#include <string>

class Ship {
    public:
        Ship();

        /**
         * @brief Construct a new Ship object
         * 
         * @param name The name of the ship
         * @param size The size of the ship
         */
        Ship(std::string name, int size);

        /* setters */
        void setVertical(bool vertical);
        void setHealth(int health);
        void setLocation(int x, int y);
        void setDestroyed(bool destroyed);

        /* getters */
        int getSize();
        std::pair<int, int> getLocation();
        bool getVertical();
        bool isDestroyed();
        int getHealth();

        /* Stream operator to print the name and size of the ship */
        friend std::ostream& operator<<(std::ostream& os, const Ship& s);
    private:
        std::string name;               // The name of the ship
        int size;                       // The size of the ship
        bool vertical;                  // If the ship is vertical
        std::pair<int, int> location;   // The x, y location of the ship
        bool destroyed = false;         // If the ship has been destroyed
        int health;                     // The health of the ship
};

#endif