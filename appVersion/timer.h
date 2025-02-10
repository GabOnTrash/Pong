#pragma once

class Timer
{
private:

	double inizio_timer;

public:

	float durata;
	bool tAttivo;

	Timer(float durata);

	void attiva();
	void disattiva();
	void Aggiorna();

};