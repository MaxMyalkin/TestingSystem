#include "Abstract.cpp"
using namespace std;

class Configuration
{
public:
	static Configuration* Instance(const char* q_file,const char* a_file,const char* s_file,const char* g_file, const char* r_file);
	static Configuration* configuration;
	const char* get_q_file(){return configuration->q_file.c_str();}
	const char* get_a_file(){return configuration->a_file.c_str();}
	const char* get_s_file(){return configuration->s_file.c_str();}
	const char* get_g_file(){return configuration->g_file.c_str();}
	const char* get_r_file(){return configuration->r_file.c_str();}

protected:
	Configuration() { }
	Configuration(const char* q_file,const char* a_file,const char* s_file,const char* g_file, const char* r_file): q_file(q_file) , a_file(a_file) , s_file(s_file), g_file(g_file), r_file(r_file){}
	void changeConf(const char* q_file,const char* a_file,const char* s_file,const char* g_file, const char* r_file)
{
	configuration->a_file = a_file;
	configuration->q_file = q_file;
	configuration->s_file = s_file;
	configuration->g_file = g_file;
	configuration->r_file = r_file;
}
private:
	string q_file;
	string a_file;
	string s_file;
	string g_file;
	string r_file;

};
