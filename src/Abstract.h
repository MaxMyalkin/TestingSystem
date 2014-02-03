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


