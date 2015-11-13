/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	place-transition.cpp	    *
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/


#include "place-transition.h"

/**
 * 
 */
Place::Place(std::string name)
{
	//this->type = PLACE;
	this->name = name;
	this->capacity = 0;
	this->min = 0;
	this->max = 0;
}

/**
 * 
 */
Place::Place(std::string name, int capacity)
{
	//this->type = PLACE;
	this->name = name;
	this->capacity = capacity;
	this->min = 0;
	this->max = 0;
}

Transition::Transition(std::string name)
{
	Transition(name, 0, PRIORITY);
}

Transition::Transition(std::string name, int value, Transition::Type type)
{
	this->name = name;
	this->value = value;
	this->type = type;
}

/*std::map<std::string, Transition *> Transition::getTransitions()
{
	return listOfTransitions;
}*/

void PlaceTransition::addInputLink(Link *link)
{
	this->inputLinks.push_back(link);
}

void PlaceTransition::addOutputLink(Link *link)
{
	this->outputLinks.push_back(link);  
}

void Place::addToken(Token* token)
{
	this->listOfTokens.push_back(token);
}

std::string PlaceTransition::getName()
{
	return this->name;
}

