#include "timer.h"
#include "raylib.h"

Timer::Timer(float durata)
	: inizio_timer(0.0), tAttivo(true)
{
    this->durata = durata;
}
void Timer::attiva()
{
    tAttivo = true;
    inizio_timer = GetTime();
}
void Timer::disattiva()
{
    tAttivo = false;
    inizio_timer = 0;
}
void Timer::Aggiorna()
{
    if (tAttivo)
    {
        double tempo_corrente = GetTime();
        if (tempo_corrente - inizio_timer >= durata) disattiva();
    }
}
