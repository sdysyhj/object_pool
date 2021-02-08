.SUFFIXES: .idl .cpp .o

PROGRAM = main_test

OBJECTS = main.o
OBJ_EXT =

CC	    = g++
CCFLAGS = --std=c++11
LIBS	= -lpthread -lgtest 

.cpp.o:
	@echo	$(CC) -c $(CCFLAGS) $<
	@	$(CC) -c $(CCFLAGS) $<

.cc.o:
	@echo	$(CC) $(CCFLAGS) $<
	@	$(CC) $(CCFLAGS) $<

$(PROGRAM): $(OBJECTS) $(OBJ_EXT)
	@echo   $(CC) -o $(PROGRAM)  $(OBJECTS) $(OBJ_EXT) $(LIBS)
	@       $(CC) -o $(PROGRAM)  $(OBJECTS) $(OBJ_EXT) $(LIBS)