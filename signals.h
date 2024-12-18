#include <csignal>

#ifndef SIGNALS_H
#define SIGNALS_H
	enum Signals {
		SKIP = SIGINT,
		PAUSE = SIGTSTP,
		RESUME = SIGCONT,
		EXIT = SIGTERM
	};
#endif
