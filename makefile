CC=g++
CPPFLAGS= -std=c++11 -Wall -I $(INCDIR)
EXEC=driver

OBJDIR=./obj
OBJ=$(addprefix $(OBJDIR)/,$(EXEC).o MTT_Board.o Solver.o)

INCDIR=./inc
INC=$(addprefix $(INCDIR)/,MTT_Board.h Solver.h)

SRCDIR=./src
SRC=$(addprefix $(SRCDIR)/,$(EXEC).cpp MTT_Board.cpp Solver.cpp)


$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(addprefix $(INCDIR)/,%.h)
	$(CC) -c -o $@ $< $(CPPFLAGS)

$(OBJDIR)/$(EXEC).o: $(SRCDIR)/$(EXEC).cpp $(INC)
	$(CC) -c -o $@ $< $(CPPFLAGS)


.PHONY: clean

clean:
	rm -f $(EXEC) $(OBJDIR)/*.o
