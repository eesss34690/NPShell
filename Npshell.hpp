# include <string>

using namespace std;

class Npshell
{
private:
	string m_input;
public:
	void set_input (string input);
	int line_cmd();	
}
