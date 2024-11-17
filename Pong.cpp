#include "raylib.h"

Color Verde = Color{ 38, 185, 154, 255 };
Color VerdeScuro = Color{ 20, 160, 133, 255 };
Color VerdeChiaro = Color{ 129, 204, 184, 255 };
Color Giallo = Color{ 243, 213, 91, 255 };

struct Frasi 
{
    const char* titolo = "Pong";
    const char* quit = "Press q to go to menu";
    const char* restart = "Restart";
    const char* resume = "Resume";
    const char* start = "Start";
};

int punt_giocatore = 0, punt_computer = 0, tempo_coll = 0;
bool active = false;
float delta_t;
Frasi testo;

class Timer 
{

public:

    double durata = 0;
    double inizio_timer = 0.0;
    bool tAttivo = true;

    void attiva()
    {
        tAttivo = true;
        inizio_timer = GetTime();
    }
    void disattiva() 
    {
        tAttivo = false;
        inizio_timer = 0;
    }
    void Aggiorna()
    {
        if (tAttivo)
        {
            int tempo_corrente = GetTime();
            if (tempo_corrente - inizio_timer >= durata) disattiva();
        }
    }               
};
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

        if (y + raggio >= GetScreenHeight()  - 25 || y - raggio <= 25)
        {
            velY *= -1;
        }

        x += velX * delta_t;
        y += velY * delta_t;
    }
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
        if (val == 'u')     // up per non andare oltre il bordo sopra
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
        else if (val == 'd')    // down per non andare oltre il bordo sotto
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
        else if (val == 'c')    // c per non andare oltre il bordo sia sopra che sotto, per il bot
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
};
class PaddleComp : public Paddle
{
public:

    void Aggiorna(Palla& palla)
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
        Limite(coord);
    }
};

static void Init(Paddle& giocatore, Palla& palla, PaddleComp& computer, const int& WIDTH, const int& HEIGHT)
{
    palla.raggio = 20;
    palla.x = WIDTH / 2;
    palla.y = HEIGHT / 2;
    palla.velX = 450; // 7 con 60 fps
    palla.velY = 450;

    giocatore.largh = 25;
    giocatore.alt = 120;
    giocatore.x = WIDTH - giocatore.largh - 10;
    giocatore.y = HEIGHT / 2 - giocatore.alt / 2;
    giocatore.vel = 440;      // 6 con 60 fps

    computer.largh = 25;
    computer.alt = 120;
    computer.vel = 450;   // 6 con 60 fps
    computer.x = 10;
    computer.y = HEIGHT / 2 - computer.alt / 2;
}
static void DisegnaAll(Paddle& giocatore, Palla& palla, PaddleComp& computer)
{
    palla.Disegna();
    giocatore.Disegna();
    computer.Disegna();
}
static void UpdateAll(Paddle& giocatore, Palla& palla, PaddleComp& computer, Timer& timer)
{
    palla.Aggiorna();
    timer.Aggiorna();
    giocatore.Aggiorna();
    computer.Aggiorna(palla);
}
static void DisegnaSfondo(const int& WIDTH, const int& HEIGHT)
{
    ClearBackground(VerdeScuro);
    DrawRectangle(WIDTH / 2, 0, WIDTH / 2, HEIGHT, Verde);
    DrawCircle(WIDTH / 2, HEIGHT / 2, 130, VerdeChiaro);

    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, WHITE);

    DrawRectangle(0, 0, WIDTH, 25, Giallo);
    DrawRectangle(0, HEIGHT - 25, WIDTH, 25, Giallo);   // 10    + 15
}
static void menu(Palla& palla, Paddle& giocatore, Paddle& computer) {

    ClearBackground(VerdeScuro);

    int titoloWidth = MeasureText(testo.titolo, 80); // centriamo il testo ottenedo la sua grandezza
    int quitWidth = MeasureText(testo.quit, 20);
    int restartWidth = MeasureText(testo.restart, 40);
    int startWidth = MeasureText(testo.start, 40);
    int resumeWidth = MeasureText(testo.resume, 60);

    DrawText(testo.titolo, (1300 - titoloWidth) / 2, 80, 80, WHITE);
    int x = GetMousePosition().x;
    int y = GetMousePosition().y;

    if (punt_giocatore > 0 || punt_computer > 0)
    {   
        
        if ((x >= 510 && x <= 800) && (y >= 290 && y <= 380)) 
        {   
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, VerdeChiaro);
            DrawText(testo.resume, (1300 - resumeWidth) / 2, 305, 60, VerdeChiaro);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) active = true;
        }
        else {
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, WHITE);
            DrawText(testo.resume, (1300 - resumeWidth) / 2, 305, 60, WHITE);
        }
        
        // DrawRectangleLinesEx(Rectangle{ 510, 410, 290, 90 }, 5, WHITE);
        if ((x >= 510 && x <= 800) && (y >= 425 && y <= 515))
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 410, 290, 90 }, 0.1, 0, 5, VerdeChiaro);
            DrawText(testo.restart, (1300 - restartWidth) / 2 - 40, 425, 60, VerdeChiaro);  // rendiamo il testo chiaro
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                palla.ResetPalla();
                giocatore.y = 400 - giocatore.alt / 2;
                computer.y = 400 - giocatore.alt / 2;
                punt_computer = 0;
                punt_giocatore = 0;
                active = true;
            }        
        }
        else 
        {   
            DrawRectangleRoundedLines(Rectangle{ 510, 410, 290, 90 }, 0.1, 0, 5, WHITE);
            DrawText(testo.restart, (1300 - restartWidth) / 2 - 40, 425, 60, WHITE);
        }
    }
    else 
    {
        if ((x >= 510 && x <= 800) && (y >= 290 && y <= 380))
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, VerdeChiaro);
            DrawText(testo.start, (1300 - startWidth) / 2 - 25, 305, 60, VerdeChiaro);  // rendiamo il testo chiaro
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {           
                palla.ResetPalla();
                active = true;
            }
        }
        else
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, WHITE);
            DrawText(testo.start, (1300 - startWidth) / 2 - 25, 305, 60, WHITE);
        }
    }

    DrawText(testo.quit, (1300 - quitWidth) / 2, 770, 20, WHITE);       // q per uscire
}

int main()
{
    const int WIDTH = 1300;
    const int HEIGHT = 830;

    InitWindow(WIDTH, HEIGHT, "Pong");
    // SetTargetFPS(120);

    Paddle giocatore;
    PaddleComp computer;
    Palla palla;
    Timer timer;
    timer.durata = 1.0;

    Init(giocatore, palla, computer, WIDTH, HEIGHT);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (active) {

            delta_t = GetFrameTime();

            ClearBackground(BLACK);
            UpdateAll(giocatore, palla, computer, timer);

            if (!timer.tAttivo) 
            {
                if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ giocatore.x, giocatore.y, giocatore.largh, giocatore.alt }))
                {
                    palla.velX *= -1;
                    timer.attiva();
                }
                if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ computer.x, computer.y, computer.largh, computer.alt }))
                {
                    palla.velX *= -1;
                    timer.attiva();
                }
            }

            DisegnaSfondo(WIDTH, HEIGHT);
            DisegnaAll(giocatore, palla, computer);

            DrawText(TextFormat("FPS: %d", GetFPS()), 5, 5, 20, BLACK);
            DrawText(TextFormat("%i", punt_computer), (WIDTH / 4) - 20, 25, 80, WHITE);
            DrawText(TextFormat("%i", punt_giocatore), (WIDTH / 4) * 3, 25, 80, WHITE);

            if (IsKeyPressed(KEY_Q)) active = false;
        }
        else
        {
            menu(palla, giocatore, computer);
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}