#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <any>
#include <algorithm>
#include <filesystem>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Tenshi/Files/FileSystem.h"
#include "Tenshi/Core/Subroutine.h"
#include "Tenshi/Audio/Audio.h"

#include "Tenshi/Utility/Colour.h"

namespace Tenshi {

struct GameConfig {
    std::string title = "TENSHI Game";

    int windowWidth = 256;
    int windowHeight = 240;

    int windowScale = 1;

    bool windowResizable = false;

    bool clearFrame = true;
    rgba clearColour = {0, 0, 0, 255};
};

class Game {
public:
    Game();
    ~Game();

    // Initialize the game
    bool init();

    // Start the game loop
    void run();

    // End the game loop
    void quit();

    void resetRenderer(GameConfig conf);
    void rendererSetFixedSize(int w, int h);

    void addSubroutine(std::unique_ptr<Subroutine> subroutine);

    FileSystem& getFileSystem();
    AudioSystem& getAudioSystem();
    GameConfig& getConfig();
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();

    // register a callback event
    template<typename Callable>
    void on(const std::string& eventName, Callable&& callback) {
        auto func = std::function(
            std::forward<Callable>(callback)
        );
        listeners[eventName].push_back(std::any(func));
    }

    // emit a callback event
    template<typename... Args>
    void emit(const std::string& eventName, Args&&... args) {
        auto it = listeners.find(eventName);
        if (it == listeners.end()) return;

        for (const auto& anyCallback : it->second) {
            using FuncType = std::function<void(Args...)>;
            try {
                auto callback = std::any_cast<FuncType>(anyCallback);
                callback(std::forward<Args>(args)...);
            } catch (const std::bad_any_cast&) {
                std::cerr << "Error: Event signature mismatch for " << eventName << "\n";
            }
        }
    }
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface* applicationSurface = nullptr;
    GameConfig config;
    
    TTF_TextEngine* text_engine = nullptr; // migrate to ui system when implemented!
    
    FileSystem filesys;
    AudioSystem audiosys;

    double targetFrameTime;
	uint64_t lastCounter;
	double deltaTime;

    bool running = false;
    bool redrawRequested = false;
    
    void step();
    void draw();

    // handle unhandled frame logic
    void pulse();

    static bool SDLCALL eventWatch(void* userdata, SDL_Event* event);
    
#ifdef __EMSCRIPTEN__
    static Game* emscriptenInstance;
    static void emscriptenStep();
#endif

    // callbacks
    std::unordered_map<std::string, std::vector<std::any>> listeners;
    std::vector<std::unique_ptr<Subroutine>> subroutines;
};

} // namespace Tenshi

