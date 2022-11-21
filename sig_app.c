#include <iostream>
#include <signal.h>
#include <unistd.h>

int is_quit = 0;

void process_exit(int sig)
{
	switch (sig) {
		case SIGINT:
			fprintf(stderr, "process exit: SIGINT: value: %d\n", sig);
			break;
		case SIGFPE:
			fprintf(stderr, "process exit: SIGFPE: value: %d\n", sig);
			break;
		case SIGABRT:
			fprintf(stderr, "process exit: SIGABRT: value: %d\n", sig);
			break;
		case SIGILL:
			fprintf(stderr, "process exit: SIGILL: value: %d\n", sig);
			break;
		case SIGSEGV:
			fprintf(stderr, "process exit: SIGSEGV: value: %d\n", sig);
			break;
		case SIGTERM:
			fprintf(stderr, "process exit: SIGTERM: value: %d\n", sig);
			break;
		default:
			fprintf(stderr, "process exit: value: %d\n", sig);
			break;
	}
 
	is_quit = 1;
}
 
void signal_type()
{
	signal(SIGINT, process_exit);
	signal(SIGFPE, process_exit);
	signal(SIGILL, process_exit);
	signal(SIGABRT, process_exit);
	signal(SIGSEGV, process_exit);
	signal(SIGTERM, process_exit);
}


int main ()
{
  int i = 0;
  for (; i < 100000 && is_quit == 0; ++i) {
    std::cout << "i:" << i << std::endl;
    sleep(1);
  }
	return 0;
}
