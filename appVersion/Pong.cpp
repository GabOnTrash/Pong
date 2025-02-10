#include "game.h"
#include <time.h>
#include <stdlib.h>

int main()
{
    const int WIDTH = 1300;
    const int HEIGHT = 830;

    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Pong");

    Game gioco = Game(WIDTH, HEIGHT);
    gioco.Run();

    return 0;
}