main:
	gcc -std=gnu99 -Wall -g -o smallsh main.c

clean:
	rm -f smallsh

debug:
	valgrind -s --leak-check=yes --track-origins=yes --show-reachable=yes ./smallsh

test: 
	chmod +x ./p3testscript
	./p3testscript 2>&1