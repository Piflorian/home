#define BLOC_TAILLE 16
#define MARIO_HEIGHT 32
#define MARIO_WIDTH 16
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 320
#define MVT_NBR 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./fonctions.h"

int main(int argc, char **argv)
{
    SDL_Window *window     = NULL;
    SDL_Renderer *renderer = NULL;
    int i, infinite_loop = 1, imageCp = 1, imageCp_g = 1, n_boucle=0, delay = 12, monde=1;

    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        QuitError("Initialisation SDL");
    // Création de la fenêtre et du rendu

    // Fenêtre
    window = SDL_CreateWindow("Super Mario Bros",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    // Moteur de rendu
    renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED);


    /**************************************/
    /******* CREATION DU BACKGROUND *******/
    /**************************************/
    background bg;
    bg.rect.x = 0;
    bg.rect.y = 0;
    bg.surf = IMG_Load("./img/bg.png");
    bg.tex  = SDL_CreateTextureFromSurface(renderer, bg.surf);
    if(bg.surf == NULL)
        QuitDestroy(window, renderer, "Image de fond non chargee");
    if(bg.tex == NULL)
        QuitDestroy(window, renderer, "Texture de fond non chargee");
    SDL_FreeSurface(bg.surf);
    if(SDL_QueryTexture(bg.tex, NULL, NULL, &bg.rect.w, &bg.rect.h) != 0)
        QuitDestroy(window, renderer, "Chargement de la texture de fond impossible");

    SDL_Surface *perdu_surf = IMG_Load("./img/score_final.png");
    SDL_Texture *perdu_tex  = SDL_CreateTextureFromSurface(renderer, perdu_surf);
    SDL_FreeSurface(perdu_surf);


    /**************************************/
    /********* CREATION DE MARIO **********/
    /**************************************/
    perso mario;
    mario.pos.x = mario.X = 0 ;
    mario.pos.y = mario.Y = SCREEN_HEIGHT - MARIO_HEIGHT - BLOC_TAILLE;
    mario.mvt       = 0;
    mario.nTex      = 4;
    mario.isJumping = 0;
    mario.nBloc     = 0;
    mario.vie       = 3;
    mario.surf[0] = IMG_Load("./img/mario_walk1.png");
    mario.surf[1] = IMG_Load("./img/mario_walk2.png");
    mario.surf[2] = IMG_Load("./img/mario_walk3.png");
    mario.surf[3] = IMG_Load("./img/mario_jump.png");
    mario.surf[4] = IMG_Load("./img/mario_stop.png");
    mario.surf[5] = IMG_Load("./img/mario_jump_inv.png");
    mario.surf[6] = IMG_Load("./img/mario_walk3_inv.png");
    mario.surf[7] = IMG_Load("./img/mario_walk2_inv.png");
    mario.surf[8] = IMG_Load("./img/mario_walk1_inv.png");
    for(i=0; i<=8; i++)
        {
            mario.tex[i]  = SDL_CreateTextureFromSurface(renderer, mario.surf[i]);
            if(mario.surf[i] == NULL)
                QuitDestroy(window, renderer, "Image de Mario non chargee");
            if(mario.tex == NULL)
                QuitDestroy(window, renderer, "Texture de Mario non chargee");
            if(SDL_QueryTexture(mario.tex[0], NULL, NULL, &mario.pos.w, &mario.pos.h) != 0)
                QuitDestroy(window, renderer, "Chargement de la texture de Mario impossible");
        }
    for(i=0; i<9; i++)
        SDL_FreeSurface(mario.surf[i]);


    /**************************************/
    /********* CREATION DE BLOCS **********/
    /**************************************/
    FILE *file_bloc = fopen("./monde1.txt","r");
    char check_bloc;
    if(file_bloc==NULL){printf("Le fichier monde1 n'a pas pu être ouvert\n"); return 1;}
    SDL_Surface *b_surf_b = IMG_Load("./img/block_ground.png");
    SDL_Surface *b_surf_B = IMG_Load("./img/block_B.png");
    SDL_Surface *b_surf_C = IMG_Load("./img/block_C.png");
    SDL_Surface *b_surf_D = IMG_Load("./img/block_D.png");
    SDL_Texture *b_tex[4];
    b_tex[0] = SDL_CreateTextureFromSurface(renderer,b_surf_b);
    b_tex[1] = SDL_CreateTextureFromSurface(renderer,b_surf_B);
    b_tex[2] = SDL_CreateTextureFromSurface(renderer,b_surf_C);
    b_tex[3] = SDL_CreateTextureFromSurface(renderer,b_surf_D);
    SDL_FreeSurface(b_surf_b);SDL_FreeSurface(b_surf_B);SDL_FreeSurface(b_surf_C);SDL_FreeSurface(b_surf_D);
    block *bloc = malloc(64 *(sizeof(int)+sizeof(SDL_Texture*)+sizeof(SDL_Surface*)+sizeof(SDL_Rect)));
    for(i=0; i<64; i++)
    {
        check_bloc = fgetc(file_bloc);
        (bloc+i)->rect.w = BLOC_TAILLE;
        (bloc+i)->rect.x = BLOC_TAILLE*i;
        switch(check_bloc)
        {
            case 'b':
                (bloc+i)->rect.h = BLOC_TAILLE;
                (bloc+i)->alive = 1;
                (bloc+i)->rect.y = SCREEN_HEIGHT - BLOC_TAILLE;
                (bloc+i)->tex = b_tex[0];
                break;
            case 'B':
                (bloc+i)->rect.h = BLOC_TAILLE*2;
                (bloc+i)->alive = 1;
                (bloc+i)->rect.y = SCREEN_HEIGHT - BLOC_TAILLE*2;
                (bloc+i)->tex = b_tex[1];
                break;
            case 'C':
                (bloc+i)->rect.h = BLOC_TAILLE*3;
                (bloc+i)->alive = 1;
                (bloc+i)->rect.y = SCREEN_HEIGHT - BLOC_TAILLE*3;
                (bloc+i)->tex = b_tex[2];
                break;
            case 'D':
                (bloc+i)->rect.h = BLOC_TAILLE*4;
                (bloc+i)->alive = 1;
                (bloc+i)->rect.y = SCREEN_HEIGHT - BLOC_TAILLE*4;
                (bloc+i)->tex = b_tex[3];
                break;
            case 'x':
                (bloc+i)->alive = 0;
                (bloc+i)->rect.y = 500;
                (bloc+i)->tex = b_tex[0];
                break;
        }
        if(SDL_QueryTexture((bloc+i)->tex, NULL, NULL, &(bloc+i)->rect.w, &(bloc+i)->rect.h) != 0)
            QuitDestroy(window, renderer, "Chargement de la texture de bloc impossible");
    }
    fclose(file_bloc);


    /**************************************/
    /********* CREATION DE GOOMBA *********/
    /**************************************/
    perso gomba;
    gomba.X = 26*BLOC_TAILLE;
    gomba.Y = SCREEN_HEIGHT - MARIO_HEIGHT - BLOC_TAILLE;
    gomba.pos.x = 26*BLOC_TAILLE;
    gomba.pos.y = SCREEN_HEIGHT - BLOC_TAILLE - BLOC_TAILLE;
    gomba.mvt       = -1;
    gomba.nTex      = 0;
    gomba.vie       = 1;
    gomba.nBloc     = 26;
    gomba.surf[0] = IMG_Load("./img/gomba_0.png");
    gomba.surf[1] = IMG_Load("./img/gomba_1.png");
    gomba.surf[2] = IMG_Load("./img/gomba_2.png");
    for(i=0; i<=2; i++)
        {
            gomba.tex[i]  = SDL_CreateTextureFromSurface(renderer, gomba.surf[i]);
            if(gomba.surf[i] == NULL)
                QuitDestroy(window, renderer, "Image de gomba non chargee");
            if(gomba.tex == NULL)
                QuitDestroy(window, renderer, "Texture de gomba non chargee");
            if(SDL_QueryTexture(gomba.tex[0], NULL, NULL, &gomba.pos.w, &gomba.pos.h) != 0)
                QuitDestroy(window, renderer, "Chargement de la texture de gomba impossible");
        }
    for(i=0; i<2; i++)
        SDL_FreeSurface(gomba.surf[i]);


    /**************************************/
    /*************** SCORE ****************/
    /**************************************/
    if(TTF_Init() < 0)
        QuitDestroy(window, renderer, "Police n'a pas pu etre chargee");
    int  score_i = 5000;
    char score_c[5];
    sprintf(score_c,"%d",score_i);
    TTF_Font *police;
    police = TTF_OpenFont("./font/myfont.ttf", SCREEN_HEIGHT - MARIO_HEIGHT);
    SDL_Color white =  {255,255,255};

    SDL_Rect score_pos;
    score_pos.x = 10;
    score_pos.y = 10 ;
    score_pos.w = 50;
    score_pos.h = 35;

    SDL_Surface *font_surf = TTF_RenderText_Blended(police,score_c,white);
    SDL_Texture *font_tex = SDL_CreateTextureFromSurface(renderer,font_surf);
    SDL_FreeSurface(font_surf);


    /**************************************/
    /********* BOUCLE INFINIE *************/
    /**************************************/
    SDL_Event event;
    while(infinite_loop)
    {
        // score descend de 1 tout les 5 tours de boucle
        if(n_boucle == 5)
            {
                scoreUpgrade(-1 ,&score_i, score_c, police, white, renderer, &font_surf, &font_tex);
                n_boucle=0;
            }
        else
            n_boucle++;

        // changement de monde
        if(mario.nBloc >= 62 && mario.isJumping==1)
        {
            printf("- - - - - - - - - -\n");
            printf("--- NIVEAU N.%d FINI ---\n", monde);
            printf("--- SCORE DE DRAPEAU: ");
            scoreUpgrade(100+(SCREEN_HEIGHT-mario.Y)*monde ,&score_i, score_c, police, white, renderer, &font_surf, &font_tex);
            printf("--- SCORE DE GOMBA: ");
            if(gomba.vie==0)
                {
                    scoreUpgrade(100,&score_i, score_c, police, white, renderer, &font_surf, &font_tex);
                }
                else printf("0\n");
            switch(monde)
            {
            case 1:
                file_bloc = fopen("./monde2.txt","r");
                monde++;
                break;
            case 2:
                file_bloc = fopen("./monde3.txt","r");
                monde++;
                break;
            case 3:
                file_bloc = fopen("./monde4.txt","r");
                monde++;
                break;
            default :
                file_bloc = fopen("./monde1.txt","r");
                monde=1;
                break;
            }
            bloc = chgMonde(&mario, &gomba, &bg, bloc, file_bloc, b_tex);
            for(i=0; i<64; i++)
                if(SDL_QueryTexture((bloc+i)->tex, NULL, NULL, &(bloc+i)->rect.w, &(bloc+i)->rect.h) != 0)
                    QuitDestroy(window, renderer, "Chargement de la texture de bloc impossible");
            fclose(file_bloc);
        }

        // COPIE DE TOUT LES RENDUS
        SDL_RenderClear(renderer);
        if(SDL_RenderCopy(renderer, bg.tex, NULL, &bg.rect) != 0)
            QuitDestroy(window, renderer, "Affichage de la texture de fond impossible");
        for(i=0; i<64; i++)
            if(SDL_RenderCopy(renderer, (bloc+i)->tex, NULL, &(bloc+i)->rect) != 0)
                QuitDestroy(window, renderer, "Affichage de la texture de bloc impossible");
        if(SDL_RenderCopy(renderer, font_tex, NULL, &score_pos) != 0)
            QuitDestroy(window, renderer, "Affichage de la police impossible");
        if(SDL_RenderCopy(renderer, mario.tex[mario.nTex], NULL, &mario.pos) != 0)
            QuitDestroy(window, renderer, "Affichage de la texture de Mario impossible");
        if(gomba.vie != 0)
            if(SDL_RenderCopy(renderer, gomba.tex[gomba.nTex], NULL, &gomba.pos) != 0)
                QuitDestroy(window, renderer, "Affichage de la texture de gomba impossible");
        SDL_RenderPresent(renderer);

        // FAIT TOMBER MARIO AU PLUS BAS POSSIBLE (sol ou trou)
        // + VERIFIE SI MARIO COURS OU PAS
        run(&mario, &gomba, mario.mvt, &bg, bloc);
        fall(&mario, bloc[mario.nBloc]);
        if(mario.vie==0)
            infinite_loop=0;

        // FAIT BOUGER LE GOMBA
        if(gomba.vie==1)
            gomba.vie = gomba_vie(&gomba, &mario);
        if(gomba.vie==1)
            gomba_run(&gomba, &mario, bloc);

        // Evenements du clavier
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_RIGHT:
                            if(mario.mvt == 0) mario.mvt = 1;
                            break;
                        case SDLK_LEFT:
                            if(mario.mvt == 0) mario.mvt = -1;
                            break;
                        case SDLK_SPACE:
                            jump(&mario, bloc[mario.nBloc]);
                            break;
                        default:
                            break;
                    }break;
                }
                case SDL_KEYUP:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_RIGHT:
                            mario.mvt = 0;
                            break;
                        case SDLK_LEFT:
                            mario.mvt = 0;
                            break;

                        default:
                            break;
                    }break;
                }
                case SDL_QUIT:
                    infinite_loop = 0;
                    break;
                default:
                    break;
            }
        }
        // ANIMATION DE MARIO + GOMBA
        if(gomba.vie != 0)
        gombaAnim(&gomba, &imageCp_g);
        persoAnim(&mario,&imageCp);

        // numero du bloc ou mario/gomba est
        mario.nBloc = (mario.X+MARIO_WIDTH/2)/BLOC_TAILLE;
        if(gomba.vie != 0)
        gomba.nBloc = gomba.X/BLOC_TAILLE;

        SDL_Delay(delay);
    }

    // affichage du score final
    score_pos.x = 215;
    score_pos.y = 249 ;
    if(SDL_RenderCopy(renderer, perdu_tex, NULL, NULL) != 0)
        QuitDestroy(window, renderer, "Affichage du bg final");
    if(SDL_RenderCopy(renderer, font_tex, NULL, &score_pos) != 0)
        QuitDestroy(window, renderer, "Affichage de la police impossible");
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);

    // Libération de la mémoire
    SDL_DestroyTexture(font_tex);
    SDL_DestroyTexture(perdu_tex);
    SDL_DestroyTexture(bg.tex);
    for(i=0; i<=8; i++)
        SDL_DestroyTexture(mario.tex[i]);
    for(i=0; i<=64; i++)
        SDL_DestroyTexture((bloc+i)->tex);
    free(bloc);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
