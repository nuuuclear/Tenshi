#include "Tenshi/Game.h"

#include <utility>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Window.h"
#include "GameHelper.h"

#include "CrashHandler.h"

namespace Tenshi {

#ifdef __EMSCRIPTEN__
Game* Game::emscriptenInstance = nullptr;

void Game::emscriptenStep() {
    if (emscriptenInstance) {
        emscriptenInstance->step();
    }
}
#endif

Game::Game() {
}

Game::~Game() {
    SDL_RemoveEventWatch(Game::eventWatch, this);

    subroutines.clear();
    audiosys.Shutdown();

    TTF_DestroyRendererTextEngine(text_engine);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

bool Game::init(GameConfig conf) {
    config = conf;

    if (!INTERNAL::Initialize()) {
        FatalError("Error!", "Initialization failed.");
        
        return false;
    }
    
    window = MakeWindow(conf);
    if (!window) return false;

    SDL_AddEventWatch(Game::eventWatch, this);

    const char* basePath = INTERNAL::getBasePath();
    std::filesystem::path root(basePath);
    filesys.setRoot(root.string());

    // needs fixing, but this'll do for now...
    if (!filesys.mountPak("", "resource0.tpk")) {
        filesys.mountDirectory("", "resources");
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(renderer, 1);

    if (!audiosys.Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Audio engine failed to initialize");
        return false;
    }

#ifndef __EMSCRIPTEN__
    if (!audiosys.Start()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Audio device failed to start");
        return false;
    }
#endif

    audiosys.SetMasterVolume(1.0f);

    lastCounter = SDL_GetPerformanceCounter();
    deltaTime = 0.0;
    targetFrameTime = 1.0 / 60;

    running = true;
    return true;
}

void Game::run() {
#ifdef __EMSCRIPTEN__
    emscriptenInstance = this;

    emscripten_set_main_loop(
        Game::emscriptenStep,
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

    deltaTime
    =   (double)(currentCounter - lastCounter) 
    /   SDL_GetPerformanceFrequency();

    lastCounter = currentCounter;

#ifdef __EMSCRIPTEN__
    // emscripten frame limiting
    if (deltaTime < targetFrameTime) {
        SDL_Delay((Uint32)((targetFrameTime - deltaTime) * 1000.0));
    }
#endif

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
#ifdef __EMSCRIPTEN__
        // browser audio must be started from a user interaction
        // retrying wont do anything until the first event happens
        audiosys.Start();
#endif
        switch (event.type) {
        case SDL_EVENT_QUIT:
            running = false;
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
    if (config.clearFrame) {
        SDL_SetRenderDrawColor(renderer, 
            config.clearColour.r,
            config.clearColour.g,
            config.clearColour.b,
            config.clearColour.a
        );

        SDL_RenderClear(renderer);
    }

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
	SDL_SetRenderLogicalPresentation(renderer, w, h, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
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

AudioSystem& Game::getAudioSystem() {
    return audiosys;
}

SDL_Window* Game::getWindow() {
    return window;
}

SDL_Renderer* Game::getRenderer() {
    return renderer;
}

} // namespace Tenshi