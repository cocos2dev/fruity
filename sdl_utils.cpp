// made by cocos
// use it like you want to use it
// just give credit and dont sell this

#include "sdl_utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_syswm.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <windows.h>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

void SetWindowStyles(SDL_Window* window) {
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);


    // removing minimize and maximze buttons
    if (SDL_GetWindowWMInfo(window, &info)) {
        HWND hwnd = info.info.win.window;
        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
        SetWindowLong(hwnd, GWL_STYLE, style);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    } else {
        std::cerr << "SDL_GetWindowWMInfo failed: " << SDL_GetError() << std::endl;
    }
}

int displayAnimation(const std::string& name, int msDelay, int winSize, int onTop) {
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator("./" + name)) {
        if (entry.path().extension() == ".png") {
            files.push_back(entry.path().string());
        }
    }
    
    if (files.size() < 2) {
        std::cerr << "You need at least 2 animation sprites!" << std::endl;
        return 1;
    }

    std::sort(files.begin(), files.end());

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Fruity Animation Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winSize, winSize, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    
    SetWindowStyles(window);
    if(onTop == 1) { SDL_SetWindowAlwaysOnTop(window, SDL_TRUE); }
    

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 50, 64, 74, 5);  
    

    bool running = true;
    while (running) {
        for (const auto& file : files) {
            SDL_Surface* image = IMG_Load(file.c_str());
            if (!image) {
                std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
                continue;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);

            if (!texture) {
                std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
                continue;
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            SDL_DestroyTexture(texture);

            std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    break;
                }
            }
            if (!running) break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
