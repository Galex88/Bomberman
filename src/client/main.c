#include "./headers/main.h"
#include "./headers/globals.h"
#include "./headers/menu.h"
#include "./headers/enter_ip.h"
#include "../server/headers/server.h"
#include "./headers/map.h"

SDL_Surface *SCREEN;
TTF_Font *FONT;
pthread_t SERVER_THREAD;

int on_server() {
    if (pthread_create(&SERVER_THREAD, NULL, main_server, NULL)) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    return GO_MENU;
}

int on_game(int ip) {
    if (ip == 0) {
        return on_enter_ip(&ip);
    }
    return GO_QUIT;
}

void init_globals()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SCREEN = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (SCREEN == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    FONT = TTF_OpenFont("./resources/polices/04B_30__.TTF", 32);
}

int main(int argc, char *argv[])
{
    int run = 1;
    int NEXT_ACTION = GO_MENU;

    init_globals();

    while(run) {
        switch (NEXT_ACTION)
        {
            case GO_MENU:
                NEXT_ACTION = on_menu();
                break;
            case GO_GAME_JOIN:
                NEXT_ACTION = draw_map();
                //NEXT_ACTION = on_game(0);
                break;
            case GO_GAME_HOST:
                NEXT_ACTION = on_game(1);
                break;
            case GO_SERVER:
                NEXT_ACTION = on_server();
                break;
            case GO_QUIT:
                pthread_cancel(SERVER_THREAD);
                run = 0;
                break;
        }
    }
    if (pthread_join(SERVER_THREAD, NULL)) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    TTF_CloseFont(FONT);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}