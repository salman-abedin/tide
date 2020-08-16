include config.mk

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
install: all
	@mkdir -p "$(DESTDIR)$(BINPREFIX)"
	@mv $(NAME) "$(DESTDIR)$(BINPREFIX)"
	@rm -f $(OBJ)
	@echo Done moving the binary to ${DESTDIR}${BINPREFIX}
uninstall:
	@rm -f "$(DESTDIR)$(BINPREFIX)/$(NAME)"
	@echo Done removing the binary from ${DESTDIR}${BINPREFIX}

.PHONY: all install uninstall
