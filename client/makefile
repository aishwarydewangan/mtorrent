CC = g++
CFLAGS = -Wall -std=c++1z -lssl -lcrypto
DEPS = clientget.h clientshare.h peerclient.h peerserver.h utils.h
OBJ = clientget.o clientshare.o peerclient.o peerserver.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^