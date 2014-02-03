/*
 * Config.cpp
 *
 *  Created on: 04 февр. 2014 г.
 *      Author: maxim
 */
#include "Config.h"
using namespace std;

Configuration* Configuration::configuration = NULL;

Configuration* Configuration::Instance(const char* q_file,const char* a_file,const char* s_file,const char* g_file, const char* r_file)
{
	if ( Configuration::configuration == NULL )
	{
		configuration = new Configuration(q_file, a_file, s_file, g_file, r_file);
	}
	else
		configuration->changeConf(q_file, a_file, s_file, g_file, r_file);
	return configuration;
}

//ВЫБОРКА ПОСЛЕДНЕГО ID В ФАЙЛЕ

template <class T>
int get_last_id(const char * file_name)
{
	//Исключение на неправильный тип
	using namespace std;
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		// хз что здесь надо возвратить
	}
	T msg;
	string s = "";
	if (!msg.ParseFromIstream(&in)) return -1;
	return msg.id();
}



