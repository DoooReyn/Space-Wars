#include "Game.h"
#include "SceneMain.h"
#include <SDL_image.h>

Game::Game()
= default;

Game::~Game() {
    isRunning = false;
    clean();
}

void Game::init() {
    frameTime = 1000 / fps;
    deltaTime = static_cast<float>(frameTime) / 1000.0f;

    // 初始化
    SDL_Log("初始化游戏");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL初始化失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 创建窗口
    SDL_Log("初始化窗口");
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL窗口创建失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 创建渲染器
    SDL_Log("初始化渲染器");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL渲染器创建失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 初始化 SDL_image
    SDL_Log("初始化图片装载器");
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL图片装载器初始化失败: %s", IMG_GetError());
        isRunning = false;
        return;
    }

    // 初始化主场景
    SDL_Log("初始化主场景");
    currentScene = new SceneMain();
    currentScene->init();

    isRunning = true;
}

void Game::run() {
    while (isRunning) {
        const auto frameStart = SDL_GetTicks();

        SDL_Event event;
        handleEvent(&event);
        update(deltaTime);
        render();

        const auto frameEnd = SDL_GetTicks();
        if (const auto diff = frameEnd - frameStart; diff < frameTime) {
            SDL_Delay(frameTime - diff);
            deltaTime = static_cast<float>(frameTime) / 1000.f;
        } else {
            deltaTime = static_cast<float>(diff) / 1000.0f;
        }
    }
}

void Game::handleEvent(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isRunning = false;
        } else if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }

        if (currentScene != nullptr) {
            currentScene->handleEvent(event);
        }
    }
}

void Game::update(const float dt) const {
    if (currentScene != nullptr)
        currentScene->update(dt);
}

void Game::render() const {
    if (currentScene != nullptr) {
        SDL_RenderClear(renderer);
        currentScene->render();
        SDL_RenderPresent(renderer);
    }
}

void Game::clean() const {
    // 销毁当前场景
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }

    // 清理 SDL_image
    IMG_Quit();

    // 销毁渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene) {
    // 如果传入的场景为空，则不进行切换
    if (scene == nullptr)
        return;

    // 如果前后场景相同，则不进行切换
    if (currentScene == scene)
        return;

    // 清理当前场景
    if (currentScene) {
        currentScene->clean();
        delete currentScene;
    }

    // 设置并初始化新场景
    currentScene = scene;
    currentScene->init();
}
