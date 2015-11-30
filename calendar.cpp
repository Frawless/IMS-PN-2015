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
 * Konstruktur záznamu o události kalendáře záznamů o událostech.
 * @param time čas, na který je událost naplánována
 * @param wait čas, po který se přechod provádí
 * @param transition ukazatel na přechod, který s událostí souvisí
 */
Event::Event(double time, double wait, Transition *transition)
{
	this->time = time;
	this->wait = wait; 
	this->trainsition = transition;
}

/**
 * Vrátí aktivační čas události
 * @return aktivační čas události
 */
double Event::getTime()
{
	return this->time;
}

/**
 * Vrátí čas provádění přechodu.
 * @return čas provádění přechodu
 */
double Event::getWait()
{
	return this->wait;
}

/**
 * Vrátí ukazatel na přechod, ke kterému se událost váže.
 * @return ukazatel na přechod
 */
Transition *Event::getTransition()
{
	return this->trainsition;
}

/**
 * Přídá token do seznamu tokenů účastnících se události.
 * @param token vkládaný token
 */
void Event::addTokenToEvent(Token* token)
{
	// ??? možná něco ke statistikám
	listOfTokens.push_back(token);
}

/**
 * Ověří, zda je seznam tokenů účástnících se události prázdný.
 * @return true - seznam tokenů je prázdný
 */
bool Event::isListOfTokensEmpty()
{
	return this->listOfTokens.empty();
}

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
 * Ověří, zda je kalendář záznamů o událostech prázdný.
 * @return true - pokud je kalendář prázdný
 */
bool Calendar::isEmpty()
{
	std::cerr<<"velikost kalendáře: "<<listOfEvents.size()<<std::endl;
	return this->listOfEvents.empty();
}

/**
 * Přidá událost zadanou parametrem do kalendáře záznamů o událostech.
 * @param event událost pro vložení
 */
void Calendar::addEvent(Event *event)
{
	this->listOfEvents.insert(event);
}

/**
 * Smaže událost zadanou parametrem z kalendáře událostí.
 * @param event mazaná událost
 */
void Calendar::deleteEvent(Event *event)
{
	listOfEvents.erase(event);
}

/**
 * Vrátí nejdříve naplánovaný záznam o události z kalendáře záznamů o událostech.
 * @return první událost z kalendáře událostí
 */
Event *Calendar::getEvent()
{
	return (*this->listOfEvents.begin());
}

/**
 * Vrátí ukazatel na seznam záznamů kalendáře záznamů o událostech.
 * @return ukazatel na seznam událostí kalendáře
 */
std::multiset<Event *, EventSort>* Calendar::getEvents()
{
	return &listOfEvents;
}

/**
 * Vytiskne postupně jednotlivé události kalendáře záznamů o událostech na strerr (debug).
 */
void Calendar::printCalendar()
{
	std::multiset<Event *>::iterator event; // iterátor pro průchod kalendářem
	
	// získání seznamu událostí
	std::multiset<Event *, EventSort>*  listOfEvents = this->getEvents();
	
	// postupné procházení seznamem událostí
	for(event = listOfEvents->begin(); event != listOfEvents->end(); event++)
		std::cerr<<"Výpis kalendáře: "<<(*event)->getTransition()->getName()<<" T: "<<(*event)->getTime()<<" W: "<<(*event)->getWait()<<std::endl;
}
