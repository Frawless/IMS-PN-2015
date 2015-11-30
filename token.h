/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	token.cpp			     	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <iostream>

using namespace std; 

class Place;
class Transition;

/**
 * Třída reprezentující token v místě
 */
class Token
{
	public:
		Token(Place *place); // konstruktor tokenu
		Place* getPlace(); // vrátí ukazatel na místo, v němž je token vložen
		void removeToken(Token *token); // odstraní token ze seznamu všech tokenů modelu
		static void printTokens(); // vypíše seznam všech tokenů
		void tokenProcessedByTransition(Transition *transition); // ???
		bool isTokenProcessedByTransition(Transition* transition); // ???
		void setFlag(bool isInEvent); // nastaví příznak tokenu značící, že je token naplánován v přechodu
		bool getFlag(); // získá příznak tokenu značící, že je token naplánován v přechodu
		std::vector<Transition *> *getListOfEvents(); // vrátí seznam všech událostí
		static std::vector <Token *>* getTokens(); // vrátí ukazatel na seznam všech tokenů modelu

	private:
		static std::vector <Token *> listOfTokens; // seznam všech tokenů z modelu
		Place * place; // ukazatel na místo v němž je token vložen
		std::vector<Transition *> listOfEvents; // seznam přechodů, ve kterých se nachází
		bool waitFlag; // příznak značící, že token čeká na přesun
};
#endif