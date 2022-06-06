include config.mk

SRC = batt_alert.c
OBJ = ${SRC:.c=.o}

all: options batt_alert

options:
	@echo batt_alert build options:
	@echo "CC       = ${CC}"

clean:
	rm -f batt_alert ${OBJ} batt_alert-${VERSION}.tar.gz

dist: clean
	mkdir -p batt_alert-${VERSION}
	cp -R Makefile config.mk ${SRC} batt_alert-${VERSION}
	tar -cf batt_alert-${VERSION}.tar batt_alert-${VERSION}
	gzip batt_alert-${VERSION}.tar
	rm -rf batt_alert-${VERSION}

install: all

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f batt_alert ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/batt_alert

