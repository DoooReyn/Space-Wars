#include "SceneMain.h"
#include "Object.h"
#include "Res.h"
#include "Game.h"
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance()) {
}

SceneMain::~SceneMain()
= default;

void SceneMain::init() {
    player.texture = IMG_LoadTexture(game.getRenderer(), RES_PLAYER);
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载主角纹理失败: %s", IMG_GetError());
        return;
    }

    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.speed = 300;
    player.position.x = static_cast<float>(game.getWindowWidth() - player.width) / 2.0f;
    player.position.y = static_cast<float>(game.getWindowHeight() - player.height);
}

void SceneMain::handleEvent(SDL_Event *) {
}

void SceneMain::keyboardControl(float deltaTime) {
    // 控制主角移动
    auto controlled = false;
    auto state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        player.position.x -= player.speed * deltaTime;
        controlled = true;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        player.position.x += player.speed * deltaTime;
        controlled = true;
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        player.position.y -= player.speed * deltaTime;
        controlled = true;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        player.position.y += player.speed * deltaTime;
        controlled = true;
    }
    // 限制活动区域
    if (controlled) {
        if (player.position.x < 0) {
            player.position.x = 0;
        }
        const auto rightEdge = static_cast<float>(game.getWindowWidth() - player.width);
        const auto bottomEdge = static_cast<float>(game.getWindowHeight() - player.height);
        if (player.position.x > rightEdge) {
            player.position.x = rightEdge;
        }
        if (player.position.y < 0) {
            player.position.y = 0;
        }
        if (player.position.y > bottomEdge) {
            player.position.y = bottomEdge;
        }
    }
}

void SceneMain::render() {
    SDL_FRect rect = {
        player.position.x,
        player.position.y,
        static_cast<float>(player.width),
        static_cast<float>(player.height),
    };
    SDL_RenderCopyF(game.getRenderer(), player.texture, nullptr, &rect);
}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
}

void SceneMain::clean() {
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
}
