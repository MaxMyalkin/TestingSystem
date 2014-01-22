#define SUCCESS 0
#define FILE_OPEN_ERR 1
#define ID_ERR 2
#define INSTANCE_ERR 3

using namespace std;

class Student: public Configuration , public Object
{
	friend void operator >> (istream& , Student& );
	friend void operator << (ostream& , Student& );

public:
	Student(): Object(){}
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

void Student::initialize_print()//печать
{
	TestingSystem::student_msg student;
	student.set_id(getID());
	student.set_name(name);
	student.set_group_id(group);
	std::string str = student.DebugString();
	std::cout<<str;
}


int Student::get(int id)//взятие из файла
{
	const char* file = Configuration::get_s_file();
	ifstream in(file, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg stud;
	string s;
	stud.set_id(-1);
	while (stud.id() != id && in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s+=c;
		if (!stud.ParseFromString(s))
			stud.set_id(-1);
		else s="";
	}
	setID(stud.id());
	name = stud.name();
	group = stud.group_id();
	course = stud.course();
	in.close();
	return SUCCESS;
};

int Student::get(const char* _name)//взятие из файла
{
	const char* file = Configuration::get_s_file();
	ifstream in(file, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg stud;
	string s;
	stud.set_id(-1);
	while (stud.name() != _name && in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s+=c;
		if (!stud.ParseFromString(s))
			stud.set_id(-1);
		else s="";
	}
	setID(stud.id());
	name = stud.name();
	group = stud.group_id();
	course = stud.course();
	in.close();
	return SUCCESS;
};

int Student::put()//запись в файл
{
	const char* file = Configuration::configuration->get_s_file();
	std::ofstream out(file, std::ios::app | ios::binary);
	if (!out.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg student;
	student.set_id(getID());
	student.set_course(course);
	student.set_group_id(group);
	student.set_name(name);
	student.SerializeToOstream(&out);
	out.close();
	return SUCCESS;
}

void Student::add(std::string _name, int _course, int _group)
{
	const char* file = Configuration::configuration->get_s_file();
	setID(get_last_id<TestingSystem::student_msg>(file) + 1);
	course = _course;
	group = _group;
	name = _name;
	put();
}

void Student::add()
{
	const char* file_name = Configuration::get_s_file();
	setID(get_last_id<TestingSystem::student_msg>(file_name) + 1);
	put();
}



int Student::delete_from_file(int id)
{
	const char* file_name = Configuration::configuration->get_s_file();
	ifstream in(file_name, ios::in | ios::binary);
	ofstream new_file("new_file", ios::app | ios::binary);
	if ( !in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	if ( !new_file.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg s_msg;
	string s;
	s_msg.set_id(-1);
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
			if (s_msg.id() != id)
				s_msg.SerializeToOstream(&new_file);
		}
	}
	in.close();
	new_file.close();
	remove(file_name);
	rename("new_file", file_name);
	return SUCCESS;
}

int Student::update(std::string _name, int _course , int _group)
{
	if (!instanced()) return INSTANCE_ERR;
	int index = getID();
	Student temp( _name , _course , _group );
	delete_from_file(index);
	temp.add();
	name = _name;
	course = _course;
	group = _group;
	return SUCCESS;
}

void Student::initialize(TestingSystem::student_msg msg)
{
	setID(msg.id());
	group = msg.group_id();
	name = msg.name();
}

int Student::initialize_print_all()
{
	const char* file_name = Configuration::configuration->get_s_file();
	ifstream in(file_name, ios::in | ios::binary);
	if (!in.is_open())
	{
		return FILE_OPEN_ERR;
	}
	TestingSystem::student_msg s_msg;
	string s;
	s_msg.set_id(-1);
	while (in.cur!= in.eof())
	{
		char c;
		in.read(&c, sizeof c);
		s += c;
		if (!s_msg.ParseFromString(s))
			s_msg.set_id(-1);
		else
		{
			Student st;
			st.initialize(s_msg);
			st.print();
			s = "";
		}
	}
	return SUCCESS;
}

void operator >> (istream& in , Student& S )
{
	cout<< "Введите имя:";
	getline(in , S.name);
	cout<< "Введите курс:";
	in >>S.course;
	cout<< "Введите группу:";
	in >>S.group;
	getchar();
}

void operator << (ostream& out , Student& S )
{
	out<< "Имя: " << S.name << endl;
	out<< "Курс:" << S.course <<endl;
}
