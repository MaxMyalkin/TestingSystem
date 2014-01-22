#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

class Object
{
public:
	Object(int);
	Object( const Object& T);
	virtual ~Object(){};

	virtual void initialize_print() = 0;
	virtual int initialize_print_all() = 0;
	virtual int delete_from_file(int) = 0;
	virtual int get(int) = 0; //взять по id из файла
	virtual int put() = 0; //записать в файл

	void print(); //печать
	void print_all();

	inline bool instanced(){return getID()!=-1;}
	int getID() const { return id; }
	void setID(int _id){ id = _id; }
private:
	int id;
};

Object::Object( int id = -1 ):id ( id ) { }

Object::Object( const Object& T) : id(T.id) { }

void Object::print(){
	cout << endl;
	cout << "Вывод объекта" << endl;
	initialize_print();
}

void Object::print_all()
{
	cout << endl;
	cout << "Вывод всех объектов: " << endl;
	initialize_print_all();
}
/*
class Creator{
public:
    virtual Object factoryMethod(int ) = 0;
    virtual ~Creator(){};
};

class QuestionCreator: public Creator
{
public:
    Object factoryMethod(int id)
    {
    	const char* file_name = conf::configuration->get_q_file();
    		ifstream in(file_name, ios::in | ios::binary);
    		if (!in.is_open())
    		{
    			//ИСКЛЮЧЕНИЕ
    		}
    		TestingSystem::question_msg q_msg;
    		string s;
    		q_msg.set_id(-1);
    		while ((q_msg.id() != id) && in >> s)
    		{
    			//getline(in, s);

    			//q_msg.ParseFromIstream(&in);
    			q_msg.ParseFromString(s);
    		}
    		if (q_msg.id() == -1)
    		{
    			//ИСКЛЮЧЕНИЕ
    		}
    		Question q;
    		q.setID(q_msg.id());
    		q.set_title(q_msg.title());
    		q.set_content(q_msg.body());
    		in.close();
        return q;
    }
};


*/
