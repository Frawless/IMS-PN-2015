/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	place-transition.h       	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
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
 * Třída obsahující společné vlastnosti tříd Place a Transition a na základě
 * ní jsou vytvořeny třídy Place a Transition (zděděno).
 */
class PlaceTransition
{
	public:
		void addInputLink(Link *link); // přidání vstupu hrany k místu/přechodu
		void addOutputLink(Link *link); // přidání výstupu hrany k místu/přechodu
		std::string getName(); // získání názvu hrany/přechodu
		int getInputLinkCount();
		int getOutputLinkCount();

	protected:
		std::string name; // jméno místa/přechodu
		std::vector<Link *> inputLinks; // pole vstupních hran vázaných k místu/přechodu
		std::vector<Link *> outputLinks; // pole výstupních hran vázaných k místu/přechoduan
};

/**
 * Třída reprezentující místo modelu.
 */
class Place: public PlaceTransition
{
	public:
		Place(std::string name); // konstruktor místa na základě místa
		Place(std::string name, int capacity); // konstruktor místa na základě místa a kapacity
		static std::map<std::string, Place *>* getPlaces(); // získání ukazatele na pole míst
		void addToken(Token* token); // přidání tokenu do místa
		static Place* getPlace(std::string name); // získání ukazatele na místo dle jeho jména
		void removeToken(Token *token); //odstranění tokenu z pole tokenů v místě
		int getCapacity();
		int getTokenCount();
		void printTokens();

	private:
		static std::map<std::string, Place *> listOfPlaces; // pole míst
		std::vector<Token *> listOfTokens;  // pole tokenů v místě
		int capacity; // kapacita místa
		unsigned int min; // ???
		unsigned int max; // ???
};

/**
 * Třída reprezentující přechod modelu
 */
class Transition: public PlaceTransition
{
	public:
		enum Type{TIMED_EXP, TIMED_CONST, STOCHASTIC, PRIORITY /*???*/};

		Transition(std::string name);
		Transition(std::string name, int value, Transition::Type type);
		static std::map<std::string, Transition *>* getTransitions();
		static Transition* getTransition(std::string name);

	private:
		static std::map<std::string, Transition *> listOfTransitions; // pole přechodů
		Type type; // typ přechodu
		unsigned int value;	// hodnota typu přechodu - čas/priorita/pravděpodobnost
};

#endif