#ifndef _RASTA_H
#define _RASTA_H

//block size = 21
#define BS 21//number of columns: 1330 + 210 + 21 + 1 = 1562
//rounds
#define RD 2

//#define ES 1562
const int ES = 1+BS + BS * (BS - 1) / 2 + BS * (BS - 1) * (BS - 2) / 6;
//mod
#define MOD(i) ((i)%BS)

struct VAR {
	int v0, v1, v2;
};

class Rasta {
private:
	bool ***ma;//2-round, ma[i]: the used linear layer
	bool **con;//con[i]: the round constants
	bool *k;//key
	bool **inverMa;//the inverse of the last linear matrix
	int **cuM;
	int **quM;
	VAR varM[BS + BS * (BS - 1) / 2];//quadratic term
	VAR varCM[ES-1];//cubic term
public:
	void initialize();
	void generateRandMatrix();
	~Rasta();

	void setKey(bool key[]);
	void encrypt(bool m[], bool c[]);
	void constructEqs(bool m[], bool c[], int index, bool **eq);
	void generateMa(int counter, int nonce);//generate used matrices
	void gauss(bool** eq,int mr,int mc,int cnt0[],int cnt1[]);

	//matrix operation
	void multiplyWithMatrix(bool input[], bool output[], bool **matrix);
	void freeMatrix(bool** eq, int row);
	void addRow(bool* src, bool* des,int size);
	void addQu(bool* m1, int m1Len, bool m1Cons, bool* m2, int m2Len, bool m2Cons, bool* res);
	void addCub(bool* m1, int m1Len, bool m1Cons, bool* m2, int m2Len, bool m2Cons, bool* res);
	void addConMul(bool conVar, bool* m1, int m1Len, bool m1Cons, bool* res,int resLen);
	int rank(bool** matrix,int r,int c);
	void inverse(bool** matrix, bool** mat, int r);

	//monomial oder
	void precomputeOrderCubic(int **start);
	void precomputeOrderQuadratic(int **map);
	int order(int x0, int x1, int x2);

	//output
	void printState(bool* s, int size);
	void printExpr(bool** eq, int row,int col);
	void checkExpr(bool** eq, int row, int col);
	void printExpr(bool* eq, int col);
	bool checkExpr(bool* eq, int col, bool c);
	void checkExprCubic(bool** eq, int row, int col);
	bool checkExprCubic(bool* eq, int col);
};

#endif
