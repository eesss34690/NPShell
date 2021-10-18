#include <stdlib.h>
#include <iostream>
#include "typedef.hpp"
#define BUFFERSIZE 4096 
#define MaxForks   2048
using namespace std;

Pipe_block::Pipe_block()
{
	exist = false;
	m_num = 0;
	m_flag = 0;
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
		::setenv(m_argv[1].c_str(), m_argv[2].c_str(), 1);
	return 0;
}

char ** Pipe_block::parse_arg()
{
	char** arg = new char*[m_argv.size() + 1];
	size_t i = 0;
	for (; i< m_argv.size(); i++)
	{
		arg[i] = strdup(m_argv.at(i).c_str());
	}
	arg[i] = NULL;
	return arg;
}

int Pipe_block::execute(Pipeline& all, bool first, bool last)
{
	if (m_flag == 5)
	{
		for (int i=0; i< MaxForks; i++)
		{
			for (auto &j: all.get_child_proc(i))
				kill(j, SIGKILL);
		}
		exit(0);
		return 0;
	}
	else
	{
		cout << "m_num: " << m_num <<endl;
		m_pipe_in = all.get_pipe(0);
		m_pipe_out = Pipe_IO::create();
		//if (!m_pipe.mode_on())
		//{
		//	cout << "create\n";
			//all.set_pipe(0, Pipe_IO::create());
		//	m_pipe = all.get_pipe(m_num);
		//}
		//else
		//{
		//m_pipe.construct_pipe();
		//}
		cout << all.get() << " pipe in: "<< m_pipe_in.get_in() <<" " <<m_pipe_out.get_out() << endl;
		cout << "\tpipe out: " << m_pipe_out.get_in() << " " <<m_pipe_out.get_out() <<endl;
		// get its Pipe
		//if (!exist)
		//{
		//	m_pipe = all.get_pipe(0);
		//	cout << "create pipe IO\n";
		//	all.set_pipe(0, Pipe_IO::create());
		//	exist = true;
		//}
		//if (m_flag < 2)
		//	all.add_pipe(m_num);
		
		// fork
		pid_t child_pid = fork();
		if (child_pid < 0)
			return 1;
		// parent proc.
		else if (child_pid > 0)
		{
			// child will do the job, close it
			::close(m_pipe_out.get_out());
			::close(m_pipe_in.get_in());
			::close(m_pipe_in.get_out());
			cout << "Own child process: " << child_pid <<endl;
			all.add_process(0, child_pid);
			return 0;
		}
		// child proc.
		else 
		{
			// deal with fd table duplication
			// case 1: !N (0)
			if (m_flag == 0)
			{
				auto fd = all.get_pipe(m_num).get_out();
				cout << "pipe 1 out: " << fd <<endl;
				if (fd != -1)
					dup2(fd, STDERR_FILENO);
				if (fd != -1)
					dup2(fd, STDOUT_FILENO);
			}
			// case 2: |N (1)
			else if (m_flag == 1)
			{
				auto fd = all.get_pipe(m_num).get_out();
				cout << "pipe 1 out: "<< fd << endl;
				if (fd != -1)
					dup2(fd, STDOUT_FILENO);
				cout << "?\n";
			}
			else if (m_flag == 2)
			{
				int fd_file = open(m_filename.c_str(), (O_RDWR | O_CREAT | O_TRUNC), 0644);
				dup2(fd_file, STDOUT_FILENO);
				cout << "!\n";
			}
			else if (m_flag > 2)
			{
				if (!first)
				{
					int fd = m_pipe_in.get_in();
					if (fd != -1)
					{
						cout <<"enter in\n";
						dup2(fd, STDIN_FILENO);
					}
				}
				if (!last)
				{
					int fd = m_pipe_out.get_out();
					if (fd != -1)
					{
						cout << "enter out\n";
						dup2(fd, STDOUT_FILENO);	
					}
				}	
			}
			cout << "final fd in : " << m_pipe_in.get_in() <<" " << m_pipe_in.get_out() << endl;
			cout << "final fd out: " << m_pipe_out.get_out()<<" "<< m_pipe_out.get_out()<<endl;
			// finish fd table reassignment, close it
			//m_pipe.close();
			::close(m_pipe_out.get_in());
			::close(m_pipe_in.get_in());
			::close(m_pipe_in.get_out());
			::close(m_pipe_out.get_out());
			
			//all.close_all();
			// execution
			
			char ** arg = parse_arg();
			if (m_flag == 3)
			{
				if (m_argv[0] == "printenv")
					return printenv();
				else if (m_argv[0] == "setenv")
					return setenv();
			}
			else if (execvp(("./bin/" + m_argv[0]).c_str(), arg) < 0)
			{
				cerr << "Unknown command: [" << m_argv[0] << "]." << endl;
				exit(0);
			}
	
			return 0;
		}
	}
	return 0;
}
