#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <random>

class Game {
public:
    Game();
    ~Game();

    bool Initialize(const char* title, int width, int height);
    void Run();
    void Shutdown();

private:
    void ProcessInput();
    void Update();
    void Render();
    void SpawnFood();
    bool CheckCollision();

    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Chunk* chompSound;
    bool isRunning;

    // Game elements
    struct Position { int x, y; };
    std::vector<Position> snake;
    Position food;
    Position direction;
    int gridSize;
    int score;

    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;
};
