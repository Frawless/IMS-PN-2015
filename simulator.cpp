/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	simulator.cpp             	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "simulator.h"
#include "model.h"
#include "link.h"
#include "calendar.h"
#include "token.h"
#include "place-transition.h"

using namespace std;    // Or using std::string

Simulator::Simulator()
{
	this->model = new Model();
}

void Simulator::createModel()
{
	model->addPlace("místo_1");
	model->addPlace("místo_2");
	model->addPlace("místo_3");
	
	model->addTransition("přechod_1");
	model->addTransition("přechod_2");
	
	model->addLink("místo_3","přechod_1",0);
	model->addLink("přechod_1","místo_1",0);
	model->addLink("místo_1","přechod_2",0);
	model->addLink("přechod_2","místo_2",0);
  
}

Model *Simulator::getModel()
{
	return this->model;
}

void Simulator::printFullModel()
{
	this->model->printModel();
}

int main()
{
	Simulator *simulator = new Simulator();
	
	try{
		simulator->createModel();
		std::cerr<<"Model vytvořen"<<std::endl;
		simulator->printFullModel();
		//simulator->model->printModel();
	}
	catch(int e)
	{
	  switch(e)
	  {
		case 1:
			std::cerr<<"Error :)"<<std::endl;
	  }
	}
	
	exit(0);
  
}