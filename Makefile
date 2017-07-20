recover: recover.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o recover recover.c -lcs50 -lm

clean:
	rm -f *.o a.out core recover log.txt
