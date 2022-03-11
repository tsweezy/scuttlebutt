#include <iostream>
#include <stdexcept>
#include <random>
#include <chrono>
#include <thread>
#include "player.hpp"
#include "styling.hpp"

int validateUserCoordinateInput(int min, int max, std::string currentPrompt, std::string nextPrompt);
void playerTurn(Player player, int turns);
void cpuTurn(Player player);

int main(int argc, char** argv) {
    
    // The human player
    Player user = Player();

    // The computer player
    Player cpu = Player();

    // user radar -> player2 board
    user.setRadar(cpu.getBoard());
    
    // cpu radar -> player1 board
    cpu.setRadar(user.getBoard());

    // some initial set-dressing
    std::pair<int, int> boardDimensions = user.getBoard().getDimensions();
    std::cout   << TITLE
                << "\nAdmiral, you're tasked with defending a territory that is\n"
                << boardDimensions.first << " units wide by "
                << boardDimensions.second << " units tall.\n" << std::endl
                << "Your fleet:\n" << std::endl;

    for (int i = 0; i < user.getShips().size(); ++i) {
        std::cout << "\t" << user.getShips()[i] << std::endl;
    }

    std::cout << user.getBoard();
    
    int x, y;
    Ship currentShip;
    std::string input = "";
    bool vertical;
    // get user input and place ships
    for (int i = 0; i < user.getShips().size(); ++i) {
        begin_ship_initialization:  // label marking the start of the current ship's initialization
        currentShip = user.getShips()[i];
        std::cout   << "\nWhere do you wish to position the " << currentShip
                    << "?\nx-coordinate: ";
        
        // x-coordinate
        x = validateUserCoordinateInput(
            0,
            boardDimensions.first - 1,
            "x-coordinate: ",
            "y-coordinate: "
        );
        // y-coordinate
        y = validateUserCoordinateInput(
            0,
            boardDimensions.second - 1,
            "y-coordinate: ",
            "vertical? (Y/n): "
        );
        // vertical
        while (getline(std::cin, input)) {
            if (input[0] == 'Y' || input[0] == 'y' || input == "") {
                vertical = true;
                break;
            } else if (input[0] == 'N' || input[0] == 'n') {
                vertical = false;
                break;
            } else {
                std::cout   << "\n\033[1;31mAdmiral, we're unsure what you meant by "
                            << input << ".\033[0m\nvertical? (Y/n): ";
            }
        }

        try {
            user.getShips()[i] = user.place(currentShip, x, y, vertical);
        } catch (std::exception &e) {
            std::cout   << "\n\033[1;31mAdmiral, I can't carry out your command.\n"
                        << e.what() << "\nPlease try again.\033[0m" << std::endl;

            // a shortcut I've been told is bad practice, but desperate times...
            goto begin_ship_initialization; // restart loop; do not iterate to next ship
        }
        
        std::cout   << "\033[2J\033[1;1H\n\033[1;32mPlacing the " << currentShip
                    << " at coordinates (" << x << ", " << y << ") to (";

        if (vertical) {
            std::cout   << x << ", " << y + (currentShip.getSize() - 1)
                        << ").\033[0m" << std::endl;
        } else {
            std::cout   << x + (currentShip.getSize() - 1) << ", " << y
                        << ").\033[0m" << std::endl;
        }
        std::cout << user.getBoard();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\033[2J\033[1;1HAdmiral, the opposing fleet is getting into position: \n" << std::endl;

    // place cpu player ships on their board
    std::pair<int, int> cpuDimensions = cpu.getBoard().getDimensions();
    for (int i = 0; i < cpu.getShips().size(); ++i) {
        begin_cpu_ship_initialization: // label marking the start of the current ship's initialization
        currentShip = cpu.getShips()[i];

        // generate a random seed
        std::mt19937 gen(time(NULL));
        std::uniform_int_distribution<> distr(0, cpuDimensions.first - 1);

        int randx = distr(gen);
        int randy = distr(gen);
        bool randv = distr(gen) % 2;

        try {
            cpu.getShips()[i] = cpu.place(currentShip, randx, randy, randv);
        } catch (std::exception &e) {
            // a shortcut I've been told is bad practice, but desperate times...
            goto begin_cpu_ship_initialization; // restart loop; do not iterate to next ship
        }
        std::cout << currentShip << " placed." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "\033[2J\033[1;1HOpponent fleet in position. Ready for battle.\n" << std::endl;

    int turns = 1;
    bool running = true;
    while(running) {
        playerTurn(user, turns);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (cpu.updateShips(false) == 0) {
            std::cout << VICTORY << "\nIn " << turns << " turns!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            running = false;
            break;
        }
        cpuTurn(cpu);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << std::endl;
        if (user.updateShips(true) == 0) {
            std::cout << GAME_OVER << "\nIn " << turns << " turns!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            running = false;
            break;
        }
        turns++;
    }

    // start playing
    return 0;
}

/**
 * @brief Validate user input within the specified range.
 * This function will continually ask the user for a valid input until one is reached.
 * 
 * @param min The minimum value in the range
 * @param max The maximum value in the range
 * @param currentPrompt The current prompt being sent to std::out preceeding user input
 * @param nextPrompt The next prompt that will be sent to std::out after user input
 * @return The successfully validated integer coordinate
 */
int validateUserCoordinateInput(int min, int max, std::string currentPrompt, std::string nextPrompt) {
    int n;
    std::string input = "";
    while (getline(std::cin, input)) {
        try {
            n = stoi(input); // will throw an exception on unexpected input
            
            if (n < min || n > max) {
                throw std::out_of_range("Not within board bounds");
            }

            std::cout << nextPrompt;
            break;
        } catch (std::out_of_range& e) {
            std::cout   << "\n\033[1;31mAdmiral, that coordinate is out of bounds!\033[0m\n";
        } catch (...) {
            std::cout   << "\n\033[1;31mAdmiral, we're unsure what you meant by "
                        << input << ".\033[0m\n";
        }
        std::cout   << "\033[1;31mAn acceptable coordinate would be a number from 0 to "
                    << max << ". You said " << input
                    << ".\033[0m\n" << currentPrompt;
    }
    return n;
}

void playerTurn(Player player, int turns) {
    int x, y;
    TileStatus result;
    std::pair<int, int> boardDimensions = player.getBoard().getDimensions();
    std::cout << "Your turn to strike. Turn " << turns << ".\n" << "\n───────── RADAR ──────────\n" << player.getRadar() << "─────── OUR FLEET ────────\n" << player.getBoard();

    for (;;) {
        std::cout << "Which coordinate do you wish to attack?\nx-coordinate: ";
        x = validateUserCoordinateInput(0, boardDimensions.first, "x-coordinate: ", "y-coordinate: ");
        y = validateUserCoordinateInput(0, boardDimensions.second, "y-coordinate: ", "");
        std::cout << std::endl;
        try {
            switch (player.strike(x, y)) {
                case SPLASH:
                    std::cout << "\033[2J\033[1;1H" << std::endl;
                    std::cout << "\033[1;31mSPLASH!\033[0m We missed!" << std::endl;
                    break;
                case FIRE:
                    std::cout << "\033[2J\033[1;1H" << std::endl;
                    std::cout << "\033[1;32mHIT!\033[0m We got 'em!" << std::endl;
                    break;
                case WATER:
                case SHIP:
                break;
            }
            return;
            
        } catch (std::exception &e) {
            std::cout << "\033[1;31mAdmiral, we can't strike there. " << e.what() << "\033[0m" << std::endl;
        }
    }
}

void cpuTurn(Player player) {
    TileStatus result;
    std::pair<int, int> boardDimensions = player.getBoard().getDimensions();
    std::cout << "\nThe enemy is making their move." << std::endl;
    std::cout << std::endl;

    for (;;) {
        // generate a random seed
        std::mt19937 gen(time(NULL));
        std::uniform_int_distribution<> distr(0, boardDimensions.first - 1);

        int randx = distr(gen);
        int randy = distr(gen);
        bool randv = distr(gen) % 2;

        try {
            switch (player.strike(randx, randy)) {
                case SPLASH:
                    std::cout << "\033[1;32mSPLASH!\033[0m They missed, those fools!" << std::endl;
                    break;
                case FIRE:
                    std::cout << "\033[1;31mHIT!\033[0m They got us!" << std::endl;
                    break;
                case WATER:
                case SHIP:
                break;
            }
            return;
            
        } catch (...) { }
    }
}
