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
#include "calendar.h"

#include <typeinfo>

std::map<std::string, Transition *> Transition::listOfTransitions;
std::map<std::string, Place *> Place::listOfPlaces;


/* ########################## class PlaceTransition ###########################*/

/**
 * Získání jména místa/přechodu
 * @return jméno místa/přechodu
 */
std::string PlaceTransition::getName()
{
	return this->name;
}

/**
 * Zkontroluje, zda je objekt typu PlaceTransition místo nebo přechod (slouží pro odlišení)
 * @return true - jedná se o místo
 */
bool PlaceTransition::checkPlace()
{
	return this->isPlace;
}

/**
 * Přidá hranu do seznamu hran vedoucích do místa/přechodu
 * @param link ukazatel na přidávanou hranu
 */
void PlaceTransition::addInputLink(Link *link)
{
	this->inputLinks.push_back(link);
}


/**
 * Přidá hranu do seznamu hran vedoucích z místa/přechodu
 * @param link ukazatel na přidávanou hranu
 */
void PlaceTransition::addOutputLink(Link *link)
{
	this->outputLinks.push_back(link);
}

/**
 * Vrátí počet hran vedoucích do místa/přechodu
 * @return 
 */
int PlaceTransition::getInputLinkCount()
{
	return inputLinks.size();
}

/**
 * Vrátí počet hran vedoucích z místa/přechodu
 * @return 
 */
int PlaceTransition::getOutputLinkCount()
{
	return outputLinks.size();
}

/**
 * Vrátí ukazatel na seznam hran vedoucích z místa/přechodu
 * @return ukazatel na seznam hran vedoucích z místa/přechodu
 */
std::vector<Link *> *PlaceTransition::getOutputLinks()
{
	return &outputLinks;
}

/**
 * Vrátí ukazatel na seznam hran vedoucích do místa/přechodu
 * @return ukazatel na seznam hran vedoucích do místa/přechodu
 */
std::vector<Link *> *PlaceTransition::getInputLinks()
{
	return &inputLinks;
}

/* ########################## class Place ###########################*/
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
		throw 1;
	}
	else if (Transition::getTransition(name) != NULL)
	{
		std::cerr<<"Název pro místo \""<<name<<" je již použit pro pojmenování přechodu."<<std::endl;
		throw 1;
	}

	this->name = name;
	this->capacity = capacity;
	this->min = 0;
	this->max = 0;
	this->isPlace = true;
	listOfPlaces.insert(std::pair<std::string, Place*>(name, this));	
}

/**
 * Získání ukazatele na místo na základě jeho jména
 * @param name jméno požadovaného místa
 * @return ukazatel požadované na místo
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
 * Vrátí kapacitu místa.
 * @return kapacita místa
 */
int Place::getCapacity()
{
	return this->capacity;
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
 * @param token ukazatel na token, který má být smazán
 */
void Place::removeToken(Token *token)
{
	std::vector <Token *>::iterator it; // iterátor pro průchod polem tokenů
	std::vector<Token *> *listOfTokens = this->getTokens(); // seznam všech tokenů modelu
	
	// prohledání pole tokenů
	for(it = listOfTokens->begin(); it != listOfTokens->end(); it++)
	{
		if(*it == token)
		{
			delete(token); // dealokace objektu tokenu
			listOfTokens->erase(it); // smazání ukazatele ze seznamu všech tokenů
			return;
		}
		
	}
}

/**
 * Vrátí počet tokenů v místě.
 * @return počet tokenů v místě
 */
int Place::getTokenCount()
{
	return listOfTokens.size();
}

/**
 * Vytiskne seznam vše tokenů v místě
 */
void Place::printTokens()
{
	std::vector <Token *>::iterator it; // iterátor pro průchod polem tokenů
	
	// prohledání pole tokenů
	for(it = listOfTokens.begin(); it != listOfTokens.end(); it++)
	{
		std::cerr<<"Token v místě: "<<*it<<std::endl;

	}
}

/**
 * Vrátí ukazatel na seznam všech tokenů v místě.
 * @return ukazatel na seznam všech tokenů
 */
std::vector<Token *> *Place::getTokens()
{
	return &listOfTokens;
}

/**
 * Získání ukazatele na pole míst
 * @return pole míst
 */
std::map<std::string, Place *>* Place::getPlaces()
{
	return &listOfPlaces;
}

/* ########################## class Transition ###########################*/

/**
 * Konstruktor defaultního přechodu zadaného jménem
 * @param name jméno vytvářeného přechodu
 */
Transition::Transition(std::string name)
{
	Transition(name, 0, PRIORITY);
}

/**
 * Konstruktor přechodu zadaného jménem a parametry
 * @param name jméno vytvářeného přechodu
 * @param value hodnota dané vlastnosti přechodu
 * @param type typ přechodu
 */
Transition::Transition(std::string name, int value, Transition::Type type)
{
	// pokud zadané jméno přechodu již existuje
	if (Transition::getTransition(name) != NULL)
	{
		std::cerr<<"Přechod se jménem \""<<name<<"\" již existuje."<<std::endl;
		throw 1;
	}
	// pokud zadané jméno již existuje jako jméno místa
	else if (Place::getPlace(name) != NULL)
	{
		std::cerr<<"Název pro přechod \""<<name<<" je již použit pro pojmenování místa."<<std::endl;
		throw 1;
	}
	// pokud typ řechodu není TIMED_EXP, TIMED_CONST, STOCHASTIC nebo PRIORITY
	else if (!(type >= Transition::TIMED_EXP && type <= Transition::PRIORITY))
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s neexistujícím typem."<<std::endl;
		throw 1;
	}
	// pokud je zadána nulová hodnota časovaného nebo pravděpodobnostního přechodu
	else if (value == 0 && type >= Transition::TIMED_EXP && type <= Transition::STOCHASTIC)
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s nulovou hodnotou zpoždění nebo pravděpodobnosti."<<std::endl;
		throw 1;
	}
	// pokud je hodnota vlastnosti přechodu záporná
	else if (value < 0)
	{
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s zápornou hodnotou jeho vlastnosti."<<std::endl;
		throw 1;
	}

	this->name = name;
	this->value = value;
	this->type = type;
	this->isPlace = false;
	this->isPerformed = false;
	this->isTimed = false;
	
	// přidání vytvořeného přechodu do seznamu všech vytvořených přechodů
	listOfTransitions.insert(std::pair<std::string, Transition*>(name, this));
}

/**
 * Vrátí typ přechodu
 * @return typ přechodu
 */
int Transition::getTransitionType()
{
	return this->type;
}

/**
 * Vrátí hodnotu daného typu přechodu (čas, pravděpodobnost, prioritu)
 * @return hodnota času, pravděpodobnosti nebo priority dle typu přechodu
 */
unsigned int Transition::getValue()
{
	return this->value;
}

/**
 * Zkontroluje, zda je ve všech výstupních místech přechodů počet tokenů větší nebo 
 * roven kapacitě hrany a vrátí true. V opačném případě vrátí false.
 * @return true - pokud je ve všech výstup. místech modelu počet tokenů větší nebo roven kapacitě hrany
 */
bool Transition::checkPlaceOutput()
{
	std::vector<Link *> ::iterator iterOutputLink; // iterátor výstupních hran přechodu
	std::vector<Link *> *outputLinks = Link::getLinks(); // seznam všech hran modelu
	Place* place; // ukazatel na místo
	
	// postupné procházení všech výstupních hran přechodu
	for(iterOutputLink = outputLinks->begin(); iterOutputLink != outputLinks->end(); iterOutputLink++ )
	{
		// pokud na výstupu hrany není místo, zpracuj další
		if(!(*iterOutputLink)->getInput()->checkPlace())
			continue;
		
		// získání místa na výstupu hrany
		place = (Place *)(*iterOutputLink)->getInput();
		
		// pokud v místě nejsou žádné tokeny, zpracuj další hranu
		if(place->getTokenCount() == 0)
			continue;
		
		// pokud počet tokenů v místě je menší než kapacita hrany
		if (place->getTokenCount() < (*iterOutputLink)->getCapacity())
			return false;
	}
	return true;
}

/**
 * Zkontroluje, zda je ve všech výstupních místech přechodů počet tokenů větší nebo 
 * roven kapacitě hrany a vrátí true. V opačném případě vrátí false.
 * @return true - pokud je ve všech výstup. místech modelu počet tokenů větší nebo roven kapacitě hrany
 */
bool Transition::checkPlaceInput()
{
	std::vector<Link *> ::iterator iterInputLink; // iterátor vstupních hran přechodu
	std::vector<Link *> *link = Link::getLinks(); // seznam všech hran modelu
	Place* place; // ukazatel na místo
	
	// postupné procházení všech vstupních hran přechodu
	for(iterInputLink = link->begin(); iterInputLink != link->end(); iterInputLink++ )
	{
		// pokud na vstupu hrany není místo, zpracuj další
		if(!(*iterInputLink)->getInput()->checkPlace())
			continue;
		
		// získání místa na výstupu hrany
		place = (Place *)(*iterInputLink)->getInput();
		
		// pokud v místě nejsou žádné tokeny, zpracuj další hranu
		if(place->getTokenCount() == 0)
			continue;
		
		// pokud počet tokenů v místě je menší než kapacita hrany
		if (place->getTokenCount() < (*iterInputLink)->getCapacity())
			return false;
	}
	return true;
}

/**
 * Nastaví příznak přechodu, že je vykonán.
 * @param value hodnota příznaku
 */
void Transition::setIsPerformed(bool value)
{
	this->isPerformed = value;
}

/**
 * Vrátí true, pokud je nastaven příznak vykonání přechodu.
 * @return true, pokud je nastaven příznak vykonání přechodu
 */
bool Transition::getIsPerformed()
{
	return this->isPerformed;
}

/**
 * Nastaví načasovaní přechodu na logickou hodnotu zadanou parametrem
 * @param value true - přechod je načasován
 */
void Transition::setIsTimed(bool value)
{
	this->isTimed = value;
}

/**
 * Vrátí true, pokud je nastaven příznak, že je přechod načasován.
 * @return true, pokud je nastaven příznak, že je přechod načasován
 */
bool Transition::getIsTimed()
{
	return this->isTimed;
}

/**
 * Získá ukazatel na seznam na seznam všech přechodů modelu.
 * @return ukazatel na seznam přechodů
 */
std::map<std::string, Transition *>* Transition::getTransitions()
{
	return &listOfTransitions;
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
 * 
 * @return 
 */
std::vector<Transition*> Transition::getRandomVectorTransitions()
{
	std::map<std::string, Transition *>::iterator iterTransition; //iterátor pro průchod seznamem přechodů
	std::vector<Transition *> checkTransitions; // pomocný seznam přechodů
	
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions(); // seznam všech tokenů v místě

	// postupné procházení přechodů a vložení jejich ukazatelů do pomocného seznamu přechodů
	for(iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
		checkTransitions.push_back(iterTransition->second);
	
	// zamíchání prvků pomocného seznamu přechodů
	std::random_shuffle(checkTransitions.begin(), checkTransitions.end());
	
	return checkTransitions;
}
