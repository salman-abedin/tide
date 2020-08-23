include config.mk

all: $(BIN) config.h
$(BIN): $(OBJ)
install: all
	@mkdir -p $(BIN_DIR)
	@mv $(BIN) $(BIN_DIR)
	@rm -f $(OBJ)
	@echo Done moving the binary to ${DESTDIR}${BIN_DIR}
uninstall:
	@rm -f $(BIN_DIR)/$(NAME)
	@echo Done removing the binary from $(BIN_DIR)

.PHONY: all install uninstall
