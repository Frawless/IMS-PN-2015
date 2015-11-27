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
 * Konstruktor hrany mezi místem/přechodem a přechodem/místem.
 * @param input místo/přechod na vstupu hrany
 * @param output místo/přechod na výstupu hrany
 * @param capacity kapacita přechodu
 */
Link::Link(std::string inputName, std::string outputName, int capacity)
{
	Transition* t; // ukazatel na přechod
	Place* p; // ukazatel na místo
	
	// pokud je hrana mezi dvěma místy
	if (Place::getPlace(inputName) != NULL && Place::getPlace(outputName) != NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 místy!"<<std::endl;
		throw 1;
	}
	// pokud je hrana mezi dvěma přechody
	else if (Transition::getTransition(inputName) != NULL && Transition::getTransition(outputName) != NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 přechody!"<<std::endl;
		throw 1;
	}	
	// pokud je hrana nezačíná ani nekončí místem
	else if (Place::getPlace(inputName) == NULL &&  Place::getPlace(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi přechodem a neexistujícím místem!"<<std::endl;
		throw 1;
	}
	// pokud je hrana nezačíná ani nekončí přechodem
	else if (Transition::getTransition(inputName) == NULL && Transition::getTransition(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi místem a neexistujícím přechodem!"<<std::endl;
		throw 1;
	}
	// pokud pokud je na vstupu hrany přechod a na výstupu místo
	else if (((t = Transition::getTransition(inputName)) != NULL) && ((p = Place::getPlace(outputName)) != NULL))
	{
		this->input = t;
		this->output = p;
		
		p->addInputLink(this); // vložení hrany k místu
		t->addOutputLink(this); // vložení hrany k přechodu
		
	}
	// pokud pokud je na vstupu hrany místo a na výstupu přechod
	else if (((p = Place::getPlace(inputName)) != NULL) && ((t = Transition::getTransition(outputName)) != NULL))
	{
		this->input = p;
		this->output = t;
		
		t->addInputLink(this); // vložení hrany k přechodu
		p->addOutputLink(this); // vložení hrany k místu
	}
	
	this->capacity = capacity;
	
	// přidání hrany do seznamu hran
	listOfLinks.push_back(this);
}

/**
 * Vrátí ukazatel na místo/přechod na vstupu hrany
 * @return ukazatel na místo/přechod na vstupu hrany
 */
PlaceTransition* Link::getInput()
{
	return this->input;
}

/**
 * Vrátí místo/přechod na výstupu hrany
 * @return ukazatel na místo/přechod na výstupu hrany
 */
PlaceTransition * Link::getOutput()
{
	return this->output;
}

/**
 * Vratí kapacitu hrany
 */
int Link::getCapacity()
{
	return this->capacity;
}

/**
 * Vrátí ukazatel na seznam všech hran modelu
 * @return ukazatel na seznam hran
 */
std::vector <Link *>* Link::getLinks()
{
	return &listOfLinks;
}