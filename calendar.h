/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	calendar.h 	            	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef CALENDAR_H
#define CALENDAR_H

#include <set>
#include <vector>

class Token;
class Transition;


class Event
{
public:
	double getTime();
	
private:
	double time;
	double wait;
	Transition *trainsition;
	std::vector<Token *> listOfTokens;
};

struct EventSort
{
	bool operator()(Event *x, Event *y)
	{
		//return x < y -> vzít event time
		return x->getTime() < y->getTime();
	}
};

class Calendar
{
public:
	Calendar();
	~Calendar();
  
private:
	std::multiset<Event *, EventSort> listOfEvents;			//kalendářní multimnožina
  
};


#endif