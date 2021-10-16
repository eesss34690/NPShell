#include <string>
#include <list>
#include <queue>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

class Pipe_block
{
private:
	int 		m_num;
	int 		m_flag;
	string 		m_filename;
	char * []	m_argv;
	int printenv();
	int setenv();

public:
	Pipe_block();
	int execute();
	void set_cnt(int num) {m_num = num;}
	void set_flag(int flag) {m_flag = flag;}
	void set_file(string filename) {m_filename = filename;}
	void set_argv(char * argv []) {m_argv = argv;}
	int get_cnt() {return m_num;}
	int get_flag() {return m_num;}
	string get_file() {return m_filename;}
	char * [] get_argv() {return m_argv;}
};
