/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	token	.cpp             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/


#include "token.h"
#include "place-transition.h"


std::vector <Token *> Token::listOfTokens;

/**
 * Konstruktor tokenu pro zadané místo
 */
Token::Token(Place * place)
{
	this->place = place;
	listOfTokens.push_back(this);
	this->waitFlag = false;
}

/**
 * Získání ukazatele na pole tokenů
 * @return ukazatel na pole tokenů
 */
std::vector <Token *>* Token::getTokens()
{
	return &listOfTokens;
}

/**
 * Získní ukazatele na místo
 * @return ukazatel na místo
 */
Place* Token::getPlace()
{
	return this->place;
}

/**
 *  Pro smazání při opuštění systému
 * @param token
 */
void Token::removeToken(Token *token)
{
	std::vector <Token *>::iterator it;		//iterátor pro průchod polem tokenů
	//prohledání pole tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		if(*it == token)
		{
			delete(token);			//smazání objektu tokenu
			listOfTokens.erase(it);		//smazání ze seznamu
		}
		return;
	}
}

/**
 * 
 */
void Token::printTokens()
{
	std::vector <Token *>::iterator it;		//iterátor pro průchod polem tokenů
	//prohledání pole tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		std::cerr<<"Token: "<<*it<<std::endl;

	}
}

/**
 * 
 * @param transition
 * @return 
 */
bool Token::isTokenProcessedByTransition(Transition* transition)
{
	std::vector<Transition*>::iterator iterToken;
	for(iterToken = listOfEvents.begin(); iterToken != listOfEvents.end(); iterToken++)
	{
		if(transition ==(*iterToken))
			return true;
	}
	return false;
}


/**
 * 
 * @param transition
 */
void Token::tokenProcessedByTransition(Transition *transition)
{
	listOfEvents.push_back(transition);
}

/**
 * 
 * @param isInEvent
 */
void Token::setFlag(bool isInEvent)
{
	this->waitFlag = isInEvent;
}