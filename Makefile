CXX = clang++
CPPFLAGS = -Wall -O2 -pedantic -std=c++11

OBJ := main.o Pipe_block.o Command.o Pipe_IO.o Pipeline.o

NPShell: $(OBJ) precompile
	$(CXX) $(CPPFLAGS) $(OBJ) -o NPShell

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

./bin/%: ./cmds/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o ./bin/%

precompile: ./cmds/noop.cpp ./cmds/number.cpp ./cmds/removetag.cpp ./cmds/removetag0.cpp
	cp /usr/bin/ls ./bin
	cp /usr/bin/cat ./bin
	$(CXX) ./cmds/noop.cpp -o ./bin/noop
	$(CXX) ./cmds/number.cpp -o ./bin/number
	$(CXX) ./cmds/removetag.cpp -o ./bin/removetag
	$(CXX) ./cmds/removetag0.cpp -o ./bin/removetag0

clean:
	rm *.o NPShell *.txt ./bin/*