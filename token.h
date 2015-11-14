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

class Place;

/**
 * Třída reprezentující token v místě
 */
class Token
{
	public:
		Token(Place *place); //konstruktor tokenu
		static std::vector <Token *>* getTokens(); // získání ukazatele na pole tokenů
		Place* getPlace(); // získání ukazatele na místo v němž je token vložen

	private:
		static std::vector <Token *> listOfTokens; // pole tokenů
		Place * place; // ukazatel na místo v němž je token vložen
};
#endif