#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "Answer.cpp"
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

