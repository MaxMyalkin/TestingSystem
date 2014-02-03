#include <iostream>
#include <vector>
#include "Student.h"

#define SUCCESS 0
#define FILE_OPEN_ERR 1
#define ID_ERR 2
#define INSTANCE_ERR 3

using namespace std;

void add_result(Student, int point, int mark);
bool result_exist(Student);
void delete_result(Student stud);
void update_result(Student stud, int point, int mark);
void student_result(Student stud);
void all_results();
void print(TestingSystem::result_msg);

void add_result(Student stud, int point, int mark)
{
	const char* res_file = Configuration::configuration->get_r_file();
	ofstream out(res_file, ios::app | ios::binary);
	if (result_exist(stud))
	{
		update_result(stud, point, mark);
		return;
	}
	TestingSystem::result_msg r_msg;
	r_msg.set_group_id(stud.get_group_id());
	r_msg.set_name(stud.get_name());
	r_msg.set_points(point);
	r_msg.set_mark(mark);
	r_msg.SerializeToOstream(&out);
	out.close();
}

bool result_exist(Student stud)
{
	const char* res_file = Configuration::configuration->get_r_file();
	ifstream in(res_file, ios::in | ios::binary);
	string s;
	TestingSystem::result_msg r_msg;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!r_msg.ParseFromString(s))
			r_msg.set_name("");
		else
		{
			if (r_msg.name() == stud.get_name() && r_msg.group_id() == stud.get_group_id())
			{
				return true;
			}
			s="";
		}
	}
	return false;
}

void delete_result(Student stud)
{
	const char* res_file = Configuration::configuration->get_r_file();
	ifstream in(res_file, ios::in | ios::binary);
	ofstream out("new_result", ios::app | ios::binary);
	string s;
	TestingSystem::result_msg r_msg;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!r_msg.ParseFromString(s))
			r_msg.set_name("");
		else
		{
			if (r_msg.name() != stud.get_name() || r_msg.group_id() != stud.get_group_id())
			{
				r_msg.SerializeToOstream(&out);
			}
			s="";
		}
	}
	in.close();
	out.close();
	remove(res_file);
	rename("new_result", res_file);
}

void update_result(Student stud, int point, int mark)
{
	delete_result(stud);
	add_result(stud, point, mark);
}

void student_result(Student stud)
{
	const char* res_file = Configuration::configuration->get_r_file();
	ifstream in(res_file, ios::in | ios::binary);
	string s;
	TestingSystem::result_msg r_msg;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!r_msg.ParseFromString(s))
			r_msg.set_name("");
		else
		{
			if (r_msg.name() == stud.get_name() && r_msg.group_id() == stud.get_group_id())
			{
				print(r_msg);
				return;
			}
			s="";
		}
	}
	cout << "No result";
}


void all_results()
{
	const char* res_file = Configuration::configuration->get_r_file();
	ifstream in(res_file, ios::in | ios::binary);
	string s;
	TestingSystem::result_msg r_msg;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!r_msg.ParseFromString(s))
			r_msg.set_name("");
		else
		{
			print(r_msg);
			s="";
		}
	}
}

void print(TestingSystem::result_msg msg)
{
	cout << endl;
	cout << "Студент: " << msg.name() << endl;
	cout << "Баллы: " << msg.points() << endl;
	cout << "Оценка: " << msg.mark() << endl;

}




