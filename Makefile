#################################################
#		Makefile 			#
#  Popis:  Pro projekt do předmětu IMS		#
#	   Simulátor Petriho síťí		#
#  Autoři: Jakub Stejskal <xstejs24>		#
#	   Petr Staněk <xstane34>		#
#  Datum:  2015/2016				#
#################################################

CC = g++
CFLAGS = -std=c++98 -Wall -pedantic -ansi -W -Wextra
LOGIN = 02_xstejs24_xstane34
PROJ_NAME = PN-simulator
FILES = simulator.o calendar.o link.o model.o token.o place-transition.o
PACK = *.cpp *.h Makefile Doxyfile dokumentace.pdf


%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
default: $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ_NAME) $^
	
pack: clean
	rm -f $(LOGIN).zip
	zip -r $(LOGIN).zip $(PACK)
	
run:
	./$(PROJ_NAME) > result
	
doxygen:
	doxygen doc/doxyfile
	
clean: 
	rm -f *.o *.out $(PROJ_NAME)