#include "stelle.h"

#define NUM_STELLE 50

Stella stelle[NUM_STELLE];

void InizializzaStelle(const int& WIDTH, const int& HEIGHT)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione = { (float)GetRandomValue(0, WIDTH), (float)GetRandomValue(0, HEIGHT) };
        stelle[i].velocita.x = (float)GetRandomValue(50, 100) / 10;
        stelle[i].velocita.y = -(float)GetRandomValue(50, 100) / 10;
    }
}
void AggiornaStelle(const int& WIDTH, const int& HEIGHT, float delta_t)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione.x += stelle[i].velocita.x * delta_t;
        stelle[i].posizione.y += stelle[i].velocita.y * delta_t ;

        if (stelle[i].posizione.x > WIDTH + 10)
            stelle[i].posizione.x = (float) GetRandomValue(-50, 0);

        if (stelle[i].posizione.y < -10)
            stelle[i].posizione.y = (float) GetRandomValue(HEIGHT, HEIGHT + 50);
    }
}
void DisegnaStelle()
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        DrawCircleV(stelle[i].posizione, 3, WHITE);
    }
}