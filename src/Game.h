#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL.h>

class Game {
public:
    static Game &getInstance() {
        static Game instance;
        return instance;
    }


    Game(const Game &) = delete; // 明确禁止拷贝（public + deleted）
    Game &operator=(const Game &) = delete; // 明确禁止拷贝赋值

    ~Game();

    void init();

    void run();

    void clean() const;

    void changeScene(Scene *scene);

    [[nodiscard]] SDL_Renderer *getRenderer() const {
        return renderer;
    }

    [[nodiscard]] SDL_Window *getWindow() const {
        return window;
    }

    [[nodiscard]] int getWindowWidth() const {
        return windowWidth;
    }

    [[nodiscard]] int getWindowHeight() const {
        return windowHeight;
    }

    [[nodiscard]] bool running() const {
        return isRunning;
    }

protected:
    void handleEvent(SDL_Event *event);

    void update(float dt) const;

    void render() const;

private:
    Game();

    SDL_Window *window{};
    SDL_Renderer *renderer{};
    Scene *currentScene{};

    const char *windowTitle = "Star Wars";
    int windowWidth = 640;
    int windowHeight = 960;
    bool isRunning = false;

    const float fps = 60.f;
    const float frameTime = 1000.f / fps;
    float deltaTime = 0;
    Uint32 currentFps = 0;
};

#endif // GAME_H
