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
		double getTime(); // získání času události

	private:
		double time; // čas události
		double wait; // ???
		Transition *trainsition; // ukazatel na přechod
		std::vector<Token *> listOfTokens; // pole tokenů
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

	private:
		std::multiset<Event *, EventSort> listOfEvents; // multimnožina záznamů v kalendáři

};

#endif