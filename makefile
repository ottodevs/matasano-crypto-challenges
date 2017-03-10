Cpp = g++

OPT = -std=c++0x -lssl -lcrypto
CC=./CC/*.cc
HH=./HH
OBJECTS=*.o
OUT_EXE=crypto
MAIN=set4.cc

build: $(CC) $(MAIN)
	$(Cpp) $(OPT) -I$(HH) $(CC) -c $(MAIN)
	$(Cpp) $(OPT) -o $(OUT_EXE) $(OBJECTS)
	rm -f set*.o

set1: $(CC) set1.cc cleanmain
	$(Cpp) $(OPT) -c $(CC) set1.cc -I$(HH)
	$(Cpp) $(OPT) -o set1 $(OBJECTS)
	rm -f set1.o


set2: $(CC) set2.cc cleanmain
	$(Cpp) $(OPT) -c $(CC) set2.cc -I$(HH)
	$(Cpp) $(OPT) -o set2 $(OBJECTS)
	rm -f set2.o

set3: $(CC) cleanmain
	$(Cpp) $(OPT) -c $(CC) set3.cc -I$(HH)
	$(Cpp) $(OPT) -o set3 $(OBJECTS)
	rm -f set3.o

reset: clean
	rm -f *.x

cleanmain:
	rm -f set*.o

clean:
	rm -f $(OBJECTS)
	rm -f *~
	rm -f *.swp

rebuild: $(MAIN)
	$(Cpp) $(OPT) -I$(HH) -c $(MAIN)
	$(Cpp) $(OPT) -o $(OUT_EXE) $(OBJECTS)

prog:
	$(Cpp) $(OPT) -I$(HH) $(CC) -c $(MAIN)
	$(Cpp) $(OPT) -o $(OUT_EXE) $(OBJECTS)
	rm -f set*.o
