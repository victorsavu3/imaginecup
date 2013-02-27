libs: $(BIN)libtmxparser.a

$(PWD)tmx-parser/libtmxparser.a: $(PWD)tmx-parser
	@echo --------------------------------
	@cd tmx-parser;  make -f Makefile.linux
	@echo --------------------------------

$(BIN)libtmxparser.a: $(PWD)tmx-parser/libtmxparser.a
	@cp $< $@

