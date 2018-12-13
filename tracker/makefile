CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = trackerget.h trackersend.h utils.h
OBJ = trackerget.o trackersend.o main.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

tracker: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^