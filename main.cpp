#include <string>
#include <iostream>
#include <signal.h>
#include "typedef.hpp"

using namespace std;

#define Close(FD) do {                                  \
    int Close_fd = (FD);                                \
    if (close(Close_fd) == -1) {                        \
      perror("close");                                  \
      fprintf(stderr, "%s:%d: close(" #FD ") %d\n",     \
              __FILE__, __LINE__, Close_fd);            \
    }                                                   \
  }while(0)

/** move oldfd to newfd */
static void redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd, newfd) != -1)
      Close(oldfd); /* successfully redirected */
    else
      cerr <<"dup2\n";
  }
}
static int child = 0;
int exec_pipeline(Command cmd, size_t pos, int in_fd)
{
  if (cmd.get_block().size() == pos + 1) { /* last command */
    redirect(in_fd, STDIN_FILENO); /* read from in_fd, write to STDOUT */
	char ** arg = cmd.get_block()[pos].parse_arg();
  if (cmd.get_block()[pos].get_flag() == 3)
  {
	if (cmd.get_block()[pos].get_argv()[0] == "printenv")
		cmd.get_block()[pos].printenv();
	else if (cmd.get_block()[pos].get_argv()[0] == "setenv")
		cmd.get_block()[pos].setenv();
  }
  else if (execvp(("./bin/" + cmd.get_block()[pos].get_argv()[0]).c_str(), arg) < 0)
  {
	cerr << "Unknown command: [" << cmd.get_block()[pos].get_argv()[0] << "]." << endl;
	exit(0);
  }
	return 0;
  }
  else { /* $ <in_fd cmds[pos] >fd[1] | <fd[0] cmds[pos+1] ... */
    int fd[2]; /* output pipe */
    if (pipe(fd) == -1)
      cerr <<"pipe\n";
	auto pid = fork();
    
    if (pid ==  -1)
      cerr<<"fork\n";
    else if(pid == 0){ /* child: execute current command `cmds[pos]` */
      child = 1;
      Close(fd[0]); /* unused */
      redirect(in_fd, STDIN_FILENO);  /* read from in_fd */
      redirect(fd[1], STDOUT_FILENO); /* write to fd[1] */
       	char ** arg = cmd.get_block()[pos].parse_arg();

	if (cmd.get_block()[pos].get_flag() == 3)
	  {
	if (cmd.get_block()[pos].get_argv()[0] == "printenv")
		cmd.get_block()[pos].printenv();
	else if (cmd.get_block()[pos].get_argv()[0] == "setenv")
		cmd.get_block()[pos].setenv();
	  }
	  else if (execvp(("./bin/" + cmd.get_block()[pos].get_argv()[0]).c_str(), arg) < 0)
	  {
		cerr << "Unknown command: [" << cmd.get_block()[pos].get_argv()[0] << "]." << endl;
		exit(0);
	  }
	cout << "child\n";
	return 0;
	}
    else{ /* parent: execute the rest of the commands */
      Close(fd[1]); /* unused */
      Close(in_fd); /* unused */
      exec_pipeline(cmd, pos + 1, fd[0]); /* execute the rest */
  	cout << " parent\n";
	return 0;
  }
  }
}

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
	
	int status = exec_pipeline(cmd, 0, STDIN_FILENO);
	cout <<"finish\n";
	// get the fd table now
	//Pipe_IO shell_fd = all.get_pipe(0);
	//if (!shell_fd.mode_on())
	//	shell_fd.construct_pipe();
	/*
	
	for (int i=0; i< cmd.get_block().size(); i++)
	//for (auto &i: cmd.get_block())
	{
		cout << cmd.get_block().size()<< " current pipe block: " << cmd.get_block()[i].get_argv()[0] <<endl;
		int status;
		while ( (status = cmd.get_block()[i].execute(all,\
			(i)? false: true, (i == cmd.get_block().size() - 1)? true: false)) == 1)  // fork error
			usleep(1500);
		if (status != 0)
			cerr << "Fail execution on" << cmd.get_block()[i].get_argv()[0] <<endl;
		usleep(1500);
	}
	//shell_fd.close();
	all.close(0);
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
*/
    }

    return 0;
}
