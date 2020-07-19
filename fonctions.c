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

int i;

void QuitError(char *err)
{
    SDL_Log("Erreur : %s -> %s \n", err, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void QuitDestroy(SDL_Window *w, SDL_Renderer *r, char *c)
{
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    QuitError(c);
}

void run(perso *p, perso *g, int d, background *bg, block bloc[64])
{
    // droite
    if(d == 1 && p->nBloc < 62)
    {
        // bloc genant sur la droite
        if (!(p->pos.x+MARIO_WIDTH >= bloc[p->nBloc+1].rect.x && p->pos.x+MARIO_WIDTH <= bloc[p->nBloc+1].rect.x+bloc[p->nBloc+1].rect.h &&
        p->pos.y > bloc[p->nBloc+1].rect.y-MARIO_HEIGHT &&
        p->pos.y < bloc[p->nBloc+1].rect.y+bloc[p->nBloc+1].rect.h))
        {
            // si mario (dessin) est avant le milieu de la fenetre
            if(p->pos.x + MARIO_WIDTH < SCREEN_WIDTH/2)
                p->pos.x+=MVT_NBR;
            else
            {
                if(bg->rect.x < bg->rect.w - SCREEN_WIDTH)
                {
                    bg->rect.x-=MVT_NBR;
                    if(g->vie==1)
                        g->pos.x-=MVT_NBR;
                    for(i=0; i<64; i++)
                    {
                        bloc[i].rect.x-=MVT_NBR;
                    }
                }
            }
            if(p->nBloc < 62)
            {
                p->X+= MVT_NBR;
            }
        }
    }
    // gauche
    if(d == -1)
    {
        //bloc genant sur la gauche
        if (!(p->pos.x >= bloc[p->nBloc-1].rect.x && p->pos.x+MARIO_WIDTH <= bloc[p->nBloc+1].rect.x+bloc[p->nBloc-1].rect.h &&
        p->pos.y > bloc[p->nBloc-1].rect.y-MARIO_HEIGHT &&
        p->pos.y < bloc[p->nBloc-1].rect.y+bloc[p->nBloc-1].rect.h))
        {
            // si mario (dessin) est apres le premier quart de la fenetre
            if(p->pos.x >= SCREEN_WIDTH/4)
                p->pos.x-= MVT_NBR;
            else
            {
                if(p->nBloc > 8)
                {
                    bg->rect.x+=MVT_NBR;
                    if(g->vie==1)
                        g->pos.x+=MVT_NBR;
                    for(i=0; i<64; i++)
                    {
                        bloc[i].rect.x+=MVT_NBR;
                    }
                }
            }
            if(p->nBloc > 8)
                p->X-= MVT_NBR;
        }
    }
}

void gomba_run(perso *g, perso *m, block b[64])
{
    if(g->mvt == -1)
    {
        if(b[g->nBloc].rect.y > g->pos.y && b[g->nBloc].rect.y < SCREEN_HEIGHT)
        {
            g->pos.x--;
            g->X--;
        }
        else
            g->mvt=1;
    }
    else
    {
        if(b[g->nBloc+1].rect.y > g->pos.y && b[g->nBloc+1].rect.y < SCREEN_HEIGHT)
        {
            g->pos.x++;
            g->X++;
        }
        else
            g->mvt=-1;
    }
}

void jump(perso *p, block b)
{
    if(p->pos.y == b.rect.y - MARIO_HEIGHT)
    {
    	p->pos.y-= MARIO_HEIGHT*2;
    	p->Y    -= MARIO_HEIGHT*2;
    	p->isJumping = 1;
    	if(p->mvt == 1)
            p->nTex = 3;
        else
            if(p->mvt == -1)
                p->nTex = 5;
            else
                p->nTex = 3;
    }
}

void fall(perso *p, block b)
{
    // si perso se trouve au dessus du bloc
    if((p->pos.y + MARIO_HEIGHT < b.rect.y)||(p->pos.y + MARIO_HEIGHT > b.rect.y+1))
    {
        p->pos.y+=MVT_NBR;
        p->Y    +=MVT_NBR;
        // si perso se trouve sur le bloc
        if(p->pos.y + MARIO_HEIGHT >= b.rect.y)
        {
            p->isJumping = 0;
            p->nTex = 4;
        }
    }
    if(p->pos.y > SCREEN_HEIGHT + BLOC_TAILLE)
        {
            p->vie--;
            printf("--- VIE -1 / vie(s) restante(s) : %d\n",p->vie);
            p->Y = p->pos.y = SCREEN_HEIGHT-MARIO_HEIGHT-4*BLOC_TAILLE;
            p->pos.x = b.rect.x-BLOC_TAILLE;
            p->X-= BLOC_TAILLE;
        }
}

void persoAnim(perso *p, int *imageCp)
{
    // Chaque 3 boucles : on remet a 0 le compteur + on change l'image
    if(*imageCp == 3)
    {
        *imageCp=1;
        // Si le perso de saute pas
        if(p->isJumping == 0)
        {
            // Si le perso va vers la droite
            if(p->mvt == 1)
            {
                if(p->nTex >= 2)
                    p->nTex = 0;
                else
                    p->nTex++;
            }
            else
            {
                // Si le perso va vers la gauche
                if(p->mvt == -1)
                {
                    if(p->nTex <= 6)
                        p->nTex = 8;
                    else
                        p->nTex--;
                }
                // Si le perso ne va dans aucune direction
                else
                    p->nTex = 4;
            }
        }
    }
    else (*imageCp)++;
}

void gombaAnim(perso *g, int *i)
{
    (*i)++;
    if(*i==20)
        g->nTex=1;
    if(*i==40)
    {
        g->nTex=0;
        (*i)=1;
    }
}

int gomba_vie(perso *g, perso *m)
{
    if(g->pos.x <= m->pos.x+MARIO_WIDTH && g->pos.x+MARIO_WIDTH >= m->pos.x &&
       g->pos.y+5>=m->pos.y+MARIO_HEIGHT && g->pos.y<=m->pos.y+MARIO_HEIGHT)
        {
            return 0;
        }

    return 1;
}

block* chgMonde(perso *mario, perso *gomba, background *bg, block *bloc, FILE *file, SDL_Texture *b_tex[])
{
    mario->mvt   = 0;
    mario->pos.x = 0;
    mario->X     = 0;
    bg->rect.x   = 0;
    gomba->pos.x = 26*BLOC_TAILLE;
    gomba->X = 26*BLOC_TAILLE;
    gomba->vie=1;

    char check_bloc;
    for(i=0; i<64; i++)
    {
        check_bloc = fgetc(file);
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
    }
    return bloc;
}

void scoreUpgrade(int x, int *score_i, char score_c[], TTF_Font *police, SDL_Color color, SDL_Renderer *renderer, SDL_Surface **surf, SDL_Texture **tex)
{
    *score_i += x;
    if(x!=-1)
        printf("%d \n",x);
    *surf = TTF_RenderText_Blended(police,score_c,color);
    *tex = SDL_CreateTextureFromSurface(renderer,*surf);
    SDL_FreeSurface(*surf);
    sprintf(score_c,"%d",*score_i);

}
