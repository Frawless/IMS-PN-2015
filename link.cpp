/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	link.cpp 	             	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/


#include "link.h"

std::vector <Link *> Link::listOfLinks;


/**
 * Konstruktor
 * @param
 * @param
 * @param
 */
Link::Link(PlaceTransition *input, PlaceTransition *output, int capacity)
{
	this->input = input;
	this->output = output;
	this->capacity = capacity;
	listOfLinks.push_back(this);
}

std::vector <Link *> Link::getLinks()
{
	return listOfLinks;
}

PlaceTransition* Link::getInput()
{
	return this->input;
}


PlaceTransition * Link::getOutput()
{
	return this->output;
}

/**
 * 
 */
int Link::getCapacity()
{
	return this->capacity;
}