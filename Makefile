.POSIX:
PREFIX = /usr/local

install:
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@for e in *.sh; do \
		cp -f $$e $${e%.*}; \
		chmod 755 $${e%.*}; \
		mv $${e%.*} ${DESTDIR}${PREFIX}/bin; \
		done
	@echo Done installing executable files to ${DESTDIR}${PREFIX}/bin
uninstall:
	@for e in *.sh;do \
		rm -f ${DESTDIR}${PREFIX}/bin/$${e%.*}; \
		done
	@echo Done removing executable files from ${DESTDIR}${PREFIX}/bin
