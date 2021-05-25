#include "Rasta.h"
#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

Rasta::~Rasta() {
	for (int i = 0; i < BS; i++) {
		delete[]quM[i];
		delete[]cuM[i];
	}
	delete[]quM;
	delete[]cuM;

	delete[]k;
	freeMatrix(con, 3);
	freeMatrix(inverMa, BS);
	for (int i = 0; i < 3; i++) {
		freeMatrix(ma[i], BS);
	}
}

void Rasta::initialize() {
	con = new bool* [3];
	inverMa = new bool* [BS];
	for (int i = 0; i < 3; i++)
		con[i] = new bool[BS];
	for (int i = 0; i < BS; i++)
		inverMa[i] = new bool[BS];
	k = new bool[BS];
	ma = new bool** [3];
	for (int i = 0; i < 3; i++) {
		ma[i] = new bool* [BS];
		for (int j = 0; j < BS; j++)
			ma[i][j] = new bool[BS];
	}

	quM = new int* [BS];
	cuM = new int* [BS];
	for (int i = 0; i < BS; i++) {
		quM[i] = new int[BS];
		cuM[i] = new int[BS];
	}
	precomputeOrderQuadratic(quM);
	precomputeOrderCubic(cuM);
	//compute varCM
	for (int i = 0; i < BS; i++) {
		for (int j = i + 1; j < BS; j++) {
			for (int t = j + 1; t < BS; t++) {
				int a = order(i, j, t);
				//cout << i << " , " << j << " , " << t << " :" << a << endl;
				varCM[a].v0 = i;
				varCM[a].v1 = j;
				varCM[a].v2 = t;
			}
		}
	}
	srand(time(NULL));

	//output
	/*cout << "quadratic map:" << endl;
	for (int i = 0; i < BS; i++) {
		for (int j = 0; j < BS; j++) {
			cout << quM[i][j] << " ";
		}
		cout << endl;
	}
	cout << "cubic map:" << endl;
	for (int i = 0; i < BS-3; i++) {
		for (int j = i+1; j < BS-2; j++) {
			cout << "(" << i << " , " << j << "): ";
			cout << cuM[i][j] << " " << endl;
			//cout << endl;
		}
	}*/
}

void Rasta::generateRandMatrix() {
	bool find = false;
	int cnt = 0;
	int dataCom = ES / (3 * BS)+1;//
	//cout << "dataCom" << ": " << dataCom << endl;
	while (cnt < 1) {
		for (int i = 0; i < BS; i++) {
			for (int j = 0; j < BS; j++) {
				ma[0][i][j] = rand() % 2;
				ma[1][i][j] = rand() % 2;
				ma[2][i][j] = rand() % 2;
				//ma[0][i][j] = 0;
				//ma[1][i][j] = 0;
				//ma[2][i][j] = 0;
			}
			//ma[0][i][i] = 1;
			//ma[1][i][i] = 1;
			//ma[2][i][i] = 1;
			con[0][i] = rand() % 2;
			con[1][i] = rand() % 2;
			con[2][i] = rand() % 2;
			//con[0][i] = 0;
			//con[1][i] = 0;
			//con[2][i] = 0;
		}
		if (rank(ma[0], BS, BS) == BS && rank(ma[1], BS, BS) == BS && rank(ma[2], BS, BS) == BS) {
			//cout << "find!" << endl;
			cnt++;
		}
	}
	//compute the inverse of ma[2]
	inverse(ma[2], inverMa, BS);

	/*cout << "ma0:" << endl;
	for (int i = 0; i < BS; i++) {
		printState(ma[0][i], BS);
	}
	cout << "ma1:" << endl;
	for (int i = 0; i < BS; i++) {
		printState(ma[1][i], BS);
	}
	cout << "ma2:" << endl;
	for (int i = 0; i < BS; i++) {
		printState(ma[2][i], BS);
	}*/
}

void Rasta::setKey(bool key[]) {
	for (int i = 0; i < BS; i++) {
		k[i] = key[i];
	}
}

void Rasta::multiplyWithMatrix(bool input[], bool output[], bool **matrix) {
	/*cout << "matrix:" << endl;
	for (int i = 0; i < BS; i++) {
		cout << i << " : ";
		for (int j = 0; j < BS; j++) {
			if (matrix[i][j]) {
				cout << j << " ";
			}
		}
		cout << endl;
	}
	cout << "input: " << endl;
	printState(input, BS);*/

	for (int i = 0; i < BS; i++) {
		output[i] = 0;
		for (int j = 0; j < BS; j++) {
			if (matrix[i][j]) {
				output[i] ^= input[j];
			}
		}
	}
}

void Rasta::addRow(bool* src, bool* des, int size) {
	for (int i = 0; i < size; i++) {
		des[i] ^= src[i];
	}
}

void Rasta::freeMatrix(bool** eq, int row) {
	for (int i = 0; i < row; i++) {
		delete[]eq[i];
	}
	delete[]eq;
}

void Rasta::precomputeOrderCubic(int **start) {
	//xix_{j} (j>i)
	start[0][1] = BS + (BS * (BS - 1)) / 2;
	for (int i = 0; i < BS - 2; i++) {
		for (int j = i + 1; j < BS - 1; j++) {
			if (j == i + 1 && i != 0) {
				start[i][i + 1] = start[i - 1][BS - 2] + 1;
			}
			if(j>i+1) {
				start[i][j] = start[i][j - 1] + BS - j;
			}
		}
	}
	//cout << "computation is over" << endl;
}

void Rasta::precomputeOrderQuadratic(int **map) {
	map[0][1] = BS;
	for (int i = 0; i < BS; i++) {
		map[i][i] = i;
		varM[i].v0 = i;
		varM[i].v1 = i;
		for (int j = i + 1; j < BS; j++) {
			if (j == i + 1 && i != 0) {
				map[i][j] = map[i - 1][BS - 1] + 1;
			}
			if(j>i+1) {
				map[i][j] = map[i][j - 1] + 1;
			}
			varM[map[i][j]].v0 = i;
			varM[map[i][j]].v1 = j;
			map[j][i] = map[i][j];
		}
	}
}

int Rasta::order(int x0, int x1, int x2) {
	//find the minimal value of (x0,x1,x2)
	int a[3];
	a[0] = x0;
	a[1] = x1;
	a[2] = x2;
	for (int i = 0; i < 3; i++) {
		for (int j = i+1; j < 3; j++) {
			if (a[j] < a[i]) {
				swap(a[i], a[j]);
			}
		}
	}
	if (a[0] == a[1] && a[1] == a[2]) {
		return a[0];
	}
	else if (a[0] == a[1] && a[1] !=a[2]) {
		return quM[a[0]][a[2]];
	}
	else if (a[0] != a[1] && a[1] == a[2]) {
		return quM[a[0]][a[2]];
	}
	else if (a[0] != a[1] && a[1] != a[2]) {
		return cuM[a[0]][a[1]] + (a[2] - a[1]) - 1;
	}
}

void Rasta::printState(bool* s, int size) {
	for (int i = 0; i < size; i++) {
		cout << s[i];
	}
	cout << endl;
}

void Rasta::encrypt(bool m[], bool c[]) {
	generateRandMatrix();

	//cout << "key:";
	//printState(k, BS);

	bool state[RD*2+1][BS];//intermeidate state
	for (int i = 0; i < BS; i++) {
		state[0][i] = k[i];
	}
	//printState(state[0], BS);
	for (int i = 0; i < RD; i++) {
		//multiply with matrix
		multiplyWithMatrix(state[2*i], state[2 * i+1], ma[i]);
		//cout << "linear layer:";
		//printState(state[2*i+1], BS);
		//constant addition
		for (int j = 0; j < BS; j++) {
			state[2 * i + 1][j] ^= con[i][j];
		}
		//cout << "constant addition:";
		//printState(state[2 * i + 1], BS);
		//chi operation
		for (int j = 0; j < BS; j++) {
			state[2 * i + 2][j] = state[2 * i + 1][j] ^
				((state[2 * i + 1][(j + 1) % BS] ^ 1) & state[2 * i + 1][(j + 2) % BS]);
		}
		//cout << "chi op:";
		//printState(state[2*i+2], BS);
	}
	//the last affine layer
	//multiply with matrix
	multiplyWithMatrix(state[2 * RD], c, ma[RD]);
	//constant addition and key addition
	for (int j = 0; j < BS; j++) {
		c[j] = c[j] ^ con[RD][j] ^ k[j];
	}
	//printState(con[0], BS);
	//printState(state[1], BS);
	//printState(state[2], BS);
	//printState(state[3], BS);
	//printState(state[4], BS);
}

void Rasta::constructEqs(bool m[], bool c[], int index, bool** eq) {
	//first, compute the output of first chi op
	bool** firstEqs;
	firstEqs = new bool* [BS];
	int fEqSize = BS+BS * (BS - 1) / 2 + 1;//degree=2
	for (int i = 0; i < BS; i++) {
		firstEqs[i] = new bool[fEqSize];
		for (int j = 0; j < fEqSize; j++) {
			firstEqs[i][j] = 0;//initialize
		}
	}
	for (int i = 0; i < BS; i++) {
		//us[i] = s[i]+con[0][i] + (s[i+1]+con[0][i+1]+1)(s[i+2]+con[0][i+2])
		//compute constant part
		firstEqs[i][fEqSize - 1] = con[0][i] ^ ((con[0][MOD(i + 1)] ^ 1) & con[0][MOD(i + 2)]);
		addRow(ma[0][i], firstEqs[i], BS);//add s[i]
		addConMul(con[0][MOD(i + 1)] ^ 1, ma[0][MOD(i + 2)], BS, 0, firstEqs[i], fEqSize-1);
		addConMul(con[0][MOD(i + 2)], ma[0][MOD(i + 1)], BS, 0, firstEqs[i], fEqSize - 1);
		addQu(ma[0][MOD(i + 1)], BS, 0, ma[0][MOD(i + 2)], BS, 0, firstEqs[i]);
	}
	//checkExpr(firstEqs, BS, fEqSize);
	//printState(con[0], BS);
	//printExpr(firstEqs, BS, fEqSize);
	//cout << "first correct" << endl;
	
	//the second round
	//first apply ma[1] to firstEqs -> second = ma[1] (first)
	bool** secondEqs;
	bool* tmpEq;
	secondEqs = new bool* [BS];
	int sEqSize = fEqSize;//degree=2
	tmpEq = new bool[fEqSize];
	for (int i = 0; i < BS; i++) {
		secondEqs[i] = new bool[sEqSize];
		for (int j = 0; j < sEqSize; j++) {
			secondEqs[i][j] = 0;//initialize
		}
	}
	//cout << "second constructed" << endl;
	//matrix mul
	for (int i = 0; i < BS; i++) {
		for (int j = 0; j < sEqSize; j++) {
			for (int k = 0; k < BS; k++) {
				secondEqs[i][j] = 
					secondEqs[i][j] ^ (ma[1][i][k] & firstEqs[k][j]);
			}
		}
	}
	//constant addition
	for (int i = 0; i < BS; i++) {
		secondEqs[i][sEqSize - 1] ^= con[1][i];
	}
	//printExpr(secondEqs, BS, sEqSize);
	//checkExpr(secondEqs, BS, sEqSize);//test passed
	//cout << "second correct" << endl;
	//system("pause");

	//sbox (use low-degree equations)
	//first, invert the last affine layer
	bool zt[BS], z[BS];//keystream
	for (int i = 0; i < BS; i++) {
		zt[i] = m[i] ^ c[i] ^ con[RD][i];
	}
	bool testValue[BS];
	//inverse z=ma[RD]^{-1} (zt)
	multiplyWithMatrix(zt, z, inverMa);//test passed
	int s = index * BS * 3,ix=0;
	//use low-degree equation in the last chi op
	for (int i = 0; i < BS; i++) {
		ix = s + 3 * i;
		if (ix >= ES-1) {
			break;
		}
		//cout << z[i] << " " << z[MOD(i + 1)] << " " << z[MOD(i + 2)] << endl;
		//No.1. (y[i+1]+z[i+1])(y[i]+z[i] + x[i])=0
		memset(tmpEq, 0, sEqSize);
		addRow(secondEqs[i], tmpEq, sEqSize);
		tmpEq[sEqSize - 1] ^= z[i];
		//-> (y[i + 1] + z[i + 1])(y[i] + x[i])=0
		addRow(inverMa[i], tmpEq, BS);
		//->(y[i + 1] + z[i + 1])x[i]=0
		bool t0=checkExpr(inverMa[MOD(i + 1)], BS, 0);
		bool t1=z[MOD(i + 1)];
		bool t2=checkExpr(tmpEq, sEqSize - 1, tmpEq[sEqSize - 1]);
		testValue[i] = (t0^t1) & t2;
		if (z[MOD(i + 1)]) {
			addRow(tmpEq, eq[ix], sEqSize - 1);
			eq[ix][ES - 1] ^= tmpEq[sEqSize - 1];
		}
		//cout << t0 << " " << t1 << " " << t2 << endl;
		//cout << checkExprCubic(eq[ix], ES) << endl;
		addCub(tmpEq, sEqSize - 1, tmpEq[sEqSize - 1], 
			inverMa[MOD(i + 1)], BS, 0, eq[ix]);
		if (checkExprCubic(eq[ix], ES) == 1) {
			cout << "wrong" << endl;
			system("pause");
		}

		//No.2. y[i]+z[i]+x[i] = (y[i+1]+z[i+1]+1)x[i+2]
		if (ix+1 >= ES-1) {
			break;
		}
		memset(tmpEq, 0, sEqSize);
		addRow(secondEqs[i], tmpEq, sEqSize);
		addRow(inverMa[i], tmpEq, BS);
		tmpEq[sEqSize - 1] ^= z[MOD(i)];
		//-> x[i] = (y[i+1]+z[i+1]+1)x[i+2]
		t0 = checkExpr(tmpEq, sEqSize - 1, tmpEq[sEqSize - 1]);
		t1 = checkExpr(inverMa[MOD(i + 1)], BS, 0);
		t2 = z[MOD(i + 1)] ^ 1;
		bool t3 = checkExpr(secondEqs[MOD(i + 2)], sEqSize - 1, secondEqs[MOD(i + 2)][sEqSize - 1]);
		testValue[i] = t0 ^ (t1 & t3) ^ (t2 & t3);
		if (testValue[i] == 1) {
			cout << i << " : " << t0 << " " << t1 << " " << t2 << endl;
		}
		//x[i] = (y[i+1]+z[i+1]+1)x[i+2]
		addRow(tmpEq, eq[ix + 1], sEqSize - 1);
		eq[ix + 1][ES - 1] ^= tmpEq[sEqSize - 1];
		addCub(secondEqs[MOD(i + 2)], sEqSize - 1, secondEqs[MOD(i + 2)][sEqSize - 1],
			inverMa[MOD(i + 1)], BS, 0, eq[ix + 1]);
		if (z[MOD(i + 1)] == 0) {
			addRow(secondEqs[MOD(i + 2)], eq[ix + 1], sEqSize - 1);
			eq[ix + 1][ES - 1] ^= secondEqs[MOD(i + 2)][sEqSize - 1];
		}
		if (checkExprCubic(eq[ix+1], ES) != 0) {
			cout << i << " wrong1" << endl;
		}
		
		//No.3. (y[i+3]+z[i+3])((y[i+2]+z[i+2])(y[i+1]+1+z[i+1])+y[i]+z[i]+x[i])
		t0 = checkExpr(inverMa[MOD(i + 2)], BS, z[MOD(i + 2)]);
		t1 = checkExpr(inverMa[MOD(i + 1)], BS, z[MOD(i + 1)] ^ 1);
		t2 = checkExpr(inverMa[i], BS, z[i]);
		t3 = checkExpr(secondEqs[i], sEqSize - 1, secondEqs[i][sEqSize - 1]);
		bool t4 = checkExpr(inverMa[MOD(i + 3)], BS, z[MOD(i + 3)]);
		testValue[i] = t4 & (t0 & t1 ^ t2 ^ t3);
		if (testValue[i] != 0) {
			cout << "wrongTest" << endl;
		}

		if (ix+2 >= ES-1) {
			break;
		}
		memset(tmpEq, 0, sEqSize);
		addRow(secondEqs[i], tmpEq, sEqSize);//constant is copied
		addRow(inverMa[i], tmpEq, BS);
		tmpEq[sEqSize - 1] ^= z[i];
		if (z[MOD(i + 1)]==0) {
			addRow(inverMa[MOD(i + 2)], tmpEq, BS);
		}
		if (z[MOD(i + 2)]) {
			addRow(inverMa[MOD(i + 1)], tmpEq, BS);
		}
		tmpEq[sEqSize - 1] = tmpEq[sEqSize - 1] ^ ((z[MOD(i + 1)] ^ 1) & z[MOD(i + 2)]);
		addQu(inverMa[MOD(i + 2)], BS, 0, inverMa[MOD(i + 1)], BS, 0, tmpEq);
		t0 = checkExpr(inverMa[MOD(i + 3)], BS, z[MOD(i + 3)]);
		t1 = checkExpr(tmpEq, sEqSize - 1, tmpEq[sEqSize - 1]);
		if ((t0 & t1) != 0) {
			cout << i << ": " << t0 << " " << t1 << endl;
		}
		addConMul(z[MOD(i + 3)], tmpEq, sEqSize - 1, tmpEq[sEqSize - 1], eq[ix + 2], ES-1);
		addCub(tmpEq, sEqSize - 1, tmpEq[sEqSize - 1],
			inverMa[MOD(i + 3)], BS, 0, eq[ix + 2]);
		if (checkExprCubic(eq[ix + 2], ES) != 0) {
			cout << i << " wrong2" << endl;
		}
	}
	freeMatrix(firstEqs,BS);
	freeMatrix(secondEqs, BS);
	delete[]tmpEq;
}

void Rasta::addQu(bool* m1, int m1Len, bool m1Cons, bool* m2, int m2Len, bool m2Cons, bool* res) {
	if (m1Cons) {
		//add m2
		addRow(m2, res, m2Len);
	}
	if (m2Cons) {
		//add m1
		addRow(m1, res, m1Len);
	}
	//add m1&m2
	for (int i = 0; i < m1Len; i++) {
		if (m1[i]) {
			for (int j = 0; j < m2Len; j++) {
				if (m2[j]) {
					res[quM[i][j]] ^= 1;
				}
			}
		}
	}
}

void Rasta::addCub(bool* m1, int m1Len, bool m1Cons, bool* m2, int m2Len, bool m2Cons, bool* res) {
	if (m1Cons) {
		//add m2
		addRow(m2, res, m2Len);
	}
	if (m2Cons) {
		//add m1
		addRow(m1, res, m1Len);
	}
	//add m1&m2 (m1 is quadratic)
	for (int i = 0; i < m1Len; i++) {
		if (m1[i]) {
			for (int j = 0; j < m2Len; j++) {
				if (m2[j]) {
					//cout << j << ", " << varM[i].v0 << ", " << varM[i].v1 << " ";
					//cout << "order: " << order(j, varM[i].v0, varM[i].v1) << endl;
					res[order(j,varM[i].v0,varM[i].v1)] ^= 1;
				}
			}
		}
	}
}

void Rasta::addConMul(bool conVar, bool* m1, int m1Len, bool m1Cons, bool* res,int resLen) {
	if (conVar) {
		res[resLen] = res[resLen]^(conVar & m1Cons);
		addRow(m1, res, m1Len);
	}
}

int Rasta::rank(bool **matrix,int mr,int mc) {
	bool **mat;  //Copy of the matrix
	mat = new bool* [mr];
	for (int i = 0; i < mr; i++) {
		mat[i] = new bool[mc];
		for (int j = 0; j < mc; j++) {
			mat[i][j] = matrix[i][j];
		}
	}
	//Transform to upper triangular matrix
	int variableNum = mc;
	bool isFirst = false;
	int targetRow = 0;

	for (int i = 0; i < variableNum; i++) {
		isFirst = true;
		for (int j = targetRow; j < mr; j++) {
			if (isFirst && mat[j][i]) {
				isFirst = false;
				swap(mat[j], mat[targetRow]);
				targetRow++;
			}
			else {
				if (mat[j][i]) {//apply Gauss
					for (int k = i; k < mc; k++) {
						mat[j][k] ^= mat[targetRow - 1][k];
					}
				}
			}
		}
	}

	/*for (int i = 0; i < mr; i++) {
		for (int j = 0; j < mc; j++) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
	cout << "target row:" << targetRow << endl;
	system("pause");
	cout << endl;*/

	freeMatrix(mat,mr);
	return targetRow;
}

void Rasta::inverse(bool** matrix, bool** res, int mr) {
	bool** mat;
	mat = new bool* [mr];
	int mc = mr * 2;
	for (int i = 0; i < mr; i++) {
		mat[i] = new bool[mc];
		for (int j = 0; j < mr; j++) {
			mat[i][j] = matrix[i][j];
		}
		for (int j = mr; j < mc; j++) {
			mat[i][j] = 0;
		}
		mat[i][i + mr] = 1;
	}
	//Transform to upper triangular matrix
	int variableNum = mc;
	bool isFirst = false;
	int targetRow = 0;

	for (int i = 0; i < variableNum; i++) {
		isFirst = true;
		for (int j = targetRow; j < mr; j++) {
			if (isFirst && mat[j][i]) {
				isFirst = false;
				swap(mat[j], mat[targetRow]);
				targetRow++;
			}
			else {
				if (mat[j][i]) {//apply Gauss
					for (int k = i; k < mc; k++) {
						mat[j][k] ^= mat[targetRow - 1][k];
					}
				}
			}
		}
	}
	//mat[0:mr][0:mr]: identity matrix
	for (int c=0; c < mr; c++) {
		for (int r = 0; r < c; r++) {
			if (mat[r][c]) {
				//add mat[c] to mat[r]
				for (int t = 0; t < mc; t++) {
					mat[r][t] ^= mat[c][t];
				}
			}
		}
	}
	//store the inverse to res
	for (int i = 0; i < mr; i++) {
		for (int j = 0; j < mr; j++) {
			res[i][j] = mat[i][j + mr];
		}
	}
	freeMatrix(mat, mr);
}

void Rasta::gauss(bool** eq, int mr, int mc,int cnt0[],int cnt1[]) {
	//Transform to upper triangular matrix
	int variableNum = mc-1;
	bool isFirst = false;
	int targetRow = 0;

	for (int i = variableNum-1; i >= 0; i--) {
		isFirst = true;
		for (int j = targetRow; j < mr; j++) {
			if (isFirst && eq[j][i]) {
				isFirst = false;
				swap(eq[j], eq[targetRow]);
				targetRow++;
			}
			else {
				if (eq[j][i]) {//apply Gauss
					for (int k = i; k >= 0; k--) {
						eq[j][k] ^= eq[targetRow - 1][k];
					}
					eq[j][mc - 1] ^= eq[targetRow - 1][mc - 1];
				}
			}
		}
	}

	bool te = 0;
	bool isLarge = false;
	//cout << "number of variables: " << ES - 1 << endl;
	//cout << "numb of eqs:" << ES - 1 << endl;
	//cout << "rank of eqs:" << targetRow << endl;
	cnt0[ES - 1 - targetRow]++;
	int size = 0;
	bool isValid = true;
	for (int i = targetRow-1; i >= targetRow - BS; i--) {
		te = 0;
		isLarge = false;
		for (int j = 0; j < mc - 1; j++) {
			if (eq[i][j]) {
				if (j < BS) {
					te ^= k[j];
				}
				else {
					isLarge = true;
				}
			}
			if (isLarge) {
				break;
			}
		}
		if (isLarge == false) {
			size++;
			if (eq[i][mc - 1] != te) {
				isValid = false;
			}
		}
	}
	//cout << "number of leqs:";
	//cout << size << endl;
	cnt1[BS - size]++;
	if (isValid) {
		cout << "key bits satisfy the final linear equations" << endl;
	}
	else {
		cout << "key bits do not satisfy the final linear equations" << endl;
	}
	//the final useful linear equtions
	/*isFirst = true;
	for (int i = 0; i < size; i++) {
		isFirst = true;
		for (int j = 0; j < BS; j++) {
			if (eq[targetRow - 1 - i][j]) {
				if (isFirst) {
					cout << "k" << j;
					isFirst = false;
				}
				else {
					cout << " + k"<< j;
				}
			}
		}
		cout << " = " << eq[i][mc - 1] << endl;
	}*/
}

void Rasta::printExpr(bool* eq, int col) {
	for (int j = 0; j < col - 1; j++) {
		if (j < BS && eq[j]) {
			cout << "x" << j << " ";
		}
		if (j >= BS && eq[j]) {
			cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
		}
	}
	cout << eq[col - 1] << endl;
}

bool Rasta::checkExpr(bool* eq, int col, bool c) {
	bool a = c;
	for (int j = 0; j < col; j++) {
		if (j < BS && eq[j]) {
			a ^= k[j];
			//cout << "x" << j << " ";
		}
		if (j >= BS && eq[j]) {
			a = a ^ (k[varM[j].v0] & k[varM[j].v1]);
			//cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
		}
	}
	return a;
}

void Rasta::printExpr(bool** eq, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col - 1; j++) {
			if (j < BS && eq[i][j]) {
				cout << "x" << j << " ";
			}
			if (j >= BS && eq[i][j]) {
				cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
			}
		}
		cout << eq[i][col - 1] << endl;
	}
}

void Rasta::checkExpr(bool** eq, int row, int col) {
	bool s[BS];
	for (int i = 0; i < row; i++) {
		s[i] = 0;
		for (int j = 0; j < col - 1; j++) {
			if (j < BS && eq[i][j]) {
				//cout << "x" << j << " ";
				s[i] ^= k[j];
			}
			if (j >= BS && eq[i][j]) {
				//cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
				s[i] = s[i] ^ (k[varM[j].v0] & k[varM[j].v1]);
			}
		}
		//cout << eq[i][col - 1] << endl;
		s[i] ^= eq[i][col - 1];
		cout << s[i];
	}
	cout << endl;
}

void Rasta::checkExprCubic(bool** eq, int row, int col) {
	bool s[BS];
	for (int i = 0; i < row; i++) {
		s[i] = 0;
		for (int j = 0; j < col - 1; j++) {
			if (j < BS && eq[i][j]) {
				//cout << "x" << j << " ";
				s[i] ^= k[j];
			}
			if (j >= BS && j<(BS+BS*(BS-1)/2) && eq[i][j]) {
				//cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
				s[i] = s[i] ^ (k[varM[j].v0] & k[varM[j].v1]);
			}
			if (j >= (BS + BS * (BS - 1) / 2) && eq[i][j]) {
				s[i] = s[i] ^ (k[varCM[j].v0] & k[varCM[j].v1] & k[varCM[j].v2]);
			}
		}
		//cout << eq[i][col - 1] << endl;
		s[i] ^= eq[i][col - 1];
		cout << s[i];
	}
	cout << endl;
}

bool Rasta::checkExprCubic(bool* eq, int col) {
	bool s = 0;
	for (int j = 0; j < col - 1; j++) {
		if (j < BS && eq[j]) {
			//cout << "x" << j << " ";
			s ^= k[j];
		}
		if (j >= BS && j < (BS + BS * (BS - 1) / 2) && eq[j]) {
			//cout << "x" << varM[j].v0 << "x" << varM[j].v1 << " ";
			s = s ^ (k[varM[j].v0] & k[varM[j].v1]);
		}
		if (j >= (BS + BS * (BS - 1) / 2) && eq[j]) {
			//cout << "x" << varCM[j].v0 << "x" << varCM[j].v1 << " " << "x" << varCM[j].v2;
			s = s ^ (k[varCM[j].v0] & k[varCM[j].v1] & k[varCM[j].v2]);
		}
	}
	s ^= eq[col - 1];
	//cout << s << endl;
	return s;
}