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