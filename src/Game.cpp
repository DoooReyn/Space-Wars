#include "Game.h"
#include "SceneMain.h"

Game::Game()
{
}

Game::~Game()
{
    isRunning = false;
    clean();
}

void Game::init()
{
    // 初始化
    SDL_Log("初始化游戏");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL 初始化失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 创建窗口
    SDL_Log("初始化窗口");
    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL 窗口创建失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 创建渲染器
    SDL_Log("初始化渲染器");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL 渲染器创建失败: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 初始化主场景
    SDL_Log("初始化主场景");
    currentScene = new SceneMain();
    isRunning = true;
}

void Game::run()
{
    while (isRunning)
    {
        SDL_Event event;
        handleEvent(&event);
        update();
        render();
    }
}

void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }

        if (currentScene != nullptr)
        {
            currentScene->handleEvent(event);
        }
    }
}
void Game::update()
{
    if (currentScene != nullptr)
        currentScene->update();
}
void Game::render()
{
    if (currentScene != nullptr)
    {
        SDL_RenderClear(renderer);
        currentScene->render();
        SDL_RenderPresent(renderer);
    }
}

void Game::clean()
{
    // 销毁当前场景
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }

    // 销毁渲染器和窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    // 如果传入的场景为空，则不进行切换
    if (scene == nullptr)
        return;

    // 如果前后场景相同，则不进行切换
    if (currentScene == scene)
        return;

    // 清理当前场景
    if (currentScene)
    {
        currentScene->clean();
        delete currentScene;
    }

    // 设置并初始化新场景
    currentScene = scene;
    currentScene->init();
}
