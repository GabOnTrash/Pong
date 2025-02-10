#pragma once

#include "raylib.h"
#include "timer.h"

class Palla
{
public:
	
	Palla();

	float x, y;
	int velX, velY, raggio;
	int scelta_vel[2];

	void Aggiorna(Timer& timer_wait, Timer& timer_bordo, int& punt_computer, int& punt_giocatore, float& delta_t);
	void Disegna() const;
	void ResetPalla(Timer& timer_wait);

};