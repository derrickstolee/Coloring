#
# Makefile for Coloring project, src/ folder.
#
#

TARGETS = 	csg_speed_test.exe \
			csg_test.exe \
			lists.exe 

OBJECTS =	csg_enumeration.o \
	choosability.o 

CC 	= gcc
CXX = g++

OPT = -O4
# -O0 -g
#DEBUG =   -stdlib=libstdc++
WARNINGS =  -Werror


UTILSDIR = ../../Utilities/src
NAUTYDIR = ../../nauty

INCLUDES = -I.   \
			-I$(NAUTYDIR)			

 
				  
NAUTYOBJS     	= $(NAUTYDIR)/nauty.o 			\
				  $(NAUTYDIR)/nausparse.o		\
				  $(NAUTYDIR)/gtools.o			\
				  $(NAUTYDIR)/nautil.o			\
				  $(NAUTYDIR)/naugraph.o		\
				  $(NAUTYDIR)/naurng.o			\
				  $(NAUTYDIR)/schreier.o



CFLAGS 		= $(OPT) $(DEBUG) $(WARNINGS) $(CPLEX_INCLUDE) $(DEFINES)
CXXFLAGS 	= $(OPT) $(DEBUG) $(WARNINGS) $(CPLEX_INCLUDE) $(DEFINES) 
LFLAGS 		= $(OPT) $(DEBUG) $(WARNINGS) $(CPLEX_FLAGS) $(DEFINES) 


LIBOBJS	= $(TREESEARCHOBJS) $(UTILSOBJS)  $(NAUTYOBJS)
			
.SUFFIXES: .c .cpp .o .obj .exe 

all: $(OBJECTS) $(TESTS) $(TARGETS)
	mv lists.exe lists
	mv lists ../bin
	mv *.o ../bin
	mv *.exe ../bin

# The default object compiler
.c.o: $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.o: $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@
        
.cpp.exe: $< $(OBJECTS) 
	$(CXX) $(LFLAGS)						\
        	$(INCLUDES)	$(DEBUG)			\
        	$(LIBOBJS) $(LIBS)					\
        $(OBJECTS)				    	   			\
            $< -o $@
	cp $@ ../bin/
        
.c.exe: $< $(COBJECTS)
	$(CC) 	$(LFLAGS)	$(DEBUG)		    						\
        	$(INCLUDES)										\
        	$(NAUTYOBJS)  $(COBJECTS) $(LIBS)		\
            $< -o $@
        
clean:
	-@rm $(OBJECTS) $(TARGETS) $(TESTS)
	
cleanexe:
	-@rm $(TARGETS)

cleantest:
	-@rm $(TESTS)

clest:
	-@rm $(TESTS)

clexe:
	-@rm $(TARGETS)
