/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	calendar.cpp             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/


#include "calendar.h"
#include "place-transition.h"

/**
 * Konstruktor kalendáře událostí
 */
Calendar::Calendar()
{
}

/**
 * Destruktor kalendáře událostí
 */
Calendar::~Calendar()
{
}

/**
 * 
 * @return 
 */
bool Calendar::isEmpty()
{
	return this->listOfEvents.empty();
}

/**
 * 
 * @param 
 */
void Calendar::addEvent(Event *event)
{
	this->listOfEvents.insert(event);
}

/**
 * 
 * @return 
 */
Event *Calendar::getEvent()
{
	return (*this->listOfEvents.begin());
}

std::multiset<Event *, EventSort>* Calendar::getEvents()
{
	return &listOfEvents;
}

/**
 * 
 */
void Calendar::printCalendar()
{
	std::multiset<Event *>::iterator event;
	
	std::multiset<Event *, EventSort>*  listOfEvents = this->getEvents();
	for(event = listOfEvents->begin(); event != listOfEvents->end(); event++)
	{
		std::cerr<<(*event)->getTransition()->getName()<<"T: "<<(*event)->getTime()<<"W: "<<(*event)->getWait()<<std::endl;
	}
}


/**
 * 
 * @param time
 * @param waint
 * @param transition
 */
Event::Event(double time, double wait, Transition *transition)
{
	this->time = time;
	this->wait = wait;
	this->trainsition = transition;
}

/**
 * Získání času události
 * @return čas události
 */
double Event::getTime()
{
	return this->time;
}

/**
 * 
 * @return 
 */
double Event::getWait()
{
	return this->wait;
}

/**
 * 
 * @return 
 */
bool Event::isEmpty()
{
	return this->listOfTokens.empty();
}

/**
 * 
 * @return 
 */
Transition *Event::getTransition()
{
	return this->trainsition;
}

/**
 * 
 * @param token
 */
void Event::addTokenToEvent(Token* token)
{
	// ??? možná něco ke statistikám
	listOfTokens.push_back(token);
}