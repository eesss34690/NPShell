CXX = g++
STD = c++11


NPShell: main.o Npshell.o Pipe_block.o
	$(CXX) main.o Npshell.o Pipe_block.o -o NPShell

Pipe_block.o: Pipe_block.cpp Pipe_block.hpp
	$(CXX) -c Pipe_block.cpp -o Pipe_block.o

Npshell.o: Npshell.cpp Npshell.hpp Pipe_block.hpp
	$(CXX) -c Npshell.cpp -o Npshell.o

main.o: main.cpp Npshell.hpp
	$(CXX) -c main.cpp -o main.o

clean:
	rm *.o
