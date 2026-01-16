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
    player.speed = 2;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::handleEvent(SDL_Event *)
{
}

void SceneMain::keyboardControl()
{
    // 控制主角移动
    auto state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
    {
        player.position.x -= player.speed;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    {
        player.position.x += player.speed;
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
    {
        player.position.y -= player.speed;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
    {
        player.position.y += player.speed;
    }
    // 限制活动区域
    if (player.position.x < 0)
    {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
    {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }
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
    keyboardControl();
}

void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
    }
}