libs: $(BIN)libBox2D.a

$(BIN)Box2D_build/Box2D/libBox2D.a: $(PWD)Box2D $(BIN)Box2D_build/
	@echo --------------------------------
	@mkdir -p $@
	@cd $(BIN)Box2D_build/; cmake $(PWD)Box2D; make -j 4
	@echo --------------------------------

$(BIN)libBox2D.a: $(BIN)Box2D_build/Box2D/libBox2D.a
	@cp $< $@
