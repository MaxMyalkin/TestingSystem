#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Question.cpp"

using namespace std;

int get_all_questions(vector <Question> &q);

class Test: public Configuration
{
public:
	Test ( const Test& );
	Test(string name = ""):name(name), count(0){}

	void set_count(int i){count = i ;}
	void get_questions();
	void get_questions(vector <Question> a);
	inline bool compare_size(){return (count <= get_test_size());} //Проверка на количество вопросов в варианте и количеством вопросов
	inline int get_variant_size(){return count;} //Количество вопросов во варианте
	void print();
	inline int get_test_size(){return questions.size();}; //Количество вопросов в тесте
	Question operator [] (int);
	~Test(){};
private:
	vector <Question> questions;
	string name;
	int count; //Количество вопросов во варианте
};
