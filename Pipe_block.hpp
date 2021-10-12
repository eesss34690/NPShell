#include <string>

using namespace std;
class Pipe_block
{
private:
	string      m_result;
	int         m_cnt;
	bool        m_flag;
public:
	Pipe_block();
	void        set_cnt(int cnt);
	void        set_flag(bool flag);
	void        set_result(string result);
	int         get_cnt();
	bool        get_flag();
	string      get_result();
};
