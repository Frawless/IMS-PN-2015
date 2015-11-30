/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	model.h						*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#ifndef MODEL_H
#define MODEL_H

#include "place-transition.h"
#include "token.h"

using namespace std;

class Link;
class PlaceTransition;
class Place;
class Transition;
class Token;

/**
 * Třída reprezentující model Petriho sítě, který bude simulován.
 */
class Model
{
	public:
		Model(); // konstruktor	modelu
		~Model(); // destruktor modelu
		void addPlace(std::string name); // přidá místo do modelu
		void addPlace(std::string name, int capacity); // přidá místo do modelu
		void addTransition(std::string name); // přidá přechod do modelu
		void addTransition(std::string name, int value, Transition::Type type); // přidá přechod do modelu
		void addLink(std::string inputName, std::string outputName, int capacity); // přidá hranu do modelu
		void addToken(std::string placeName); // přidá token do místa 
		void addToken(std::string placeName, int count); // přidá zadaný počet tokenů do místa 
		static void modelValidate(); // ověří syntaktickou správnou modelu
		void printTokenCount(); // vytiskne počty tokenů v jednotlivých místech modelu
		void printModel(); // vytiskne přehled celého modelu
		void printStats();
		
	private:
};

#endif