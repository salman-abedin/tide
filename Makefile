.POSIX:

NAME   := tide
BINPREFIX ?= /usr/local/bin

install:
	@mkdir -p ${DESTDIR}${BINPREFIX}
	@cp -p tide.sh ${DESTDIR}${BINPREFIX}/${NAME}
	@chmod 755 ${DESTDIR}${BINPREFIX}/${NAME}
	@echo Done installing executable files to ${DESTDIR}${BINPREFIX}
uninstall:
	@rm -f ${DESTDIR}${BINPREFIX}/${NAME}
	@echo Done removing executable files from ${DESTDIR}${BINPREFIX}

.PHONY: install uninstall
