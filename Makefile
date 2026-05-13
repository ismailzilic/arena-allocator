SRC := ./src
EXEC := ./build/exec
COMPILER_FLAGS := -Wall -Wextra -Werror -pedantic -std=c99

.PHONY: build debug clean

build: 
	gcc $(COMPILER_FLAGS) -O3 $(SRC)/**.c -o $(EXEC) 

debug:
	gcc $(COMPILER_FLAGS) -Og -g $(SRC)/**.c -o $(EXEC) 

clean: $(EXEC)
	rm $(EXEC)
