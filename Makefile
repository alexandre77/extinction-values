CC = g++
INCDIR = .
LIBDIR = .

SOURCES = \
	main.cpp \
	extinctions.cpp \
	gettime.cpp \
	lista.cpp \
	maxtree.cpp \
	mm.cpp


OBJECTS = $(SOURCES:.cpp=.o)



##### REGRAS #####

.cpp.o:
	$(CC) -c -I$(INCDIR) $(CFLAGS) $(GLUT_CFLAGS) $<



##### ACOES #####

default: $(LIBDIR)

clean:
	-rm -f *.o
	-rm -f *~

# Construindo a biblioteca
$(LIBDIR): $(OBJECTS)
	$(CC) -o extinction $(OBJECTS)

