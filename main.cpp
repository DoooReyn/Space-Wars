#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_image load image failed: %s", imgPath);
        return 1;
    }

    int tex_w = 0, tex_h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);

    // 加载音频
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "SDL_mixer init failed: %s", Mix_GetError());
        return 1;
    }

    const char *musicPath = "../assets/music/time_for_adventure.mp3";
    Mix_Music *music = Mix_LoadMUS(musicPath);
    if (music == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "SDL_mixer load music failed: %s", musicPath);
        return 1;
    }
    Mix_PlayMusic(music, -1);

    // 加载ttf
    if (TTF_Init() != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf load failed: %s", TTF_GetError());
        return 1;
    }

    const char *ttfPath = "../assets/fonts/PixelOperator8.ttf";
    TTF_Font *ttfFont = TTF_OpenFont(ttfPath, 18);
    if (ttfFont == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf load font failed: %s", ttfPath);
        return 1;
    }

    SDL_Color color = {255, 255, 24, 255};
    SDL_Surface *ttfSurface = TTF_RenderUTF8_Solid(ttfFont, "Hello, SDL2.", color);
    SDL_Texture *ttfTexture = SDL_CreateTextureFromSurface(renderer, ttfSurface);

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

        // 绘制文本
        SDL_Rect ttfRect = {300, 300, ttfSurface->w, ttfSurface->h};
        SDL_RenderCopy(renderer, ttfTexture, NULL, &ttfRect);

        // 恢复画笔
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 销毁字体资源
    SDL_FreeSurface(ttfSurface);
    SDL_DestroyTexture(ttfTexture);
    TTF_CloseFont(ttfFont);
    TTF_Quit();

    // 销毁音乐
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    // 销毁纹理
    SDL_DestroyTexture(texture);
    IMG_Quit();

    // 销毁 SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
