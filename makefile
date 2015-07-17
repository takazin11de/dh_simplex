
SRC		= test.cpp pipe.cpp mat.cpp beziers.cpp figure.cpp dh_simplex.cpp time_for_filename.cpp fifth_polynomial.cpp
PROGRAM		= a.out

#####################
.SUFFIXES: .tex .dvi .pdf .cpp .hpp .c .h .exe .out .o 
CC		= g++
CFLAGS		= -std=c++11
LDFLAGS		= 
LIBS		= -static-libgcc -static-libstdc++ -llapack
OBJ		= $(SRC:%.cpp=%.o)
DEPS		= $(SRC:%.cpp=%.d)

all:		$(PROGRAM)
-include $(DEPS)
$(PROGRAM):	$(OBJ)
		$(CC) -o $(PROGRAM) $(OBJ) $(LDFLAGS) $(LIBS) 
.cpp.o:
		$(CC) -c -MMD -MP $(CFLAGS) $<
clean:
		rm -f  $(PROGRAM) $(OBJ) $(DEPS)
