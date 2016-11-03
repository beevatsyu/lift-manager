#define NFLOORS	9
#define LMF    -2
#define UMF	6

class LiftManager {
	struct Lift {
		int cf;
		int dir;
	} lift;
	bool req_floor[NFLOORS];
	int lmf;
	int umf;
public:
	void request(int);
	void service();
}

void LiftManager::request(int fn) {
	if (!req_floor[fn]) {
		req_floor[fn] = true;
		if (fn < lmf || !req_floor[lmf]) {
			lmf = fn;
		}
		if (fn > umf || !req_floor[umf]) {
			umf = fn;
		}
	}
}

void LiftManager::service() {
	while (1) {
		while (lift.dir >= 0 && umf >= lift.cf) {
			if (lift.cf == umf) {
				req_floor[lift.cf] = false;
				lift.dir = 0; // stop the lift
				break;
			}
			if (req_floor[lift.cf]) {
				sleep(2); // time for lift to open and close
				req_floor[lift.cf] = false;
			}
			sleep(1); // time taken to go to next floor
			lift.cf++;
		}
		while (lift.dir <= 0 && lmf <= lift.cf) {
			if (lift.cf == lmf) {
				req_floor[lift.cf] = false;
				lift.dir = 0;
				break;
			}
			if (req_floor[lift.cf] < 0) {
				sleep(2);
				req_floor[lift.cf] = false;
			}
			sleep(1);
			lift.cf--;
		}
	}
}
