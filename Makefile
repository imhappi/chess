CXX=g++          # define compiler to use for C++ programs
CXXFLAGS= -Wall -g -I/usr/X11R6/include -L/usr/X11R6/lib -lX11  # define alternate flags to use

# Define some variables to reduce duplication and possible errors.
OBJS=cell.o controller.o computer.o view.o textdisplay.o graphicdisplay.o window.o game.o piece.o pawn.o king.o queen.o rook.o knight.o bishop.o player.o human.o main.o
EXEC=pp9k

# Define first/default rule
$(EXEC): $(OBJS)
	${CXX} ${OBJS} -o ${EXEC} ${CXXFLAGS}
# Define rule to clean up.
clean:
	rm *.o $(EXEC)
