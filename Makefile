all: threads_fib.c
	g++ -o fib_threads threads_fib.c -pthread
clean: fib_threads
	rm fib_threads
