#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "Scene.h"

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();
    void clean();
    void changeScene(Scene* scene);

    bool running() {
        return isRunning;
    }

protected:
    void handleEvent(SDL_Event *event);
    void update();
    void render();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Scene *currentScene;
    const char* windowTitle = "Star Wars";
    int windowWidth = 600;
    int windowHeight = 800;
    bool isRunning = false;
};

#endif // GAME_H