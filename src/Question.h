#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "Answer.h"

#define SUCCESS 0
#define FILE_OPEN_ERR 1
#define ID_ERR 2
#define INSTANCE_ERR 3

using namespace std;

int get_question_last_id(const char * file_name);

class Question: public Configuration , public Object
{
	friend void operator << (ostream& out , Question& Q );
	friend void operator >> (istream& in , Question& Q );

public:
	Question(std::string, std::string);
	Question();
	Question( const Question & );

	virtual ~Question(){};
	Question& operator = ( const Question& );

	void initialize_print();
	int initialize_print_all();
	int put();
	int get(int id);
	void initialize_obj(TestingSystem::question_msg);
	bool initialized();
	int update(std::string, std::string);//, std::vector<Answer>);
	void save();
	void add(std::string, std::string);
	void add();
	int delete_from_file(int id);

	void set_title(string s){title = s;}
	void set_content(string s){contents = s;}
	string get_title() {return title;}
	string get_contents() {return contents;}
	vector<Answer> get_answers();

private:
	std::string title; //title of question
	std::string contents;	//body of question
};


Question::Question(const Question& Q): Object(Q.getID()) , title(Q.title) , contents(Q.contents) { };

Question::Question(): Object(-1){ }

Question::Question(std::string _title, std::string _contents): Object(-1) , title(_title) , contents(_contents) {}


Question & Question::operator = (const Question & T)
{
	contents = T.contents;
	title = T.title;
	setID(T.getID());
	return *this;
};

bool Question::initialized()
{
	if (title != "" && contents != "" && getID() != -1)
		return true;
	return false;
}

void Question::save()
{
	update(get_title(), get_contents());
}

int Question::delete_from_file(int id)
{
	const char* file_name = Configuration::configuration->get_q_file();
	ifstream in(file_name, ios::in | ios::binary);
	ofstream new_file("new_file", ios::app | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	if (!new_file.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::question_msg q_msg;
	string s;
	q_msg.set_id(-1);
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
			if (q_msg.id() != id)
				q_msg.SerializeToOstream(&new_file);
		}
	}
	in.close();
	new_file.close();
	remove(file_name);
	rename("new_file", file_name);
	return SUCCESS;
}

void Question::initialize_print(){
	TestingSystem::question_msg q;
	q.set_title(this->title);
	q.set_body(this->contents);
	q.set_id(getID());
	std::string s1;
	std::cout << q.DebugString();
}

int Question::initialize_print_all()
{
	const char* file_name = Configuration::configuration->get_q_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::question_msg q_msg;
	string s;
	q_msg.set_id(-1);
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!q_msg.ParseFromString(s))
			q_msg.set_id(-1);
		else
		{
			Question q;
			q.initialize_obj(q_msg);
			q.print();
			s="";
		}
	}
	return SUCCESS;
}

void Question::initialize_obj(TestingSystem::question_msg msg)
{
	setID(msg.id());
	title = msg.title();
	contents = msg.body();
}

//ДОБАВЛЕНИЕ В КОНЕЦ ФАЙЛА


int Question::put()
{
	const char* file_name = Configuration::configuration->get_q_file();
	if (getID() <= get_last_id<TestingSystem::question_msg>(file_name))
	{
		return ID_ERR;
	}
	using namespace std;
	ofstream out(file_name, ios::app | ios::binary);
	if (!out.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::question_msg q;
	q.set_title(title);
	q.set_body(contents);
	q.set_id(getID());
	q.SerializeToOstream(&out);
	out.close();
	return SUCCESS;
}

//ЧТЕНИЕ ИЗ ФАЙЛА ОБЪЕКТА ПО ID
int Question::get(int id)
{
	const char* file_name = Configuration::configuration->get_q_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::question_msg q_msg;
	string s;
	q_msg.set_id(-1);
	while (q_msg.id() != id && in.cur!= in.eof())
		{
			char c;
			in.read((char*)&c, sizeof c);
			s+=c;
			if (!q_msg.ParseFromString(s))
				q_msg.set_id(-1);
			else s="";
		}

	if (q_msg.id() == -1)
	{
		return ID_ERR;
	}
	setID(q_msg.id());
	title = q_msg.title();
	contents = q_msg.body();
	in.close();
	return SUCCESS;
}

//ОБНОВЛЯЕТ ПО ID
int Question::update(std::string _title, std::string body)
{
	if (!instanced()) return INSTANCE_ERR;
	int index = getID();
	Question temp(_title, body);
	delete_from_file(index);
	temp.add();
	int new_id = temp.getID();
	title = _title;
	contents = body;

	const char* answers_file = Configuration::configuration->get_a_file();
	ifstream in(answers_file, ios::in | ios::binary);
	TestingSystem::answer_msg a_msg;
	ofstream new_file("new_file", ios::app | ios::binary);
	if( !in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	if( !new_file.is_open())
	{
		return FILE_OPEN_ERR;
	}
	string s;
	vector <Answer> answers;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s+=c;
		if (!a_msg.ParseFromString(s))
			a_msg.set_id(-1);
		else
		{
			s = "";
			if (a_msg.q_id() != index)
			{
				a_msg.SerializeToOstream(&new_file);
			}
			else
			{
				Answer cur_answer;
				cur_answer.set_contents(a_msg.body());
				cur_answer.set_correct(a_msg.correct());
				cur_answer.set_q_id(new_id);
				answers.push_back(cur_answer);
			}
		}
	}
	in.close();
	new_file.close();
		remove(answers_file);
		rename("new_file", answers_file);
	for (int i = 0; i < answers.size(); i++)
		answers[i].add();
	return SUCCESS;
}

vector<Answer> Question::get_answers()
{
	const char* answers_file = Configuration::configuration->get_a_file();
	ifstream in(answers_file, ios::in | ios::binary);
	if (!in.is_open())
	{
		//ИСКЛЮЧЕНИЕ
	}
	TestingSystem::answer_msg a_msg;
	string s;
	vector<Answer> result;
	while (in.cur!= in.eof())
	{
		char c;
		in.read((char*)&c, sizeof c);
		s+=c;
		if (!a_msg.ParseFromString(s))
			a_msg.set_id(-1);
		else
		{
			s = "";
			if (a_msg.q_id() == getID())
			{
				Answer a;
				a.set_contents(a_msg.body());
				a.set_correct(a_msg.correct());
				a.set_q_id(getID());
				a.setID(a_msg.id());
				result.push_back(a);
			}
		}
	}
	in.close();
	return result;
}

void Question::add(std::string _title, std::string body)
{
	const char* file_name = Configuration::configuration->get_q_file();
	setID(get_last_id<TestingSystem::question_msg>(file_name) + 1);
	title = _title;
	contents = body;
	put();
}

void Question::add()
{
	const char* file_name = Configuration::configuration->get_q_file();
	setID(get_last_id<TestingSystem::question_msg>(file_name) + 1);
	put();
}

void operator >> (istream& in , Question& Q )
{
	cout<< "Введите заголовок:";
	getline(in , Q.title);
	cout<< "Введите вопрос:";
	getline(in , Q.contents);
}

void operator << (ostream& out , Question& Q )
{
	out<< Q.title<< endl;
	out<< Q.contents<<endl;
}

