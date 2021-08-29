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

PROJECT		= imgview

SOURCES = image.cpp imgview.cpp OIIOFiles.cpp

${PROJECT}:	${SOURCES:%.${C}=%.o}
	${CC} ${CFLAGS} -o ${SOURCES:%.${C}=%.o} ${LDFLAGS} $@

${SOURCES:%.$(C)=%.o}:	%.o: %.${C}
	${CC} -o $@ ${CFLAGS} -c $<

clean:
	rm -f core.* *.o *~ ${PROJECT}
