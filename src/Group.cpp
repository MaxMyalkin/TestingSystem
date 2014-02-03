/*
 * Group.cpp
 *
 *  Created on: 04 февр. 2014 г.
 *      Author: maxim
 */
#include "Group.h"
using namespace std;

#define SUCCESS 0
#define FILE_OPEN_ERR 1
#define ID_ERR 2
#define INSTANCE_ERR 3


Group & Group::operator = (const Group & T)
{
	name = T.name;
	setID(T.getID());
	return *this;
};

Group::Group(const Group& Q): Object(Q.getID())
{
	name = Q.name;
}

//ВЫВОД ОБЪЕКТА В КОНСОЛЬ
void Group::initialize_print()
{
	TestingSystem::group_msg q;
	q.set_name(this->name);
	q.set_id(getID());
	std::string s1;
	std::cout << q.DebugString();
}

//ДОБАВЛЕНИЕ В КОНЕЦ ФАЙЛА
int Group::put()
{
	const char* file_name =Configuration::configuration->get_g_file();
	if (getID() <= get_last_id<TestingSystem::group_msg>(file_name))
	{
		return ID_ERR;
	}
	ofstream out(file_name, ios::app | ios::binary);
	if (!out.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::group_msg g;
	g.set_name(this->name);
	g.set_id(this->getID());
	//q.SerializeToOstream(&out);
	g.SerializeToOstream(&out);
	out.close();
	return SUCCESS;
}

//ЧТЕНИЕ ИЗ ФАЙЛА ОБЪЕКТА ПО ID
int Group::get(int id)
{
	const char* file_name = Configuration::configuration->get_g_file();
		ifstream in(file_name, ios::in | ios::binary);
		if (!in.is_open())
		{
			return FILE_OPEN_ERR;
		}
		TestingSystem::group_msg g_msg;
		string s;
		g_msg.set_id(-1);
		while (g_msg.id() != id && in.cur!= in.eof())
			{
				char c;
				in.read((char*)&c, sizeof c);
				s+=c;
				if (!g_msg.ParseFromString(s))
					g_msg.set_id(-1);
				else s="";
			}

		if (g_msg.id() == -1)
		{
			return ID_ERR;
		}
		setID(g_msg.id());
		name = g_msg.name();
		in.close();
		return SUCCESS;
}

int Group::get(const char * _name)
{
	const char* file_name = Configuration::configuration->get_g_file();
		ifstream in(file_name, ios::in | ios::binary);
		if (!in.is_open())
		{
			return FILE_OPEN_ERR;
		}
		TestingSystem::group_msg g_msg;
		string s;
		g_msg.set_id(-1);
		while (g_msg.name() != _name && in.cur!= in.eof())
			{
				char c;
				in.read((char*)&c, sizeof c);
				s+=c;
				if (!g_msg.ParseFromString(s))
					g_msg.set_id(-1);
				else s="";
			}

		if (g_msg.id() == -1)
		{
			return ID_ERR;
		}
		setID(g_msg.id());
		name = g_msg.name();
		in.close();
		return SUCCESS;
}

vector<Student> Group::get_students()
{
	const char* student_file = Configuration::configuration->get_s_file();
		ifstream in(student_file, ios::in | ios::binary);
		if (!in.is_open())
		{
			//ИСКЛЮЧЕНИЕ
		}
		TestingSystem::student_msg s_msg;
		string s;
		vector<Student> result;
		while (in.cur!= in.eof())
		{
			char c;
			in.read((char*)&c, sizeof c);
			s+=c;
			if (!s_msg.ParseFromString(s))
				s_msg.set_id(-1);
			else
			{
				s = "";
				if (s_msg.group_id() == getID())
				{
					Student s;
					s.set_group_id(s_msg.group_id());
					s.set_name(s_msg.name());
					s.setID(s_msg.id());
					result.push_back(s);
				}
			}
		}
		in.close();
		return result;
}

void Group::add(std::string name)
{
	const char* file_name = Configuration::configuration->get_g_file();
	setID(get_last_id<TestingSystem::group_msg>(file_name) + 1);
	this->name = name;
	this->put();
}

int Group::delete_from_file(int id)
{
	const char* file_name = Configuration::configuration->get_g_file();
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
	TestingSystem::group_msg g_msg;
	string s;
	g_msg.set_id(-1);
	while (in.cur!= in.eof())
	{
		char c;
		in.read((char*)&c, sizeof c);
		s+=c;
		if (!g_msg.ParseFromString(s))
			g_msg.set_id(-1);
		else
		{
			s = "";
			if (g_msg.id() != id)
				g_msg.SerializeToOstream(&new_file);
		}
	}
	in.close();
	new_file.close();
	remove(file_name);
	rename("new_file", file_name);
	return SUCCESS;
}

int Group::update(std::string _name)
{
	if (!instanced()) return INSTANCE_ERR ;
	int index = getID();
	Group temp(_name);
	delete_from_file(index);
	temp.add();
	int new_id = temp.getID();
	name = _name;

	const char* student_file = Configuration::configuration->get_s_file();
	ifstream in(student_file, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg s_msg;
	ofstream new_file("new_file", ios::app | ios::binary);
	string s;
	vector <Student> student;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s+=c;
		if (!s_msg.ParseFromString(s))
			s_msg.set_id(-1);
		else
		{
			s = "";
			if (s_msg.group_id() != index)
			{
				s_msg.SerializeToOstream(&new_file);
			}
			else
			{
				Student cur_stud;
				cur_stud.set_name(s_msg.name());
				cur_stud.set_group_id(new_id);
				student.push_back(cur_stud);
			}
		}
	}
	in.close();
	new_file.close();
	remove(student_file);
	rename("new_file", student_file);
	for (int i = 0; i < student.size(); i++)
		student[i].add();
	return SUCCESS;
}

void Group::add()
{
	const char* file_name = Configuration::configuration->get_g_file();
	setID(get_last_id<TestingSystem::group_msg>(file_name) + 1);
	put();
}

int Group::initialize_print_all()
{
	const char* file_name = Configuration::configuration->get_g_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::group_msg g_msg;
	string s;
	g_msg.set_id(-1);
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!g_msg.ParseFromString(s))
			g_msg.set_id(-1);
		else
		{
			Group g;
			g.initialize_obj(g_msg);
			g.print();
			s="";
		}
	}
	return SUCCESS;
}

void Group::initialize_obj(TestingSystem::group_msg msg)
{
	setID(msg.id());
	name = msg.name();
}


void operator >> (istream& in , Group& g)
{
	cout<<"Введите название группы:";
	in>>g.name;
}

void operator << (ostream& out , Group& g)
{
	out<<g.name;
}

void group_result(Group g)
{
	const char* res_file = Configuration::configuration->get_r_file();
	ifstream in(res_file, ios::in | ios::binary);
	string s;
	TestingSystem::result_msg r_msg;
	int i = 0;
	int summary = 0;
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!r_msg.ParseFromString(s))
			r_msg.set_name("");
		else
		{
			if (r_msg.group_id() == g.getID())
			{
				cout<<"Студент "<<++i<<endl;
				print(r_msg);
				summary+=r_msg.points();
				cout << endl;
			}
			s="";
		}
	}
	cout<< "Средний балл по группе: " << summary/i<<endl;
}


