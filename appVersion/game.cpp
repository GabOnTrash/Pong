#include "game.h"
#include "time.h"

Color Giallo = Color{ 243, 213, 91, 255 };
Color GrigioChiaro = Color{ 65, 65, 65, 255 };
Color GrigioScuro = Color{ 30, 30, 30, 255 };

Game::Game(const int& WIDTH, const int& HEIGHT)
    : WIDTH(WIDTH), HEIGHT(HEIGHT), active(false), punt_giocatore(0), punt_computer(0), delta_t(0)
{
}
void Game::UpdateAll()
{
    palla.Aggiorna(timer_bordo, timer_wait, punt_computer, punt_giocatore, delta_t);
    timer_paddle.Aggiorna();
    timer_bordo.Aggiorna();
    timer_wait.Aggiorna();
    giocatore.Aggiorna(delta_t);
    computer.Aggiorna(palla, delta_t);
}
void Game::DisegnaSfondo() const
{
    ClearBackground(GrigioScuro);
    DrawRectangle(WIDTH / 2, 0, WIDTH / 2, HEIGHT, GrigioChiaro);
    DrawCircle(WIDTH / 2, HEIGHT / 2, 130, GRAY);

    DrawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT, WHITE);

    DrawRectangle(0, 0, WIDTH, 25, Giallo);
    DrawRectangle(0, HEIGHT - 25, WIDTH, 25, Giallo);
}
void Game::DisegnaAll()
{
    palla.Disegna();
    giocatore.Disegna();
    computer.Disegna();
}
void Game::Init()
{
    giocatore.x = WIDTH - giocatore.largh - 10;
    giocatore.y = HEIGHT / static_cast<float>(2) - giocatore.alt / 2;
    giocatore.vel = 340;

    computer.vel = 396;
    computer.x = 10;
    computer.y = HEIGHT / static_cast<float>(2) - computer.alt / 2;

    timer_wait.attiva();
}
void Game::Menu()
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
                timer_wait.attiva();    // non si stoppa allo start :(
                palla.ResetPalla(timer_wait);
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
void Game::Run()
{
    Init();
    InizializzaStelle(WIDTH, HEIGHT);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        delta_t = GetFrameTime();

        if (active)
        {
            ClearBackground(BLACK);
            UpdateAll();

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

            DisegnaSfondo();
            DisegnaAll();

            DrawText(TextFormat("FPS: %d", GetFPS()), 5, 5, 20, BLACK);
            DrawText(TextFormat("%i", punt_computer), (WIDTH / 4) - 20, 25, 80, WHITE);
            DrawText(TextFormat("%i", punt_giocatore), (WIDTH / 4) * 3, 25, 80, WHITE);

            if (IsKeyPressed(KEY_Q)) active = false;
        }
        else
        {
            Menu();
        }

        EndDrawing();
    }

    CloseWindow();
}