/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor Petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	model.cpp		        	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

/**
 * @file model.cpp
 * @brief Soubor obsahující třídu reprezentující model Petriho sítě, který bude simulován.
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

#include "model.h"

/**
 * Výčet konstant použitých v programu
 */
enum constants
{
	DEFAULT_PLACE_CAPACITY = 0,
	DEFAULT_TRANSITION_VALUE = 0,
	DEFAULT_TOKEN_CNT = 1
};

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
	std::map<std::string, Place *>::iterator iterPlace; // iterátor pro průchod seznamem míst
	std::map<std::string, Transition *>::iterator iterTransition; // iterátor pro průchod seznamem přechodů
	std::vector <Token *>::iterator iterToken; // iterátor pro průchod polem tokenů
	std::vector<Link *> ::iterator iterLink; // iterátor pro průchod seznamem hran
	
	std::vector<Token*>* listOfTokens = Token::getTokens();

	// průchod míst
	for(iterPlace = Place::getPlaces()->begin(); iterPlace != Place::getPlaces()->end(); iterPlace++)
		delete(iterPlace->second);
	Place::getPlaces()->clear();
	
	// průchod přechodů
	for(iterTransition = Transition::getTransitions()->begin(); iterTransition != Transition::getTransitions()->end(); iterTransition++)
		delete(iterTransition->second);
	Transition::getTransitions()->clear();

	// průchod tokenů
	for(iterToken = listOfTokens->begin(); iterToken != listOfTokens->end(); iterToken++)
		delete (*iterToken);
	listOfTokens->clear();
	 
	// průchod hran		
	for(iterLink = Link::getLinks()->begin(); iterLink != Link::getLinks()->end(); iterLink++)
		delete(*iterLink);
	Link::getLinks()->clear();	
}

/**
 * Přidá místo do modelu.
 * @param name jméno místa pro přidání
 */
void Model::addPlace(std::string name)
{
	Model::addPlace(name, DEFAULT_PLACE_CAPACITY);
}

/**
 * Přidá místo do modelu.
 * @param name jméno místa
 * @param capacity kapacita místa
 */
void Model::addPlace(std::string name, int capacity)
{
	new Place(name, capacity);

}

/**
 * Přidá přechod do modelu
 * @param name jméno přechodu
 * @param value parametr typu přechodu (čas, hodnota pravděpodobnosti, priorita)
 * @param type typ přechodu (časovaný, pravděpodobnostní, prioritní)
 */
void Model::addTransition(std::string name, int value, Transition::Type type)
{
		new Transition(name, value, type);
}

/**
 * Přidá přechod do modelu
 * @param name název přechodu
 */
void Model::addTransition(std::string name)
{
	Model::addTransition(name, DEFAULT_TRANSITION_VALUE, Transition::PRIORITY);
}

/**
 * Přidá hranu do modelu
 * @param inputName název místa/přechodu na vstupní části hrany
 * @param outputName název místa/přechodu na výstupní části hrany
 * @param capacity kapacita hrany
 */
void Model::addLink(std::string inputName, std::string outputName, int capacity)
{
	new Link(inputName, outputName, capacity); // vytvoření hrany
}

/**
 * Přidá zadaný počet tokenů do místa zadaného jménem.
 * @param placeName název místa, kam má být token vložen
 * @param count počet vložench tokenů
 */
void Model::addToken(std::string placeName, int count)
{
	Place * place;
	Token *token;

	// pokud zadané místo neexistuje
	if((place = Place::getPlace(placeName)) == NULL)
	{
		std::cerr<<"Nelze vložit značku do neexistujícího místa."<<std::endl;
		throw 1;
	}
	
	// vložení tokenu do místa count-krát
	for(int i = 0; i < count; i++)
	{
		token = new Token(place);
		place->addToken(token);
	}
}

/**
 * Přidá jeden token do místa zadaného jménem.
 * @param placeName název místa
 */
void Model::addToken(std::string placeName)
{
	return Model::addToken(placeName, DEFAULT_TOKEN_CNT); 
}

/**
 * Ověřeí, zda byl model syntakticky správně zapsán.
 */
void Model::modelValidate()
{
	Place *place; // ukazatel na místo
	Transition *transition; // ukazatel na přechod
	Link *link; // ukazatel na hranu
	double isFirstPriorityOrDelay = false; // první v seznamu je prioritní nebo časovaný přechod
	int probabilityValueCnt = 0; // součet pravděpodobností převděpodobnostních přechodů
	
	// deklarace iterátorů pro místa, přechody a výstupní hrany
	std::map<std::string, Place *>::iterator iterPlace;
	std::map<std::string, Transition *>::iterator iterTransition;
	std::vector<Link *> ::iterator iterOutputLink;
	std::vector<Link *> ::iterator iterOutputLink_2;
	
	// získání seznamu míst a přechodů
	std::map<std::string, Place *> *listOfPlaces = Place::getPlaces();
	std::map<std::string, Transition *> *listOfTransitions = Transition::getTransitions();

	// postupné procházení jednotlivých přechodů
	for (iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		// získání dílčího přechodu
		transition = (*iterTransition).second;
		
		// pokud přechod nemá alespoň jednu vstupní nebo výstupní hranu
		if(!(transition->getInputLinkCount() >= 1 || transition->getOutputLinkCount() >= 1))
		{
			std::cerr<<"Přechod \""<< transition->getName() <<"\" nemá alespoň jednu vstupní nebo výstupní hranu!"<<std::endl;
			throw 1;
		}
	}
	
	// postupné procházení jednotlivých míst 
	for (iterPlace = listOfPlaces->begin(); iterPlace != listOfPlaces->end(); iterPlace++)
	{
		// získání dílčího místa
		place = (*iterPlace).second;
		
		// vypsání místa
		// ???std::cerr<<"DEBUG:Místo: "<<place->getName()<<std::endl;
		
		// pokud počet tokenů v místě přesahuje jeho kapacitu
		if(place->getTokenCount() > place->getCapacity() && place->getCapacity() != 0)
		{
			std::cerr<<"Počet značek v místě \""<< place->getName() <<"\" přesahuje jeho kapacitu!"<<std::endl;
			throw 1;
		}
		// pokud místo nemá alespoň jednu vstupní nebo výstupní hranu
		if(!(place->getInputLinkCount() >= 1 || place->getOutputLinkCount() >= 1))
		{
			std::cerr<<"Místo \""<< place->getName() <<"\" nemá ani vstupní ani výstupní hranu!"<<std::endl;
			throw 1;
		}
		
		// získání seznamu výstupních hran z právě procházeného místa
		std::vector<Link *> *outputLinks = place->getOutputLinks();
		
		// pokud má místo nějaké výstupní hrany
		if (outputLinks->size() != 0)
		{
			// pokud je první procházený přechod prioritní nebo časovaný (tak i všechny ostatní musí být 
			// tohoto typu, nastaven příznak typu prvního na true)
			if (((Transition *)((*outputLinks->begin())->getOutput()))->getTransitionType() != Transition::PROBABILITY)
				isFirstPriorityOrDelay = true;

			// postupné procházení hran vedoucích z právě zpracovávaného místa
			for(iterOutputLink = outputLinks->begin(); iterOutputLink != outputLinks->end(); iterOutputLink++ )
			{
				// právě procházená hrana
				link = *iterOutputLink;
				
				// přechodu na konci procházené hrany
				transition = ((Transition *)((*iterOutputLink)->getOutput()));

				// pokud je typ právě procházeného přechodu PROBABILITY
				if (transition->getTransitionType() == Transition::PROBABILITY) 
				{
					// pokud již byl nalezen časovaný nebo prioritní typ přechodu připojený na dané místo
					if (isFirstPriorityOrDelay == true)
						std::cerr<<"Místo \""<< place->getName() <<"\" kombinuje časovaný nebo prioritní přechod s pravděpodobnostním!"<<std::endl;				
				
					// pokud na vstupu pravděpodobnostního přechodu není právě jedno místo
					if (transition->getInputLinkCount() != 1)
						std::cerr<<"Na vstupu pravděpodobnostního přechodu \""<< transition->getName() <<"\" smí být pouze jedno místo!"<<std::endl;				
					
					// pokud na vstupu pravděpodobnostního přechodu není hrana s impilicitní kapacitou
					if(transition->getInputLinks()->front()->getCapacity() != 1)
						std::cerr<<"Na vstupu pravděpodobnostního přechodu \""<< transition->getName() <<"\" nesmí být hrana s kapacitou!"<<std::endl;				

					// pokud existují výstupní hrany
					if(transition->getOutputLinkCount() > 0)
					{
						// naplnění pole linek výstupními linkami přechodu
						std::vector<Link *> *outputLinksTransition = transition->getOutputLinks();

						// procházení výstupních linek přechodu a ověřování kapacity míst
						for (iterOutputLink_2 = outputLinksTransition->begin(); iterOutputLink_2 != outputLinksTransition->end(); iterOutputLink_2++)
						{
							link = *iterOutputLink_2;
							if(((Place*)(link->getOutput()))->getCapacity() > 0)
								std::cerr<<"Na výstupu pravděpodobnostního přechodu \""<< transition->getName() <<"\" nesmí být místo s kapacitou!"<<std::endl;				

						}	
					}
					probabilityValueCnt += transition->getValue();					
				}
				// pokud je typ právě procházeného přechodu TIMED_EXP, TIMED_CONST nebo PRIORITY
				else
				{
					// pokud již byl nalezen pravděpodobnostní typ přechodu připojený na dané místo
					if (isFirstPriorityOrDelay == false)
						std::cerr<<"Místo \""<< place->getName() <<"\" kombinuje časovaný nebo prioritní přechod s pravděpodobnostním!"<<std::endl;
				}
			}
			
			// pokud je součet pravděpodobnostních přechodů různých od 100 & byly zpracovávány pravděpodobnostních přechody
			if(probabilityValueCnt != 100 && probabilityValueCnt != 0)
				std::cerr<<"Součet pravděpodobnostních přechodů není roven 100!"<<std::endl;
		}
		
		isFirstPriorityOrDelay = false;	// reset příznaku prioritního nebo časovaného přechodu
		probabilityValueCnt = 0;	// reset součtu hodnot pravděpodobnostních přechodů
	}
}

/**
 * Vytiskne počty tokenů ve všech místech modelu.
 */
void Model::printTokenCount()
{
	std::map<std::string, Place *>::iterator it; // iterátor pro průchod seznamem všech míst modelu
	std::map<std::string, Place *> *listOfPlaces = Place::getPlaces(); // získání seznamu všech míst modelu
	
	std::cout<<"Počet tokenů v místech:"<<std::endl;
	// postupné procházení seznamu všech míst modelu
	for(it = listOfPlaces->begin(); it != listOfPlaces->end(); it++)
		std::cout<<"| "<<it->second->getName()<<" ("<<it->second->getTokenCount()<<") ";
	
	std::cout<<"|"<<std::endl;
}

/**
 * Vytiskne přehled celého modelu
 */
void Model::printModel()
{
	// deklarace iterátorů pro průchod polí míst, přechodů a hran
	std::map<std::string, Place *>::iterator modelPlace;
	std::map<std::string, Transition *>::iterator modelTransition;
	std::vector<Link *>::iterator modelLink;
	
	// průchod míst
	for(modelPlace = Place::getPlaces()->begin(); modelPlace != Place::getPlaces()->end(); modelPlace++)
		std::cerr<<"Místo: "<<modelPlace->second->getName()<<std::endl;
	
	// průchod přechodů
	for(modelTransition = Transition::getTransitions()->begin(); modelTransition != Transition::getTransitions()->end(); modelTransition++)
		std::cerr<<"Přechod: "<<modelTransition->second->getName()<<" (type:"<<modelTransition->second->getTransitionType()<<")"<<std::endl;

	// průchod hran
	for(modelLink = Link::getLinks()->begin(); modelLink != Link::getLinks()->end(); modelLink++)
		std::cerr<<"Linka z: "<<(*modelLink)->getInput()->getName()<<" do: "<<(*modelLink)->getOutput()->getName()<<std::endl;
}

/**
 * Vytiskne souhrnné statistiky
 */
void Model::printAllStats()
{
	// deklarace iterátorů pro průchod polí míst, přechodů a hran
	std::map<std::string, Place *>::iterator modelPlace;
	std::map<std::string, Transition *>::iterator modelTransition;
	std::vector<Link *>::iterator modelLink;
	
	std::cout<<std::endl;
	std::cout<<"Výpis souhrnné statistiky pro místa a přechody"<<std::endl;
	
	// průchod míst
	for(modelPlace = Place::getPlaces()->begin(); modelPlace != Place::getPlaces()->end(); modelPlace++)
	{
		modelPlace->second->printStats(); // vytisknutí statistiku procházeného místa	
	}
	
	// průchod přechodů pro časované přechody
	for(modelTransition = Transition::getTransitions()->begin(); modelTransition != Transition::getTransitions()->end(); modelTransition++)
	{	
		if(modelTransition->second->getTransitionType() == Transition::TIMED_EXP ||
		   modelTransition->second->getTransitionType() == Transition::TIMED_CONST)
			modelTransition->second->printTimedStats(); // vytisknutí statistiku procházeného časovaného přechodu
	}
	
	// průchod přechodů
	for(modelTransition = Transition::getTransitions()->begin(); modelTransition != Transition::getTransitions()->end(); modelTransition++)
	{
		if(!(modelTransition->second->getTransitionType() == Transition::TIMED_EXP ||
		   modelTransition->second->getTransitionType() == Transition::TIMED_CONST))
			modelTransition->second->printStats(); // vytisknutí statistiku procházeného nečasovaného přechodu
	}	
		
}

