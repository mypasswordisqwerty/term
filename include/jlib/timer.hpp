#ifndef TIMER_H
#define TIMER_H

class Timer{
public:
	Timer(int time): delay(time), current(time) {}

	bool update(int delta) {
		current -= delta;
		bool ret = current <= 0;
		if (ret){
			current = delay;
		}
		return ret;
	}

private:
	int delay;
	int current;
};


#endif
