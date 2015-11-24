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
 * Třída reprezentující model Petriho sítě.
 */
class Model
{
	public:
		Model(); // konstruktor	
		~Model(); // destruktor
		void addPlace(std::string name); // přidání místa
		void addPlace(std::string name, int capacity); // přidání místa
		void addTransition(std::string name); // přidání přechodu
		void addTransition(std::string name, int value, Transition::Type type); // přidání přechodu
		void addLink(std::string inputName, std::string outputName, int capacity); // přidání hrany
		Token* addToken(std::string placeName); // přidání tokenu do místa 
		Token* addToken(std::string placeName, int count); // přidání tokenu do místa 
		void printModel(); // vytisknutí modelu
		void removeToken(Token *token); //odebrání značky z místa
		static void modelValidate(); //validace modelu
	private:
};

#endif