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


class Link
{
	public:
		Link(std::string inputName, std::string outputName, int capacity); //konstruktor hrany
		static std::vector <Link *>* getLinks(); // získání ukazatele na pole hran
		PlaceTransition *getInput(); // získání místa/přechodu na vstupu hrany
		PlaceTransition *getOutput(); // získání místa/přechodu na výstupu hrany
		int getCapacity(); // získání kapacity


	private:
		static std::vector <Link *> listOfLinks; // pole hran
		PlaceTransition *input;	// ukazatel na místo/přechod na vstupu hrany
		PlaceTransition *output; // ukazatel na místo/přechod na výstupu hrany
		int capacity; // kapacita hrany
	};

#endif