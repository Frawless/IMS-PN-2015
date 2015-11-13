/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	simulator.h	            	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef MAIN_H
#define MAIN_H

class Link;
class PlaceTransition;
class Place;
class Transition;
class Token;
class Model;
class Calendar;

class Simulator
{
public:
	Simulator();
	~Simulator();
	
	void createModel();
	Model *getModel();
	
	void printFullModel();
  
private:
	Model *model;
	Calendar *calendar;
	double simTime;
	double maxSimTime;
};

#endif