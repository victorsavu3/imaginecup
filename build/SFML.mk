libs: $(BIN)SFML_build/

$(BIN)SFML_build/: $(LIB) $(PWD)SFML
	@echo --------------------------------
	@mkdir -p $@
	@cd $(BIN)SFML_build/; cmake $(PWD)SFML; make -j 4
	@cp $(BIN)SFML_build/lib/libsfml* $(LIB)
	@echo --------------------------------
