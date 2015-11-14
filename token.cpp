/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	token.cpp	             	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/


#include "token.h"

std::vector <Token *> Token::listOfTokens;

/**
 * 
 */
Token::Token(Place * place)
{
	this->placePtr = place;
}

std::vector <Token *> Token::getTokens()
{
	return listOfTokens;
}

Place* Token::getPlace()
{
	return this->placePtr;
}