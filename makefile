PWD := $(shell pwd)/

BIN := $(PWD)bin/
LIB := $(BIN)lib/
OBJ := $(BIN)obj/
SRC := $(PWD)src/
BUILD := $(PWD)build/

LDFLAGS = -L $(BIN)lib -ltinyxml -lz -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lBox2D
CPPFLAGS = -O0 -g3 -ggdb -DDEBUG -I tmx-parser -I SFML/include

.PHONY: all
all: build

$(OBJ)%.o: $(SRC)%.cpp | $(OBJ) makefile $(BUILD)*.mk
	@echo $(<F)
	@g++ -o $@ -c $< -I $(INC) $(CPPFLAGS) -MD -MT $@ -MP

.PHONY: clean
clean:
	rm -rf bin

#Create directories:
.PRECIOUS: %/
%/:
	mkdir -p $@

include $(BUILD)*.mk
