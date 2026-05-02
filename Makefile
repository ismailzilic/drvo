SRC := ./src
EXEC := ./build/drvo
COMPILER_FLAGS := -Wall -Werror -pedantic -std=c++20

.PHONY: build debug clean

build: 
	g++ $(COMPILER_FLAGS) -O2 $(SRC)/**.cpp -o $(EXEC) 

debug:
	g++ $(COMPILER_FLAGS) -Og -g $(SRC)/**.cpp -o $(EXEC) 

clean: $(EXEC)
	rm $(EXEC)
