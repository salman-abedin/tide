include config.mk

all: $(NAME)
$(NAME): $(OBJ)
install: all
	@mkdir -p $(BIN_DIR)
	@mv $(NAME) $(BIN_DIR)
	@rm -f $(OBJ)
	@echo Done moving the binary to ${DESTDIR}${BIN_DIR}
uninstall:
	@rm -f $(BIN_DIR)/$(NAME)
	@echo Done removing the binary from $(BIN_DIR)

.PHONY: all install uninstall
