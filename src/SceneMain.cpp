#include "SceneMain.h"
#include "Object.h"
#include "Res.h"
#include "Game.h"
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    player.texture = IMG_LoadTexture(game.getRenderer(), RES_PLAYER);
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载主角纹理失败: %s", IMG_GetError());
        return;
    }
    
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::handleEvent(SDL_Event *event)
{
    // SDL_Log("SceneMain::handleEvent: %d", event->type);
}

void SceneMain::render()
{
    // SDL_Log("SceneMain::render");
    SDL_Rect rect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height,
    };
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &rect);
}

void SceneMain::update()
{
    // SDL_Log("SceneMain::update");
}

void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
}