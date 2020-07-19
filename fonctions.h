#include <stdio.h>
#include <stdlib.h>

typedef struct perso_t
{
    int mvt;
    int nTex;
    int isJumping;
    int nBloc;
    int X;
    int Y;
    int vie;
    SDL_Surface *surf[10];
    SDL_Texture *tex[10];
    SDL_Rect     pos;
}perso;

typedef struct background_t
{
    SDL_Surface *surf;
    SDL_Texture *tex;
    SDL_Rect     rect;
}background;

typedef struct block_t
{
    int alive;
    SDL_Surface *surf;
    SDL_Texture *tex;
    SDL_Rect     rect;
}block;

void QuitError(char*);
void QuitDestroy(SDL_Window*, SDL_Renderer*, char*);
void run(perso*, perso*, int, background*, block[]);
void gomba_run(perso*, perso*, block[]);
void jump(perso*, block);
void fall(perso*, block);
void persoAnim(perso*, int*);
void gombaAnim(perso*, int*);
int gomba_vie(perso*, perso*);
block* chgMonde(perso*, perso*, background*, block*, FILE*, SDL_Texture *b_tex[]);
void scoreUpgrade(int, int*, char score_c[], TTF_Font*, SDL_Color, SDL_Renderer*, SDL_Surface**, SDL_Texture**);
