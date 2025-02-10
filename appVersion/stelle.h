#pragma once

#include <raylib.h>

struct Stella
{
    Vector2 posizione;
    Vector2 velocita;
};

void InizializzaStelle(const int& WIDTH, const int& HEIGHT);
void AggiornaStelle(const int& WIDTH, const int& HEIGHT, float delta_t);
void DisegnaStelle();