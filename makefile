PWD := $(shell pwd)/

BIN := $(PWD)bin/
LIB := $(BIN)lib/
OBJ := $(BIN)obj/
SRC := $(PWD)src/
BUILD := $(PWD)build/

LDFLAGS = -L $(BIN)lib -ltinyxml -lz -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CPPFLAGS = -O0 -g3 -ggdb -DDEBUG -I $(PWD)tmx-parser -I $(PWD)SFML/include -I $(PWD)spine/spine-cpp/includes -I $(PWD)Box2D

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
