resize: resize.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o resize resize.c -lcs50 -lm

clean:
	rm -f *.o a.out core resize log.txt
