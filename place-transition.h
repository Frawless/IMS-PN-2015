/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor Petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	place-transition.h       	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

/**
 * @file place-transition.h
 * @brief Hlavičkový soubor obsahující třídu společné vlastnosti tříd Place a Transition a na základě 
 * ní jsou vytvořeny třídy Place a Transition (zděděno).
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

#ifndef PLACE_TRANSITION_H
#define PLACE_TRANSITION_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

#include "link.h"

class Link;
class Token;
class Event;

/**
 * @class PlaceTransition
 * @brief Třída obsahující společné vlastnosti tříd Place a Transition a na základě 
 * ní jsou vytvořeny třídy Place a Transition (zděděno).
 */
class PlaceTransition
{
	public:
		std::string getName(); // získá název hrany/přechodu
		bool checkPlace(); // vrátí true, pokud se jedná o místo (rozlišuje objekty typu PlaceTransition)
		void addInputLink(Link *link); // přidá hranu do seznamu hran vedoucích do místa/přechodu
		void addOutputLink(Link *link); // přidá hranu do seznamu hran vedoucích z místa/přechodu
		int getInputLinkCount(); // vrátí počet hran vedoucích do místa/přechodu
		int getOutputLinkCount(); // vrátí počet hran vedoucích z místa/přechodu
		std::vector<Link *> *getOutputLinks(); // vrátí ukazatel na seznam hran vedoucích z místa/přechodu
		std::vector<Link *> *getInputLinks(); // vrátí ukazatel na seznam hran vedoucích do místa/přechodu
		
	protected:
		std::string name; // jméno místa/přechodu
		bool isPlace; // rozlišení objektu typu PlaceTransition
		std::vector<Link *> inputLinks; // seznam hran vedoucích do místa/přechodu
		std::vector<Link *> outputLinks; // seznam hran vedoucích z místa/přechodu
};

/**
 * @class Place
 * @brief Třída reprezentující místo modelu.
 */ 
class Place: public PlaceTransition
{
	public:
		Place(std::string name); // konstruktor místa na základě místa
		Place(std::string name, int capacity); // konstruktor místa na základě místa a kapacity
		static Place* getPlace(std::string name); // vrátí ukazatel na místo zadané jménem
		int getCapacity(); // vrátí kapacitu místa
		void addToken(Token* token); // přidá token do místa
		void removeToken(Token *token); // odstraní token z místa
		int getTokenCount(); // vrátí počet tokenů v místě
		void printTokens(); // vytiskne seznam tokenů v místě
		std::vector<Token *> *getTokens(); // vratí ukazatel na seznam všech tokenů v místě
		static std::map<std::string, Place *>* getPlaces(); // získá ukazatel na seznam všech míst modelu
		
		// Statistiky
		void setStats(); // uloží statistiku místa
		void printStats(); // vytiskne statistiku místa
	
	private:
		static std::map<std::string, Place *> listOfPlaces; // seznam všech míst modelu
		int capacity; // kapacita místa
		std::vector<Token *> listOfTokens; // sezam všech tokenů v místě
		
		// Statistiky
		int min; // minimální počet tokenů v místě
		int max; // maximální počet tokenů v mí
		int performCount; // počet provedení výpočtu statistik
		double average; // průměrná hodnota počtu tokenů v místě
		
};

/**
 * @class Transition
 * @brief Třída reprezentující přechod modelu
 */
class Transition: public PlaceTransition
{
	public:
		enum Type{TIMED_EXP, TIMED_CONST, PROBABILITY, PRIORITY}; // výčet typů přechodů
		Transition(std::string name); // konstruktor přechodu zadáním jména
		Transition(std::string name, int value, Transition::Type type); // konstruktor přechodu zadáním místa a parametrů
		int getTransitionType(); // vrátí typ přechodu
		unsigned int getValue(); // vrátí hodnotu typu přechodu
		void setGeneratedValue(double value); // nastaví vypočítanou hodnotu
		double getGeneratedValue(); // vrátí vypočítanou hodnotu
		bool checkPlaceOutput(); // zkontroluje, zda je ve všech výstupních místech přechodů počet tokenů větší nebo roven kapacitě hrany a vrátí true
		bool checkPlaceInput(); // zkontroluje, zda je ve všech vstupních místech přechodů počet tokenů větší nebo roven kapacitě hrany a vrátí true
		void setIsTimed(bool value); // nastaví načasovaní přechodu na logickou hodnotu zadanou parametem
		bool getIsTimed(); // Vrátí true, pokud je přechod načasován
		static std::map<std::string, Transition *>* getTransitions(); // vrátí ukazatel na seznam všech přechodů modelu
		static Transition* getTransition(std::string name); // vrátí ukazatel na přechod zadaný jménem
		static std::vector<Transition*> getRandomVectorTransitions(); // vrátí ukazatel na náhodně uspořádaný seznam všech přechodů modelu
				
		// Statistiky
		void setStats(); // uloží statistiky přechodu
		void printStats(); // tiskne statistiky nečasovaného přechodu
		void printTimedStats(); // tiskne statistiky časovaného přechodu
		// ??? podle emailu zakomentovat/nechat
		void recomputeStatsWithDeleteEventWait(Event *event); // přepočítá statistiky pomocí hodnot smazaných při poruše
				
		
	private:
		static std::map<std::string, Transition *> listOfTransitions; // seznam všech přechodů modelu
		Type type; // typ přechodu
		unsigned int value;	// hodnota typu přechodu - čas/priorita/pravděpodobnost		//bool isPerformed; // značí, že byl přechod vykonán
		bool isTimed; // značí, že je přechod načasován
		double generatedValue; // vygenerovaná hodnota z value
		
		// Statistiky přechodů
		double min; // maximální hodnota vygenerovaného času přechodu
		double max; // minimální hodnota vygenerovaného času přechodu
		double average; // průměrná hodnota vygenerovaného času přechodu
		int performCount; // počet provedených přechodů

};

#endif
