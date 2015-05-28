Cpp = g++

CC=./CC/*.cc
HH=./HH
OBJECTS=*.o
OUT_EXE=crypto.x
MAIN=set3.cc

build: $(CC) $(MAIN)
	$(Cpp) -lssl -lcrypto -pthread -I$(HH) $(CC) -c $(MAIN)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS)

set1.x: $(CC) set1.cc
	$(Cpp) -lssl -lcrypto -c $(CC) set1.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set1.x $(OBJECTS)

set2.x: $(CC) set2.cc
	$(Cpp) -lssl -lcrypto -c $(CC) set2.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set2.x $(OBJECTS)

set3.x: $(CC)
	$(Cpp) -lssl -lcrypto -c $(CC) set3.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set3.x $(OBJECTS)

reset: clean
	rm -f *.x

clean:
	rm -f $(OBJECTS)
	rm -f *~
	rm -f *.swp

rebuild: $(MAIN)
	$(Cpp) -lssl -lcrypto -c $(MAIN)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS)

