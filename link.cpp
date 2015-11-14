/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	link.cpp		         	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#include "link.h"

std::vector <Link *> Link::listOfLinks;

/**
 * Konstruktor hrany mezi místem/přechodem a místem/přechodem
 * @param input místo/přechod na vstupu hrany
 * @param output místo/přechod na výstupu hrany
 * @param capacity kapacita přechodu
 */
Link::Link(PlaceTransition *input, PlaceTransition *output, int capacity)
{
	this->input = input;
	this->output = output;
	this->capacity = capacity;
	listOfLinks.push_back(this);
}

/**
 * Získání ukazatele na pole hran
 * @return uazatel na pole hran
 */
std::vector <Link *>* Link::getLinks()
{
	return &listOfLinks;
}

/**
 * Získání místa/přechodu na vstupu hrany
 * @return ukazatel na místo/přechod na vstupu hrany
 */
PlaceTransition* Link::getInput()
{
	return this->input;
}

/**
 * Získání místa/přechodu na výstupu hrany
 * @return místo/přechod na výstupu hrany
 */
PlaceTransition * Link::getOutput()
{
	return this->output;
}

/**
 * Získání kapacity linky
 */
int Link::getCapacity()
{
	return this->capacity;
}