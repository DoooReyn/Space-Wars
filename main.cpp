#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int main(int, char const **)
{
    SDL_Log("Hello SDL %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

    // 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL init failed: %s", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Space Wars", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL create window failed.");
        return 1;
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL create renderer failed.");
        return 1;
    }

    // SDL_image 初始化
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(flags) != flags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init failed: %s", IMG_GetError());
        return 1;
    }

    // 加载图片
    const char *imgPath = "../assets/image/1.png";
    SDL_Texture *texture = IMG_LoadTexture(renderer, imgPath);
    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image init failed: %s", imgPath);
        return 1;
    }

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);

    // 运行标记
    bool running = true;

    // 绘制原理（双层帧缓冲）
    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }

        // 检查运行标记
        if (!running)
        {
            break;
        }

        // 清屏
        SDL_RenderClear(renderer);

        // 设置画笔
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // 绘制矩形
        SDL_Rect rect = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &rect);

        // 绘制纹理
        SDL_Rect imgRect = {100 + (200 - tex_w) / 2, 100 + (200 - tex_h) / 2, tex_w, tex_h};
        SDL_RenderCopy(renderer, texture, NULL, &imgRect);

        // 恢复画笔
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 销毁纹理
    SDL_DestroyTexture(texture);
    // 退出 SDL_image 模块
    IMG_Quit();

    // 销毁渲染器
    SDL_DestroyRenderer(renderer);
    // 销毁窗口
    SDL_DestroyWindow(window);
    // 退出
    SDL_Quit();

    return 0;
}
