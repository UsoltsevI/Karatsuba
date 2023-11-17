all: KaratsubaMul.o KaratsubaTest.o ReadStrings.o
	gcc -O2 KaratsubaMul.o KaratsubaTest.o ReadStrings.o -l m -o K.exe
	@echo 'Usage option one: make run'
	@echo 'Usage option two: ./K.exe <inputfile> <outputfile>'

KaratsubaMul.o: KaratsubaMul.c
	gcc -O2 KaratsubaMul.c -c

KaratsubaTest.o: KaratsubaTest.c
	gcc -O2 KaratsubaTest.c -c

ReadStrings.o: ReadStrings.c
	gcc -O2 ReadStrings.c -c

cleank:
	rm KaratsubaMul.o KaratsubaTest.o

cleanr:
	rm ReadStrings.o

compilegen:
	gcc -O2 KaratsubaGener.c -o KG.exe
	@echo 'Usage: ./KG.exe <numelem> <filename>'
	
run:
	valgrind ./K.exe Tests/MainTest.txt Tests/Results.txt
