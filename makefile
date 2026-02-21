OBJ = $(patsubst %.c,%.o,$(wildcard *.c))

inverted_search.exe : $(OBJ)
	gcc -g -o $@ $^

.PHONY : clean
clean :
	rm -f inverted_search.exe *.o
