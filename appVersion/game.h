#pragma once

#include <raylib.h>
#include "timer.h"
#include "palla.h"
#include "paddle.h"
#include "stelle.h"

class Game 
{
public:

    Game(const int& WIDTH, const int& HEIGHT);
    void Run();

    bool active;
    int punt_giocatore, punt_computer;
    float delta_t;

    int WIDTH;
    int HEIGHT;
  
    struct Frasi
    {
        const char* titolo = "Pong";
        const char* quit = "Press q to go to menu";
        const char* restart = "Restart";
        const char* resume = "Resume";
        const char* start = "Start";
    };

    Frasi testo;
    Paddle giocatore;
    PaddleComp computer;
    Palla palla;
    Timer timer_paddle = Timer(1.0);
    Timer timer_bordo = Timer(0.3);
    Timer timer_wait = Timer(0.3);

    void Init();
    void UpdateAll();
    void DisegnaAll();
    void Menu();
    void DisegnaSfondo() const;
};

