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
 * Získní ukazatele na místo
 * @return ukazatel na místo
 */
Place* Token::getPlace()
{
	return this->place;
}

/**
 * Pro smazání při opuštění systému
 * @param token ukazatel na tokne
 */
void Token::removeToken(Token *token)
{
	std::vector <Token *>::iterator it;	 // iterátor pro průchod seznamem tokenů
	// procházení seznamu tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		if(*it == token)
		{
			delete(token); // smazání objektu tokenu
			listOfTokens.erase(it);	 // smazání tokenu ze seznamu všech tokenů
			return;
		}	
	}
}

/**
 * Vytisknutí všech tokenů.
 */
void Token::printTokens()
{
	std::vector <Token *>::iterator it;	 // iterátor pro průchod polem tokenů
	
	// procházení seznamu tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		std::cerr<<"Token: "<<*it<<std::endl;

	}
}

/**
 * Vloží přechod do seznamu přechodů, které zpracovávají daný token.
 * @param transition přechod, který mý být vložen
 */
void Token::tokenProcessedByTransition(Transition *transition)
{
	listOfEvents.push_back(transition);
}

/**
 * Zjistí, zda přechod zadaný parametrem zpracovává daný token.
 * @param transition testovaný přechod
 * @return true - pokud přechod zpracovává token
 */
bool Token::isTokenProcessedByTransition(Transition* transition)
{
	std::vector<Transition*>::iterator iterToken; // iterátor pro token
	this->listOfEvents.clear(); // vymazání seznamu událostí
	
	// pokud je seznam událstí prázdný
	if(this->listOfEvents.size() ==0)
		return false;
	
	// posutupné procházení tokenů
	for(iterToken = listOfEvents.begin(); iterToken != listOfEvents.end(); iterToken++)
	{
		if(transition ==(*iterToken))
			return true;
	}
	return false;
}


/**
 * Nastaví hodnotu příznaku značícího, že je token zpracováván.
 * @param isInEvent nová hodnota příznaku
 */
void Token::setFlag(bool isInEvent)
{
	this->waitFlag = isInEvent;
}

/**
 * Vrátí hodnotu příznaku značícího, že je token zpracováván.
 * @return true - pokud je token zpracováván
 */

bool Token::getFlag()
{
	return this->waitFlag;
}

/**
 * Získání ukazatele na seznam všech přechodů modelu.
 * @return ukazatel na seznam všech událostí
 */
std::vector<Transition *> *Token::getListOfEvents()
{
	return &(this->listOfEvents);
}

/**
 * Získání ukazatele na pole tokenů
 * @return ukazatel na pole tokenů
 */
std::vector <Token *>* Token::getTokens()
{
	return &listOfTokens;
}
