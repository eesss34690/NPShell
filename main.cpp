#include <string>
#include <iostream>
#inlcude "Command.hpp"
using namespace std;

int main(int argc, char *argv[]){
    string input;
   
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
		auto status = i.execute();
		while (status == 1)  // fork error
			usleep(1500);
		if (status == 2)  // no such command
			cout << "Unknown command: [" << task.GetFile() << "]." << endl;
	}
    }
    return 0;
}
