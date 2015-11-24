/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	place-transition.cpp       	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/
#include "place-transition.h"
#include "token.h"

std::map<std::string, Transition *> Transition::listOfTransitions;
std::map<std::string, Place *> Place::listOfPlaces;

/* ############################## class Place ################################*/

/**
 * Konstruktor místa zadáním jména
 */
Place::Place(std::string name)
{
	Place(name, 0);
}

/**
 * Konstruktor místa zadáním jména a kapacity
 */
Place::Place(std::string name, int capacity)
{
	if (Place::getPlace(name) != NULL)
	{
		std::cerr<<"Místo se jménem \""<<name<<" již existuje\"."<<std::endl;
		throw 10;
	}
	else if (Transition::getTransition(name) != NULL)
	{
		std::cerr<<"Název pro místo \""<<name<<" je již použit pro pojmenování přechodu."<<std::endl;
		throw 20;
	}

	this->name = name;
	this->capacity = capacity;
	this->min = 0;
	this->max = 0;
	listOfPlaces.insert(std::pair<std::string, Place*>(name, this));	
}

/**
 * Získání ukazatele na pole míst
 * @return pole míst
 */
std::map<std::string, Place *>* Place::getPlaces()
{
	return &listOfPlaces;
}

/**
 * Získání ukazatele na místo na základě jeho jména
 * @param name jméno požadovaného místa
 * @return 
 */
Place* Place::getPlace(std::string name)
{
	// deklarace iterátoru
	std::map<std::string, Place *>::iterator iterPlace;
	
	// nalezení místa v poli míst
	iterPlace = listOfPlaces.find(name);
	
	// pokud je hledané místo nalezeno, je vráceno
	if (iterPlace != listOfPlaces.end())
		return iterPlace->second;
	else 
		return NULL;
}

/**
 * Přidání tokenu do místa
 * @param token ukazatel na token
 */
void Place::addToken(Token* token)
{
	this->listOfTokens.push_back(token);
}

/**
 * Smazání zadaného tokenu z místa.
 * @param token
 */
void Place::removeToken(Token *token)
{
	std::vector <Token *>::iterator it;		//iterátor pro průchod polem tokenů
	//prohledání pole tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		if(*it == token)
		{
			listOfTokens.erase(it);		//smazání ze seznamu
		}
		return;
	}
}

/**
 * 
 * @return 
 */
int Place::getCapacity()
{
	return this->capacity;
}

/**
 * 
 * @return 
 */
int Place::getTokenCount()
{
	return listOfTokens.size();
}

/**
 * 
 */
void Place::printTokens()
{
	std::vector <Token *>::iterator it;		//iterátor pro průchod polem tokenů
	//prohledání pole tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		std::cerr<<"Token v místě: "<<*it<<std::endl;

	}
}

/* ############################ class Transition ##############################*/

/**
 * Konstruktor defaultního přechodu zadaného jménem
 * @param name
 */
Transition::Transition(std::string name)
{
	Transition(name, 0, PRIORITY);
}


/**
 * Konstruktor přechodu zadaného jménem a parametry
 * @param name
 * @param value
 * @param type
 */
Transition::Transition(std::string name, int value, Transition::Type type)
{
	if (Transition::getTransition(name) != NULL)
	{
		std::cerr<<"Přechod se jménem \""<<name<<"\" již existuje."<<std::endl;
		throw 1;
	}
	else if (Place::getPlace(name) != NULL)
	{
		std::cerr<<"Název pro přechod \""<<name<<" je již použit pro pojmenování místa."<<std::endl;
		throw 1;
	}
	else if (!(type >= Transition::TIMED_EXP && type <= Transition::PRIORITY))
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s neexistujícím typem."<<std::endl;
		throw 1;
	}
	else if (value == 0 && type >= Transition::TIMED_EXP && type <= Transition::STOCHASTIC)
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s nulovou hodnotou zpoždění nebo pravděpodobnosti."<<std::endl;
		throw 1;
	}
	else if (value < 0)
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s zápornou hodnotou jeho vlastnosti."<<std::endl;
		throw 1;
	}

	this->name = name;
	this->value = value;
	this->type = type;
	listOfTransitions.insert(std::pair<std::string, Transition*>(name, this));
}

/**
 * Získání ukazatele na přechod zadaného jménem
 * @param name jméno přechodu
 * @return ukazatel na přechod
 */
Transition* Transition::getTransition(std::string name)
{
	// deklarace iterátoru
	std::map<std::string, Transition *>::iterator iterTransition;
	
	// nalezení přechodu v poli přechodů
	iterTransition = listOfTransitions.find(name);
	
	// pokud byl přechod nalezen, je vrácen
	if (iterTransition != listOfTransitions.end())
		return iterTransition->second;
	else 
		return NULL;
}

/**
 * Získání ukazatele na pole přechodů
 * @return ukazatel na pole přechodů
 */
std::map<std::string, Transition *>* Transition::getTransitions()
{
	return &listOfTransitions;
} 

/* ########################## class PlaceTransition ###########################*/
/**
 * Přidání vstupu hrany do pole místa/přechodu
 * @param link přidávaná hrana
 */
void PlaceTransition::addInputLink(Link *link)
{
	this->inputLinks.push_back(link);
}

/**
 * Přidání výstupu hrany do pole místa/přechodu
 * @param link přidávaná hrana
 */
void PlaceTransition::addOutputLink(Link *link)
{
	this->outputLinks.push_back(link);  
}

/**
 * Získání jména místa/přechodu
 * @return jméno místa/přechodu
 */
std::string PlaceTransition::getName()
{
	return this->name;
}

/**
 * 
 */
int PlaceTransition::getInputLinkCount()
{
	return inputLinks.size();
}

/**
 * 
 */
int PlaceTransition::getOutputLinkCount()
{
	return outputLinks.size();
}