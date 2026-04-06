#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class AudioManager {
public:
    Mix_Chunk *slice = nullptr;
    Mix_Chunk *explosion = nullptr;

    AudioManager() {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cout << "Erro SDL: " << SDL_GetError() << std::endl;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "Erro Mixer: " << Mix_GetError() << std::endl;
        }

        slice = Mix_LoadWAV("assets/slice.wav");
        if (!slice) {
            std::cout << "Erro slice: " << Mix_GetError() << std::endl;
        }

        explosion = Mix_LoadWAV("assets/explosion.wav");
        if (!explosion) {
            std::cout << "Erro explosion: " << Mix_GetError() << std::endl;
        }
    }

    void playSlice() {
        if (slice) Mix_PlayChannel(-1, slice, 0);
    }

    void playExplosion() {
        if (explosion) Mix_PlayChannel(-1, explosion, 0);
    }

    ~AudioManager() {
        Mix_FreeChunk(slice);
        Mix_FreeChunk(explosion);
        Mix_CloseAudio();
        SDL_Quit();
    }
};