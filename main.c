#include "proto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>

/* Déclaration des variables / structures utilisées par le jeu */

SDL_Window *screen;
SDL_Renderer *renderer; 

Game game;  
Input input;

Glass glassU;
Glass glassD;
Glass glassT;

int main()
{
    init(TITLE);

    loadGame();
 
    atexit(cleanup);

    unsigned int frameLimit = SDL_GetTicks() + NBR_FPS;

    bool continuer = true;

  
    // Boucle principale
    while (continuer)
    {
        getInput(&input);

        updateGame();

        drawGame();     

        // Gestion des fps 
        delay(frameLimit);
        frameLimit = SDL_GetTicks() + NBR_FPS;
    }
 
    exit(0);

    return(EXIT_SUCCESS);
 
}
 
void drawGame(void)
{
    drawBackground();

    drawGlass();

    SDL_RenderPresent(getrenderer());
    
    SDL_Delay(1);
}

void drawGlass()
{
    if (game.animation == NONE)
    {    
        if (glassU.selected == true)
            drawImage(game.glassSelectedTexture, glassU.position.x , glassU.position.y);
        else
            drawImage(game.glassTexture, glassU.position.x , glassU.position.y);

        if (glassD.selected == true)
            drawImage(game.glassSelectedTexture, glassD.position.x , glassD.position.y);
        else
            drawImage(game.glassTexture, glassD.position.x , glassD.position.y);

        if (glassT.selected == true)
            drawImage(game.glassSelectedTexture, glassT.position.x , glassT.position.y);
        else
            drawImage(game.glassTexture, glassT.position.x , glassT.position.y);
    }
    else
    {
        drawImage(game.glassTexture, glassU.position.x , glassU.position.y);
        drawImage(game.glassTexture, glassD.position.x , glassD.position.y);
        drawImage(game.glassTexture, glassT.position.x , glassT.position.y);
    }
}

void drawBackground()
{
    SDL_SetRenderDrawColor(getrenderer(), 92, 98, 178, 255);
    SDL_RenderClear(getrenderer());
}
 
 
void delay(unsigned int frameLimit)
{
    // Gestion des fps
    unsigned int ticks = SDL_GetTicks();
 
    if (frameLimit < ticks)
    {
        return;
    }
 
    if (frameLimit > ticks + NBR_FPS)
    {
        SDL_Delay(NBR_FPS);
    }
 
    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

SDL_Texture *loadImage(char *name)
    {      
        SDL_Surface *loadedImage = NULL;
        SDL_Texture *texture = NULL;

        loadedImage = IMG_Load(name);
     
        if (loadedImage != NULL)
        {
            // Conversion de l'image en texture
            texture = SDL_CreateTextureFromSurface(getrenderer(), loadedImage);
     
            // On se débarrasse du pointeur vers une surface
            SDL_FreeSurface(loadedImage);
            loadedImage = NULL;
        }
        else
            printf("L'image n'a pas pu être chargée! SDL_Error : %s\n", SDL_GetError());
     
        return texture;
}
 
 
void drawImage(SDL_Texture *image, int x, int y)
{
     
    SDL_Rect position;
     
    position.x = x;
    position.y = y;
     
    SDL_QueryTexture(image, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(getrenderer(), image, NULL, &position);
 
}

SDL_Renderer *getrenderer(void)
{
    return renderer;
}
 
 
void init(char *title)
{
    //Génération de l'écran principal (centré) et du renderer avec la synchro verticale : VSYNC
 
    screen = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);
 
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
 
    if (screen == NULL || renderer == NULL)
    {
        printf("Impossible d'initialiser le mode écran à %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }
 
    //Initialisation de SDL_Image 2

    int imgFlags = IMG_INIT_PNG;

    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError() );
        exit(1);
    }
 
    /*//Initialisation de SDL_TTF 2

    if (TTF_Init() < 0)
    {
        printf("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
        exit(1);
    }*/

    //SDL_ShowCursor(SDL_DISABLE);
 
}
 
 
 
void cleanup()
{
    
    //Pointeurs
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(screen);
    screen = NULL;
 
    //SDL_TTF 2
    //TTF_Quit();
 
    //SDL
    SDL_Quit();
}

void loadGame()
{
    //Chargement des données du jeu

    //Chargement des textures
    game.glassTexture = loadImage("div/glass.png");
    game.glassSelectedTexture = loadImage("div/glass_selected.png");


    //iniatilisation des variables
    game.animation = NONE;

    game.nbrTourMax = 2; //(+1 = 3)

    game.tour = 0;

    game.spaceBetweenGlass = (SCREEN_WIDTH - (GLASS_WIDTH * NBR_GLASS)) / (NBR_GLASS + 1);

    glassU.position.y = glassD.position.y = glassT.position.y = (SCREEN_HEIGHT - GLASS_HEIGHT) / 2;
    glassU.position.x = (SCREEN_WIDTH - ((GLASS_WIDTH * NBR_GLASS) + ((NBR_GLASS - 1) * game.spaceBetweenGlass))) / 2 ;

    glassD.position.x = glassU.position.x + GLASS_WIDTH + game.spaceBetweenGlass;
    glassT.position.x = glassD.position.x + GLASS_WIDTH + game.spaceBetweenGlass;

    glassU.dest = glassD.dest = glassT.dest = 0;

    glassU.position.w = glassD.position.w = glassT.position.w = GLASS_WIDTH;
    glassU.position.h = glassD.position.h = glassT.position.h = GLASS_HEIGHT;

    glassU.goToTop = glassD.goToTop = glassT.goToTop = false;

    //final
    drawGame();
}
 
 
void getInput(Input *input)
{
    SDL_Event event;
 
    // Keymapping 
 
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
 
            case SDL_QUIT:
                exit(0);
            break;
 
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit(0);
                    break;
                    
                    case SDLK_SPACE:
                        if (game.animation == NONE)
                        {
                            enableAnimation();
                            changeGlass();
                        }

                    default:
                    break;
                }
            break;

            case  SDL_MOUSEMOTION:

                if (game.animation == NONE)
                {
                    glassU.selected = testBelonging(event.motion.x, event.motion.y, glassU.position);
                    glassD.selected = testBelonging(event.motion.x, event.motion.y, glassD.position);
                    glassT.selected = testBelonging(event.motion.x, event.motion.y, glassT.position);
                }
           

            break;
        }
 
    }
}

void pastRound()
{
    if (game.tour == game.nbrTourMax)
    {
        game.tour = 0;
        game.nbrTourMax++;
        enableAnimation();
    }
    else
    {
        game.tour++;
        changeGlass();
    }

}

void updateGame()
{
    if (game.animation != NONE)
    {
        updateGlass();
    }

}

void updateGlass()
{
    int hauteurMax = 112 - GLASS_HEIGHT;
    int hauteurMin = 112 + GLASS_HEIGHT;
    int baseY = 112;

    //Glass 1

    //en haut
    if (glassU.goToTop == true && glassU.position.y > hauteurMax && glassU.dest != 0)
    {
        moveGlass(1, &glassU.position.x, &glassU.position.y, &hauteurMax);
    }
    
    //en bas
    if (glassU.goToTop == false && glassU.position.y < hauteurMin && glassU.dest != 0)
    {
        moveGlass(2, &glassU.position.x, &glassU.position.y, &hauteurMin);
    }

    //sur le cote
    if (glassU.position.y == hauteurMax || glassU.position.y == hauteurMin)
    {
        if (glassU.dest != 0)
        {
            if (glassU.dest < 0)
            {
                moveGlass(4, &glassU.position.x, &glassU.position.y, &glassU.dest);
            }
            else if (glassU.dest > 0)
            {
                moveGlass(3, &glassU.position.x, &glassU.position.y, &glassU.dest);
            }
        }
    }

    //remise en position
    if (glassU.position.y >= hauteurMax || glassU.position.y <= hauteurMin)
    {
        if (glassU.dest == 0)
        {
            if (glassU.goToTop ==  true)
            {
                moveGlass(2, &glassU.position.x, &glassU.position.y, &baseY);
                if (glassU.position.y == baseY)
                {
                    glassU.goToTop = false;
                    pastRound();
                }
            }
            else
            {
                moveGlass(1, &glassU.position.x, &glassU.position.y, &baseY);
            }
        }
    }


    //Glass 2

    //en haut
    if (glassD.goToTop == true && glassD.position.y > hauteurMax && glassD.dest != 0)
    {
        moveGlass(1, &glassD.position.x, &glassD.position.y, &hauteurMax);
    }
    
    //en bas
    if (glassD.goToTop == false && glassD.position.y < hauteurMin && glassD.dest != 0)
    {
        moveGlass(2, &glassD.position.x, &glassD.position.y, &hauteurMin);
    }

    //sur le cote
    if (glassD.position.y == hauteurMax || glassD.position.y == hauteurMin)
    {
        if (glassD.dest != 0)
        {
            if (glassD.dest < 0)
            {
                moveGlass(4, &glassD.position.x, &glassD.position.y, &glassD.dest);
            }
            else if (glassD.dest > 0)
            {
                moveGlass(3, &glassD.position.x, &glassD.position.y, &glassD.dest);
            }
        }
    }

    //remise en position
    if (glassD.position.y >= hauteurMax || glassD.position.y <= hauteurMin)
    {
        if (glassD.dest == 0)
        {
            if (glassD.goToTop ==  true)
            {
                moveGlass(2, &glassD.position.x, &glassD.position.y, &baseY);
                if (glassD.position.y == baseY)
                {
                    glassD.goToTop = false;
                    pastRound();
                }
            }
            else
            {
                moveGlass(1, &glassD.position.x, &glassD.position.y, &baseY);
            }
        }
    }



    //Glass 3

    //en haut
    if (glassT.goToTop == true && glassT.position.y > hauteurMax && glassT.dest != 0)
    {
        moveGlass(1, &glassT.position.x, &glassT.position.y, &hauteurMax);
    }
    
    //en bas
    if (glassT.goToTop == false && glassT.position.y < hauteurMin && glassT.dest != 0)
    {
        moveGlass(2, &glassT.position.x, &glassT.position.y, &hauteurMin);
    }

    //sur le cote
    if (glassT.position.y == hauteurMax || glassT.position.y == hauteurMin)
    {
        if (glassT.dest != 0)
        {
            if (glassT.dest < 0)
            {
                moveGlass(4, &glassT.position.x, &glassT.position.y, &glassT.dest);
            }
            else if (glassT.dest > 0)
            {
                moveGlass(3, &glassT.position.x, &glassT.position.y, &glassT.dest);
            }
        }
    }

    //remise en position
    if (glassT.position.y >= hauteurMax || glassT.position.y <= hauteurMin)
    {
        if (glassT.dest == 0)
        {
            if (glassT.goToTop ==  true)
            {
                moveGlass(2, &glassT.position.x, &glassT.position.y, &baseY);
                if (glassT.position.y == baseY)
                {
                    glassT.goToTop = false;
                    pastRound();
                }
            }
            else
            {
                moveGlass(1, &glassT.position.x, &glassT.position.y, &baseY);
            }
        }
    }


}

void moveGlass(int direction, int *x, int *y, int *distanceMax)
{
    /*1: haut
    2: bas
    3: droite
    4: gauche*/

    //permet de controler le choix de la vitesse

    switch(direction)
    {
        case 1:
            if (*y - GLASS_SPEED < *distanceMax)
            {
                *y = *distanceMax;
            }
            else
            {
                *y -= GLASS_SPEED;
            }
        break;

        case 2:
            if (*y + GLASS_SPEED > *distanceMax)
            {
                *y = *distanceMax;
            }
            else
            {
                *y += GLASS_SPEED;
            }
        break;

        case 3:
            if (*distanceMax - GLASS_SPEED < 0)
            {
                *x += *distanceMax;
                *distanceMax = 0;
            }
            else
            {
                *x += GLASS_SPEED;
                *distanceMax -= GLASS_SPEED;
            }
        break;

        case 4:
            if (*distanceMax + GLASS_SPEED > 0)
            {
                *x += *distanceMax;
                *distanceMax = 0;
            }
            else
            {
                *x -= GLASS_SPEED;
                *distanceMax += GLASS_SPEED;
            }
        break;
    }
}


void changeGlass()
{
    SDL_Rect posGlass1;
    SDL_Rect posGlass2;

    int a, b;

    generate2NumbersRandom(&a, &b);

    int glass1 = a;
    int glass2 = b;

    if(glass1 == 1)
    {
        posGlass1 = glassU.position;
        glassU.goToTop = true;
    }
    else if (glass1 == 2)
    {
        posGlass1 = glassD.position;
        glassD.goToTop = true;
    }
    else if(glass1 == 3)
    {
        posGlass1 = glassT.position;
        glassT.goToTop = true;
    }


    if(glass2 == 1)
    {
        posGlass2 = glassU.position;
    }
    else if (glass2 == 2)
    {
        posGlass2 = glassD.position;
    }
    else if (glass2 == 3)
    {
        posGlass2 = glassT.position;
    }


    int distance = posGlass2.x - posGlass1.x;

    if(glass1 == 1)
    {
        glassU.dest = distance;
    }
    else if (glass1 == 2)
    {
        glassD.dest = distance;
    }
    else if (glass1 == 3)
    {
        glassT.dest = distance;
    }    


    if(glass2 == 1)
    {
        glassU.dest = 0 - distance;
    }
    else if (glass2 == 2)
    {
        glassD.dest = 0 - distance;
    }
    else if (glass2 == 3)
    {
        glassT.dest = 0 - distance;
    }
}

void generate2NumbersRandom(int *a, int *b)
{
    srand(time(NULL));
    *a = randint(100, 399);
    *a = *a / 100;

    do
    {
        *b = randint(100, 399);
        *b = *b / 100; 

    } while (*a == *b);

}

int randint(int a, int b)
{
    return rand()%(b-a) +a;
}

void enableAnimation()
{
    if (game.animation == ChangeGlass)
    {
        game.animation = NONE;
    }
    else
    {
        game.animation = ChangeGlass;
    }
}

bool testBelonging(int mouseX, int mouseY, SDL_Rect box)
{
    if (mouseX >= box.x
        && mouseX < box.x + box.w
        && mouseY >= box.y
        && mouseY < box.y + box.h)
    {
        return true;
    }
    else
    {
        return false;
    }
}