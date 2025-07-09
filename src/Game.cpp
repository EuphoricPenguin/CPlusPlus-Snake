#include "Game.h"
#include <iostream>

// Embedded sound data
const unsigned char chomp_wav[] = {
  0x52,0x49,0x46,0x46,0x24,0x08,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
  0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x22,0x56,0x00,0x00,0x44,0xac,0x00,0x00,
  0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0x08,0x00,0x00,0x00,0x00,0xff,0xff,
  0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,
  0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,
  0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,
  0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00
  // ... (truncated for example - full file would contain complete data)
};

const unsigned int chomp_wav_len = sizeof(chomp_wav);

Game::Game() : window(nullptr), renderer(nullptr), isRunning(true),
               gridSize(20), score(0), gen(rd()), dist(0, 19), chompSound(nullptr) {
    // Initialize snake with 3 segments
    snake = {{10, 10}, {10, 11}, {10, 12}};
    direction = {0, -1}; // Initial direction: up
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize audio: " << Mix_GetError() << std::endl;
        return false;
    }
    std::cout << "Audio system initialized successfully!" << std::endl;
    std::cout << "Audio drivers: " << SDL_GetCurrentAudioDriver() << std::endl;

    // Temporary: Load sound from file
    std::string soundPath = "sfx/chomp.wav";
    chompSound = Mix_LoadWAV(soundPath.c_str());
    if (!chompSound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return false;
    }
    std::cout << "Sound loaded from file successfully!" << std::endl;

    SpawnFood();
    return true;
}

void Game::Run() {
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
        SDL_Delay(100);
    }
}

void Game::Shutdown() {
    std::cerr << "Shutting down game...\n";
    try {
        if (chompSound) {
            Mix_FreeChunk(chompSound);
            chompSound = nullptr;
            std::cerr << "Audio resources freed\n";
        }
    } catch (...) {
        std::cerr << "Error during audio shutdown!\n";
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

void Game::SpawnFood() {
    food.x = dist(gen);
    food.y = dist(gen);
}

void Game::ProcessInput() {
    std::cout << "Processing input...\n";
    if (!window) {
        throw std::runtime_error("Window is null during input processing!");
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (direction.y == 0) direction = {0, -1};
                    break;
                case SDLK_DOWN:
                    if (direction.y == 0) direction = {0, 1};
                    break;
                case SDLK_LEFT:
                    if (direction.x == 0) direction = {-1, 0};
                    break;
                case SDLK_RIGHT:
                    if (direction.x == 0) direction = {1, 0};
                    break;
            }
        }
    }
}

void Game::Update() {
    std::cout << "Updating game state...\n";
    if (snake.empty()) {
        throw std::runtime_error("Snake array is empty!");
    }
    // Move snake
    Position newHead = {snake.front().x + direction.x, snake.front().y + direction.y};
    snake.insert(snake.begin(), newHead);
    snake.pop_back();

    // Check for food collision
        if (snake.front().x == food.x && snake.front().y == food.y) {
            std::cout << "Playing sound effect..." << std::endl;
            int channel = Mix_PlayChannel(-1, chompSound, 0);
            if (channel == -1) {
                std::cerr << "ERROR: Failed to play sound: " << Mix_GetError() << std::endl;
            } else {
                std::cout << "Sound playing on channel: " << channel << std::endl;
            }
            score++;
            SpawnFood();
            // Grow snake by adding new segment
            snake.push_back(snake.back());
        }

    // Check for wall/self collision
    if (CheckCollision()) {
        isRunning = false;
    }
}

void Game::Render() {
    std::cout << "Rendering frame...\n";
    if (!renderer) {
        throw std::runtime_error("Renderer is null!");
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render food
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {food.x * gridSize, food.y * gridSize, gridSize, gridSize};
    SDL_RenderFillRect(renderer, &foodRect);

    // Render snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& segment : snake) {
        SDL_Rect segmentRect = {segment.x * gridSize, segment.y * gridSize, gridSize, gridSize};
        SDL_RenderFillRect(renderer, &segmentRect);
    }

    SDL_RenderPresent(renderer);
}

bool Game::CheckCollision() {
    // Check wall collision
    if (snake.front().x < 0 || snake.front().x >= 20 ||
        snake.front().y < 0 || snake.front().y >= 20) {
        return true;
    }

    // Check self collision (skip head)
    for (size_t i = 1; i < snake.size(); i++) {
        if (snake.front().x == snake[i].x && snake.front().y == snake[i].y) {
            return true;
        }
    }

    return false;
}
