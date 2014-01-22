#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Question.h"

#define SUCCESS 0
#define FILE_OPEN_ERR 1
#define ID_ERR 2
#define INSTANCE_ERR 3

using namespace std;

int get_all_questions(vector <Question> &q);

class Test: public Configuration
{
public:
	Test ( const Test& );
	Test(string name = ""):name(name){}

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

Test::Test(const Test& T): count ( T.count ) , name ( T.name ) , questions ( T.questions ) { }
Question Test::operator [](int a)
{
	if (a >= questions.size())
		return questions[0];
	return questions[a];
}
void Test::print()
{
	for (vector<Question>::iterator iter = questions.begin(); iter != questions.end(); ++iter)
		iter->print();
}

void Test::get_questions()
{
	get_all_questions(questions);
	count = questions.size();
};

int get_all_questions(vector <Question> &q)
{
	q.clear();
	const char* file_name = Configuration::configuration->get_q_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::question_msg q_msg;
	string s;
	Question current_question;
	while (in.cur!= in.eof())
	{
		char c;
		in.read((char*)&c, sizeof c);
		s+=c;
		if (!q_msg.ParseFromString(s))
			q_msg.set_id(-1);
		else
		{
			s = "";
			current_question.setID(q_msg.id());
			current_question.set_title(q_msg.title());
			current_question.set_content(q_msg.body());
			q.push_back(current_question);
		}
	}
	return SUCCESS;
}
