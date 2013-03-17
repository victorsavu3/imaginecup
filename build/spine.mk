SPINE_SRC := $(PWD)spine/spine-cpp/src/
SPINE_OBJ := $(OBJ)spine/

SPINE_SRCS := $(wildcard $(SPINE_SRC)json/*.cpp) $(wildcard $(SPINE_SRC)spine/*.cpp) $(wildcard $(SPINE_SRC)spine-sfml/*.cpp)

SPINE_OBJS := $(patsubst $(SPINE_SRC)%.cpp,$(SPINE_OBJ)%.o,$(SPINE_SRCS))

$(SPINE_OBJ)%.o: $(SPINE_SRC)%.cpp | $(SPINE_OBJ) $(SPINE_OBJ)json/ $(SPINE_OBJ)spine/ $(SPINE_OBJ)spine-sfml/ $(BUILD)spine.mk
	@echo spine - $(<F)
	@g++ -o $@ -c $< -I $(INC) $(CPPFLAGS) -MD -MT $@ -MP

$(BIN)libspine.a: $(SPINE_OBJS)
	@echo Creating $@
	@ar cr $@ $^

-include $(SPINE_OBJ)json/*.d
-include $(SPINE_OBJ)spine/*.d
-include $(SPINE_OBJ)spine-sfml/*.d

