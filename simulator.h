/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	simulator.h             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "model.h"
#include "link.h"
#include "calendar.h"
#include "token.h"
#include "place-transition.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Link;
class PlaceTransition;
class Place;
class Transition;
class Token;
class Model;
class Calendar;
class Event;

/**
 * Třída reprezentující simulátor Petriho sítě.
 */
class Simulator
{
	public:
		Simulator(); // konstruktor simulátoru
		~Simulator(); // destruktor simulátoru
		void setSimTime(double simTime); // nastaví simulační čas
		void setMaxSimTime(double maxSimTime); // nastaví konec simulačního času	
		void createModel(); // vytvoří model
		Model *getModel(); // získá ukazatel na model
		void printModel(); // vytiskne model
		void simStart(); // zahájí simulace
		Calendar *getCalendar(); // získá ukazatele na model
		// ??? metoda pro volbu prioritního přechodu
		// ???vykonání časovaného přechodu
		void performTransitionFromEvent(Event *event); // ???
		void performTransition(Transition *transition); // vykonání konkrétního přechodu
		void performTransitions(); // vykoná nečasové přechody a naplánuje časované přechody
		// ???naplánovat událost
		static void clearPerformedTransition(); // nastavení příznaku vykonání přechodů na false
		void planTransition(Transition *transition, double wait); // naplánování časovaného přechodu
		void planEvents(Transition *transition, double wait); // plánování událostí
		bool transitionCanBePerformed(Transition *transition); // ???	
		double Random(); // vygenerování náhodného čísla
		double Exponential(double mv); // vygenerování čísla s exponeniálním rozložením se střeem mv
		
		static unsigned ix; // seed pro generator
	private:
		Model *model; // ukazatel na model
		Calendar *calendar; // ukazatel na kalednář záznamů o událostech
		double simTime; // aktuální hodnota simulačnho času
		double maxSimTime; // maximální hodnota simulačního času (čas. rámec)
};

#endif