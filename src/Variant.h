#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <iomanip>
#include "Result.cpp"
#include <stdlib.h>
#include <stdio.h>
#include "algorithm"

bool has(const vector<int>::iterator, const vector<int>::iterator, int);

using namespace std;

class Variant
{
public:
	Variant(int num = 0, int count = 0 ):number(num), questions(count) {}
	Variant(const Variant& V);
	virtual ~Variant() {}

	Question &operator [] (int x){ return questions[x];	}
	void print();
	vector<Question>& getVector(){ return questions; }
	int TestMePlease(Student);
private:
	int number; //Номер варианта
	vector <Question> questions;
};
