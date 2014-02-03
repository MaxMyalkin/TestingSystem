using namespace std;

class Student: public Configuration , public Object
{
	friend void operator >> (istream& , Student& );
	friend void operator << (ostream& , Student& );

public:
	Student(): Object(),course(0), group(-1){}
	Student( string name , int course, int group_id ):Object(), name(name), course(course), group(group_id){};

	void initialize_print();
	int put();
	int get(int id);
	int get(const char* name);
	int delete_from_file(int id);
	void initialize(TestingSystem::student_msg);
	int initialize_print_all();

	void set_name (string s){ name = s; }
	void set_group_id (int gr) { group = gr; }
	int get_group_id () { return group; }
	string get_name (){ return name; }

	void add(std::string, int, int);
	void add();
	int update(std::string, int , int);

private:
	string name;
	int course;
	int group;
};
