#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    try {
        Game game;
        const int windowWidth = 400;
        const int windowHeight = 400;
        
        std::cout << "Initializing game...\n";
        if (!game.Initialize("Snake Game", windowWidth, windowHeight)) {
            std::cerr << "Failed to initialize game!\n";
            std::cin.get(); // Keep console open
            return 1;
        }

        std::cout << "Running game loop...\n";
        game.Run();
    } 
    catch (const std::exception& e) {
        std::cerr << "CRASH: " << e.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred!\n";
    }

    std::cout << "Game ended. Press Enter to exit...\n";
    std::cin.get();
    
    return 0;
}
