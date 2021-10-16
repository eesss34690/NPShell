#include <stdlib.h>
#include "Npshell.hpp"

#define BUFFERSIZE 4096 

using namespace std;

size_t Command::find_char(string cmd, char target, int start_idx)
{
	return cmd.find(target, start_idx);
}

string Command::separate_output(string cmd, int start_idx, int end_idx)
{
	while (cmd[start_idx] == ' ')
		start_idx++;
	if (end_idx != string::npos)
		while (cmd[--end_idx] == ' ');
	return cmd.substr(start_idx, 1 + end_idx - start_idx);		
}

Command::Command( string cmd )
{
	auto end_idx = 0;
	auto start_idx = 0;
	string single_cmd;
	
	m_block.push_back( new *(Pipe_block) );
	while (end_idx < cmd.length())
	{
		end_idx = find_char(cmd, '|', start_idx);
		if (end_idx == string::npos)
			end_idx = cmd.length();
		
		// special case: |N
		// the counter of redirect output should be set
		// note the cmd as redirect command
		if (start_idx > 0 && cmd[start_idx] != ' ')
		{
			m_block.back().set_cnt(stoi(separate_output(cmd, start_idx, end_idx)));
			m_block.back().set_flag(1);
			break;
		}
		// special case: > $(file)
		auto ge_idx = find_char(cmd, '>', 0);
		if (ge_idx != string::npos)
		{
			m_block.back().set_flag(2);
			m_block.back().set_file(separate_output(cmd, ge_idx + 1, string::npos));
			break;
		}	

		// the command in the middle should be set without the final space
		single_cmd = separate_output(cmd, start_idx, end_idx);
			
		// find the command action 
		space = find_char(single_cmd, ' ', 0);
		string action = separate_output(single_cmd, 0, space);
		
		// type: builtin
		if (action == "setenv")
		{
			auto space_2 = find_char(m_cmd.front(), ' ', space + 1);
			m_block.back().set_flag(3);
			char *real[3] = {"setenv", separate_output(single_cmd, space + 1, space_2).c_str(), \
				separate_output(single_cmd, space_2 + 1, single_cmd.length()).c_str()};
			m_block.back().set_argv(real);
			//setenv(separate_output(m_cmd.front(), space + 1, space_2).c_str(), \
				separate_output(m_cmd.front(), space_2 + 1, m_cmd.front().length()).c_str(), 1);
		}
		else if (action == "printenv")
		{
			char *real[2] = {"printenv", separate_output(m_cmd.front(),\
			 space + 1, m_cmd.front().length()).c_str()};
			m_block.back().set_argv(real);
			m_block.back().set_flag(3);
			//const char * env = getenv(separate_output(m_cmd.front(),\
			 space + 1, m_cmd.front().length()).c_str());
			//if (env != NULL)
			//	cout << env << endl;
		}
		else if (action == "exit")
		{
			m_block.back().set_flag(5);
		}
		// type: bin (4)
		else {
			size_t idx = space, idx2 = space;
			size_t pos = 1;
			string argv[256];
			char *real[256];
			argv[0] = action;
			while (idx != string::npos)
			{
				idx2 = find_char(single_cmd, ' ', idx + 1);
				int n = idx2 - idx;
				argv[pos] = separate_output(single_cmd, idx + 1, idx2);
				idx = idx2;
				pos = pos + 1;
			}
			for (int i=0; i< pos; i++)
				real[i] = const_cast<char*>(argv[i].c_str());
			real[pos] = NULL;
			m_block.back().set_flag(4);
			m_block.back().set_argv(real);
			m_block.back().set_file("./bin/" + action);
		}
		start_idx = end_idx + 1;
	}
}
