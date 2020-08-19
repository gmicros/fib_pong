#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>

struct pairs {
    int a {};
    int b{};
};

void fib1(int socket, struct pairs pair) {
    struct pairs fib_pair = pair;
    while(fib_pair.b < 1000) {
	const int temp = fib_pair.a + fib_pair.b;
	fib_pair.a = fib_pair.b;
	fib_pair.b = temp;
	write(socket, &fib_pair, sizeof(fib_pair));
	read(socket, &fib_pair, sizeof(fib_pair));
	printf("[%d, %d]\n", fib_pair.a, fib_pair.b);

    }
}

void fib2(int socket) {
   struct pairs fib;
   // TODO: this loop never ends
   while(read(socket, &fib, sizeof(fib)) ) {
	const int temp = fib.a + fib.b;
	fib.a = fib.b;
	fib.b = temp;
	write(socket, &fib, sizeof(fib));
   }
}

int main() {
    struct pairs fib_pair;
    fib_pair.a = 0;
    fib_pair.b = 1;

    int pipefd[2];
    int socket_ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);

    std::thread thread_fib1(fib1, pipefd[0], fib_pair);
    std::thread thread_fib2(fib2, pipefd[1]);
    
    thread_fib1.join();
    thread_fib2.join();

    return 0;
}
