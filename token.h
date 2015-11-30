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
		Token(Place *place); //konstruktor tokenu
		Place* getPlace(); // získání ukazatele na místo v němž je token vložen
		void removeToken(Token *token); //odstranění tokenu ze seznamu všech tokenů modelu
		static void printTokens(); //vypsání všech tokenů
		void tokenProcessedByTransition(Transition *transition); // ???
		bool isTokenProcessedByTransition(Transition* transition); // ???
		void setFlag(bool isInEvent); //???	
		bool getFlag(); // ???
		std::vector<Transition *> *getListOfEvents(); // získání seznamu všech událostí
		static std::vector <Token *>* getTokens(); // získání ukazatele na seznam všech tokenů modelu

	private:
		static std::vector <Token *> listOfTokens; // seznam všech tokenů z modelu
		Place * place; // ukazatel na místo v němž je token vložen
		std::vector<Transition *> listOfEvents; // seznam přechodů, ve kterých se nachází
		bool waitFlag; // ???
};
#endif