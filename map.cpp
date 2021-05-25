#include "Rasta.h"
#include <iostream>
#include <ctime>
using namespace std;

int main() {
	srand(time(NULL));
	Rasta rasta;
	rasta.initialize();

	int cnt0[100], cnt1[100];
	for (int i = 0; i < 100; i++) {
		cnt0[i] = 0;
		cnt1[i] = 0;
	}
	bool** eq;
	eq = new bool* [ES - 1];
	for (int i = 0; i < ES - 1; i++) {
		eq[i] = new bool[ES];
		for (int j = 0; j < ES; j++) {
			eq[i][j] = 0;
		}
	}
	int testTimes = 100;

	bool m[BS], k[BS],c[BS];
	for (int t = 0; t < testTimes; t++) {
		for (int i = 0; i < BS; i++) {
			m[i] = 0;
			k[i] = rand() % 2;
		}
		rasta.setKey(k);

		for (int i = 0; i < ES - 1; i++) {
			for (int j = 0; j < ES; j++) {
				eq[i][j] = 0;
			}
		}

		int dataCom = (ES - 1) / (3 * BS) + 1;
		for (int i = 0; i < dataCom; i++) {
			rasta.encrypt(m, c);
			rasta.constructEqs(m, c, i, eq);
		}
		cout <<endl<< "current times: " << t + 1 << endl;
		cout << "start gaussian elimination..." << endl;
		rasta.gauss(eq, ES - 1, ES, cnt0, cnt1);
	}

	cout << "number of polynomials: "<<ES - 1 << endl;
	cout << "number of monomials:" << ES << endl;
	cout << "key size:" << BS << endl;
	cout << "EQB-EQA:" << endl;
	for (int i = 0; i < 100; i++) {
		if (cnt0[i] > 0) {
			cout << i << ": " << cnt0[i] << endl;
		}
	}
	/*cout << "n-EQL:" << endl;
	for (int i = 0; i < 100; i++) {
		if (cnt1[i] > 0) {
			cout << i << ": " << cnt1[i] << endl;
		}
	}*/
	rasta.freeMatrix(eq, ES - 1);
	return 0;
}