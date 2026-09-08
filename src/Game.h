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

#include "FileSystem.h"
#include "Subroutine.h"

namespace Tenshi {

struct GameConfig {
    std::string title = "game title unset";

    int windowWidth = 256;
    int windowHeight = 240;

    int windowScale = 1;
};

class Game {
public:
    Game();
    ~Game();

    bool init(GameConfig conf);
    void run(); // starts it running
    void quit();

    void reset(GameConfig conf);
    void rendererSetFixedSize(int w, int h);

    void addSubroutine(std::unique_ptr<Subroutine> subroutine);

    template<typename Callable>
    void on(const std::string& eventName, Callable&& callback) {
        auto func = std::function(std::forward<Callable>(callback));
        listeners[eventName].push_back(std::any(func));
    }

    template<typename... Args>
    void emit(const std::string& eventName, Args&&... args) {
        auto it = listeners.find(eventName);
        if (it != listeners.end()) {
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
    }

    FileSystem& getFileSystem();

    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Surface* applicationSurface = nullptr;
    TTF_TextEngine* text_engine = nullptr;
    GameConfig config;

    FileSystem filesys;

    double targetFrameTime;

	uint64_t lastCounter;
	double deltaTime;

    bool running = false;

    // std::unique_ptr<Player> player;
	// std::unique_ptr<Scene> currentScene;

    void step();
    void draw();

    // handle unhandled frame logic
    void pulse();
    bool redrawRequested = false;

    // emscripten
#ifdef __EMSCRIPTEN__
    static Game* emscriptenInstance;
    static void emscriptenStep();
#endif

    static bool SDLCALL eventWatch(void* userdata, SDL_Event* event);

    // callbacks
    std::unordered_map<std::string, std::vector<std::any>> listeners;

    std::vector<std::unique_ptr<Subroutine>> subroutines;
};

} // namespace Tenshi

