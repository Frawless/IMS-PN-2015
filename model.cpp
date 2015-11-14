/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	model.cpp 	             	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/


#include "model.h"


/**
 * 	Konstruktor
 */
Model::Model()
{
  
}

/**
 * 	Destruktor
 */
Model::~Model()
{
  
  
}
/*
	
	void addToken
 */
/*void addLink(std::string inputName, std::string outputName)
{
	
}*/
void Model::addPlace(std::string name)
{
	/*
	 * kontrola
	 */
	Model::addPlace(name, 0);
}

void Model::addPlace(std::string name, int capacity)
{
	/*
	 * kontrola
	 */
	new Place(name, capacity);
}

void Model::addTransition(std::string name, int value, Transition::Type type)
{
	new Transition(name, value, type);
}

void Model::addTransition(std::string name)
{
	Model::addTransition(name, 0, Transition::PRIORITY);
}

void Model::addLink(std::string inputName, std::string outputName, int capacity)
{
	std::map<std::string, Place *> places = Place::getPlaces();
	std::map<std::string, Transition *> transitions = Transition::getTransitions();
	std::vector <Link *> links = Link::getLinks();

	
	// vytvoření interátorů pro vyhledávání v asociativních polích míst a přechodů
	std::map<std::string, Place *>::iterator iterInputPlace;
	std::map<std::string, Place *>::iterator iterOutputPlace;
	std::map<std::string, Transition *>::iterator iterInputTransition;
	std::map<std::string, Transition *>::iterator iterOutputTransition;

	// vyhledání místa a přechodu v asociativním poli dle jeho názvu
	iterInputPlace = places.find(inputName);
	iterOutputPlace = places.find(outputName);
	iterInputTransition = transitions.find(inputName);
	iterOutputTransition = transitions.find(outputName);
	
	if ((iterInputTransition != transitions.end()) && (iterOutputPlace != places.end()))
	{
		Place *p = iterOutputPlace->second;
		Transition *t = iterInputTransition->second;
		Link *l = new Link(iterInputTransition->second, iterOutputPlace->second, capacity);
		
		p->addOutputLink(l);
		t->addInputLink(l);
	}
	else if((iterInputPlace != places.end()) && (iterOutputTransition != transitions.end()))
	{
		Place *p = iterInputPlace->second;
		Transition *t = iterOutputTransition->second;
		Link *l = new Link(iterInputPlace->second, iterOutputTransition->second, capacity);
		
		p->addInputLink(l);
		t->addOutputLink(l);
	}
	else
		std::cerr<<"nejde to"<<std::endl;
	
}

void Model::addToken(std::string placeName, int count)
{
	std::map<std::string, Place *> places = Place::getPlaces();
	std::map<std::string, Place *>::iterator iterPlace;
	
	iterPlace = places.find(placeName);
	if(iterPlace == places.end())
	{
		std::cerr<<"Značka v neznámém místě!"<<std::endl;
		throw 1;
	}
	
	for(int i = 0; i < count; i++)
	{
		Token *token = new Token(iterPlace->second);
		Token::getTokens().push_back(token);
		iterPlace->second->addToken(token);  
	}
}

void Model::addToken(std::string placeName)
{
	Model::addToken(placeName,1); 
}

void Model::printModel()
{
	std::map<std::string, Place *> places = Place::getPlaces();
	std::map<std::string, Transition *> transitions = Transition::getTransitions();
	std::vector <Link *> links = Link::getLinks();
	
	std::map<std::string, Place *>::iterator modelPlace;
	std::map<std::string, Transition *>::iterator modelTransition;
	std::vector<Link *>::iterator modelLink;
	
	for(modelPlace = places.begin(); modelPlace != places.end(); modelPlace++)
	{
		std::cerr<<"Místo: "<<modelPlace->second->getName()<<std::endl;
	}
	
	for(modelTransition = transitions.begin(); modelTransition != transitions.end(); modelTransition++)
	{
		std::cerr<<"Přechod: "<<modelTransition->second->getName()<<std::endl;
	}	

	
	for(modelLink = links.begin(); modelLink != links.end(); modelLink++)
	{
		
		std::cerr<<"Linka z: "<<(*modelLink)->getInput()->getName()<<" do: "<<(*modelLink)->getOutput()->getName()<<std::endl;
		//links->second->
	}	
}