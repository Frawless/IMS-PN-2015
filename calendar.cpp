/*************************************************
*		  Projekt: 	Projekt do předmětu IMS     * 
* 					Simulátor petriho sítí		*
*		   Autoři:	Jakub Stejskal <xstejs24>	*
*		   			Petr Staněk <xstane34>      *
*   Nazev souboru: 	calendar.cpp             	*
*			Datum:  14. 11. 2015				*
*			Verze:	1.0							*
************************************************/


#include "calendar.h"

/**
 * Konstruktor kalendáře událostí
 */
Calendar::Calendar()
{
}

/**
 * Destruktor kalendáře událostí
 */
Calendar::~Calendar()
{
}

/**
 * Získání času události
 * @return čas události
 */
double Event::getTime()
{
	return this->time;
}