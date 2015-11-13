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
	Place* p = new Place(name, capacity);
	this->listOfPlaces.insert(std::pair<std::string, Place*>(name, p));
}

void Model::addTransition(std::string name, int value, Transition::Type type)
{
	Transition* t = new Transition(name, value, type);
	this->listOfTransitions.insert(std::pair<std::string, Transition*>(name, t));
}

void Model::addTransition(std::string name)
{
	Model::addTransition(name, 0, Transition::PRIORITY);
}

void Model::addLink(std::string inputName, std::string outputName, int capacity)
{
	// vytvoření interátorů pro vyhledávání v asociativních polích míst a přechodů
	std::map<std::string, Place *>::iterator iterInputPlace;
	std::map<std::string, Place *>::iterator iterOutputPlace;
	std::map<std::string, Transition *>::iterator iterInputTransition;
	std::map<std::string, Transition *>::iterator iterOutputTransition;

	// vyhledání místa a přechodu v asociativním poli dle jeho názvu
	iterInputPlace = this->listOfPlaces.find(inputName);
	iterOutputPlace = this->listOfPlaces.find(outputName);
	iterInputTransition = this->listOfTransitions.find(inputName);
	iterOutputTransition = this->listOfTransitions.find(outputName);
	
	if ((iterInputTransition != this->listOfTransitions.end()) && (iterOutputPlace != this->listOfPlaces.end()))
	{
		Place *p = iterOutputPlace->second;
		Transition *t = iterInputTransition->second;
		Link *l = new Link(iterInputTransition->second, iterOutputPlace->second, capacity);
		
		p->addOutputLink(l);
		t->addInputLink(l);
		this->listOfLinks.push_back(l);
	}
	else if((iterInputPlace != this->listOfPlaces.end()) && (iterOutputTransition != this->listOfTransitions.end()))
	{
		Place *p = iterInputPlace->second;
		Transition *t = iterOutputTransition->second;
		Link *l = new Link(iterInputPlace->second, iterOutputTransition->second, capacity);
		
		p->addInputLink(l);
		t->addOutputLink(l);
		this->listOfLinks.push_back(l);
	}
	else
		std::cerr<<"nejde to"<<std::endl;
	
	/*Link* l = new Link("jakub)
	this->listOfLinks.insert();*/
}

void Model::addToken(std::string placeName, int count)
{
	std::map<std::string, Place *>::iterator iterPlace;
	
	iterPlace = this->listOfPlaces.find(placeName);
	if(iterPlace == this->listOfPlaces.end())
	{
		std::cerr<<"Značka v neznámém místě!"<<std::endl;
		throw 1;
	}
	
	for(int i = 0; i < count; i++)
	{
		Token *token = new Token(iterPlace->second);
		this->listOfTokens.push_back(token);
		iterPlace->second->addToken(token);  
	}
}

void Model::addToken(std::string placeName)
{
	Model::addToken(placeName,1); 
}

void Model::printModel()
{
	std::map<std::string, Place *>::iterator modelPlace;
	std::map<std::string, Transition *>::iterator modelTransition;
	std::vector<Link *>::iterator modelLink;
	
	for(modelPlace = listOfPlaces.begin(); modelPlace != listOfPlaces.end(); modelPlace++)
	{
		std::cerr<<"Místo: "<<modelPlace->second->getName()<<std::endl;
	}
	
	for(modelTransition = listOfTransitions.begin(); modelTransition != listOfTransitions.end(); modelTransition++)
	{
		std::cerr<<"Přechod: "<<modelTransition->second->getName()<<std::endl;
	}	
	//std::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it
	//for(listOfLinks = listOfLinks.begin(); listOfLinks != listOfLinks.end(); listOfLinks)
	for(modelLink = listOfLinks.begin(); modelLink != listOfLinks.end(); modelLink++)
	{
		
		std::cerr<<"Linka z: "<<(*modelLink)->getInput()->getName()<<" do: "<<(*modelLink)->getOutput()->getName()<<std::endl;
		//listOfLinks->second->
	}	
}