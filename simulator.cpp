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
	model->addPlace("místo_1");
	model->addPlace("místo_2");
	model->addPlace("místo_3");
	
	model->addTransition("přechod_1");
	model->addTransition("přechod_2");
	
	model->addLink("místo_3","přechod_1", 0);
	model->addLink("přechod_1","místo_1", 0);
	model->addLink("místo_1","přechod_2", 0);
	model->addLink("přechod_2","místo_2", 0);
	
	//Token * t = model->addToken("místo_1");
	
	
	
	model->addToken("místo_1");
	
	model->modelValidate();
	
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

int main()
{
	// vytvoření instance simulátoru
	Simulator *simulator = new Simulator();
	
	try
	{
		simulator->createModel(); // vytvoření modelu
		std::cerr<<"Model vytvořen"<<std::endl;
		simulator->getModel()->printModel(); // vytisknutí modelu
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