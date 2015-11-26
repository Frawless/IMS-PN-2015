/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	simulator.cpp             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/

#include "simulator.h"

#include <cmath>
#include <algorithm>

using namespace std; 

/**
 * Konstruktor simulátoru
 */
Simulator::Simulator()
{
	this->model = new Model();
}

/**
 * Vytvoření modelu Petriho sítě
 */
void Simulator::createModel()
{
	
	model->addPlace("m_pocitace");
	model->addPlace("m_hraci");
	model->addPlace("m_hrajici");
	model->addPlace("m_dohrali");
	model->addPlace("m_nerozhodnuti");
	model->addPlace("m_cekajici");
	model->addPlace("m_odchazejici");
	model->addPlace("m_znovu_prichozi");
	 

	model->addTransition("p_hrac_prichazi", 10,Transition::TIMED_EXP);
	model->addTransition("p_jde_hrat", 1, Transition::PRIORITY);
	model->addTransition("p_hraje", 100, Transition::TIMED_EXP);
	model->addTransition("p_dohral_odchazi", 0, Transition::PRIORITY);

	model->addTransition("p_nemuze_hrat",0, Transition::PRIORITY);
	model->addTransition("p_chce_cekat", 60,Transition::STOCHASTIC);
	model->addTransition("p_muze_hrat", 2,Transition::PRIORITY);

	model->addTransition("p_odchazi", 40, Transition::STOCHASTIC);
	model->addTransition("p_uplne_odchazi", 80, Transition::STOCHASTIC);
	model->addTransition("p_chce_prijit_znovu",  20,Transition::STOCHASTIC);
	model->addTransition("p_znovu_prichazi", 25,Transition::TIMED_EXP);

	model->addLink("p_hrac_prichazi","m_hraci",1);
	model->addLink("m_hraci","p_jde_hrat",1);
	model->addLink("m_pocitace", "p_jde_hrat",1);
	model->addLink("p_jde_hrat","m_hrajici",1);
	model->addLink("m_hrajici","p_hraje",1);
	model->addLink("p_hraje", "m_dohrali",1);
	model->addLink("m_dohrali", "p_dohral_odchazi",1);
	model->addLink("p_hraje", "m_pocitace",1);

	model->addLink("m_hraci","p_nemuze_hrat",1);
	model->addLink("p_nemuze_hrat", "m_nerozhodnuti",1);
	model->addLink("m_nerozhodnuti", "p_chce_cekat",1);
	model->addLink("p_chce_cekat", "m_cekajici",1);
	model->addLink("m_cekajici", "p_muze_hrat",1);
	model->addLink("m_pocitace", "p_muze_hrat",1);
	model->addLink("p_muze_hrat", "m_hrajici",1);

	model->addLink("m_nerozhodnuti", "p_odchazi",1);
	model->addLink("p_odchazi", "m_odchazejici",1);
	model->addLink("m_odchazejici", "p_uplne_odchazi",1);
	model->addLink("m_odchazejici", "p_chce_prijit_znovu",1);
	model->addLink("p_chce_prijit_znovu", "m_znovu_prichozi",1);
	model->addLink("m_znovu_prichozi", "p_znovu_prichazi",1);
	model->addLink("p_znovu_prichazi", "m_hraci", 2);

	model->addToken("m_pocitace",10);
	
	/*model->addPlace("místo_1");
	model->addPlace("místo_2");
	model->addPlace("místo_3");
	
	//model->addTransition("přechod_1");
	model->addTransition("přechod_1");
	
	model->addTransition("přechod_2");
	model->addTransition("přechod_3");
	//model->addTransition("přechod_3", 1, Transition::PRIORITY);
	model->addTransition("přechod_4", 50, Transition::PRIORITY);
	model->addTransition("přechod_5", 50, Transition::STOCHASTIC);
	
	model->addLink("místo_3","přechod_1", 0);
	model->addLink("místo_3","přechod_3", 0);
	
	model->addLink("přechod_1","místo_1", 0);
	model->addLink("místo_1","přechod_2", 0);
	model->addLink("místo_3","přechod_2", 0);
	model->addLink("přechod_2","místo_2", 0);
	
	model->addLink("místo_2","přechod_4",1);
	model->addLink("místo_2","přechod_5",1);
	
	//Token * t = model->addToken("místo_1");
	
	
	
	model->addToken("místo_1");*/

	/*Token::printTokens();
	
	Place::getPlace("místo_1")->printTokens();
	Place::getPlace("místo_1")->removeToken(t);
	model->removeToken(t);
	std::cerr<<"print"<<endl;
	Token::printTokens();
	//Place::getPlace("místo_1")->printTokens();*/
}

/**
 * Vrácení ukazatele na model
 * @return ukazatel na model
 */
Model *Simulator::getModel()
{
	return this->model;
}

/**
 * Vytisknutí modelu
 */
void Simulator::printModel()
{
	this->model->printModel();
}

/**
 * 
 * @param simTime
 */
void Simulator::setSimTime(double simTime)
{
	this->simTime = simTime;
}

/**
 * 
 * @param maxSimTime
 */
void Simulator::setMaxSimTime(double maxSimTime)
{
	this->maxSimTime = maxSimTime;
}

/**
 * 
 * @return 
 */
Calendar *Simulator::getCalendar()
{
	return this->calendar;
}

/**
 * 
 */
void Simulator::simStart()
{
	model->printTokenCount();
	this->performTransitions();
}

/**
 * 
 * @param transition
 */
void Simulator::performTransition(Transition *transition)
{	
	//když je vykonán přechod vezmou se značky ze všech vstupů a dají se do všech výstupů
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem linek
	std::vector<Link *> *listOfInputLinks =  transition->getInputLinks(); // seznam vstupních linek	přechodu
	std::vector<Link *> *listOfOutputLinks =  transition->getOutputLinks(); // seznam výstupních linek	přechodu
	Token * token;
	Place *place;
	
	std::vector<Token *> checkTokens; // vektor vektoru ukazatelů všech přechodů
	std::vector<Token *> tokensToMove; // vektor tokenů přo přesun
	std::vector<Token *>::iterator iterPlaceTokens; //iterátor pro průchod seznamem přechodů
	std::vector<Token *> *listOfTokens; // seznam přechodů
	
	// průchodu seznemu vstupních linek
	for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
	{
		place = ((Place*)((*iterLink)->getInput()));
		listOfTokens = place->getTokens();

		// získání tokentu z místa
		// vložení ukazatelů na všechny přechody do vektoru přechodů
		if(!listOfTokens->empty())
		{
			std::cerr<<"DEBUG: přesouvám token z místa: "<<place->getName()<<std::endl;
			//std::cerr<<listOfTokens->size()<<std::endl;
			// ??? tady možná natane problém při jednom tokenu v poli
			// procházení seznamu tokenů daného místa
			for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
			{
				//std::cerr<<"V cyklu"<<std::endl;
				checkTokens.push_back((*iterPlaceTokens));
			}
			// shuffle s vectorem
			std::random_shuffle(checkTokens.begin(), checkTokens.end());	
			//získání náhodného tokenu ze vstupního místa
			token = checkTokens.back();
			checkTokens.pop_back();
			//smazání tokenu ze vstupního místa
			place->removeToken(token);
			tokensToMove.push_back(token);
		}		
	}

	// průchodu seznamu výstupních linek
	for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
	{
		std::cerr<<"DEBUG: vkládám token do místa: "<<place->getName()<<std::endl;
		// vložení tokenu na výtupní místo
		token = tokensToMove.back();
		tokensToMove.pop_back();
		place->addToken(token);		
	}	
}

/**
 * 
 */
void Simulator::performTransitions()
{
	Transition *transition, *tmpTransition;
	Place * place;
	double wait;
	unsigned int random;
	unsigned int tmp;
	
	std::vector<Transition *> checkTransitions; // vektor vektoru ukazatelů všech přechodů
	std::map<std::string, Transition *>::iterator iterTransition; //iterátor pro průchod seznamem přechodů
	std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions(); // seznam přechodů
	
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem linek
	std::vector<Link *> *listOfOutputLinks; // seznam výstupních linek	přechodu

	// vložení ukazatelů na všechny přechody do vektoru přechodů
	for(iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		checkTransitions.push_back(iterTransition->second);
	}
	
	// shuffle s vectorem
	std::random_shuffle(checkTransitions.begin(), checkTransitions.end());

	while(!checkTransitions.empty())
	{
		// získán ukazatel na vybraný přechod
		transition = checkTransitions.back();
		checkTransitions.pop_back();
		
		switch(transition->getTransitionType())
		{
			case Transition::TIMED_CONST:
				std::cerr<<"Const"<<std::endl;
				wait = transition->getValue(); // nastavení hodnoty zpoždění
				// vložit do kalendáře
				std::cerr<<wait<<std::endl;
				planTransition(transition, wait);
				break;
			case Transition::TIMED_EXP:
				std::cerr<<"EXP"<<std::endl;		
				// vygenerovat exponenciální zpoždění a vložit do kelenadáře
				wait = Exponential(transition->getValue());
				// vložit do kalendáře
				std::cerr<<wait<<std::endl;
				planTransition(transition, wait);
				break;
			case Transition::PRIORITY:
				std::cerr<<"Priority"<<std::endl;
				
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());
				
				listOfOutputLinks =  place->getOutputLinks(); // seznam výstupních linek	přechodu

				tmp = transition->getValue();
				// průchodu seznamu výstupních linek daného místa
				for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
				{
					//kontrola priorit
					tmpTransition = ((Transition*)((*iterLink)->getOutput()));
					if(tmp < tmpTransition->getValue())
						transition = tmpTransition;
					
					tmp = transition->getValue();
				}
				std::cerr<<"DEBUG: Vybrán přechod \""<<transition->getName()<<"\" s prioritou: "<<transition->getValue()<<std::endl;
				
				//vykonání přechodu
				this->performTransition(transition);
				this->clearPerformedTransition(); //nastavení všech přechodů na false
				break;
			case Transition::STOCHASTIC:
				std::cerr<<"Stochastic"<<std::endl;
				
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());

				listOfOutputLinks =  place->getOutputLinks(); // seznam výstupních linek	přechodu
				
				//random hodnota 1-100
				random = rand()%100+1;
				tmp = 0;
				
				// průchodu seznamu výstupních linek daného místa
				for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
				{
					//vybrání stochastického přechodu
					if(tmp < random)	
					{
						// aktualizuju přechod jen v případě, když random hodnota je vyšší než hodnota předchozího přechodu
						tmpTransition = ((Transition*)((*iterLink)->getOutput()));
						tmp += tmpTransition->getValue();
					}

				}
				transition = tmpTransition;
				std::cerr<<"DEBUG: Vybrán přechod \""<<transition->getName()<<"\" s % hodnotou: "<<transition->getValue()<<" Na základě random čísla: "<<random<<std::endl;
				
				
				//this->performTransition(transition);
				this->clearPerformedTransition(); //nastavení všech přechodů na false
				break;
		}
	}
}

/**
 * 
 * @param transition
 * @param wait
 */
void Simulator::planTransition(Transition *transition, double wait)
{
	Event *event;
	Place *place;
	Link *link;
	Token *token;
	
	std::vector<Token *> checkTokens; // vektor vektoru ukazatelů všech přechodů
	std::vector<Token *>::iterator iterPlaceTokens; //iterátor pro průchod seznamem přechodů
	std::vector<Token *> *listOfTokens; // seznam přechodů
	
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem linek
	std::vector<Link *> *listOfLinks =  transition->getInputLinks(); // seznam vstupních linek	přechodu

	event = new Event(this->simTime+wait, wait, transition); // vytvoření události
	
	// vložení ukazatelů na všechny přechody do vektoru přechodů
	for(iterLink = listOfLinks->begin(); iterLink != listOfLinks->end(); iterLink++)
	{
		link = (*iterLink);
		place = (Place*)(*iterLink)->getInput();
		listOfTokens = place->getTokens();
		
		// vložení ukazatelů na všechny přechody do vektoru přechodů
		for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
		{
			checkTokens.push_back((*iterPlaceTokens));
		}

		// shuffle s vectorem
		std::random_shuffle(checkTokens.begin(), checkTokens.end());
		
		// ošetření prázdnosti místa -> v simulaci by snad nemělo nastat
		if(checkTokens.empty())
		{
			delete event;
			return;
		}
			
		for(int i = 0; i < link->getCapacity(); i++)
		{
			// získání  náhodného tokenu
			token = checkTokens.back();
			if(token->isTokenProcessedByTransition(transition))
				continue;
			
			if(checkTokens.empty())
			{
				delete event;
				return;
			}
			
			token->tokenProcessedByTransition(transition);
			token->setFlag(true);
			event->addTokenToEvent(token);
			checkTokens.pop_back(); // 
		}
		this->calendar->addEvent(event);
	}
}

/**
 * Metoda pro vykonán přechodu
 * 1 - smazat značky ze vstupního místa
 * 2 - vložit značky na výstupní místo 
 */

/**
 * Metoda pro plánování  událostí v kalendáři
 */

//PlanTransitionEvents->planTimedEvent->addEvent

/**
 * 
 * @return 
 */
double Simulator::Random()
{
	return rand()/(RAND_MAX+1.0);
}

/**
 *  SIMLIB
 * @param mv
 * @return 
 */
double Simulator::Exponential(double mv)
{
  double exp = -mv * std::log(Random());
//  _Print("Exponential(%g),%g = %g\n", mv, r, exp);
  return exp;
}

/**
 * 
 */
void Simulator::clearPerformedTransition()
{
	std::map<std::string, Transition *>::iterator iterTransition;
	std::map<std::string, Transition *> *listOfTransitions = Transition::getTransitions();
	
	for(iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		(*iterTransition).second->setIsPerformed(false);
	}
}

int main()
{
	// vytvoření instance simulátoru
	Simulator *simulator = new Simulator();
	
	try
	{
		simulator->createModel(); // vytvoření modelu
		std::cerr<<"Model zapsán"<<std::endl;
		std::cerr<<"Zacinam validovat"<<std::endl;
		simulator->getModel()->modelValidate();
		std::cerr<<"Model zvalidován"<<std::endl;
		
		simulator->simStart();
		
		std::cerr<<"Vypišuji model"<<std::endl;
		//simulator->getModel()->printModel(); // vytisknutí modelu
	}
	
	// ošetření výjimek
	catch(int e)
	{
	  
		switch(e)
		{
			case 1:
				std::cerr<<"Error"<<std::endl;
		}
	}
	
	exit(EXIT_SUCCESS);
  
}