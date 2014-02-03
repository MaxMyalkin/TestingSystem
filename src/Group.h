#include<fstream>
#include<vector>
using namespace std;

class Group: public Configuration , public Object
{
	friend void operator >> (istream& , Group& );
	friend void operator << (ostream& , Group& );

public:
	Group(string _name = "") : name(_name) { };
	Group( const Group & );
	Group& operator = ( const Group& );
	virtual ~Group(void){};

	void initialize_print();
	int initialize_print_all();
	int put();
	int get(int id);
	int get(const char*);
	void add(string);
	void add();
	void set_name(string s){name = s;}
	string get_name(){return name;}
	vector<Student> get_students();
	int delete_from_file(int id);
	int update(string);
	void initialize_obj(TestingSystem::group_msg msg);
private:
	string name;
};

