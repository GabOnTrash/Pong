#pragma once

#include "palla.h"
#include "raylib.h"

class Paddle
{

public:

    Paddle();

    float x, y, alt, largh;
    int vel;
    char coord;

    virtual void Disegna();  // virtual ci server per sovrascrivere metodi nelle classi figlie
    void Aggiorna(float& delta_t);
 
protected:  // usiamo protected perchè esso permette di condividere le funzioni e le variabili con le classi figlie, ciò che private non fa

    void Limite(char& coord, float& delta_t);
};
class PaddleComp : public Paddle
{
public:

    void Aggiorna(Palla& palla, float& delta_t);
    void Disegna() override;
};