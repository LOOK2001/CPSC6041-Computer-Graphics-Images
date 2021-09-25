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

PROJECT = imgview

SOURCES = imgview.cpp image.cpp OIIOFiles.cpp imageOperator.cpp

$(PROJECT):	$(SOURCES:%.$(C)=%.o)
	$(CC) $(SOURCES:%.$(C)=%.o) $(INCLUDES) $(LDFLAGS) -o $@

$(SOURCES:%.$(C)=%.o):	%.o: %.$(C)
	$(CC) -o $@ -c $(CFLAGS) $<

clean:
	rm -f core.* *.o *~ ${PROJECT}
