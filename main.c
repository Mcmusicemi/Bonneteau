#include "proto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

    drawBall();

    drawGlass();

    if (game.animation == ECHEC)
    {
    	drawThumb();
    }

    SDL_RenderPresent(getrenderer());
    
    SDL_Delay(1);
}

void drawThumb()
{
    drawImage(game.thumbTexture, glassD.position.x, game.thumbY);
}

void drawBall()
{
    if (game.animation != REVELATION)
    {
        game.positionBall.x = glassD.position.x + ((glassD.position.w - game.positionBall.w) / 2);
        game.positionBall.y = glassD.position.y + glassD.position.h - game.positionBall.h;
    }
    else if (game.animation == REVELATION)
    {
        drawImage(game.ballTexture, game.positionBall.x, game.positionBall.y);
    }
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

    //SDL_ShowCursor(SDL_DISABLE);
 
}
 
 
 
void cleanup()
{
    
    //Pointeurs
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(screen);
    screen = NULL;
 
    //SDL
    SDL_Quit();
}

void loadGame()
{
    //Chargement des données du jeu

    //Chargement des textures
    game.glassTexture = loadImage("div/glass.png");
    game.glassSelectedTexture = loadImage("div/glass_selected.png");
    game.ballTexture = loadImage("div/ball.png");
    game.thumbTexture = loadImage("div/thumb.png");


    //iniatilisation des variables
    game.animation = NONE;

    game.firstTour = true;

    game.nbrTourMax = 2; //(+1 = 3)

    game.tour = 0;

    game.glassReveal = 0;

    int spaceBetweenGlass = (SCREEN_WIDTH - (GLASS_WIDTH * NBR_GLASS)) / (NBR_GLASS + 1);

    glassU.position.y = glassD.position.y = glassT.position.y = (SCREEN_HEIGHT - GLASS_HEIGHT) / 2;
    glassU.position.x = (SCREEN_WIDTH - ((GLASS_WIDTH * NBR_GLASS) + ((NBR_GLASS - 1) * spaceBetweenGlass))) / 2 ;

    glassD.position.x = glassU.position.x + GLASS_WIDTH + spaceBetweenGlass;
    glassT.position.x = glassD.position.x + GLASS_WIDTH + spaceBetweenGlass;

    glassU.dest = glassD.dest = glassT.dest = 0;

    glassU.position.w = glassD.position.w = glassT.position.w = GLASS_WIDTH;
    glassU.position.h = glassD.position.h = glassT.position.h = GLASS_HEIGHT;

    glassU.goToTop = glassD.goToTop = glassT.goToTop = false;
    game.goToBottom = false;

    game.positionBall.w = game.positionBall.h = 128;
	game.thumbY = -128;    

    //final
    drawGame();

    etablishedReveal(2);
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
                    
                    /*case SDLK_SPACE:
                        if (game.animation == NONE && game.firstTour == true)
                        {
                            enableAnimation();
                            changeGlass();
                        }

                    default:
                    break;*/
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

            case SDL_MOUSEBUTTONUP:

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (game.animation ==  NONE && game.firstTour == false)
                    {
                        if (testBelonging(event.button.x, event.button.y, glassU.position))
                        {
                            etablishedReveal(1);
                        }
                        else if (testBelonging(event.button.x, event.button.y, glassD.position))
                        {
                            etablishedReveal(2);
                        }
                        else if (testBelonging(event.button.x, event.button.y, glassT.position))
                        {
                            etablishedReveal(3);
                        }
                    }
                }
            break;
        }
 
    }
}

void etablishedReveal(int glass)
{
    game.glassReveal = glass;
    game.animation = REVELATION;
}

void pastRound()
{
    if (game.tour == game.nbrTourMax)
    {
        if (game.firstTour == true)
        {
            game.firstTour = false;
        }

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
    int bas = SCREEN_HEIGHT;

    //CHANGEGLASS

    if (game.animation == ChangeGlass)
    {
        //Glass 1

        //en haut
        if (glassU.goToTop == true && glassU.position.y > hauteurMax && glassU.dest != 0)
        {
            moveGlass(HAUT, &glassU.position.x, &glassU.position.y, &hauteurMax);
        }
        
        //en bas
        if (glassU.goToTop == false && glassU.position.y < hauteurMin && glassU.dest != 0)
        {
            moveGlass(BAS, &glassU.position.x, &glassU.position.y, &hauteurMin);
        }

        //sur le cote
        if (glassU.position.y == hauteurMax || glassU.position.y == hauteurMin)
        {
            if (glassU.dest != 0)
            {
                if (glassU.dest < 0)
                {
                    moveGlass(GAUCHE, &glassU.position.x, &glassU.position.y, &glassU.dest);
                }
                else if (glassU.dest > 0)
                {
                    moveGlass(DROITE, &glassU.position.x, &glassU.position.y, &glassU.dest);
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
                    moveGlass(BAS, &glassU.position.x, &glassU.position.y, &baseY);
                    if (glassU.position.y == baseY)
                    {
                        glassU.goToTop = false;
                        pastRound();
                    }
                }
                else
                {
                    moveGlass(HAUT, &glassU.position.x, &glassU.position.y, &baseY);
                }
            }
        }


        //Glass 2

        //en haut
        if (glassD.goToTop == true && glassD.position.y > hauteurMax && glassD.dest != 0)
        {
            moveGlass(HAUT, &glassD.position.x, &glassD.position.y, &hauteurMax);
        }
        
        //en bas
        if (glassD.goToTop == false && glassD.position.y < hauteurMin && glassD.dest != 0)
        {
            moveGlass(BAS, &glassD.position.x, &glassD.position.y, &hauteurMin);
        }

        //sur le cote
        if (glassD.position.y == hauteurMax || glassD.position.y == hauteurMin)
        {
            if (glassD.dest != 0)
            {
                if (glassD.dest < 0)
                {
                    moveGlass(GAUCHE, &glassD.position.x, &glassD.position.y, &glassD.dest);
                }
                else if (glassD.dest > 0)
                {
                    moveGlass(DROITE, &glassD.position.x, &glassD.position.y, &glassD.dest);
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
                    moveGlass(BAS, &glassD.position.x, &glassD.position.y, &baseY);
                    if (glassD.position.y == baseY)
                    {
                        glassD.goToTop = false;
                        pastRound();
                    }
                }
                else
                {
                    moveGlass(HAUT, &glassD.position.x, &glassD.position.y, &baseY);
                }
            }
        }



        //Glass 3

        //en haut
        if (glassT.goToTop == true && glassT.position.y > hauteurMax && glassT.dest != 0)
        {
            moveGlass(HAUT, &glassT.position.x, &glassT.position.y, &hauteurMax);
        }
        
        //en bas
        if (glassT.goToTop == false && glassT.position.y < hauteurMin && glassT.dest != 0)
        {
            moveGlass(BAS, &glassT.position.x, &glassT.position.y, &hauteurMin);
        }

        //sur le cote
        if (glassT.position.y == hauteurMax || glassT.position.y == hauteurMin)
        {
            if (glassT.dest != 0)
            {
                if (glassT.dest < 0)
                {
                    moveGlass(GAUCHE, &glassT.position.x, &glassT.position.y, &glassT.dest);
                }
                else if (glassT.dest > 0)
                {
                    moveGlass(DROITE, &glassT.position.x, &glassT.position.y, &glassT.dest);
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
                    moveGlass(BAS, &glassT.position.x, &glassT.position.y, &baseY);
                    if (glassT.position.y == baseY)
                    {
                        glassT.goToTop = false;
                        pastRound();
                    }
                }
                else
                {
                    moveGlass(HAUT, &glassT.position.x, &glassT.position.y, &baseY);
                }
            }
        }
    }



    //REVELATION

    else if (game.animation == REVELATION)
    {
        if (game.goToBottom == true)
        {
            if (glassU.position.y < baseY)
            {
                moveGlass(BAS, &glassU.position.x, & glassU.position.y, &baseY);
            }
            else if (glassD.position.y < baseY)
            {
                moveGlass(BAS, &glassD.position.x, & glassD.position.y, &baseY);
            }
            else if (glassT.position.y < baseY)
            {
                moveGlass(BAS, &glassT.position.x, & glassT.position.y, &baseY);
            }


            if (glassU.position.y == baseY && glassD.position.y == baseY && glassT.position.y == baseY)
            {
                if (game.glassReveal == 2)
                {
                    game.goToBottom = false;
                    enableAnimation();
                    changeGlass();
                }
                else
                {
                    game.animation = ECHEC;
                }
            }
        }
        //Glass 1
        else if (game.glassReveal == 1)
        {
            if (glassU.position.y > hauteurMax)
            {
                moveGlass(HAUT, &glassU.position.x, &glassU.position.y, &hauteurMax);
            }
        }
        //Glass 2
        else if (game.glassReveal ==  2)
        {
            if (glassD.position.y > hauteurMax)
            {
                moveGlass(HAUT, &glassD.position.x, &glassD.position.y, &hauteurMax);
            }
        }
        //Glass 3
        else if (game.glassReveal == 3)
        {
            if (glassT.position.y > hauteurMax)
            {
                moveGlass(HAUT, &glassT.position.x, &glassT.position.y, &hauteurMax);
            }
        }


        if (glassU.position.y == hauteurMax || glassD.position.y == hauteurMax || glassT.position.y == hauteurMax)
        {
            game.goToBottom = true;
            SDL_Delay(500);
        }

    }

    // ECHEC
    else if (game.animation == ECHEC)
    {
        moveGlass(BAS, &glassU.position.x, &glassU.position.y, &bas);
        moveGlass(BAS, &glassD.position.x, &glassD.position.y, &bas);
        moveGlass(BAS, &glassT.position.x, &glassT.position.y, &bas);

		moveGlass(BAS, &baseY, &game.thumbY, &baseY);
		//1er &baseY ne sert à rien c'est seulement pour mettre une variable
    }

}

void moveGlass(Direction direction, int *x, int *y, int *distanceMax)
{
    //permet de controler le choix de la vitesse

    switch(direction)
    {
        case HAUT:
            if (*y - GLASS_SPEED < *distanceMax)
            {
                *y = *distanceMax;
            }
            else
            {
                *y -= GLASS_SPEED;
            }
        break;

        case BAS:
            if (*y + GLASS_SPEED > *distanceMax)
            {
                *y = *distanceMax;
            }
            else
            {
                *y += GLASS_SPEED;
            }
        break;

        case DROITE:
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

        case GAUCHE:
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
