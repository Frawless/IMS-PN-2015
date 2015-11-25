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

#include <typeinfo>

std::vector <Link *> Link::listOfLinks;

/**
 * Konstruktor hrany mezi místem/přechodem a místem/přechodem
 * @param input místo/přechod na vstupu hrany
 * @param output místo/přechod na výstupu hrany
 * @param capacity kapacita přechodu
 */
Link::Link(std::string inputName, std::string outputName, int capacity)
{
	Transition* t; // ukazatel na přechod
	Place* p; // ukazatel na místo
	
	// pokud je hrana mezi přechodem a neexistujícím místem
	if (Place::getPlace(inputName) != NULL && Place::getPlace(outputName) != NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 místy!"<<std::endl;
		throw 1;
	}
	else if (Transition::getTransition(inputName) != NULL && Transition::getTransition(outputName) != NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 přechody!"<<std::endl;
		throw 1;
	}	
	else if (Place::getPlace(inputName) == NULL &&  Place::getPlace(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi přechodem a neexistujícím místem!"<<std::endl;
		throw 1;
	}
	else if (Transition::getTransition(inputName) == NULL && Transition::getTransition(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi místem a neexistujícím přechodem!"<<std::endl;
		throw 1;
	}	
	else if (((t = Transition::getTransition(inputName)) != NULL) && ((p = Place::getPlace(outputName)) != NULL))
	{
		this->input = t;
		this->output = p;
		
		p->addInputLink(this); // vložení hrany k místu
		t->addOutputLink(this); // vložení hrany k přechodu // je potřeba ???
		
	}
	// pokud pokud je na vstupu hrany místo a na výstupu přechod
	else if (((p = Place::getPlace(inputName)) != NULL) && ((t = Transition::getTransition(outputName)) != NULL))
	{
		this->input = p;
		this->output = t;
		
		t->addInputLink(this); // vložení hrany k přechodu // je potřeba ???
		p->addOutputLink(this); // vložení hrany k místu
	}
	
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
/*
Place* Link::getInput2()
{
	if(typeid(Place*) == this->input)
		return (Place*)this->input;
	else
		return NULL;
}*/

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