#include "power.hpp"
#include "max31855.hpp"

#define PK 0.01
#define IK 0.3
#define DK 0.3
#define PID_PERIOD 5000
#define DT PID_PERIOD/1000.f

class Pid;

struct IPidCallback {
	virtual void onPidDone(Pid&) =0;
};

class Pid: public Object {
public:
	Pid():pwr(nullptr), period(0), prev(0), integ(0) {
		clearTarget();
		setPID(PK, IK, DK);
	}

	Pid* init(Power& pwr) {
		this->pwr = &pwr;
		return this;
	}

	void setPID(float pk, float ik, float dk){
		this->pk = pk;
		this->ik = ik;
		this->dk = dk;
	}

	void setTarget(MAX31855* temp, int target, IPidCallback* callback) {
		this->temp = temp;
		this->target = target;
		this->callback = callback;
		if (temp){
			goup = target > temp->temperature();
		}
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

		int err = target - inp;
		float out = err * pk;
		out += (err - prev) * dk / DT;
		prev = err;
		integ += err * DT;
		integ = std::min(0.5f, std::max(-0.5f, integ));
		out += integ * ik;
		pwr->setPower(std::min(1.0f, std::max(0.f, out)));
		bool done = goup ? inp>=target : inp<=target;
		if (callback && done){
			callback->onPidDone(*this);
		}
	}

private:
	Power* pwr;
	int32_t period;
	float pk, ik, dk;
	int prev;
	int target;
	MAX31855* temp;
	float integ;
	IPidCallback* callback;
	bool goup;
};
