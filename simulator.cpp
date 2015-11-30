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
#include <climits>
#include <sys/types.h>
#include <unistd.h>

using namespace std; 
unsigned Simulator::ix = 0; // ???

/**
 * Konstruktor simulátoru
 */
Simulator::Simulator()
{
	this->calendar = new Calendar(); // vytvoření nové instance kalendář
	this->model = new Model(); // vytvoření nové instance modelu
	srand((unsigned int)(time(0)+getpid())); // definice chování funkce rand())
	ix = rand()%UINT_MAX; // ???
	this->simTime = 0; // inicializace simulačního času
}

/**
 * Destruktur simulátoru
 */
Simulator::~Simulator()
{
	delete calendar;
	delete model;
}

/**
 * Nastaví simulační čas na hodnotu zadanou parametrem.
 * @param simTime nastavovaný simulační čas
 */
void Simulator::setSimTime(double simTime)
{
	this->simTime = simTime;
}

/**
 * Nastaví maximální simulační čas
 * @param maxSimTime maximální hodnota simulačního času
 */
void Simulator::setMaxSimTime(double maxSimTime)
{
	this->maxSimTime = maxSimTime;
}

/**
 * Vytvoří model Petriho sítě
 */
void Simulator::createModel()
{
	// vymyslet jiný model ???
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
}

/**
 * Vrátí ukazatel na model
 * @return ukazatel na model
 */
Model *Simulator::getModel()
{
	return this->model;
}

/**
 * Vytiskne model
 */
void Simulator::printModel()
{
	this->model->printModel();
}

/**
 * Zahájí simulaci modelu.
 */
void Simulator::simStart() // ??? upravit komentáře po dokončení
{
	Event *event; // událost kalendáře
	
	model->printTokenCount();
	std::cerr<<"DEBUG: Začátek simulace."<<std::endl;
	std::cerr<<"DEBUG: Hodnota simulačního času: "<<this->simTime<<std::endl;
	
	// vykonání nečasovaných přechodů a nastavení časovaných
	this->performTransitions();
	
	// pokud je kalendář neprázdný
	while(!this->calendar->isEmpty())
	{	
		model->printTokenCount();
		std::cerr<<"DEBUG: Hodnota simulačního času: ----------------->"<<this->simTime<<std::endl;
		this->calendar->printCalendar();
		
		//std::cerr<<"DEBUG: Kalendář není prázdný1"<<std::endl;
		
		// pokud je přesažena maximální hodnota simulačního času
		if(this->simTime > this->maxSimTime)
		{
			std::cerr<<"DEBUG: Konec simulace"<<std::endl;
			std::cout<<"Konec simulace! Ukončuji simulátor..."<<std::endl;
			std::cerr<<"DEBUG: Hodnota simulačního času: "<<this->simTime<<std::endl;
			exit(0);
		}
		//std::cerr<<"DEBUG: Kalendář není prázdný2"<<std::endl;
		// vybrání události z kalendáře
		event = this->calendar->getEvent();
		this->simTime = event->getTime();
		//std::cerr<<"DEBUG: Kalendář není prázdný3"<<std::endl;
		/**/
		//vykonání časovaného přechodu dané události
		this->performTransitionFromEvent(event);
		//std::cerr<<"DEBUG: Kalendář není prázdný4"<<std::endl;
		
		// smazání provedené události z kalendáře
		this->calendar->deleteEvent(event);
		//std::cerr<<"DEBUG: Kalendář není prázdný5"<<std::endl;
		//std::cerr<<this->calendar->isEmpty()<<std::endl;
		
		// vykonání všechn nečasovaných přechodů a nastavení časovaných přechodů
		this->performTransitions(); 
		//std::cerr<<"DEBUG: Kalendář není prázdný6"<<std::endl;
	}
}

/**
 * Vrátí ukazatel na kalendář událostí.
 * @return ukazatel na kalendář událostí
 */
Calendar *Simulator::getCalendar()
{
	return this->calendar;
}

/**
 * Vykoná přechod z události kalendáře zadané ukazatelem.
 * @param event
 */
void Simulator::performTransitionFromEvent(Event *event)
{	
	/*Token * token; // ukazatel na token
	Place *place; // ukazatel na místo
	std::vector<Link *>::iterator iterLink; // iterátor pro průchod seznamem hran
	std::vector<Token *>::iterator iterPlaceTokens; // iterátor pro průchod seznamem přechodů
	std::vector<Token *> checkTokens; // pomocný seznam tokenů v místě
	std::vector<Token *> *listOfTokens; // seznam všech tokenů v místě*/
	
	// získání přechodu z požadované události
	Transition *transition = event->getTransition();
	this->performTransition(transition);
/*
	std::vector<Link *> *listOfInputLinks =  transition->getInputLinks(); // seznam vstupních hran přechodu
	std::vector<Link *> *listOfOutputLinks =  transition->getOutputLinks(); // seznam výstupních hran přechodu
	
	// pokud je vykonán přechod, vezmou se značky ze všech vstupů a dají se do všech výstupů
	if(transitionCanBePerformed(transition) )
	{
		// postupný průchod seznamem vstupních hran
		for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
		{
			place = ((Place*)((*iterLink)->getInput())); // místo na vstupu hrany
			listOfTokens = place->getTokens(); // seznam tokenů v místě na vstupu hrany

			// pokud jsou v místě nějaké tokeny
			if(!listOfTokens->empty())
			{
				std::cerr<<"DEBUG: PerformOnly->přesouvám token z místa: "<<place->getName()<<std::endl;
				//std::cerr<<listOfTokens->size()<<std::endl;
				// ??? tady možná natane problém při jednom tokenu v poli
				
				// postupné procházení seznamu tokenů v aktuálním místě
				for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
				{
					//std::cerr<<"V cyklu"<<std::endl;
					// vložení aktuálního tokenu v aktuálním místě do pomocného seznamu tokenů v místě
					checkTokens.push_back((*iterPlaceTokens));
				}
				// zamíchání prvků pomocného seznamu tokenů v aktuálním místě
				std::random_shuffle(checkTokens.begin(), checkTokens.end());	
				
				//získání náhodného tokenu ze vstupního místa (náhodný, jelikož byl seznam zamíchán)
				token = checkTokens.back();
				checkTokens.pop_back(); // odstranění získaného tokenu z pomocného seznamu tokenů v aktuálním místě
				
				// smazání tokenu z aktulálního vstupního místa
				place->removeToken(token);
				
				//???model->removeToken(token);
				std::cerr<<"DEBUG: Počet značek v místě \""<<place->getName()<<"\" : "<<place->getTokenCount()<<std::endl;
				std::cerr<<"Mažu hrače ze systému"<<std::endl;
			}
			checkTokens.clear(); // vymazání pomocného seznamu tokenů v aktuálním místě			
		}
	}
	// pokud seznam vstupních hran není prázdný ??? proč
	else if(!listOfInputLinks->empty())
	{
		return;
	}

	// procházení seznamu výstupních linek
	for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
	{
		for(int i = 0; i < (*iterLink)->getCapacity(); i++)
		{
			std::cerr<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;
			// získání místa na výstupu hrany
			place = ((Place*)((*iterLink)->getOutput()));
			std::cerr<<"DEBUG: EVENT->vkládám token do místa: "<<place->getName()<<std::endl;	

			// přidání tokenu do místa 
			model->addToken(place->getName());
			std::cerr<<"DEBUG: Počet značek v místě \""<<place->getName()<<"\" : "<<place->getTokenCount()<<std::endl;
		}
	}	*/
	// ???
	event->getTransition()->setIsTimed(false);
}


/**
 * Vykoná přechod zadaný parametrem.
 * @param transition ukazatel na přechod k vykonání
 */
void Simulator::performTransition(Transition *transition)
{	
	Token * token; // ukazatel na token
	Place *place; // ukazatel na místo
	
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem hran
	std::vector<Token *>::iterator iterPlaceTokens; //iterátor pro průchod seznamem přechodů
	std::vector<Token *> checkTokens; // pomocný seznam tokenů v místě
	std::vector<Token *> *listOfTokens; // seznam všech tokenů v místě
	
	std::vector<Link *> *listOfInputLinks =  transition->getInputLinks(); // seznam vstupních hran přechodu
	std::vector<Link *> *listOfOutputLinks =  transition->getOutputLinks(); // seznam výstupních hran přechodu
	
	// pokud je vykonán přechod vezmou se značky ze všech vstupů a dají se do všech výstupů
	if(transitionCanBePerformed(transition))
	{
		// postupný průchod seznamem vstupních hran
		for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
		{
			place = ((Place*)((*iterLink)->getInput())); // místo na vstupu hrany
			listOfTokens = place->getTokens(); // seznam tokenů v místě na vstupu hr

			// pokud jsou v místě nějaké tokeny
			if(!listOfTokens->empty())
			{
				std::cerr<<"DEBUG: PerformOnly->přesouvám token z místa: "<<place->getName()<<std::endl;
				//std::cerr<<listOfTokens->size()<<std::endl;
				// ??? tady možná natane problém při jednom tokenu v poli
				
				// postupné procházení seznamu tokenů v aktuálním místě
				for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
				{
					//std::cerr<<"V cyklu"<<std::endl;
					// vložení aktuálního tokenu v aktuálním místě do pomocného seznamu tokenů v místě
					checkTokens.push_back((*iterPlaceTokens));
				}
				// zamíchání prvků pomocného seznamu tokenů v aktuálním místě
				std::random_shuffle(checkTokens.begin(), checkTokens.end());	
				
				//získání náhodného tokenu ze vstupního místa (náhodný, jelikož byl seznam zamíchán)
				token = checkTokens.back();
				checkTokens.pop_back(); // odstranění získaného tokenu z pomocného seznamu tokenů v aktuálním místě
				
				// smazání tokenu z aktulálního vstupního místa
				place->removeToken(token);
				std::cerr<<"DEBUG: Počet značek v místě \""<<place->getName()<<"\" : "<<place->getTokenCount()<<std::endl;

			}		
			checkTokens.clear(); // vymazání pomocného seznamu tokenů v aktuálním místě		
		}
	}
	// pokud seznam vstupních hran není prázdný ??? proč
	else if(!listOfInputLinks->empty())
	{
		return;
	}
	
	// získání místa na výstupu hrany
	for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
	{
		for(int i = 0; i < (*iterLink)->getCapacity(); i++)
		{
			place = ((Place*)((*iterLink)->getOutput()));
			std::cerr<<"DEBUG: PerformOnly->vkládám token do místa: "<<place->getName()<<std::endl;

			// přidání tokenu do místa
			model->addToken(place->getName());	
			std::cerr<<"DEBUG: Počet značek v místě \""<<place->getName()<<"\" : "<<place->getTokenCount()<<std::endl;		
		}
	}	
}

/**
 * Vykoná přechody ??? jaké, které
 */
void Simulator::performTransitions()
{
	Transition *transition, *tmpTransition;
	Place * place; // ukazatel na místo
	double wait; // doba zpoždění přechodu
	unsigned int random;
	unsigned int priority;
	//std::map<std::string, Transition *>::iterator iterTransition; //iterátor pro průchod seznamem přechodů
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem hran
	std::vector<Transition *> checkTransitions; // pomocný seznam přechodů
	
	//std::map<std::string, Transition *> *listOfTransitions =  Transition::getTransitions(); // seznam všech tokenů v místě
	std::vector<Link *> *listOfOutputLinks; // seznam výstupních linek	přechodu

	// postupné procházení přechodů a vložení jejich ukazatelů do pomocného seznamu přechodů
	/*for(iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
		checkTransitions.push_back(iterTransition->second);*/
	
	checkTransitions =Transition::getRandomVectorTransitions();
	
	// zamíchání prvků pomocného seznamu přechodů
	//std::random_shuffle(checkTransitions.begin(), checkTransitions.end());

	// pokud seznam přechodů není prázdný
	while(!checkTransitions.empty())
	{
		//std::random_shuffle(checkTransitions.begin(), checkTransitions.end());
		// získání ukazatele na náhodný přechod
		transition = checkTransitions.back();
		
		
		// rozdělení dle typu přechodu
		switch(transition->getTransitionType())
		{
			case Transition::TIMED_CONST:
				//std::cerr<<"DEBUG: Const"<<std::endl;
				wait = transition->getValue(); // získání hodnoty zpoždění přechodu
				
				std::cerr<<"DEBUG: Vygenerováno zpoždění pro přechod \""<<transition->getName()<<"\" (CONST) : "<<wait<<std::endl;
				
				if(!this->transitionCanBePerformed(transition))
				{
					checkTransitions.pop_back();
					break;
				}
					
				
				// vložení náhodně vybraného přechodu do kalendáře	
				planEvents(transition, wait);
				checkTransitions.pop_back();
				break;
			case Transition::TIMED_EXP:
				//std::cerr<<"DEBUG: EXP"<<std::endl;		
				// vygenerování exponenciálního zpoždění
				wait = Exponential(transition->getValue());

				std::cerr<<"DEBUG: Vygenerováno zpoždění pro přechod \""<<transition->getName()<<"\" (EXP) : "<<wait<<std::endl;
				
				if(!this->transitionCanBePerformed(transition) && !transition->getInputLinkCount() == 0)
				{
					checkTransitions.pop_back();
					break;
				}
				
				// vložení náhodně vybraného přechodu do kalende
				planEvents(transition, wait);
				checkTransitions.pop_back();
				break;
			case Transition::PRIORITY:
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());
				
				listOfOutputLinks =  place->getOutputLinks(); // seznam výstupních hran	místa

				// získání hodnoty priority přechodu z náhodného přechodu
				priority = transition->getValue();
				
				// průchod seznamem výstupních hran z daného místa
				for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
				{
					// získání priority přechodu
					tmpTransition = ((Transition*)((*iterLink)->getOutput()));
					
					// pokud nemůže být přechod proveden, jdeme na další iteraci
					if(!transitionCanBePerformed(tmpTransition))
						continue;
					
					// pokud je priorita priotia aktuálního přechodu větší priorita náhodného přechodu
					if(priority < tmpTransition->getValue())
						transition = tmpTransition;
					
					priority = transition->getValue();
					std::cerr<<"Vypiš prioritu přechodu: "<<priority<<std::endl;
				}
				//std::cerr<<"DEBUG: Vybrán přechod \""<<transition->getName()<<"\" s prioritou: "<<transition->getValue()<<std::endl;
				std::cerr<<"DEBUG: Hodnota přechodu \""<<transition->getName()<<"\" (PRIORITY) : "<<transition->getValue()<<std::endl;
				//vykonání přechodu
				if(!this->transitionCanBePerformed(transition))
				{
					checkTransitions.pop_back();
					break;
				}
				
				this->performTransition(transition); //vykonání zadaného přechodu
				//this->clearPerformedTransition(); // nastavení všech přechodů na false ???
				checkTransitions =Transition::getRandomVectorTransitions();
				break;
			case Transition::STOCHASTIC:
				//std::cerr<<"DEBUG: Stochastic"<<std::endl;
				
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());

				listOfOutputLinks =  place->getOutputLinks(); // seznam výstupních hran z místa
				
				// random hodnota 1-100
				random = rand()%100+1;
				priority = 0;
				
				// průchod seznamem výstupních hran daného místa
				for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
				{
					// vybrání stochastického přechodu
					if(priority < random)	
					{
						// aktualizace přechodu jen v případě, když random hodnota je vyšší než hodnota předchozího přechodu
						tmpTransition = ((Transition*)((*iterLink)->getOutput()));
						priority += tmpTransition->getValue();
					}

				}
				transition = tmpTransition;
				//std::cerr<<"DEBUG: Vybrán přechod \""<<transition->getName()<<"\" s % hodnotou: "<<transition->getValue()<<" Na základě random čísla: "<<random<<std::endl;
				std::cerr<<"DEBUG: Hodnota přechodu \""<<transition->getName()<<"\" (STOCHASTIC) : "<<transition->getValue()<<std::endl;
				
				if(!this->transitionCanBePerformed(transition))
				{
					checkTransitions.pop_back();
					break;
				}
				
				this->performTransition(transition); // vykonání zvoleného přechodu
				//this->clearPerformedTransition(); // nastavení všech přechodů na false
				checkTransitions =Transition::getRandomVectorTransitions();
				break;
		}
	}
}

/**
 * Vymaže u všech přechodů přízank isPerformed 
 */
void Simulator::clearPerformedTransition()
{
	std::map<std::string, Transition *>::iterator iterTransition; // iterátor pro průchod přechodů
	std::map<std::string, Transition *> *listOfTransitions = Transition::getTransitions(); // získání seznamu všech přechodů modelu
	
	// postupné procházení všech přechodů
	for(iterTransition = listOfTransitions->begin(); iterTransition != listOfTransitions->end(); iterTransition++)
	{
		(*iterTransition).second->setIsPerformed(false);
	}
}

/**
 * Naplánování zadaného přechodu ???
 * @param transition přechod pro naplánování
 * @param wait zpoždění přechodu
 */
void Simulator::planTransition(Transition *transition, double wait)
{
	Event *event;
	Place *place;
	Link *link;
	Token *token;
	
	std::vector<Token *> *listOfTokens; // seznam tokenů v místě
	std::vector<Token *> checkTokens; // pomocný seznam všech tokenů v místě
	std::vector<Token *>::iterator iterPlaceTokens; //iterátor pro průchod seznamem přechodů
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem linek
	
	std::vector<Link *> *listOfLinks =  transition->getInputLinks(); // seznam vstupních hran přechodu
	
	event = new Event(this->simTime+wait, wait, transition); // vytvoření události
	
	//std::cerr<<"DEBUG: planTransition->přidávání do kalendáře1"<<std::endl;
	// postupné procházení seznamu všech vstupních hran přechodu
	for(iterLink = listOfLinks->begin(); iterLink != listOfLinks->end(); iterLink++)
	{
		//std::cerr<<"DEBUG: planTransition->přidávání do kalendáře2"<<std::endl;
		link = (*iterLink); // hrana
		place = (Place*)(*iterLink)->getInput(); // vstupní místo
		listOfTokens = place->getTokens(); // seznam tokenů v místě
		
		// vložení ukazatelů na všechny přechody do vektoru přechodů
		//std::cerr<<listOfTokens->size()<<std::endl;
		// postupné procházení seznamu všech tokenů v místě
		for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
		{
			// pokud je token už nastaven někde
			// ?X?
			// tohle bude asi trošku chybka, máme funkci  "isTokenProcessedByTransition" a měla by to dělat ona, nejspíš špatně funguje nebo tak -> zkus to opravit kdybych se k tomu endostal navečer
			// doplnit koment ???
			if((*iterPlaceTokens)->getFlag() == true)
				continue;
			//std::cerr<<"DEBUG: planTransition->přidávání do kalendáře3"<<std::endl;
			//??? nebude sort?? + koment
			checkTokens.push_back((*iterPlaceTokens));
		}
		
		// pokud nejsou v místě žádné tokeny (???v simulaci by snad nemělo nastat)
		if(checkTokens.empty())
		{
			delete event;
			return;
		}
		
		// zamíchání pomocného seznamu všech tokenů v místě
		std::random_shuffle(checkTokens.begin(), checkTokens.end());
			
		// postupné zvyšování kapacity (i) až do maximální kapacity hrany
		for(int i = 0; i < link->getCapacity(); i++)
		{					
			while(1)
			{
				// získání  náhodného tokenu
				token = checkTokens.back();
				
				// ??? koment
				if(token->isTokenProcessedByTransition(transition))
				{
					checkTokens.pop_back(); // smazání z pomocného seznamu tokenů v místě
					continue;
				}
				else	
					break;
					
				// pokud je seznam tokenů prázdný
				if(checkTokens.empty())
				{
					delete event;
					return;
				}				
			}			
			
			// získání  náhodného tokenu
			token = checkTokens.back();				
			
			// ??? komentáře násled 4 řádky
			token->tokenProcessedByTransition(transition);
			token->setFlag(true);
			event->addTokenToEvent(token);
			checkTokens.pop_back(); 
		}
		checkTokens.clear(); // vymazání pomocného seznamu všech tokenů v místě
	}
	// ??? koment
	transition->setIsTimed(true);
	std::cerr<<"DEBUG: Do kalendáře vložen Event s přechodem: "<<transition->getName()<<std::endl;
	this->calendar->addEvent(event);	
	std::cerr<<"DEBUG: Provádění eventu: Start -> "<<this->simTime<<" Konec -> "<<this->simTime+wait<<std::endl;
}

/**
 * Naplánování zadaného přechodu.
 * @param transition přechod k naplánování
 * @param wait zpoždění přechodu
 */
void Simulator::planEvents(Transition* transition, double wait)
{
	Place *place, *tmpPlace;
	double tmp = 6666; // ??? nešlo by jinak?
	
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem hran
	std::vector<Link *> *listOfLinks = transition->getInputLinks(); // seznam vstupních hran přechodu

	//std::cerr<<"DEBUG: Exponenciální přechod"<<std::endl;
	// žádná vtupní linka -> vstupní přechod -> jedna událost
	
	// pokud neexistují vstupní hrany ???
	if(transition->getInputLinks()->empty())
	{
		// ???
		if(!transition->getIsTimed())
			//std::cerr<<"DEBUG: planEvent ->"<<transition<<std::endl;
			this->planTransition(transition, wait); // ???
	}
	else
	{
		// ???
		// postupné procházení všech hran
		for(iterLink = listOfLinks->begin(); iterLink != listOfLinks->end(); iterLink++)
		{
			tmpPlace = ((Place*)(*iterLink)->getInput()); // získání vstupního místa
			
			// ????
			if(tmp > tmpPlace->getTokenCount() / (*iterLink)->getCapacity())
				place = tmpPlace;
			
			// ????
			tmp = tmpPlace->getTokenCount() / (*iterLink)->getCapacity();
		}
		
		// pro každý token v místě
		for(int i = 0; i < place->getTokenCount(); i++)
		{
			//std::cerr<<"DEBUG: planEvents ->"<<transition<<std::endl;
			// naplánování přechodu
			this->planTransition(transition, wait);
			
			// ?? není tady chyba????????????????????????????,
			if(transition->getTransitionType() != Transition::TIMED_CONST)
			{
				wait = Exponential(transition->getValue());
			}
		}
	}		
}

/**
 * ???
 * @param transition ukazatel na ověřovaný přechod
 * @return ???
 */
bool Simulator::transitionCanBePerformed(Transition *transition)
{
	bool canBePerformed = false;
	//když je vykonán přechod vezmou se značky ze všech vstupů a dají se do všech výstupů
	Place *place;
	Link *link;
	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem hran
	std::vector<Token *>::iterator iterPlaceTokens; //iterátor pro průchod seznamem přechodů
	std::vector<Link *> *listOfInputLinks =  transition->getInputLinks(); // seznam vstupních hran	přechodu
	std::vector<Token *> checkTokens; // vektor vektoru ukazatelů všech přechodů
	
	// průchodu seznamem vstupních hran
	for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
	{
		link = (*iterLink); // aktuální hrana
		place = ((Place*)(link->getInput())); // vstupní místo aktuální linky
		
		// pokudj je počet tokenů >= kapacitě hrany
		if(place->getTokenCount() >= link->getCapacity())
			canBePerformed = true;
		else
			canBePerformed = false;
		
		// pokud nelze přechod vykonat
		if(!canBePerformed)
			break;
	}
	
	return canBePerformed;
}

/**
 * Získání náhodného čísla ???? interal??
 * @return 
 */
double Simulator::Random()
{
	return rand()/(RAND_MAX+1.0);
}

/**
 * Výpočte hodnotu exponenciálního rozložení se středem mv (převzato ze SIMLIB)
 * @param mv střed exponenciálního rozložení
 * @return hodnota exponenciálního rozložení
 */
double Simulator::Exponential(double mv)
{
  double exp = -mv * std::log(Random());
//  ??? _Print("Exponential(%g),%g = %g\n", mv, r, exp);
  return exp;
}

/**
 * Main programu
 * @return návratový kód chyby
 */
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
		
		simulator->setMaxSimTime(10000);
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
	
	delete simulator;
	exit(EXIT_SUCCESS);
  
}