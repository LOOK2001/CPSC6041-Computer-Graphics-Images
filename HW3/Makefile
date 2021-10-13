CC		= g++
C		= cpp

CFLAGS		= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

INCLUDES = -I ./

PROJECT1 = alphamask
PROJECT2 = compose

SOURCE = alphamask.cpp compose.cpp image.cpp OIIOFiles.cpp imageOperator.cpp
SOURCES1 = alphamask.cpp image.cpp OIIOFiles.cpp imageOperator.cpp
SOURCES2 = compose.cpp image.cpp OIIOFiles.cpp imageOperator.cpp

all: ${PROJECT1} ${PROJECT2}

$(PROJECT1):	$(SOURCES1:%.$(C)=%.o)
	$(CC) $(SOURCES1:%.$(C)=%.o) $(INCLUDES) $(LDFLAGS) -o $@

$(PROJECT2):	$(SOURCES2:%.$(C)=%.o)
	$(CC) $(SOURCES2:%.$(C)=%.o) $(INCLUDES) $(LDFLAGS) -o $@

$(SOURCE:%.$(C)=%.o):	%.o: %.$(C)
	$(CC) -o $@ -c $(CFLAGS) $<


clean:
	rm -f core.* *.o *~ ${PROJECT1}
	rm -f core.* *.o *~ ${PROJECT2}
