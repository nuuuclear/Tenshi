#include "Game.h"

#include <utility>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Tenshi {
Game::Game() {
}

Game::~Game() {
    SDL_RemoveEventWatch(Game::eventWatch, this);

    subroutines.clear();

    TTF_DestroyRendererTextEngine(text_engine);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

bool Game::init(GameConfig conf) {
    config = &conf;

    SDL_SetHint(SDL_HINT_WINDOWS_ENABLE_MESSAGELOOP, "1");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Video failed to init: %s", SDL_GetError());
    }
    
    window = SDL_CreateWindow(
        config->title.c_str(), 
        config->windowWidth, 
        config->windowHeight, 
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Game failed create window: %s", SDL_GetError());
        return false;
    }

    SDL_AddEventWatch(Game::eventWatch, this);

    bool ttf = TTF_Init();
    if (!ttf) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF failed to init: %s", SDL_GetError());
        return false;
    }

    const char* basePath = SDL_GetBasePath();

    if (!basePath) {
        throw std::runtime_error(
            "Could not determine application path"
        );
    }

    std::filesystem::path root(basePath);
    filesys.setRoot(root);

    filesys.set("resources",
        root / "resources"
    );

    #ifdef __EMSCRIPTEN__
            renderer = SDL_CreateRenderer(window, NULL);
    #else
            // renderer = SDL_CreateGPURenderer(NULL, window);

            // FIX THIS: GPURenderer is disabled when resizing/moving a window, so drawing to it will cause a crash.
            renderer = SDL_CreateRenderer(window, NULL);
    #endif

    SDL_SetRenderVSync(renderer, 1);

    lastCounter = SDL_GetPerformanceCounter();
    deltaTime = 0.0;

    targetFrameTime = 1.0 / 60;

    running = true;
    return true;
}

void Game::run() {
#ifdef __EMSCRIPTEN__

    emscripten_set_main_loop(
        step,
        0,
        true
    );

#else
    while (running) {
        step();
    }
#endif
}

void Game::quit() {
    running = false;

#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

void Game::addSubroutine(std::unique_ptr<Subroutine> subroutine) {
    if (!subroutine) return;

    subroutines.push_back(std::move(subroutine));
}

void Game::step() {
    uint64_t currentCounter = SDL_GetPerformanceCounter();

    deltaTime =
        (double)(currentCounter - lastCounter) /
        SDL_GetPerformanceFrequency();

    lastCounter = currentCounter;

    // emscripten frame limiting
#ifdef __EMSCRIPTEN__
        if (deltaTime < targetFrameTime) {
            SDL_Delay((Uint32)((targetFrameTime - deltaTime) * 1000.0));
        }
#endif

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                emit("windowResize");
                break;
        }

        // signal all events
        for (auto& subroutine : subroutines) {
            subroutine->event(event);
        }
    }

    // signal all updates
    for (auto& subroutine : subroutines) {
        subroutine->update(deltaTime);
    }
   
    draw();

    redrawRequested = false;
}

void Game::draw() {
    SDL_SetRenderDrawColor(
        renderer,
        32, 32, 32, 255 // dark grey
    );

    SDL_RenderClear(renderer);

    // signal all drawing
    for (auto& subroutine : subroutines) {
        subroutine->draw();
    }

    SDL_RenderPresent(renderer);
}

void Game::pulse() {
    if (redrawRequested) {
        draw();
    }
}

void Game::reset(GameConfig conf) {
    SDL_SetWindowSize(window, 
        conf.windowWidth  * conf.windowScale,
        conf.windowHeight * conf.windowScale
    );

}

void Game::rendererSetFixedSize(int w, int h) {
	SDL_SetRenderLogicalPresentation(renderer, h, w, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

bool SDLCALL Game::eventWatch(void* userdata, SDL_Event* event) {
    Game* game = static_cast<Game*>(userdata);

    if (event->type == SDL_EVENT_WINDOW_EXPOSED) {
        game->redrawRequested = true;
    }

    game->pulse();

    return true;
}

FileSystem& Game::getFileSystem() {
    return filesys;
}

SDL_Window* Game::getWindow() {
    return window;
}

SDL_Renderer* Game::getRenderer() {
    return renderer;
}

} // namespace Tenshi