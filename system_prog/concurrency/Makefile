
all:
	$(CC) -Wall -o atomic_var atomic_var.c -lpthread -std=c11 -pedantic
	$(CC) -Wall -o thread_race thread_race.c -lpthread -std=c11 -pedantic

clean:
	rm -f atomic_var