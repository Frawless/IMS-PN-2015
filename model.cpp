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
		std::cerr<<"Přechod: "<<modelTransition->second->getName()<<" (type:"<<modelTransition->second->getTransitionType()<<")"<<std::endl;
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
		
	 */
	token->removeToken(token);
}

/**
 * Ověření, zda je model syntakticky správný.
 */
void Model::modelValidate()
{
	Place *place;  // ukazatel na místo
	Transition *transition; // ukazatel na přechod
	Link *link; // ukazatel na hranu
	double isFirstPriorityOrDelay = false;
	int stochasticValueCnt = 0;
	
	// deklarace iterátorů pro místa, přechody a výstupní hrany
	std::map<std::string, Place *>::iterator iterPlace;
	std::map<std::string, Transition *>::iterator iterTransition;
	std::vector<Link *> ::iterator iterOutputLink;
	std::vector<Link *> ::iterator iterOutputLink_2;
	
	// získání seznamu míst a přechodů
	std::map<std::string, Place *> *listOfPlaces =  Place::getPlaces();
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions();

	/// postupné procházení jednotlivých přechodů
	for (iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		// získání dílčího přechodu
		transition = (*iterTransition).second;
		
		// pokud přechod nemá alespoň jednu vstupní nebo výstupní hranu
		if(!(transition->getInputLinkCount() >= 1 || transition->getOutputLinkCount() >= 1))
		{
			std::cerr<<"Přechod \""<< transition->getName() <<"\" nemá alespoň jednu vstupní nebo  výstupní hranu!"<<std::endl;
			throw 1;
		}
		
		// řeším dole
		/*// pokud se jedná o časovaný nebo prioritní přechod (TIMED_EXP, TIMED_CONST, STOCHASTIC, PRIORITY)
		if(transition->getTransitionType() != Transition::STOCHASTIC)
		{
			//ověření okolních přechodů
			for(int i = 0; i < transition->getInputLinkCount(); i++)
			{
				
			}
		}*/
	}
	
	// postupné procházení jednotlivých míst 
	for (iterPlace = listOfPlaces->begin(); iterPlace != listOfPlaces->end(); iterPlace++)
	{
		// získání dílčího místa
		place = (*iterPlace).second;
		
		// vypsání místa
		//???std::cerr<<"DEBUG:Místo: "<<place->getName()<<std::endl;
		
		// pokud počet tokenů v místě přesahuje jeho kapacitu
		if(place->getTokenCount() > place->getCapacity()  && place->getCapacity() != 0)
		{
			std::cerr<<"Počet značek v místě \""<< place->getName() <<"\" přesahuje jeho kapacitu!"<<std::endl;
			throw 1;
		}
		/// pokud místo nemá alespoň jednu vstupní nebo výstupní hranu
		if(!(place->getInputLinkCount() >= 1 || place->getOutputLinkCount() >= 1))
		{
			std::cerr<<"Místo \""<< place->getName() <<"\" nemá ani vstupní ani výstupní hranu!"<<std::endl;
			throw 1;
		}
		
		// získání seznamu výstupních hran z právě procházeného místa
		std::vector<Link *> *outputLinks = place->getOutputLinks();
		
		// pokud má místo nějaké výstupní hrany
		if (outputLinks->size()  != 0)
		{
			// pokud je první procházený přechod prioritní nebo časovaný (tak i všechny ostatní musí být 
			// tohoto typu, nastaven příznak typu prvního na  true
			if (((Transition *)((*outputLinks->begin())->getOutput()))->getTransitionType() != Transition::STOCHASTIC)
				isFirstPriorityOrDelay = true;

			// postupné procházení hran vedoucích z právě zpracovávaného místa
			for(iterOutputLink = outputLinks->begin(); iterOutputLink != outputLinks->end(); iterOutputLink++ )
			{
				//std::cerr<<(Transition *)((*iterOutputLink)->getOutput())<<std::endl;
				// právě procházená hrana
				link = *iterOutputLink;
				// přiřazení přechodu na konci procházené hrany
				transition = ((Transition *)((*iterOutputLink)->getOutput()));

				// pokud je typ právě procházeného přechodu STOCHASTIC
				if (transition->getTransitionType() == Transition::STOCHASTIC) 
				{

					// pokud již byl nalezen časovaný nebo priotiní typ přechodu připojený na dané místo
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
					stochasticValueCnt += transition->getValue();					
				}
				// pokud je typ právě procházeného přechodu TIMED_EXP, TIMED_CONST nebo PRIORITY
				else
				{
					// pokud již byl nalezen pravděpodobnostní typ přechodu připojený na dané místo
					if (isFirstPriorityOrDelay == false)
						std::cerr<<"Místo \""<< place->getName() <<"\" kombinuje časovaný nebo prioritní přechod s pravděpodobnostním!"<<std::endl;
				}
			}
			
			if(stochasticValueCnt != 100 && stochasticValueCnt != 0)
				std::cerr<<"Součet pravděpodobnostních přechodů není roven 100!"<<std::endl;
		}
		isFirstPriorityOrDelay = false;	// reset hodnoty 
		stochasticValueCnt = 0;	// reset součtu pravděpodobnostních přechodů
	}
}

/**
 * 
 */
void Model::printTokenCount()
{
	std::map<std::string, Place *>::iterator it;		//iterátor pro průchod polem tokenů
	std::map<std::string, Place *> *listOfPlaces =  Place::getPlaces();
	//prohledání pole tokenů
	for(it = listOfPlaces->begin(); it != listOfPlaces->end(); it++)
	{
		std::cerr<<"Místo: "<<it->second->getName()<<"Počet tokenů: "<<it->second->getTokenCount()<<std::endl;
	}
}

/**
 * 
 */
int Model::getTransitionCount()
{
	int transitions = 0;
	std::map<std::string, Transition *>::iterator it;		//iterátor pro průchod polem tokenů
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions();
	
	//prohledání pole tokenů
	for(it = listOfTransitions->begin(); it != listOfTransitions->end(); it++)
	{
		transitions += 1;
	}
	
	return transitions;
}

/**
 * 
 * @param random
 * @return 
 */
Transition * Model::getTransitionToParse(int random)
{
	std::map<std::string, Transition *>::iterator it;		//iterátor pro průchod polem tokenů
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions();
	
	int i = 0;
	it = listOfTransitions->begin();
	
	//prohledání pole tokenů a hledání příslušného podle random	
	while(i < (random))
	{
		it++;
		i++;
	}

	return (*it).second;
}
