/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	model.h 	            	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "place-transition.h"
#include "token.h"

class Link;
class PlaceTransition;
class Place;
class Transition;
class Token;

/**
 * 
 */
class Model
{
public:
	Model();			//konstruktor	
	~Model();			//destruktor
	void addPlace(std::string name, int capacity);
	void addPlace(std::string name);
	void addTransition(std::string name, int value, Transition::Type type);
	void addTransition(std::string name);
	void addLink(std::string inputName, std::string outputName, int capacity);
	void addToken(std::string placeName);  
	void addToken(std::string placeName, int count); 
	
	void printModel();
private:
	std::map<std::string, Transition *> listOfTransitions;	//asociativní pole přechodů
	std::map<std::string, Place *> listOfPlaces;			//asociativní pole míst
	std::vector <Link *> listOfLinks;						//seznam hran
	std::vector <Token *> listOfTokens;						//seznam značek v místech
	
};

#endif