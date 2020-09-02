include config.mk

all: $(BIN) config.h
$(BIN): $(OBJ)
install: all
	@mkdir -p $(BIN_DIR)
	@mv $(BIN) $(BIN_DIR)/$(NAME)
	@rm -f $(OBJ)
	@echo Done with installing the binary.
uninstall:
	@rm -f $(BIN_DIR)/$(NAME)
	@echo Done removing the binary.

.PHONY: all install uninstall
