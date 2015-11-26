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
 * Třída reprezentující událost v kalendáři
 * @return 
 */
class Event
{
	public:
		Event(double time, double wait, Transition *transition); //konstruktor
		double getTime(); // získání času události
		double getWait(); // získání zpoždění události
		//přidání/odebrání tokenu dos eznamu tokenu nalezici udalosti ???
		bool isEmpty(); // je seznam tokenů prázdný?
		Transition *getTransition(); // vrátí ukazatel na přechod
		void addTokenToEvent(Token *token);
		
		

	private:
		double time; // čas události
		double wait; // ??? doba čekání
		Transition *trainsition; // ukazatel na přechod
		std::vector<Token *> listOfTokens; // pole tokenů události
	};

	/**
	 * Deklarace řazení seznamu událostí
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
		
		bool isEmpty(); //je kalendář prádzný?
		void addEvent(Event *event); //přidání události 
		Event* getEvent(); //funkce pro získání následující události
		//delete first
		void printCalendar(); //vytisknutí kalendáře		
		std::multiset<Event *, EventSort>* getEvents();
		
		void deleteEvent(Event *event);
	private:
		std::multiset<Event *, EventSort> listOfEvents; // multimnožina záznamů v kalendáři

};

#endif