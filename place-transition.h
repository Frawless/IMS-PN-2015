/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	place-transition.h	 	    *
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef PLACE_H
#define PLACE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "link.h"

class Link;
class Token;

/**
 * 
 */
class PlaceTransition
{
public:
	//enum Type{PLACE, TRANSITION};		//enumerace typu objektu
	
	void addInputLink(Link *link);
	void addOutputLink(Link *link);
	std::string getName();
  
protected:
	//Type type;							//typ objektu
	std::string name;					//jméno objektu
	std::vector<Link *> inputLinks;		//pole vstupních hran
	std::vector<Link *> outputLinks;	//pole výstupních hran
};


/**
 * 
 */
class Place: public PlaceTransition
{
public:
    Place(std::string name);
    Place(std::string name, int capacity);
    static std::map<std::string, Place *> getPlaces();
    void addToken(Token* token);
  
private:
    static std::map<std::string, Place *> listOfPlaces;
    std::vector<Token *> listOfTokens; 
    int capacity;
    unsigned int min;
    unsigned int max;
};

/**
 * 
 */
class Transition: public PlaceTransition
{
public:
	enum Type{TIMED_EXP, TIMED_CONST, PRIORITY, STOCHASTIC /*???*/};
  
	Transition(std::string name);
	Transition(std::string name, int value, Transition::Type type);
        static std::map<std::string, Transition *> getTransitions();
  
private:
    static std::map<std::string, Transition *> listOfTransitions;
    Type type;						//typ přechodu
    unsigned int value;				//hodnota přechodu - čas/priorita/pravděpodobnost
};

#endif