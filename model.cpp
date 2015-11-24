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
	try
	{
		new Place(name, capacity);
	}
	catch(int e)
	{
		std::cerr<<"ahoj"<<std::endl;
	}
	//??? statistiky
}

/**
 * Přidání přechodu do modelu
 * @param name jméno přechodu
 * @param value parametr přechodu (čas, pravděpodobnost, priorita)
 * @param type typ přechodu (časovaný, pravděpodobnostní, prioritní)
 */
void Model::addTransition(std::string name, int value, Transition::Type type)
{
	try
	{
		new Transition(name, value, type);
	}
	catch(int e)
	{
		std::cerr<<"cau"<<std::endl;
	}
	//??? statistiky
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
	try
	{
		new Link(inputName, outputName, capacity); // vytvoření hrany
	}
	catch(int e)
	{
	
	}
	//??? statistiky
}

/**
 * Přidání zadaného počtu tokenů do místa zadaného jménem.
 * @param placeName název místa, kam má být token vložen
 * @param count počet vložench tokenů
 */
Token* Model::addToken(std::string placeName, int count)
{
	Place * place;
	Token *token;

	// pokud zadané místo neexistuje
	if((place = Place::getPlace(placeName)) == NULL)
	{
		std::cerr<<"Nelze vložit značku do neexistujícího místa."<<std::endl;
		throw 1;
	}
	
	// vložení tokenu do místa i-krát
	for(int i = 0; i < count; i++)
	{
		token = new Token(place);
		place->addToken(token);  
	}
	return token;	//???smazat a vrátit metodu na void
	//??? statistiky
}

/**
 * Přidání jednoho tokenu do místa zadaného jménem.
 * @param placeName název místa
 */
Token* Model::addToken(std::string placeName)
{
	return Model::addToken(placeName, 1); 
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

/**
 * 
 * @param token
 */
void Model::removeToken(Token *token)
{
	Place *place = token->getPlace();
	place->removeToken(token);	
	/*???
	 if(pokud není v událostech)
		
	 */token->removeToken(token);
}

/**
 * 
 */
void Model::modelValidate()
{
	Place *place;
	// deklarace iterátoru
	std::map<std::string, Place *>::iterator iterPlace;
	std::map<std::string, Transition *>::iterator iterTransition;
	
	std::map<std::string, Place *> *listOfPlaces =  Place::getPlaces();
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions();
	
	for (iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		
	}
	

	for (iterPlace = listOfPlaces->begin(); iterPlace != listOfPlaces->end(); iterPlace++)
	{
		place = (*iterPlace).second;
		std::cerr<<place->getName()<<std::endl;
		//velikost místa < počet tokenů v místě
		if(place->getCapacity() < place->getTokenCount() && place->getCapacity() != 0)
		{
			std::cerr<<"Počet značek v místě \""<< place->getName() <<"\" přesahuje jeho kapacitu!"<<std::endl;
			throw 1;
		}
		//min. jedna vstupní nebo výstupní hrana
		if(place->getInputLinkCount() < 1 && place->getOutputLinkCount() < 1)
		{
			std::cerr<<"Místo \""<< place->getName() <<"\" nemá ani vstupní ani výstupní hranu!"<<std::endl;
			throw 1;
		}
	}
}