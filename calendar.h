/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	calendar.h	             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#ifndef CALENDAR_H
#define CALENDAR_H

#include <set>
#include <vector>

class Token;
class Transition;

/**
 * Třída reprezentující záznam o události v kalendáři
 * @return 
 */
class Event
{
	public:
		Event(double time, double wait, Transition *transition); // konstruktor události
		double getTime(); // vratí aktivační čas události
		double getWait(); // vrátí čas provádění prechodu
		Transition *getTransition(); // vrátí ukazatel na přechod učástnící se události
		void addTokenToEvent(Token *token); // přidá token seznamu tokenů události
		bool isListOfTokensEmpty(); // ověří, zda je seznam tokenů události prázdný

	private:
		double time; // aktivační čas události
		double wait; // doba provádění přechodu
		Transition *trainsition; // ukazatel na přechod
		std::vector<Token *> listOfTokens; // seznam tokenů události
	};

	/**
	 * Deklarace řazení seznamu kaládáře záznamů o událostech
     */
	struct EventSort
	{
		bool operator()(Event *x, Event *y)
		{
			//return x < y -> vzít event time
			return x->getTime() < y->getTime();
		}
	};
	
/**
 * Třída reprezentující kalendář záznamů o událostech.
 */
class Calendar
{
	public:
		Calendar(); // konstruktor kalendáře
		~Calendar(); // destruktor kalendáře
		bool isEmpty(); // ověří, zda je kalendář prázdný
		void addEvent(Event *event); // přídá událost do kalendáře
		void deleteEvent(Event *event); // smaže událost z kalendáře
		Event* getEvent(); // vrátí nejblíže naplánovanou událost z kalendáře
		std::multiset<Event *, EventSort>* getEvents(); // vrátí ukazatel na seznam záznamů o událostech
		void printCalendar(); // vytiskne kalendář
		
	
	private:
		std::multiset<Event *, EventSort> listOfEvents; // seznam záznamů o událostech v kalendáři

};

#endif