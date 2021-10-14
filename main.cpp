#include "Npshell.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    Npshell npshell;
    std::string input;
    
    while(1){
        cout << "% ";
        getline(cin, input);
        
	if(input.empty()) continue;
        npshell.set_input(input);
        npshell.line_cmd();
    }
    return 0;
}
