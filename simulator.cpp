/************************************************
*	 Projekt: 	Projekt do předmětu IMS		   * 
* 					Simulátor Petriho sítí	   *
*	Autoři:	Jakub Stejskal <xstejs24>		   *
*		   	Petr Staněk <xstane34>		   *
*	Nazev souboru: 	simulator.cpp			   *
*			Datum:  7. 12. 2015			   *
*			Verze:	1.0		                     *
************************************************/

/**
 * @file simulator.cpp
 * @brief Soubor obsahující třídu reprezentující simulátor Petriho sítě.
 * @author Staněk Petr <xstane34@stud.fit.vutbr.cz>
 * @author Stejskal Jakub <xstejs24@stud.fit.vutbr.cz>
 */

#include "simulator.h"

#include <cmath>
#include <algorithm>
#include <climits>
#include <sys/types.h>
#include <unistd.h>

using namespace std; 

/**
 * Konstruktor simulátoru
 */
Simulator::Simulator()
{
	srand(time(0)+time_seed());
	this->calendar = new Calendar(); // vytvoření nové instance kalendář
	this->model = new Model(); // vytvoření nové instance modelu
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
 * Nastaví maximální simulační čas.
 * @param maxSimTime maximální hodnota simulačního času
 */
void Simulator::setMaxSimTime(double maxSimTime)
{
	this->maxSimTime = maxSimTime;
}

/**
 * Vytvoří model Petriho sítě
 */
void Simulator::createModel(bool  modelCase)
{
	if(modelCase)
	{
		// model ze cvičení - Překladiště
		// přechody
		model->addTransition("p_generator_aut",60, Transition::TIMED_EXP);
		model->addTransition("p_prijezd_auta",0, Transition::PRIORITY);
		model->addTransition("p_vykladka",1, Transition::PRIORITY);
		model->addTransition("p_volna_rampa",0, Transition::PRIORITY);
		model->addTransition("p_plneni_vlaku",0, Transition::PRIORITY);
		model->addTransition("p_obsluha_jerabu",2, Transition::TIMED_EXP);
		model->addTransition("p_vlak_odjede",0, Transition::PRIORITY);

		// místě
		model->addPlace("m_transakce");
		model->addPlace("m_rampa");
		model->addPlace("m_kontejnery");
		model->addPlace("m_jerab");
		model->addPlace("m_kapacita_vlaku");
		model->addPlace("m_naklad");
		model->addPlace("m_obsluha_jerabu");
		model->addPlace("m_pocitadlo");

		// hrany
		model->addLink("p_generator_aut","m_transakce",1);
		model->addLink("m_transakce","p_prijezd_auta",1);
		model->addLink("m_rampa","p_prijezd_auta",1);
		model->addLink("p_prijezd_auta","m_kontejnery",20);
		model->addLink("p_prijezd_auta","m_jerab",1);

		model->addLink("m_jerab","p_volna_rampa",1);
		model->addLink("m_jerab","p_vykladka",1);
		model->addLink("p_volna_rampa","m_rampa",1);
		model->addLink("p_vykladka","m_naklad",1);
		model->addLink("m_naklad","p_plneni_vlaku",1);

		model->addLink("m_kontejnery","p_vykladka",1);

		model->addLink("p_plneni_vlaku","m_obsluha_jerabu",1);
		model->addLink("m_obsluha_jerabu","p_obsluha_jerabu",1);
		model->addLink("p_obsluha_jerabu","m_jerab",1);

		model->addLink("p_obsluha_jerabu","m_pocitadlo",1);
		model->addLink("m_pocitadlo","p_vlak_odjede",100);
		model->addLink("p_vlak_odjede","m_kapacita_vlaku",100);
		model->addLink("m_kapacita_vlaku","p_plneni_vlaku",1);

		model->addToken("m_kapacita_vlaku",100);
		model->addToken("m_rampa",1);
	}
	else
	{
		// Model ze cvičení - Herna
		// přechody
		model->addTransition("p_generator_hracu", 15,Transition::TIMED_EXP);
		model->addTransition("p_generator_poruch", 300,Transition::TIMED_EXP);
		model->addTransition("p_hraní", 100,Transition::TIMED_EXP);
		model->addTransition("p_opravovani_zavazne_poruchy", 50,Transition::TIMED_CONST);
		model->addTransition("p_opravovani_normalni_poruchy", 4,Transition::TIMED_CONST);

		model->addTransition("p_pocitac_neni_volny", 0,Transition::PRIORITY);
		model->addTransition("p_jde_hrat", 1,Transition::PRIORITY);
		model->addTransition("p_nelze_hrat_porucha", 5,Transition::PRIORITY);
		model->addTransition("p_nastala_porucha_pri_hrani", 1,Transition::PRIORITY);
		model->addTransition("p_porucha_opravena", 10,Transition::PRIORITY);

		model->addTransition("p_dohral_odchazi", 0,Transition::PRIORITY);
		model->addTransition("p_nehral_odchazi", 0,Transition::PRIORITY);
		model->addTransition("p_porucha_opousti_system", 0,Transition::PRIORITY);

		model->addTransition("p_zavazna_porucha", 15,Transition::PROBABILITY);
		model->addTransition("p_normalni_porucha", 85,Transition::PROBABILITY);

		// místa
		model->addPlace("m_hrac_prisel");
		model->addPlace("m_hraje");
		model->addPlace("m_pocitace");
		model->addPlace("m_dohrali");
		model->addPlace("m_odchazejici");
		model->addPlace("m_oprava_poruchy");
		model->addPlace("m_porucha_opousti_system");
		model->addPlace("m_porucha_opravena");
		model->addPlace("m_oprava_zavazne_poruchy");
		model->addPlace("m_oprava_normalni_poruchy");
		model->addPlace("m_indikátor_poruchy");

		// tokeny
		model->addToken("m_pocitace",10);

		// hrany
		model->addLink("p_generator_hracu","m_hrac_prisel",1);
		model->addLink("m_hrac_prisel","p_pocitac_neni_volny",1);
		model->addLink("p_pocitac_neni_volny","m_odchazejici",1);
		model->addLink("m_odchazejici","p_nehral_odchazi",1);

		model->addLink("m_hrac_prisel","p_jde_hrat",1);
		model->addLink("p_jde_hrat","m_hraje",1);
		model->addLink("m_hraje","p_hraní",1);
		model->addLink("p_hraní","m_dohrali",1);
		model->addLink("m_dohrali","p_dohral_odchazi",1);

		model->addLink("m_hrac_prisel","p_nelze_hrat_porucha",1);
		model->addLink("p_nelze_hrat_porucha","m_odchazejici",1);
		model->addLink("m_odchazejici","p_nehral_odchazi",1);

		model->addLink("p_nelze_hrat_porucha","m_indikátor_poruchy",1);
		model->addLink("m_indikátor_poruchy","p_nelze_hrat_porucha",1);

		model->addLink("p_generator_poruch","m_indikátor_poruchy",1);	
		model->addLink("p_generator_poruch","m_oprava_poruchy",1);
		model->addLink("m_oprava_poruchy","p_zavazna_porucha",1);
		model->addLink("m_oprava_poruchy","p_normalni_porucha",1);
		model->addLink("p_zavazna_porucha","m_oprava_zavazne_poruchy",1);
		model->addLink("p_normalni_porucha","m_oprava_normalni_poruchy",1);

		model->addLink("m_oprava_normalni_poruchy","p_opravovani_normalni_poruchy",1);
		model->addLink("m_oprava_zavazne_poruchy","p_opravovani_zavazne_poruchy",1);

		model->addLink("p_opravovani_normalni_poruchy","m_porucha_opravena",1);
		model->addLink("p_opravovani_zavazne_poruchy","m_porucha_opravena",1);

		model->addLink("m_porucha_opravena","p_porucha_opravena",1);
		model->addLink("p_porucha_opravena","m_porucha_opousti_system",1);
		model->addLink("m_porucha_opousti_system","p_porucha_opousti_system",1);	
		model->addLink("m_indikátor_poruchy","p_porucha_opravena",1);

		model->addLink("m_indikátor_poruchy","p_nastala_porucha_pri_hrani",1);
		model->addLink("p_nastala_porucha_pri_hrani","m_indikátor_poruchy",1);

		model->addLink("m_pocitace","p_jde_hrat",1);

		model->addLink("p_hraní","m_pocitace",1);	

		model->addLink("p_nastala_porucha_pri_hrani","m_dohrali",1);
		model->addLink("m_hraje","p_nastala_porucha_pri_hrani",1);
		model->addLink("p_nastala_porucha_pri_hrani","m_pocitace",1);
	}
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
void Simulator::simStart() 
{
	Event *event; // událost kalendáře
	
	// výpis začátek simulace
	std::cout<<"Začátek provádění simulace v čase: "<<this->simTime<<std::endl;
	std::cout<<std::endl;
	//model->printModel();
	
	// vykonání nečasovaných přechodů a nastavení časovaných
	this->performTransitions();
	
	// pokud je kalendář neprázdný
	while(!this->calendar->isEmpty())
	{	
		//model->printTokenCount();
		//this->calendar->printCalendar();
		//std::cerr<<this->calendar->getEvents()->size()<<std::endl;
		
		// vybrání události z kalendáře
		event = this->calendar->getEvent();
		
		
		// pokud je přesažena maximální hodnota simulačního času
		if(event->getTime() > this->maxSimTime)
		{
			// výpis konce simulace
			std::cout<<"Konec simulace v čase: "<<this->simTime<<std::endl;
			this->model->printAllStats();
			return;
		}

		this->simTime = event->getTime();
		// vykonání časovaného přechodu dané události
		this->performTransitionFromEvent(event);
		// 
		
		// smazání provedené události z kalendáře
		this->calendar->deleteEvent(event);
		// vykonání všechn nečasovaných přechodů a nastavení časovaných přechodů
		this->performTransitions(); 
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
 * @param event ukazatel na přechod k vykonání
 */
void Simulator::performTransitionFromEvent(Event *event)
{	
	// získání přechodu z požadované události
	Transition *transition = event->getTransition();
	transition->setGeneratedValue(event->getWait());
	this->performTransition(transition);
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
	
	std::vector<Link *>::iterator iterLink; // iterátor pro průchod seznamem hran
	std::vector<Token *>::iterator iterPlaceTokens; // iterátor pro průchod seznamem přechodů
	std::vector<Token *> checkTokens; // pomocný seznam tokenů v místě
	std::vector<Token *> *listOfTokens; // seznam všech tokenů v místě
	
	std::vector<Link *> *listOfInputLinks = transition->getInputLinks(); // seznam vstupních hran přechodu
	std::vector<Link *> *listOfOutputLinks = transition->getOutputLinks(); // seznam výstupních hran přechodu
	
	// pokud je vykonán přechod vezmou se značky ze všech vstupů a dají se do všech výstupů
	if(transitionCanBePerformed(transition))
	{		
		// Výpočet statistiky pro přechod
		transition->setStats();
		// Vypsání informací o prováděném přechodu
		std::cout<<"Proveden přechod: "<<transition->getName()<<" v čase: "<<this->simTime<<std::endl;
		
		// postupný průchod seznamem vstupních hran
		for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
		{
			place = ((Place*)((*iterLink)->getInput())); // místo na vstupu hrany
			listOfTokens = place->getTokens(); // seznam tokenů v místě na vstupu hr

			// pokud jsou v místě nějaké tokeny
			if(!listOfTokens->empty())
			{
				// postupné procházení seznamu tokenů v aktuálním místě
				for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
					// vložení aktuálního tokenu v aktuálním místě do pomocného seznamu tokenů v místě
					checkTokens.push_back((*iterPlaceTokens));
				
				for(int i = 0; i < (*iterLink)->getCapacity(); i++)
				{
					// zamíchání prvků pomocného seznamu tokenů v aktuálním místě
					std::random_shuffle(checkTokens.begin(), checkTokens.end());	

					// získání náhodného tokenu ze vstupního místa (náhodný, jelikož byl seznam zamíchán)
					token = checkTokens.back();
					checkTokens.pop_back(); // odstranění získaného tokenu z pomocného seznamu tokenů v aktuálním místě					

					// smazání tokenu z aktulálního vstupního místa
					place->removeToken(token);

					// Výpočet statistiky pro místo
					place->setStats();

					//token->printTransitions();
					if(transition != token->getTransition() && token->getTransition() != NULL)
					{
						this->deleteEventByTransition(token->getTransition());
					}
				}
			}		
			checkTokens.clear(); // vymazání pomocného seznamu tokenů v aktuálním místě
			std::cout<<"	Přesun tokenu z místa: "<<place->getName()<<std::endl;		
		}
	}
	// pokud se nejedná o přechod bez vstupních linek (== nejedná se o generátor)
	else if(!listOfInputLinks->empty())
		return;
	else if(listOfInputLinks->empty())
	{
		// Výpočet statistiky pro přechod
		transition->setStats();
		// Vypsání informací o prováděném přechodu
		std::cout<<"Proveden přechod: "<<transition->getName()<<" v čase: "<<this->simTime<<std::endl;
	}

	// získání místa na výstupu hrany
	for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
	{
		for(int i = 0; i < (*iterLink)->getCapacity(); i++)
		{
			place = ((Place*)((*iterLink)->getOutput()));
			// přidání tokenu do místa
			model->addToken(place->getName());		
			
			// Výpis informací o provedení přechodu
			std::cout<<"	Vložení tokenu do místa: "<<place->getName()<<std::endl;
		
			// Výpočet statistiky pro místo
			place->setStats();
		}
	}
	// Vypsání počtu tokenů v jednotlivých místech
	model->printTokenCount();
	std::cout<<std::endl;	
}

/**
 * Pokusí se vykonat všechny přechody.
 */
void Simulator::performTransitions()
{
	Transition *transition, *tmpTransition;
	Place * place; // ukazatel na místo
	double wait; // doba zpoždění přechodu
	unsigned int random;
	unsigned int priority;

	std::vector<Link *>::iterator iterLink; //iterátor pro průchod seznamem hran
	std::vector<Transition *> checkTransitions; // pomocný seznam přechodů
	
	std::vector<Link *> *listOfOutputLinks; // seznam výstupních linek	přechodu

	// postupné procházení přechodů a vložení jejich ukazatelů do pomocného seznamu přechodů
	checkTransitions =Transition::getRandomVectorTransitions();
	
	// pokud seznam přechodů není prázdný
	while(!checkTransitions.empty())
	{
		// získání ukazatele na náhodný přechod
		transition = checkTransitions.back();
		
		// rozdělení dle typu přechodu
		switch(transition->getTransitionType())
		{
			case Transition::TIMED_CONST:
				wait = transition->getValue(); // získání hodnoty zpoždění přechodu
				checkTransitions.pop_back();
				
				// ověření proveditelnosti přechodu
				if(!this->transitionCanBePerformed(transition))					
					break;
				
				// vložení náhodně vybraného přechodu do kalendáře	
				planEvents(transition, wait);
				break;
			case Transition::TIMED_EXP:	
				// vygenerování exponenciálního zpoždění
				wait = Exponential(transition->getValue());
				checkTransitions.pop_back();

				// ověření proveditelnosti přechodu
				if(!this->transitionCanBePerformed(transition) && !transition->getInputLinkCount() == 0)
					break;
			
				// vložení náhodně vybraného přechodu do kalende
				planEvents(transition, wait);
				break;
			case Transition::PRIORITY:
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());
				
				listOfOutputLinks = place->getOutputLinks(); // seznam výstupních hran	místa

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
					//std::cerr<<"Vypiš prioritu přechodu: "<<priority<<std::endl;
				}
				//vykonání přechodu
				if(!this->transitionCanBePerformed(transition))
				{
					checkTransitions.pop_back();
					break;
				}
				
				this->performTransition(transition); //vykonání zadaného přechodu
				checkTransitions =Transition::getRandomVectorTransitions();
				break;
			case Transition::PROBABILITY:
				
				// získání vstupního místa
				place = (Place*)(transition->getInputLinks()->front()->getInput());

				listOfOutputLinks = place->getOutputLinks(); // seznam výstupních hran z místa
				
				// random hodnota 1-100
				random = rand()%100+1;
				priority = 0;
				
				// průchod seznamem výstupních hran daného místa
				for(iterLink = listOfOutputLinks->begin(); iterLink != listOfOutputLinks->end(); iterLink++)
				{
					// vybrání pravděpodobnostního přechodu
					if(priority < random)	
					{
						// aktualizace přechodu jen v případě, když random hodnota je vyšší než hodnota předchozího přechodu
						tmpTransition = ((Transition*)((*iterLink)->getOutput()));
						priority += tmpTransition->getValue();
					}

				}
				transition = tmpTransition;
				// ověření proveditelnosti přechodu
				if(!this->transitionCanBePerformed(transition))
				{
					checkTransitions.pop_back();
					break;
				}
				
				this->performTransition(transition); // vykonání zvoleného přechodu
				checkTransitions =Transition::getRandomVectorTransitions();
				break;
		}
	}
}

/**
 * Naplánuje přechod zadaný ukazatelem a vloží ho do kalendáře.
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
	std::vector<Token *>::iterator iterPlaceTokens; // iterátor pro průchod seznamem přechodů
	std::vector<Link *>::iterator iterLink; // iterátor pro průchod seznamem linek
	
	std::vector<Link *> *listOfLinks = transition->getInputLinks(); // seznam vstupních hran přechodu
	
	event = new Event(this->simTime+wait, wait, transition); // vytvoření události
	
	// postupné procházení seznamu všech vstupních hran přechodu
	for(iterLink = listOfLinks->begin(); iterLink != listOfLinks->end(); iterLink++)
	{
		link = (*iterLink); // hrana
		place = (Place*)(*iterLink)->getInput(); // vstupní místo
		listOfTokens = place->getTokens(); // seznam tokenů v místě
		
		// vložení ukazatelů na všechny přechody do vektoru přechodů
		// postupné procházení seznamu všech tokenů v místě
		for(iterPlaceTokens = listOfTokens->begin(); iterPlaceTokens != listOfTokens->end(); iterPlaceTokens++)
		{
			// pokud je už token nastaven jinde
			if((*iterPlaceTokens)->getFlag() == true)
				continue;

			// plnění pomocného seznamu tokenů (s flag na false)
			checkTokens.push_back((*iterPlaceTokens));
		}
		
		// pokud nejsou v místě žádné tokeny
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
				// získání náhodného tokenu
				token = checkTokens.back();
				
				if(token->getFlag())
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
			// získání náhodného tokenu
			token = checkTokens.back();				
			
			token->tokenProcessedByTransition(transition); // nastaví, že token je zpracováván přechodem
			token->setFlag(true); // nastavení příznaku k tokenu, že je zpracováván
			event->addTokenToEvent(token); // přidání tokenu do události
			checkTokens.pop_back(); // vymazání tokenu ze seznamu tokenů ke zpracování
		}
		checkTokens.clear(); // vymazání pomocného seznamu všech tokenů v místě
	}
	transition->setIsTimed(true); // nastavení příznaku k přechodu, že je načasován
	this->calendar->addEvent(event); // přidání události do kalendáře
}

/**
 * Naplánování zadaného přechodu.
 * @param transition přechod k naplánování
 * @param wait zpoždění přechodu
 */
void Simulator::planEvents(Transition* transition, double wait)
{
	Place *place;
	
	std::vector<Link *>::iterator iterLink; // iterátor pro průchod seznamem hran
	std::vector<Link *> *listOfLinks = transition->getInputLinks(); // seznam vstupních hran přechodu

	// žádná vtupní linka -> vstupní přechod -> jedna událost
	
	// pokud neexistují vstupní hrany
	if(transition->getInputLinks()->empty())
	{
		// pokud přechod není v nějaké události
		if(!transition->getIsTimed())
			// std::cerr<<"DEBUG: planEvent ->"<<transition<<std::endl;
			this->planTransition(transition, wait); // přechod se naplánuje do kalendáře
	}
	else
	{
		std::random_shuffle(listOfLinks->begin(), listOfLinks->end());
		
		// postupné procházení všech hran
		for(iterLink = listOfLinks->begin(); iterLink != listOfLinks->end(); iterLink++)
		{
			// získání místa ze seznamu linek
			place = ((Place*)(*iterLink)->getInput());
			
			// pro každý token v místě
			for(int i = 0; i < place->getTokenCount(); i++)
			{
				// std::cerr<<"DEBUG: planEvents ->"<<transition<<std::endl;
				// naplánování přechodu
				this->planTransition(transition, wait);

				// pokud se jedná o časovaný přechod
				if(transition->getTransitionType() == Transition::TIMED_EXP)
				{
					wait = Exponential(transition->getValue());
				}
			}			
		}
	}		
}
/**
 * Ověří, zda může být přechod zadaný parametrem vykonán.
 * @param transition ukazatel na ověřovaný přechod
 * @return true - pokud může být vykonán
 */
bool Simulator::transitionCanBePerformed(Transition *transition)
{
	bool canBePerformed = false;
	// když je vykonán přechod vezmou se značky ze všech vstupů a dají se do všech výstupů
	Place *place;
	Link *link;
	std::vector<Link *>::iterator iterLink; // iterátor pro průchod seznamem hran
	std::vector<Token *>::iterator iterPlaceTokens; // iterátor pro průchod seznamem přechodů
	std::vector<Link *> *listOfInputLinks = transition->getInputLinks(); // seznam vstupních hran	přechodu
	std::vector<Token *> checkTokens; // vektor vektoru ukazatelů všech přechodů
	
	// průchodu seznamem vstupních hran
	for(iterLink = listOfInputLinks->begin(); iterLink != listOfInputLinks->end(); iterLink++)
	{
		link = (*iterLink); // aktuální hrana
		place = ((Place*)(link->getInput())); // vstupní místo aktuální linky
		
		// pokud je počet tokenů >= kapacitě hrany
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
 * Smaže událost s kalendáře na základě spojitosti s přechodem a tokenem.
 * @param token
 */
void Simulator::deleteEventByTransition(Transition *transition)
{
	std::multiset<Event *, EventSort>::iterator iterEvents; // iterátor
	std::multiset<Event *, EventSort>* listOfEvents = this->calendar->getEvents(); // multiset ukazatelů na event
	
	// procházení seznamu eventů a hledání shody
	for(iterEvents = listOfEvents->begin(); iterEvents != listOfEvents->end(); iterEvents++)
	{
		if((*iterEvents)->getTransition() == transition)
		{
			// nejspíš bude nutné pro tuto hodnotu vytvořit další proměnnou ve statistice přechodu
			transition->recomputeStatsWithDeleteEventWait((*iterEvents));
			// smazání eventu z kalendáře
			this->calendar->deleteEvent((*iterEvents));
			return;

		}
	}
}
/**
 * Vygeneruje náhodné číslo
 * @return vygenerované náhodné číslo
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
	return -mv * std::log(Random());
}

/**
 * Main programu
 * @return návratový kód chyby
 */
int main(int argc, char *argv[])
{
	bool modelCase = false;
	int maxSimTime = 10000; // implicitní hodnota
	int c;
	char *pend;
	
	// zpracování argumentů
	while((c = getopt(argc,argv, "s:p")) != -1)
	{
		switch(c)
		{
			case 'p':
				modelCase = true;
				break;
			case 's':
				maxSimTime = strtol(optarg,&pend,10);
				if(*pend != '\0')
				{
					std::cerr<<"Špatně zvolná doba simulace! Zadejte celé číslo!"<<std::endl;
					exit(0);
				}
				break;
			default:
				std::cerr<<"Špatně zadané parametry programu!"<<std::endl;
				std::cerr<<"Dodržte prosím formát vstupních parametrů následovně:"<<std::endl;
				std::cerr<<"	-s delka_simulace (delka_simulace je celé kladné číslo)"<<std::endl;
				std::cerr<<"	-p (spustí simulaci modelu \"Překaldiště\")"<<std::endl;
				std::cerr<<"Bez zadání argumentů se spustí model \"Herna\" s časem simulace \"10000\""<<std::endl;
				exit(0);
				break;
		}
	}
	
	// vytvoření instance simulátoru
	Simulator *simulator = new Simulator();
	
	try
	{
		simulator->createModel(modelCase); // vytvoření modelu
		simulator->getModel()->modelValidate();	
		simulator->setMaxSimTime(maxSimTime); // nastavení maximálního simulačního času
		simulator->simStart(); // zahájení simulace
		// simulator->getModel()->printModel(); // vytisknutí modelu
	}
	
	// ošetření výjimek
	catch(int e)
	{
		// dealokace místa
		simulator->getCalendar()->~Calendar();
		simulator->getModel()->~Model();
		delete simulator;
		exit(EXIT_FAILURE);
	}
	
	// dealokace místa
	simulator->getCalendar()->~Calendar();
	simulator->getModel()->~Model();
	delete simulator;
	exit(EXIT_SUCCESS);
  
}

/**
 * Vratí seed pro inicializaci generátoru pseudo-náhodných čísel
 * (použit seed generátor ze zdroje: http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx)
 * @return seed
 */
unsigned time_seed()
{
    time_t now = time(0);
    unsigned char *p = (unsigned char *)&now;
    unsigned seed = 0;

    for (size_t i = 0; i < sizeof now; i++)
    {
        seed = seed * (UCHAR_MAX + 2U) + p[i];
    }

    return seed;
}