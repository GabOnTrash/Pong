#include <raylib.h>
#include <time.h>
#include <stdlib.h>

#define NUM_STELLE 50

Color Giallo = Color{ 243, 213, 91, 255 };
Color GrigioChiaro = Color{ 65, 65, 65, 255 };
Color GrigioScuro = Color{ 30, 30, 30, 255 };

struct Stella 
{
    Vector2 posizione;
    Vector2 velocita;
};
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
Stella stelle[NUM_STELLE];

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
            double tempo_corrente = GetTime();
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

    void Disegna() const
    {
        DrawCircle(x, y, raggio, Giallo);
    }
    void Aggiorna(Timer& timer_bordo, Timer& timer_wait)
    {
        if (timer_wait.tAttivo) // se il timer è attivo non chiama nessuna altra funzione per la palla, ma aspetta che esso diventi inattivo
        {
            return;
        }
        if (x + raggio >= GetScreenWidth())
        {
            punt_computer++;
            ResetPalla(timer_wait);
            timer_wait.attiva();
        }
        if (x - raggio <= 0)
        {
            punt_giocatore++;   // se la palla va oltre lo schermo aumenta il punteggio;
            ResetPalla(timer_wait);
            timer_wait.attiva();
        }
        
        if (y + raggio >= GetScreenHeight() - 25 || y - raggio <= 25)
        {
            if (!timer_bordo.tAttivo)
            {
                velY *= -1;
                timer_bordo.attiva();
            }
        }
        x += velX * delta_t;
        y += velY * delta_t;
    }
    void ResetPalla(Timer& timer_wait)
    {   
        x = GetScreenWidth() / static_cast<float>(2);
        y = GetScreenHeight() / static_cast<float>(2);

        if (!timer_wait.tAttivo)
        {
            velX *= scelta_vel[GetRandomValue(0, 1)];  // usiamo la funzione built in di Raylib pe ottenere l'indice della lista della velocità per ottenere
            velY *= scelta_vel[GetRandomValue(0, 1)];  // la direzione da dare alla palla, sia per la x che per la y, indipendentemente da chi vince il round
        }
    }
};
class Paddle
{

public:

    float x = 0, y = 0, alt = 0, largh = 0;
    int vel = 0;
    char coord = 'n';

    virtual void Disegna()  // virtual ci server per sovrascrivere metodi nelle classi figlie
    {
        DrawRectangleRounded(Rectangle{ x, y, largh, alt }, 0.8, 0, BLUE); // invece di creare il rettangolo squadrato lo creiamo con i bordi
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

    void Disegna() override
    {
        DrawRectangleRounded(Rectangle{ x, y, largh, alt }, 0.8, 0, RED);
    }
    void Aggiorna(Palla palla)
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

static void InizializzaStelle(const int& WIDTH, const int& HEIGHT)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione = { (float) GetRandomValue(0, WIDTH), (float) GetRandomValue(0, HEIGHT) };
        stelle[i].velocita.x = (float) GetRandomValue(50, 100) / 10;
        stelle[i].velocita.y = -(float) GetRandomValue(50, 100) / 10;
    }
}
static void AggiornaStelle(const int& WIDTH, const int& HEIGHT, float delta_t)
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        stelle[i].posizione.x += stelle[i].velocita.x * delta_t;
        stelle[i].posizione.y += stelle[i].velocita.y * delta_t;

        if (stelle[i].posizione.x > WIDTH + 10)
            stelle[i].posizione.x = (float) GetRandomValue(-50, 0);

        if (stelle[i].posizione.y < -10)
           stelle[i].posizione.y = (float) GetRandomValue(HEIGHT, HEIGHT + 50);
    }
}
static void DisegnaStelle()
{
    for (int i = 0; i < NUM_STELLE; i++)
    {
        DrawCircleV(stelle[i].posizione, 3, WHITE);
    }
}

static void Init(Paddle& giocatore, Palla& palla, PaddleComp& computer, const int& WIDTH, const int& HEIGHT)
{
    palla.raggio = 20;
    palla.x = WIDTH / static_cast<float>(2);
    palla.y = HEIGHT / static_cast<float>(2);
    palla.velX = 450;
    palla.velY = 452;

    giocatore.largh = 25;
    giocatore.alt = 120;
    giocatore.x = WIDTH - giocatore.largh - 10;
    giocatore.y = HEIGHT / static_cast<float>(2) - giocatore.alt / 2;
    giocatore.vel = 340;

    computer.largh = 25;
    computer.alt = 120;
    computer.vel = 396;
    computer.x = 10;
    computer.y = HEIGHT / static_cast<float>(2) - computer.alt / 2;
}
static void DisegnaAll(Paddle& giocatore, Palla& palla, PaddleComp& computer)
{
    palla.Disegna();
    giocatore.Disegna();
    computer.Disegna();
}
static void UpdateAll(Paddle& giocatore, Palla& palla, PaddleComp& computer, Timer& timer_paddle, Timer& timer_bordo, Timer& timer_wait)
{
    palla.Aggiorna(timer_bordo, timer_wait);
    timer_paddle.Aggiorna();
    timer_bordo.Aggiorna();
    timer_wait.Aggiorna();
    giocatore.Aggiorna();
    computer.Aggiorna(palla);
}
static void DisegnaSfondo(const int& WIDTH, const int& HEIGHT)
{
    ClearBackground(GrigioScuro);
    DrawRectangle(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GrigioChiaro);
    DrawCircle(WIDTH / 2, HEIGHT / 2, 130, GRAY);

    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, WHITE);

    DrawRectangle(0, 0, WIDTH, 25, Giallo);
    DrawRectangle(0, HEIGHT - 25, WIDTH, 25, Giallo);
}
static void menu(Palla& palla, Paddle& giocatore, Paddle& computer, Timer& timer_wait, float& delta_t) 
{
    ClearBackground(GrigioScuro);

    int titoloWidth = MeasureText(testo.titolo, 80);
    int quitWidth = MeasureText(testo.quit, 20);
    int restartWidth = MeasureText(testo.restart, 40);
    int startWidth = MeasureText(testo.start, 40);
    int resumeWidth = MeasureText(testo.resume, 60);
    
    int x = GetMousePosition().x;
    int y = GetMousePosition().y;

    if ((x >= 554 && x <= 554 + titoloWidth) && (y >= 88 && y <= 160))
    {   
        DrawText(testo.titolo, (1300 - titoloWidth) / 2, 80, 80, GrigioChiaro);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            OpenURL("https://www.google.com/search?q=Pong");
        }
    }
    else 
    {
        DrawText(testo.titolo, (1300 - titoloWidth) / 2, 80, 80, WHITE);    // DrawRectangle(554, 88, 192, 72, WHITE);
    }
    
    if (punt_giocatore > 0 || punt_computer > 0)
    {       
        if ((x >= 510 && x <= 800) && (y >= 290 && y <= 380)) 
        {   
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, GrigioChiaro);
            DrawText(testo.resume, (1300 - resumeWidth) / 2, 305, 60, GrigioChiaro);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) active = true;
        }
        else 
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, WHITE);
            DrawText(testo.resume, (1300 - resumeWidth) / 2, 305, 60, WHITE);
        }
        
        // DrawRectangleLinesEx(Rectangle{ 510, 410, 290, 90 }, 5, WHITE);
        if ((x >= 510 && x <= 800) && (y >= 425 && y <= 515))
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 410, 290, 90 }, 0.1, 0, 5, GrigioChiaro);
            DrawText(testo.restart, (1300 - restartWidth) / 2 - 40, 425, 60, GrigioChiaro);  // rendiamo il testo chiaro
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                timer_wait.attiva();
                palla.ResetPalla(timer_wait);
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
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, GrigioChiaro);
            DrawText(testo.start, (1300 - startWidth) / 2 - 25, 305, 60, GrigioChiaro);  // rendiamo il testo chiaro
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                palla.ResetPalla(timer_wait);
                timer_wait.attiva();
                active = true;
            }
        }
        else
        {
            DrawRectangleRoundedLines(Rectangle{ 510, 290, 290, 90 }, 0.1, 0, 5, WHITE);
            DrawText(testo.start, (1300 - startWidth) / 2 - 25, 305, 60, WHITE);
        }
    }

    DisegnaStelle();
    AggiornaStelle(1300, 830, delta_t);
    DrawText(testo.quit, (1300 - quitWidth) / 2, 770, 20, WHITE);       // q per uscire
}

int main()
{
    const int WIDTH = 1300;
    const int HEIGHT = 830;
    
    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Pong");

    Paddle giocatore;
    PaddleComp computer;
    Palla palla;
    Timer timer_paddle;
    Timer timer_bordo;
    Timer timer_wait;
    timer_paddle.durata = 1.0;
    timer_bordo.durata = 0.3;
    timer_wait.durata = 0.3;

    Init(giocatore, palla, computer, WIDTH, HEIGHT);
    InizializzaStelle(WIDTH, HEIGHT);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        delta_t = GetFrameTime();

        if (active) {

            ClearBackground(BLACK);
            UpdateAll(giocatore, palla, computer, timer_paddle, timer_bordo, timer_wait);

            if (!timer_paddle.tAttivo) 
            {
                if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ giocatore.x, giocatore.y, giocatore.largh, giocatore.alt }))
                {
                    palla.velX *= -1;
                    timer_paddle.attiva();
                }
                if (CheckCollisionCircleRec(Vector2{ palla.x, palla.y }, palla.raggio, Rectangle{ computer.x, computer.y, computer.largh, computer.alt }))
                {
                    palla.velX *= -1;
                    timer_paddle.attiva();
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
            menu(palla, giocatore, computer, timer_wait, delta_t);
        }
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
