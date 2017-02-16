
rs:=$(shell echo $$ROOTSYS)
date:=$(shell date +%Y%m%d_%H%M%S)
mpath := $(shell cat ~/.rootrc | grep -v -e "^\#" | grep Unix | grep DynamicPath | awk '{print $$2}')
mpath2:=$(shell echo  $(mpath) | sed  's/:/ /g')


all:  shspe.C 
	$(rs)/bin/root -n -b -q  compile.C  >/dev/null


clean:
	rm shspe_C.so


install: shspe_C.so 
	@echo I TRY TO FIND PATH for dynamic .SO load;\
	echo dynamic paths is $(mpath2);\
	TARG=""; \
	for i in $(mpath2); do \
	echo "testing ... " $$i;\
	if [ -d "$$i" ]; then \
	 	if [ $$i = "./" ]; then echo "       current dir ";else \
		if [ "$$TARG" = "" ]; then \
			TARG=$$i;\
			cp shspe_C.so $$TARG/ ;\
			if [ $$? != 0 ]; then TARG="";fi; \
		fi; \
	 	echo "       OK " $$i "  targ =" $$TARG; \
		fi; \
	 else \
		 echo "       no such dir"; \
	fi; \
	done;\
	echo "Selected= <$$TARG>";\
	if [ "$$TARG" != "" ];then \
		ls -l $$TARG/*.so;\
	else\
		echo ERROR; echo "  " NO PATH FOR MACRO. CREATE SOME inside ~/.rootrc;\
	fi;


