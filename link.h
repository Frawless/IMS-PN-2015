/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	link.h				     	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#ifndef LINK_H
#define LINK_H

#include <vector>
#include <iostream>

#include "place-transition.h"

class PlaceTransition;
class Transition;
class Place;

/**
 * Třída reprezentující hranu modelu.
 */
class Link
{
	public:
		Link(std::string inputName, std::string outputName, int capacity); // konstruktor hrany
		PlaceTransition *getInput(); // vrátí ukazatel na místo/přechod na vstupu hrany
		PlaceTransition *getOutput(); // vrátí ukazatel na místo/přechod na výstupu hrany
		int getCapacity(); // vrátí kapacitu hrany
		static std::vector <Link *>* getLinks(); // vrátí ukazatel na seznam všech hran modelu

	private:
		
		PlaceTransition *input;	// ukazatel na místo/přechod na vstupu hrany
		PlaceTransition *output; // ukazatel na místo/přechod na výstupu hrany
		int capacity; // kapacita hrany
		static std::vector <Link *> listOfLinks; // seznam všech hran modelu
	};

#endif