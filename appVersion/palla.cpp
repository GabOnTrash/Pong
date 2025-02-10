#include "palla.h"

    Palla::Palla()
        : x(1300 / static_cast<float>(2)), y(830 / static_cast<float>(2)), velX(450), velY(452), raggio(20), scelta_vel{ -1, 1 }
    {
    }
    void Palla::Disegna() const
    {
        DrawCircle(x, y, raggio, { 243, 213, 91, 255 });
    }
    void Palla::Aggiorna(Timer& timer_wait, Timer& timer_bordo, int& punt_computer, int& punt_giocatore, float& delta_t)
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
    void Palla::ResetPalla(Timer& timer_wait)
    {
        x = GetScreenWidth() / static_cast<float>(2);
        y = GetScreenHeight() / static_cast<float>(2);

        if (!timer_wait.tAttivo)
        {
            velX *= scelta_vel[GetRandomValue(0, 1)];  // usiamo la funzione built in di Raylib pe ottenere l'indice della lista della velocità per ottenere
            velY *= scelta_vel[GetRandomValue(0, 1)];  // la direzione da dare alla palla, sia per la x che per la y, indipendentemente da chi vince il round
        }
    }