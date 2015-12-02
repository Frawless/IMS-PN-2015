/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor Petriho sítí		*
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
 * @file simulator.h
 * @brief Hlavičkový soubor obsahující třídu reprezentující simulátor Petriho sítě.
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

/**
 * @class Simulator
 * @brief Třída reprezentující simulátor Petriho sítě.
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
		void performTransitionFromEvent(Event *event); // vykoná přechod z události kalendáře
		void performTransition(Transition *transition); // vykoná přechod zadaný parametrem
		void performTransitions(); // vykoná nečasové přechody a naplánuje časované přechody
		static void clearPerformedTransition(); // nastavení příznaku vykonání přechodů na false
		void planTransition(Transition *transition, double wait); // naplánování časovaného přechodu
		void planEvents(Transition *transition, double wait); // plánování událostí
		bool transitionCanBePerformed(Transition *transition); // zjistí, zda může být přechod vykonán	
		double Random(); // vyeneruje náhodné číslo
		double Exponential(double mv); // vygeneruje číslo s exponeniálním rozložením se střeem mv
		
		// test
		void deleteEventByToken(Token *token);
	
	private:
		Model *model; // ukazatel na model
		Calendar *calendar; // ukazatel na kalednář záznamů o událostech
		double simTime; // aktuální hodnota simulačnho času
		double maxSimTime; // maximální hodnota simulačního času (čas. rámec)
};

unsigned time_seed(); // ratí seed pro inicializaci generátoru pseudo-náhodných čísel

#endif