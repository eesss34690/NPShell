CXX = g++


NPShell: main.o Npshell.o Pipe_block.o precompile
	$(CXX) main.o Npshell.o Pipe_block.o -o NPShell

Pipe_block.o: Pipe_block.cpp Pipe_block.hpp
	$(CXX) -c Pipe_block.cpp -o Pipe_block.o

Npshell.o: Npshell.cpp Npshell.hpp Pipe_block.hpp
	$(CXX) -c Npshell.cpp -o Npshell.o

main.o: main.cpp Npshell.hpp
	$(CXX) -c main.cpp -o main.o

precompile: ./cmds/noop.cpp ./cmds/number.cpp ./cmds/removetag.cpp ./cmds/removetag0.cpp
	cp /usr/bin/ls ./bin
	cp /usr/bin/cat ./bin
	$(CXX) ./cmds/noop.cpp -o ./bin/noop
	$(CXX) ./cmds/number.cpp -o ./bin/number
	$(CXX) ./cmds/removetag.cpp -o ./bin/removetag
	$(CXX) ./cmds/removetag0.cpp -o ./bin/removetag0

clean:
	rm *.o NPShell *.txt ./bin/*
