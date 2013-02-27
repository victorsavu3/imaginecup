SRCS := main.cpp Game.cpp Tilemap.cpp Layer.cpp
OBJS := $(addprefix $(OBJ), $(addsuffix .o, $(basename $(SRCS))))

all: $(BIN)game

$(BIN)game: $(OBJS) $(BIN)libtmxparser.a | $(BIN) libs
	@echo Linking $(@F)
	@g++ -o $@ $^ $(CFLAGS) $(LDFLAGS)

-include  $(OBJ)*.d
