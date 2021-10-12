#include <stdlib.h>
#include "Npshell.hpp"

#define BUFFERSIZE 4096 

using namespace std;

void Npshell::set_input (string input)
{
	m_input = input;
}

int Npshell::line_cmd()
{
	// step 1: separate the input
	auto end_idx = 0;
	auto start_idx = 0;
	string single_cmd;
	
	m_pipe.push_back(*(new Pipe_block));
	while (end_idx < m_input.length())
	{
		end_idx = m_input.find('|', start_idx);
		if (end_idx == string::npos)
			end_idx = m_input.length();
		
		// special case: |N
		// the counter of redirect output should be set
		// note the cmd as redirect command
		if (start_idx && m_input[start_idx]- '0' >= 0 && m_input[start_idx]- '0'< 10)
		{
			m_pipe.back().set_cnt(stoi(m_input.substr(start_idx, end_idx- start_idx)));
			m_pipe.back().set_flag(1);
		}
		// deal with space 
		else
		{
			while (m_input[start_idx] == ' ')
				start_idx++;
		}

		// the command in the middle should be set without the final space
		single_cmd = (end_idx == m_input.length())? m_input.substr(start_idx, end_idx - start_idx) : m_input.substr(start_idx, end_idx - start_idx - 1);
		
		// if there is > redirect output, separate to 2 commands
		auto ge_idx = single_cmd.find('>', 0);
		if (ge_idx != string::npos)
		{
			m_cmd.push(single_cmd.substr(0, ge_idx - 1));
			m_cmd.push(single_cmd.substr(ge_idx, single_cmd.length() - ge_idx));
		}		
		else
		{
			m_cmd.push(single_cmd);
		}
		start_idx = end_idx + 1;
	}

	// step 2: deal with the commands with dfs
	string result;
	char buf[BUFFERSIZE];
	bool prev_input_insert = true;
	pid_t child_pid;
	// clean buffer
	memset(buf, 0, sizeof(buf));
	// output the pipeline first
	for (auto i = m_pipe.begin(); i != m_pipe.end(); i++)
	{
		if(i->get_flag())
		{
			if (!i->get_cnt())
			{
				result += i->get_result();
				i = m_pipe.erase(i);	
			}
			else
				i->set_cnt(i->get_cnt() - 1);
		} 
		
	}
	
	// deal with each cmds
	size_t space;
	while (!m_cmd.empty())
	{
		// find the command action 
		space = m_cmd.front().find(' ', 0);
		string cmd = m_cmd.front().substr(0, space);
		
		// different action
		if (cmd == "setenv")
		{
				auto space_2 = m_cmd.front().find(' ', space + 1);
				setenv(m_cmd.front().substr(space + 1, space_2 - space).c_str(), \
					m_cmd.front().substr(space_2 + 1, m_cmd.front().length() - space_2 - 1).c_str(), 1);
				m_cmd.pop();
		}
		else if (cmd == "printenv")
		{
				getenv(m_cmd.front().substr(space + 1, m_cmd.front().length() - space - 1).c_str());
				m_cmd.pop();
		}
		else if (cmd == ">")
		{
				FILE* file;
				file = fopen(m_cmd.front().substr(space + 1, m_cmd.front().length() - space - 1).c_str(), "w");
				fputs(buf, file);
				fclose(file);
				m_cmd.pop();
		}
		else if (cmd == "exit")
			exit(0);
		else
		{
				// use the command from ./bin
				string file = "./bin" + cmd;
				// pipe 2 fd table
				pipe(fd_table_1);
				pipe(fd_table_2);
				signal(SIGCHLD, SIG_IGN);
				child_pid = fork();
				
				// child process
				if (child_pid == 0)
				{
					// close pipe1-Write, pipe2-Read
					close(fd_table_1[1]);
					close(fd_table_2[0]);
					// duplicate pipe1-Read to original STDIN \
					// pipe2-Write to original STDOUT
					dup2(fd_table_1[0], STDIN_FILENO);
					dup2(fd_table_2[1], STDOUT_FILENO);
					//execute the command
					if (space == string::npos)
					{
						auto status = execl(file.c_str(),cmd.c_str(),NULL);
						if (status < 0)
							perror(("error on "+ cmd).c_str());
					}					
					else
					{
						string arg = m_cmd.front().substr(space + 1, m_cmd.front().length() - space - 1);
						auto status = execl(file.c_str(), cmd.c_str(), arg.c_str(), NULL);
						if (status < 0)
							perror(("error on "+ cmd+ " with arg: "+ arg).c_str());	
					}
					// close the fd table
					close(fd_table_1[0]);
					close(fd_table_2[1]);
					exit(0);
				}
				// parent process
				else
				{
					// Pause the child process
					signal(SIGCHLD, SIG_DFL);
					// Fill the parent result into child
					// close pipe1-Read, pipe2-Write
					close(fd_table_1[0]);
					close(fd_table_2[1]);
					//write the previous queued result
					if (prev_input_insert)
					{
						strcpy(buf, result.c_str());
					}
					// try to add it into pipe1 to let child read
					if (write(fd_table_1[1], buf, sizeof(buf)) < 0)
						perror("cannot write to pipe1 for read");
					close(fd_table_1[1]);
					// after copy the output for child, kill and reset the result buffer \
					// and then, wait for pipe2 child process finish its task
					int status;
					kill(child_pid, SIGCHLD);
					memset(buf, 0, sizeof(buf));
					read(fd_table_2[0], buf, sizeof(buf));
					close(fd_table_2[0]);
					wait(&status);
					
					// special case when this is the end of the line
					// set the line result for either STDOUT or put into lineup result queue
					if (m_cmd.size() == 1)
					{
						if (!m_pipe.back().get_flag())
						{
							std::cout << buf;
							m_pipe.pop_back();
						}
						else
							m_pipe.back().set_result(m_pipe.back().get_result() + buf);
					}
					prev_input_insert = false;
					m_cmd.pop();
				}
		}			
	}
	return 0;			
}
