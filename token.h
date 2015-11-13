/************************************************
*                                               *
*   Autoři:			Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	token.h 	             	*
*   Projekt: 		projek do předmětu IMS      * 
* 					Simulátor petriho síťí		*
* 	Datum:   		2015/2016					*
*                                               *
************************************************/

#ifndef TOKEN_H
#define TOKEN_H

class Place;

/**
 * 
 */
class Token
{
public:
	Token(Place *place);				//konstruktor
        Place* getPlace();
  
private:
	Place * placePtr;			//ukazatel na místo
		
  
};
#endif