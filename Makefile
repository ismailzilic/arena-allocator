SRC := ./src
EXEC := ./build/exec
COMPILER_FLAGS := -Wall -Wextra -Werror -pedantic -std=c99

.PHONY: build debug clean

build: 
	gcc -o $(EXEC) $(SRC)/**.c $(COMPILER_FLAGS) 

debug:
	gcc -o $(EXEC) $(SRC)/**.c $(COMPILER_FLAGS) -Og -g

clean: $(EXEC)
	rm $(EXEC)
