all: KaratsubaMul.o KaratsubaTest.o ReadStrings.o
	gcc KaratsubaMul.o KaratsubaTest.o ReadStrings.o -l m -o K.exe

KaratsubaMul.o: KaratsubaMul.c
	gcc KaratsubaMul.c -c

KaratsubaTest.o: KaratsubaTest.c
	gcc KaratsubaTest.c -c

ReadStrings.o: ReadStrings.c
	gcc ReadStrings.c -c

cleank:
	rm KaratsubaMul.o KaratsubaTest.o

cleanr:
	rm ReadStrings.o
	
run:
	./K.exe Tests/MainTest.txt Tests/Results.txt