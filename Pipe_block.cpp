#include <stdlib.h>

#define BUFFERSIZE 4096 

using namespace std;

Pipe_block::Pipe_block()
{
	m_num = 0;
	m_floag = 0;
}

int Pipe_block::printenv()
{
	if (m_argv.size() < 2)
		cerr << "Invalid arguments: printenv\n";
	else
	{
		const char * env = getenv(m_argv[1].c_str());
		if (env != NULL)
			cout << env << endl;
		else
			cout << endl;
	}
	return 0;
}

int Pipe_block::setenv()
{
	if (m_argv.size() < 3)
		cerr << "Invalid arguments: setenv\n";
	else
		setenv(m_argv[1].c_str(), m_argv[2].c_str());
	return 0;
}

int Pipe_block::execute()
{
	// |N, >: don't care
	if (m_flag > 2)
		return 0;
	// builtin function
	if (m_flag == 3)
	{
		if (m_argv[0] == "printenv")
			return printenv();
		else if (m_argv[0] == "setenv")
			return setenv();
	}
	else if (m_flag == 5)
	{
		for ()
		exit(0)
		return 0;
	}
	pid_t child_pid;
	
}
