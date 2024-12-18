#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H
	enum Signals {
		SKIP = SIGINT,
		PAUSE = SIGTSTP,
		RESUME = SIGCONT,
		EXIT = SIGTERM
	};
#endif
