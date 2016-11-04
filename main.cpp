#include <iostream>
#include <functional>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define NFLOORS	9
#define LMF	0
#define UMF	8

class LiftManager {
	struct Lift {
		int cf;
		Lift() {cf = LMF;}
	} lift;
	bool req_floor[NFLOORS];
	int lmf;
	int umf;
public:
	LiftManager();
	void request(int);
	void service();
};

LiftManager::LiftManager() {
	lmf = umf = LMF;
	for (int i = 0; i < NFLOORS; i++) {
		req_floor[i] = false;
	}
}

void LiftManager::request(int fn) {
	if (fn < LMF || fn > UMF) {
		cout<< "request@floor#" << fn << " invalid!\n";
		return;
	}
	if (!req_floor[fn]) {
		req_floor[fn] = true;
		if (fn < lmf || !req_floor[lmf]) {
			if (!req_floor[umf]) {
				umf = lmf;
			}
			lmf = fn;
		}
		if (fn > umf || !req_floor[umf]) {
			if (!req_floor[lmf]) {
				lmf = umf;
			}
			umf = fn;
		}
		cout<< "request@floor#" << fn << " lmf: "
			<< lmf << " umf: " << umf << '\n';
	}

}

void LiftManager::service() {
	cout<< "lift@floor#" << lift.cf
		<< " lmf: " << lmf << req_floor[lmf]
		<< " umf: " << umf << req_floor[umf]
		<< " service started\n";
	while (1) {
		while (req_floor[umf] && umf >= lift.cf) {
			if (lift.cf == umf) {
				req_floor[lift.cf] = false;
				cout<< "lift@floor#" << lift.cf
					<< " lmf: " << lmf << req_floor[lmf]
					<< " umf: " << umf << req_floor[umf]
					<< endl;
				break;
			}
			if (req_floor[lift.cf]) {
				sleep(2); // time for lift to open and close
				req_floor[lift.cf] = false;
				cout<< "lift@floor#" << lift.cf
					<< " lmf: " << lmf << req_floor[lmf]
					<< " umf: " << umf << req_floor[umf]
					<< endl;
			}
			sleep(1); // time taken to go to next floor
			lift.cf++;
		}
		while (req_floor[lmf] && lmf <= lift.cf) {
			if (lift.cf == lmf) {
				req_floor[lift.cf] = false;
				cout<< "lift@floor#" << lift.cf
					<< " lmf: " << lmf << req_floor[lmf]
					<< " umf: " << umf << req_floor[umf]
					<< endl;
				break;
			}
			if (req_floor[lift.cf] < 0) {
				sleep(2);
				req_floor[lift.cf] = false;
				cout<< "lift@floor#" << lift.cf
					<< " lmf: " << lmf << req_floor[lmf]
					<< " umf: " << umf << req_floor[umf]
					<< endl;
			}
			sleep(1);
			lift.cf--;
		}
	}
}

void* service_start(void *arg) {
	LiftManager *lmp = (LiftManager *) arg;
	lmp->service();
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	LiftManager lm;
	pthread_t th;
	pthread_create(&th, NULL, service_start, &lm);
	sleep(2);
	lm.request(3);
	lm.request(5);
	sleep(10);
	lm.request(1);
	pthread_join(th, NULL);
	//lm.service();
	return 0;
}
