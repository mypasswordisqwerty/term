#include "power.hpp"
#include "max31855.hpp"

#define P 0.2
#define I 0.05
#define D 0.02
#define PID_PERIOD 1000
#define DT 1.f

class Pid;

struct IPidCallback {
	virtual void onPidDone(Pid&) =0;
};

class Pid: public Object {
public:
	Pid():period(0), prev(0), pwr(nullptr), target(0), temp(nullptr), integ(0), callback(nullptr) {
	}

	Pid* init(Power& pwr) {
		this->pwr = &pwr;
		return this;
	}

	void setTarget(MAX31855* temp, int target, IPidCallback* callback) {
		this->temp = temp;
		this->target = target;
		this->callback = callback;
	}

	void clearCallback() {
		this->callback = nullptr;
	}


	void clearTarget() {
		setTarget(nullptr, 0, nullptr);
	}

	virtual void loop(uint32_t delay) override {
		if (!pwr || !temp){
			return;
		}
		period -= delay;
		if (period>0) return;
		period = PID_PERIOD;

		int inp = temp->temperature();
		int dinp = prev - inp;
		prev = inp;

		int err = target - inp;
		float out = err * P;
		out += dinp * D / DT;
		integ += err * I * DT;
		integ = std::min(1.0f, std::max(0.f, integ));
		out += integ;
		pwr->setPower(std::min(1.0f, std::max(0.f, out)));
		if (callback && target>=inp){
			callback->onPidDone(*this);
		}
	}

private:
	int32_t period;
	int prev;
	Power* pwr;
	int target;
	MAX31855* temp;
	float integ;
	IPidCallback* callback;
};
