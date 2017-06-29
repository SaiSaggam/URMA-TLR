
all:
	./chk-lzma
	
clean:
	if [ -e src/Makefile ]; then make -C src clean; fi;

distclean:
	if [ -e src/Makefile ]; then make -C src distclean; fi;

.PHONY: all clean
	make -C src 

clean:
	make clean -C src
