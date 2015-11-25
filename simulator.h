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

class Link;
class PlaceTransition;
class Place;
class Transition;
class Token;
class Model;
class Calendar;

/**
 * Třída reprezentující simulátor Petriho sítě.
 */
class Simulator
{
	public:
		Simulator(); // konstruktor
		~Simulator(); // destruktor
		void createModel(); // vytvoření modelu
		Model *getModel(); // získání ukazatele na model
		void printModel(); // vytisknutí modelu
		Calendar *getCalendar(); // získání ukazatele na model
		
		void setSimTime(double simTime); // nastavení simulačního času
		void setMaxSimTime(double maxSimTime); // nastavení konce simulačního času
		
		// metoda pro volbu prioritního přechodu
		
		void simStart(); // zahájení simulace
		void performTransitions(); // vykoná nečasové přechody a naplánuje časované přechody
		// vykonání konkrétního přechodu
		// vykonání časovaného přechodu
		// naplánovat událost
		

	private:
		Model *model; // ukazatel na model
		Calendar *calendar; // ukazatel na kalednář záznamů o událostech
		double simTime; // aktuální hodnota simulačnho času
		double maxSimTime; // maximální hodnota simulačního času (čas. rámec)
};

#endif