libs: $(BIN)libtmxparser.a

TMX_SRC := $(PWD)tmx-parser/TmxParser/
TMX_OBJ := $(OBJ)tmx-parser/

TMX_SRCS := $(wildcard $(TMX_SRC)*.cpp) $(wildcard $(TMX_SRC)base64/*.cpp)
TMX_OBJS := $(patsubst $(TMX_SRC)%.cpp,$(TMX_OBJ)%.o,$(TMX_SRCS))

$(TMX_OBJ)%.o: $(TMX_SRC)%.cpp | $(TMX_OBJ) $(TMX_OBJ) $(TMX_OBJ)base64/ $(BUILD)tmx-parser.mk
	@echo tmx-parser - $(<F)
	@g++ -o $@ -c $< -I $(INC) $(CPPFLAGS) -MD -MT $@ -MP

$(BIN)libtmxparser.a: $(TMX_OBJS)
	@echo Creating $@
	@ar cr $@ $^
