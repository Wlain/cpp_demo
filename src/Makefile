CC = gcc
STD = c89
LDFLAGS = -Wall -pedantic -ansi
LIBS = -lglfw -lm -ldl -lfreetype
INC = -I/usr/include -I./include

out: main.c render.c gl_utils.c texture.c math_linear.c lib/glad.c ../src/array_stack.c ../src/ecs.c
	$(CC) $(LDFLAGS) $(INC) $(LIBS) -std=$(STD) $^

clean:
	rm -f *.o ./**/*.gch *.out gp val.txt

prof:
	gprof a.out gmon.out > gp && kak ./gp
