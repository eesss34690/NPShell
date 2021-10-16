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

class Command
{
private:
	list<Pipe_block> m_block;
	size_t find_char(string cmd, char target, int start_idx);
	string separate_output(string cmd, int start_idx, int end_idx);	

public:
	Command(string cmd);
	list<Pipe_block> get_block () {return m_block;}

};
