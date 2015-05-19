Cpp=g++

CC=./CC/*.cc
HH=./HH
OBJECTS=*.o
OUT_EXE=crypto.x
MAIN=set3

build: $(FILES)
	$(Cpp) -lssl -lcrypto -c $(CC) $(MAIN).cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS) $(MAIN).o

set1.x:
	$(Cpp) -lssl -lcrypto -c $(CC) set1.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set1.x $(OBJECTS)

set2.x:
	$(Cpp) -lssl -lcrypto -c $(CC) set2.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set2.x $(OBJECTS)

set3.x:
	$(Cpp) -lssl -lcrypto -c $(CC) set2.cc -I$(HH)
	$(Cpp) -lssl -lcrypto -o set2.x $(OBJECTS)

reset: clean
	rm *.x

clean:
	rm -f $(OBJECTS)

rebuild: $(MAIN)
	$(Cpp) -lssl -lcrypto -c $(MAIN)
	$(Cpp) -lssl -lcrypto -o $(OUT_EXE) $(OBJECTS)

