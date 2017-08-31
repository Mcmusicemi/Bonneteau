#ifndef DEF_STRUCT
#define DEF_STRUCT
 
#include "const.h"
 
typedef int bool;
#define true 1
#define false 0

typedef enum Direction Direction;
enum Direction {HAUT, BAS, DROITE, GAUCHE};

typedef enum Animation Animation;
enum Animation {NONE, ChangeGlass, REVELATION, ECHEC};

//Gestion de l'input du clavier
typedef struct Input
{
  
} Input;


typedef struct Glass
{
	int dest;

	bool goToTop;

	bool selected;

	SDL_Rect position;

} Glass;


typedef struct Game
{
	SDL_Texture *glassTexture;
	SDL_Texture *glassSelectedTexture;
	SDL_Texture *ballTexture;
	SDL_Texture *thumbTexture;

	SDL_Rect positionBall;
	int thumbY;

	bool firstTour;

	bool goToBottom;

	int nbrTourMax, tour;

	Animation animation;

	int glassReveal; 

} Game;

#endif