whodunit: whodunit.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o whodunit whodunit.c -lcs50 -lm

clean:
	rm -f *.o a.out core whodunit log.txt
