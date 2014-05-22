
.PHONY: all lib default depend clean

plugins=ADAMS Simulink


all  default:
	cd common; $(MAKE) all

lib:
# Note: lib will not make the manager!
	cd common; $(MAKE) lib

install: default
	for i in ${plugins} ; do \
		( cd $$i ; $(MAKE) install ) \
	done


depend:
	cd common; $(MAKE) depend

clean:
	cd common; $(MAKE) clean
	for i in ${plugins} ; do \
		( cd $$i ; $(MAKE) clean ) \
	done
