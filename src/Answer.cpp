/*
 * Answer.cpp
 *
 *  Created on: 04 февр. 2014 г.
 *      Author: maxim
 */
#include "Answer.h"
using namespace std;

#define SUCCESS 0
#define FILE_OPEN_ERR 1

Answer::Answer():Object(), contents("") , is_correct(false) , question_id(-1){};

Answer::Answer (std::string contents, bool is_correct , int question_id )
	: Object() , contents(contents) , is_correct(is_correct) , question_id(question_id) {};

void Answer::initialize_print()//печать
{
	TestingSystem::answer_msg ans;
	ans.set_id(getID());
	ans.set_body(contents);
	ans.set_correct(is_correct);
	ans.set_q_id(question_id);
	std::string str = ans.DebugString();
	std::cout<<str;
}

void Answer::initialize(TestingSystem::answer_msg msg)
{
	setID(msg.id());
	is_correct = msg.correct();
	contents = msg.body();
	question_id = msg.q_id();
}

int Answer::initialize_print_all()
{
	const char* file_name = Configuration::configuration->get_a_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::answer_msg a_msg;
	string s;
	a_msg.set_id(-1);
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!a_msg.ParseFromString(s))
			a_msg.set_id(-1);
		else
		{
			Answer a;
			a.initialize(a_msg);
			a.print();
			s="";
		}
	}
	return SUCCESS;
}

void Answer::set_contents(std::string content)
{
	contents = content;
}
void Answer::set_correct(bool correct)
{
	is_correct = correct;
}
void Answer::set_q_id(int id)
{
	question_id = id;
}

int Answer::get(int id)//взятие из файла
{
	const char* file = Configuration::get_a_file();
	ifstream in(file, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::answer_msg ans;
	string s;
	ans.set_id(-1);
	while (ans.id() != id && in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s+=c;
		if (!ans.ParseFromString(s))
			ans.set_id(-1);
		else s="";
	}
	setID(ans.id());
	contents = ans.body();
	is_correct = ans.correct();
	question_id = ans.q_id();
	in.close();
	return SUCCESS;
};

int Answer::put()//запись в файл
{
	const char* file = Configuration::get_a_file();
	std::ofstream out(file, std::ios::app | ios::binary);
	if (!out.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::answer_msg ans;
	ans.set_body(this->contents);
	ans.set_id(this->getID());
	ans.set_q_id(this->question_id);
	ans.set_correct(this->is_correct);

	ans.SerializeToOstream(&out);
	out.close();
	return SUCCESS;
}

void Answer::add(std::string body, bool correct, int q_id)
{
	const char* file_name = Configuration::get_a_file();
	setID(get_last_id<TestingSystem::answer_msg>(file_name) + 1);
	contents = body;
	is_correct = correct;
	question_id = q_id;
	put();
}

void Answer::add()
{
	const char* file_name = Configuration::get_a_file();
	setID(get_last_id<TestingSystem::answer_msg>(file_name) + 1);
	put();
}

//ОБНОВЛЯЕТ ПО ID
void Answer::update(std::string _body, bool _correct , int _q_id)
{
	if (!instanced()) return;
	int index = getID();
	Answer temp(_body, _correct , _q_id);
	delete_from_file(index);
	temp.add();
	contents = _body;
	is_correct = _correct;
	question_id = _q_id;
}


int Answer::delete_from_file(int id)
{
	const char* file_name = Configuration::configuration->get_a_file();
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
	TestingSystem::answer_msg a_msg;
	string s;
	a_msg.set_id(-1);
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
			if (a_msg.id() != id)
				a_msg.SerializeToOstream(&new_file);
		}
	}
	in.close();
	new_file.close();
	remove(file_name);
	rename("new_file", file_name);
	return SUCCESS;
}

void operator >> (istream& in , Answer& A )
{
	fflush(stdin);
	cout << "Введите ответ:";
	getline(in , A.contents);
	cout << "Введите правильность:";
	in >> A.is_correct;
	cout << "Введите номер вопроса:";
	int k;
	in >> k;
	getchar();
	A.question_id = k - 1;
}

void operator << (ostream& out , Answer& A )
{
	out<< A.contents<< endl;
}


