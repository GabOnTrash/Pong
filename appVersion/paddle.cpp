#include "paddle.h"

Paddle::Paddle()
    : alt(120), largh(25), vel(0), x(0), y(0), coord('n')
{
}
void Paddle::Disegna()
{
    DrawRectangleRounded(Rectangle{ x, y, largh, alt }, 0.8, 0, BLUE);
}
void Paddle::Aggiorna(float& delta_t)
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        coord = 'u';
        Limite(coord, delta_t);
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        coord = 'd';
        Limite(coord, delta_t);
    }
}
void Paddle::Limite(char& coord, float& delta_t)
{
    if (coord == 'u')     // up per non andare oltre il bordo sopra
    {
        if (y >= 25)
        {
            y -= vel * delta_t;
        }
        else
        {
            y -= 0;
        }
    }
    else if (coord == 'd')    // down per non andare oltre il bordo sotto
    {
        if (y + alt <= GetScreenHeight() - 25)
        {
            y += vel * delta_t;
        }
        else
        {
            y += 0;
        }
    }
    else if (coord == 'c')    // c per non andare oltre il bordo sia sopra che sotto, per il bot
    {
        if (y >= 25)
        {
            y -= vel * delta_t;
        }
        else
        {
            y -= 0;
        }

        if (y + alt <= GetScreenHeight() - 25)
        {
            y += vel * delta_t;
        }
        else
        {
            y += 0;
        }
    }
}

void PaddleComp::Aggiorna(Palla& palla, float& delta_t)
{
    if (y + alt / 2 > palla.y)
    {
        y -= vel * delta_t;
    }
    else if (y + alt / 2 < palla.y)
    {
        y += vel * delta_t;
    }

    coord = 'c';
    Limite(coord, delta_t);

}
void PaddleComp::Disegna()
{
    DrawRectangleRounded(Rectangle{ x, y, largh, alt }, 0.8, 0, RED);
}