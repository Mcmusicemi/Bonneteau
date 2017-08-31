#ifndef DEF_PROTOTYPES
#define DEF_PROTOTYPES

#include "struct.h"

extern void cleanup(void);
extern void delay(unsigned int frameLimit);
extern void drawGame(void);
extern void drawImage(SDL_Texture *, int, int);
extern void getInput(Input *input);
extern SDL_Renderer *getrenderer(void);
extern void init(char *);
extern void loadGame(void);
extern SDL_Texture *loadImage(char *name);
extern void updateGame();
extern void drawBackground();
extern void drawGlass();
extern void changeGlass();
extern void moveGlass(Direction direction, int *x, int *y, int *distanceMax);
extern void generate2NumbersRandom(int *a, int *b);
extern int randint(int a, int b);
extern void updateGlass();
extern void enableAnimation();
extern void pastRound();
extern bool testBelonging(int mouseX, int mouseY, SDL_Rect box);
extern void etablishedReveal(int glass);
extern void drawBall();
extern void drawThumb();

#endif