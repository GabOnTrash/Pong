#include "raylib.h"

Color Verde = Color{ 38, 185, 154, 255 };
Color VerdeScuro = Color{ 20, 160, 133, 255 };
Color VerdeChiaro = Color{ 129, 204, 184, 255 };
Color Giallo = Color{ 243, 213, 91, 255 };

int punt_giocatore = 0;
int punt_computer = 0;

class Palla
{
public:

    float x = 0, y = 0;
    int velX = 0, velY = 0;
    int raggio = 20;
    int scelta_vel[2] = { -1, 1 };

    void Disegna()
    {
        DrawCircle(x, y, raggio, Giallo);
    }
    void Aggiorna()
    {

        if (x + raggio >= GetScreenWidth())
        {
            punt_computer++;
            ResetPalla();
        }
        if (x - raggio <= 0)
        {
            punt_giocatore++;   // se la palla va oltre lo schermo aumenta il punteggio;
            ResetPalla();
        }

        if (y + raggio >= GetScreenHeight() || y - raggio <= 0)
        {
            velY *= -1;
        }

        x += velX;
        y += velY;
    }

private:

    void ResetPalla()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        velX *= scelta_vel[GetRandomValue(0, 1)];  // usiamo la funzione built in di Raylib pe ottenere l'indice della lista della velocità per ottenere
        velY *= scelta_vel[GetRandomValue(0, 1)];  // la direzione da dare alla palla, sia per la x che per la y, indipendentemente da chi vince il round
    }
};
class Paddle
{
public:

    float x = 0, y = 0, alt = 0, largh = 0;
    int vel = 0;
    char coord = 'n';

    void Disegna()
    {
        DrawRectangleRounded(Rectangle{ x, y, largh, alt }, 0.8, 0, WHITE); // invece di creare il rettangolo squadrato lo creiamo con i bordi
    }
    void Aggiorna()
    {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            coord = 'u';
            Limite(coord);
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            coord = 'd';
            Limite(coord);
        }
    }

protected:  // usiamo protected perchè esso permette di condividere le funzioni e le variabili con le classi figlie, ciò che private non fa

    void Limite(char& val)
    {
        if (val == 'u')
        {
            if (y >= 10)
            {
                y -= vel;
            }
            else
            {
                y -= 0;
            }
        }
        else if (val == 'd')
        {
            if (y + alt <= GetScreenHeight() - 10)
            {
                y += vel;
            }
            else
            {
                y += 0;
            }
        }
        else if (val == 'c')
        {
            if (y >= 10)
            {
                y -= vel;
            }
            else
            {
                y -= 0;
            }

            if (y + alt <= GetScreenHeight() - 10)
            {
                y += vel;
            }
            else
            {
                y += 0;
            }
        }
    }
};
class PaddleComp : public Paddle
{
public:

    void Aggiorna(Palla& palla)
    {

        if (y + alt / 2 > palla.y)
        {
            y -= vel;
        }
        else if (y + alt / 2 < palla.y)
        {
            y += vel;
        }

        coord = 'c';
        Limite(coord);
    }
};

void Init(Paddle& giocatore, Palla& palla, PaddleComp& computer, const int& WIDTH, const int& HEIGHT)
{

    palla.raggio = 20;
    palla.x = WIDTH / 2;
    palla.y = HEIGHT / 2;
    palla.velX = 7;
    palla.velY = 7;

    giocatore.largh = 25;
    giocatore.alt = 120;
    giocatore.x = WIDTH - giocatore.largh - 10;
    giocatore.y = HEIGHT / 2 - giocatore.alt / 2;
    giocatore.vel = 6;

    computer.largh = 25;
    computer.alt = 120;
    computer.vel = 6;
    computer.x = 10;
    computer.y = HEIGHT / 2 - computer.alt / 2;


}
void DisegnaAll(Paddle& giocatore, Palla& palla, PaddleComp& computer)
{
    palla.Disegna();
    giocatore.Disegna();
    computer.Disegna();
}
void UpdateAll(Paddle& giocatore, Palla& palla, PaddleComp& computer)
{
    palla.Aggiorna();
    giocatore.Aggiorna();
    computer.Aggiorna(palla);
}
void DisegnaSfondo(const int& WIDTH, const int& HEIGHT)
{
    ClearBackground(VerdeScuro);
    DrawRectangle(WIDTH / 2, 0, WIDTH / 2, HEIGHT, Verde);
    DrawCircle(WIDTH / 2, HEIGHT / 2, 130, VerdeChiaro);

    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, WHITE);

    DrawRectangle(0, 0, WIDTH, 10, Giallo);
    DrawRectangle(0, HEIGHT - 10, WIDTH, 10, Giallo);
}

int main()
{
    const int WIDTH = 1300;
    const int HEIGHT = 800;

    InitWindow(WIDTH, HEIGHT, "Pong");
    SetTargetFPS(60);

    Paddle giocatore;
    PaddleComp computer;
    Palla palla;

    Init(giocatore, palla, computer, WIDTH, HEIGHT);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (IsCursorOnScreen()) HideCursor();

        UpdateAll(giocatore, palla, computer);

        if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ giocatore.x, giocatore.y, giocatore.largh, giocatore.alt }))
            palla.velX *= -1;
        if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ computer.x, computer.y, computer.largh, computer.alt }))
            palla.velX *= -1;

        DisegnaSfondo(WIDTH, HEIGHT);
        DisegnaAll(giocatore, palla, computer);

        DrawText(TextFormat("%i", punt_computer), (WIDTH / 4) - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", punt_giocatore), (WIDTH / 4) * 3, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}