SRCS := main.cpp Game.cpp Layer.cpp Map.cpp Player.cpp conversions.cpp DebugDraw.cpp
OBJS := $(addprefix $(OBJ), $(addsuffix .o, $(basename $(SRCS))))

all: $(BIN)game

$(BIN)game: $(OBJS) $(BIN)libtmxparser.a | $(BIN) libs
	@echo Linking $(@F)
	@g++ -o $@ $^ $(CFLAGS) $(LDFLAGS)

-include  $(OBJ)*.d
