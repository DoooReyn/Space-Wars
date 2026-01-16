#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL.h>

class Game
{
public:
    static Game &getInstance()
    {
        static Game instance;
        return instance;
    }

    ~Game();

    void init();
    void run();
    void clean();
    void changeScene(Scene *scene);

    SDL_Renderer *getRenderer()
    {
        return renderer;
    }

    SDL_Window *getWindow()
    {
        return window;
    }

    int getWindowWidth()
    {
        return windowWidth;
    }

    int getWindowHeight()
    {
        return windowHeight;
    }

    bool running()
    {
        return isRunning;
    }

protected:
    void handleEvent(SDL_Event *event);
    void update(float deltaTime);
    void render();

private:
    Game();
    // 删除默认构造函数和拷贝构造函数
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    SDL_Window *window;
    SDL_Renderer *renderer;
    Scene *currentScene;

    const char *windowTitle = "Star Wars";
    int windowWidth = 640;
    int windowHeight = 960;
    bool isRunning = false;

    int fps = 60;
    Uint32 frameTime = 0;
    float deltaTime = 0;
};

#endif // GAME_H