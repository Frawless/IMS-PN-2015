/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	model.cpp		        	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#include "model.h"

/**
 * 	Konstruktor modelu
 */
Model::Model()
{ 
}

/**
 * 	Destruktor modelu
 */
Model::~Model()
{ 
}

/**
 * Přidání místa do modelu zadaného jménem
 * @param name jméno místa
 */
void Model::addPlace(std::string name)
{
	// kontrola ???
	Model::addPlace(name, 0);
}

/**
 * Přidá místo do modelu
 * @param name jméno místa
 * @param capacity kapacita místa ???
 */
void Model::addPlace(std::string name, int capacity)
{
	if (Place::getPlace(name) != NULL)
		std::cerr<<"Místo se jménem \""<<name<<" již existuje\"."<<std::endl;
	else if (Transition::getTransition(name) != NULL)
		std::cerr<<"Název pro místo \""<<name<<" je již použit pro pojmenování přechodu."<<std::endl;
	else
		new Place(name, capacity);
}

/**
 * Přidání přechodu do modelu
 * @param name jméno přechodu
 * @param value parametr přechodu (čas, pravděpodobnost, priorita)
 * @param type typ přechodu (časovaný, pravděpodobnostní, prioritní)
 */
void Model::addTransition(std::string name, int value, Transition::Type type)
{
	if (Transition::getTransition(name) != NULL)
		std::cerr<<"Přechod se jménem \""<<name<<"\" již existuje."<<std::endl;
	else if (Place::getPlace(name) != NULL)
		std::cerr<<"Název pro přechod \""<<name<<" je již použit pro pojmenování místa."<<std::endl;
	else if (!(type >= Transition::TIMED_EXP && type <= Transition::PRIORITY))
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s neexistujícím typem."<<std::endl;
	else if (value == 0 && type >= Transition::TIMED_EXP && type <= Transition::STOCHASTIC)
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s nulovou hodnotou zpoždění nebo pravděpodobnosti."<<std::endl;
	else if (value < 0)
		std::cerr<<"Nelze vložit přechod \""<<name<<"\" s zápornou hodnotou jeho vlastnosti."<<std::endl;
	else
		new Transition(name, value, type);
}

/**
 * Přidání přechodu do modelu
 * @param name název přechodu
 */
void Model::addTransition(std::string name)
{
	Model::addTransition(name, 0, Transition::PRIORITY);
}

/**
 * Přidání hrany do modelu
 * @param inputName název místa/přechodu na vstupní části hrany
 * @param outputName název místa/přechodu na výstupní části hrany
 * @param capacity kapacita hrany
 */
void Model::addLink(std::string inputName, std::string outputName, int capacity)
{
	Transition* t; // ukazatel na přechod
	Place* p; // ukazatel na místo
	
	// pokud je hrana mezi přechodem a neexistujícím místem
	if (Transition::getTransition(inputName) != NULL && Place::getPlace(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi přechodem a neexistujícím místem.!"<<std::endl;
	}
	else if (Place::getPlace(inputName) != NULL && Transition::getTransition(outputName) == NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi místem a neexistujícím přechodem.!"<<std::endl;
	}
	else if (Place::getPlace(inputName) != NULL && Place::getPlace(outputName) != NULL)
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 místy.!"<<std::endl;
	}
	else if (Transition::getTransition(inputName) != NULL && Transition::getTransition(outputName))
	{
		std::cerr<<"Nelze vytvořit hranu mezi 2 přechody.!"<<std::endl;
	}
	else if (((t = Transition::getTransition(inputName)) != NULL) && ((p = Place::getPlace(outputName)) != NULL))
	{
		Link *l = new Link(t, p, capacity); // vytvoření hrany
		
		p->addOutputLink(l); // vložení hrany k přechodu // je potřeba ???
		t->addInputLink(l); // vložení hrany k místu
	}
	// pokud pokud je na vstupu hrany místo a na výstupu přechod
	else if (((p = Place::getPlace(inputName)) != NULL) && ((t = Transition::getTransition(outputName)) != NULL))
	{
		Link *l = new Link(p, t, capacity); // vytvoření hrany
		
		p->addInputLink(l); // vložení hrany k přechodu // je potřeba ???
		t->addOutputLink(l); // vložení hrany k místu
	}
	else
		std::cerr<<"Chybná kombinace místo-přechod (více ošetřit?)"<<inputName<<"("<<(p==NULL)<<"):"<<outputName<<"("<<(t==NULL)<<")"<<std::endl;
}

/**
 * Přidání zadaného počtu tokenů do místa zadaného jménem.
 * @param placeName název místa, kam má být token vložen
 * @param count počet vložench tokenů
 */
void Model::addToken(std::string placeName, int count)
{
	Place * place;

	// pokud zadané místo neexistuje
	if((place = Place::getPlace(placeName)) == NULL)
	{
		std::cerr<<"Nelze vložit značku do neexistujícího místa."<<std::endl;
		throw 1;
	}
	
	// vložení tokenu do místa i-krát
	for(int i = 0; i < count; i++)
	{
		Token *token = new Token(place);
		place->addToken(token);  
	}
}

/**
 * Přidání jednoho tokenu do místa zadaného jménem.
 * @param placeName název místa
 */
void Model::addToken(std::string placeName)
{
	Model::addToken(placeName, 1); 
}

/**
 * Vytisknutí modelu
 */
void Model::printModel()
{
	// deklarace iterátorů pro průchod polí míst, přechodů a hran
	std::map<std::string, Place *>::iterator modelPlace;
	std::map<std::string, Transition *>::iterator modelTransition;
	std::vector<Link *>::iterator modelLink;
	
	// průchod míst
	for(modelPlace = Place::getPlaces()->begin(); modelPlace != Place::getPlaces()->end(); modelPlace++)
	{
		std::cerr<<"Místo: "<<modelPlace->second->getName()<<std::endl;
	}
	
	// průchod přechodů
	for(modelTransition = Transition::getTransitions()->begin(); modelTransition != Transition::getTransitions()->end(); modelTransition++)
	{
		std::cerr<<"Přechod: "<<modelTransition->second->getName()<<std::endl;
	}	

	// průchod hran
	for(modelLink = Link::getLinks()->begin(); modelLink != Link::getLinks()->end(); modelLink++)
	{
		
		std::cerr<<"Linka z: "<<(*modelLink)->getInput()->getName()<<" do: "<<(*modelLink)->getOutput()->getName()<<std::endl;
	}	
}