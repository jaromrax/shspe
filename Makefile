#
# toto je makefile. bude obohacen o ROOTSYS path a macro
#
#
rs   := $(shell echo $$ROOTSYS)
date := $(shell date +%Y%m%d_%H%M%S)
mpath := $(shell cat ~/.rootrc | grep -v -e "^\#" | grep Unix | grep MacroPath | awk '{print $$2}')
mpath2 := $(shell echo  $(mpath) | sed  's/:/ /g')



all:  shspe.C 
	root -n -b -q  compile.C  

clean:
	rm shspe_C.so



install: shspe_C.so 
	@echo I TRY TO FIND ROOTSYS AND MACRO;\
	echo MACRO AT  $(mpath2);\
	for i in $(mpath2); do \
	if [ -d $$i ]; then \
	 	if [ $$i = "." ]; then echo "  ";else \
		TARG=$$i;\
	 	echo $$i "  " IS OK; \
	 	break;\
	 	fi \
	fi; \
	done;\
	echo "TESTING <$$TARG>";\
	if [ "$$TARG" != "" ];then \
		echo cp shspe_C.so $$TARG/ ;\
		cp shspe_C.so $$TARG/ ;\
		ls -l $$TARG/*.so;\
	else\
		echo ERROR; echo "  " NO PATH FOR MACRO. CREATE SOME IN ~/.rootrc;\
	fi;


