/************************************************
*	 Projekt: 	Projekt do předmětu IMS		   * 
* 					Simulátor Petriho sítí	   *
*	Autoři:	Jakub Stejskal <xstejs24>		   *
*		   	Petr Staněk <xstane34>		   *
*	Nazev souboru: 	token.h				   *
*			Datum:  7. 12. 2015			   *
*			Verze:	1.0		                     *
************************************************/

/**
 * @file token.h
 * @brief Hlavičkový soubor obsahující třídu reprezentující token v místě
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <iostream>

using namespace std; 

class Place;
class Transition;

/**
 * @class Token
 * @brief Třída reprezentující token v místě
 */
class Token
{
	public:
		Token(Place *place); // konstruktor tokenu
		Place* getPlace(); // vrátí ukazatel na místo, v němž je token vložen
		static void printTokens(); // vypíše seznam všech tokenů
		void setFlag(bool isInEvent); // nastaví příznak tokenu značící, že je token naplánován v přechodu
		bool getFlag(); // získá příznak tokenu značící, že je token naplánován v přechodu
		static std::vector <Token *>* getTokens(); // vrátí ukazatel na seznam všech tokenů modelu
		void deleteTokenFromList(Token *token); // maže zadaný token ze seznamu všech tokenů
		void tokenProcessedByTransition(Transition *transition); // spojí token s přechodem, ve kterém je načasován
		bool isTokenProcessedByTransition(); // zjistí, zda je token spojen s nějakým přechodem pro provedení
		Transition* getTransition(); // vrátí ukazatel na přechod, ve kterém je token zpracováván
		
		// test
		void printTransitions();

	private:
		static std::vector <Token *> listOfTokens; // seznam všech tokenů z modelu
		Place * place; // ukazatel na místo, v němž je token vložen
		bool waitFlag; // příznak značící, že token čeká na přesun
		Transition *transition;
};

#endif

