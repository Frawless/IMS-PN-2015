/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	link.h 		            	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef LINK_H
#define LINK_H

#include <vector>

class PlaceTransition;


class Link
{
public:
	Link(PlaceTransition *input, PlaceTransition *output, int capacity);			//konstruktor
        static std::vector <Link *> getLinks();
	PlaceTransition *getInput();
	PlaceTransition *getOutput();
	int getCapacity();
  
private:
	static std::vector <Link *> listOfLinks;
        PlaceTransition *input;			//ukazatel na vstup místa/přechodu
	PlaceTransition *output;		//ukazatel na výstup místa/přechodu
	int capacity;					//kapacity hrany
};


#endif