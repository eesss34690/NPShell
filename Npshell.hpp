#include <string>
#include "Pipe_block.hpp"
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

class Npshell
{
private:
	string           m_input;
	list<Pipe_block> m_pipe;
	queue<string>    m_cmd;
	int              fd_table_1[2];
	int              fd_table_2[2];
public:
	void set_input (string input);
	int line_cmd();
	size_t find_char(string cmd, char target, int start_idx);
	string separate_output(string cmd, int start_idx, int end_idx);	
};
