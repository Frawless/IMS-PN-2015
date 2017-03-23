/************************************************
*	 Projekt: 	Projekt do předmětu IMS		   * 
* 					Simulátor Petriho sítí	   *
*	Autoři:	Jakub Stejskal <xstejs24>		   *
*		   	Petr Staněk <xstane34>		   *
*	Nazev souboru: 	link.h		   *
*			Datum:  7. 12. 2015			   *
*			Verze:	1.0		                     *
************************************************/

/**
 * @file link.h
 * @brief Hlavičkový soubor obsahující třídu reprezentující hranu modelu.
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

#ifndef LINK_H
#define LINK_H

#include <vector>
#include <iostream>

#include "place-transition.h"

class PlaceTransition;
class Transition;
class Place;

/**
 * @class Link
 * @brief Třída reprezentující hranu modelu.
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