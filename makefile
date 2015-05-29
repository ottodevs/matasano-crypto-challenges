Cpp = g++

CC=./CC/*.cc
HH=./HH
OBJECTS=*.o
OUT_EXE=crypto.x
MAIN=set4.cc

build: $(CC) $(MAIN)
	$(Cpp) -lssl -lcrypto -I$(HH) $(CC) -c $(MAIN)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS)
	rm -f set*.o

set1.x: $(CC) set1.cc clean
	$(Cpp) -lssl -lcrypto -c $(CC) set1.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set1.x $(OBJECTS)
	rm -f set1.o


set2.x: $(CC) set2.cc clean
	$(Cpp) -lssl -lcrypto -c $(CC) set2.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set2.x $(OBJECTS)
	rm -f set2.o

set3.x: $(CC) clean
	$(Cpp) -lssl -lcrypto -c $(CC) set3.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set3.x $(OBJECTS)
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
	$(Cpp) -lssl -lcrypto -I$(HH) -c $(MAIN)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS)

