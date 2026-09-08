#include "Engine.h"

#include <SDL3/SDL.h>

namespace Tenshi {

unsigned long GetTicks() {
	return SDL_GetTicks();
}

unsigned long CountFPS() {
	unsigned long now;
	static bool first = true;
	static unsigned long max_count;
	static unsigned long count;
	static unsigned long wait;

	if (first) {
		wait = GetTicks();
		first = false;
	}

	now = GetTicks();
	count++;

	if (wait + 1000 <= now) {
		wait += 1000;
		max_count = count;
		count = 0;
	}

	return max_count;
}

void Delay(uint32_t ticks) {
	SDL_Delay(ticks);
}

} // name Tenshi