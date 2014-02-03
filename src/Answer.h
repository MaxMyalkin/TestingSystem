#include "TestingSystem.pb.h"
#include <iostream>
#include <fstream>
#include "Config.cpp"

using namespace std;
class Answer : public Object, public Configuration
{
	friend void operator << (ostream& out , Answer& A );
	friend void operator >> (istream& in , Answer& A );

public:
	Answer();
	Answer(std::string , bool , int );

	virtual ~ Answer(){};

	int get(int id );
	void initialize_print();
	void initialize(TestingSystem::answer_msg);
	int put();
	int delete_from_file(int);
	int initialize_print_all();
	void update( string , bool , int );
	void add(std::string body, bool correct, int);
	void add();
	inline void set_contents(std::string);
	inline void set_correct(bool);
	inline void set_q_id(int id);
	inline bool get_correct() {return is_correct;};

private:
	std::string contents;	//содержание ответа
	bool is_correct;//Правильный или нет
	int question_id;
};


