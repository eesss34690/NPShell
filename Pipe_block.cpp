#include "Pipe_block.hpp"

using namespace std;


Pipe_block::Pipe_block()
{
	m_cnt = 0;
	m_flag = false;
}

void Pipe_block::set_cnt(int cnt)
{
	m_cnt = cnt;	
}

void Pipe_block::set_flag(bool flag)
{
	m_flag = flag;
}

int Pipe_block::get_cnt()
{
	return m_cnt;
}

bool Pipe_block::get_flag()
{
	return m_flag;
}

void Pipe_block::set_result(string result)
{
	m_result = result;
}

string Pipe_block::get_result()
{
	return m_result;
}

