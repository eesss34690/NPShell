#include <string>
#include <iostream>
#include <signal.h>
#include "typedef.hpp"

using namespace std;

int main(int argc, char *argv[]){
    string input;
    Pipeline all;   
    // set the signal handler
    signal(SIGCHLD, [](int signo) {
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0);
    }); 
    while(1){
        cout << "% ";
        getline(cin, input);
        
	if(input.empty()) continue;
        Command cmd(input);
	
	for (auto &i: cmd.get_block())
	{
		cout << cmd.get_block().size()<< " current pipe block: " << i.get_argv()[0] <<endl;
		auto status = i.execute(all);
		while (status == 1)  // fork error
			usleep(1500);
		if (status != 0)
			cerr << "Fail execution on" << i.get_argv()[0] <<endl;
	}
	
	auto last = cmd.get_block().back();
	if (last.get_flag() == 1)
	{
		all.add_all_proc(last.get_cnt(), all.get_child_proc(0));
	}
	for (auto &i: all.get_child_proc(0))
	{
		int status;
		waitpid(i, &status, 0);
	}
	all.next_();

    }

    return 0;
}
