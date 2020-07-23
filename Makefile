PREFIX = /usr/local
install:
	@cp tide.sh tide
	@chmod 755 tide
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@mv tide ${DESTDIR}${PREFIX}/bin
	@echo Done installing executable files to ${DESTDIR}${PREFIX}/bin
uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/tide
	@echo Done removing executable files from ${DESTDIR}${PREFIX}/bin
