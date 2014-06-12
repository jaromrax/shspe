shspe
=====

root macro to SHow SPEctra

**shspe compilation**

you may install the *~/.rootrc* and *~/.rootlogon* with
	init_scripts/install_initscripts 
and then 
```
make 
make install
```


**root compilation**  (example for 64bit)
Append to   .bashrc :  
```
export ROOTSYS=$HOME/root
export PATH=$ROOTSYS/bin:~/00_central:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$ROOTSYS/lib/root:$LD_LIBRARY_PATH
```

compile
```
 ./configure linuxx8664gcc  --prefix=$HOME/root --etcdir=$HOME/root/etc --enable-opengl --enable-mysql --enable-minuit2 --enable-xml --enable-python --enable-roofit --enable-fftw3 --enable-gsl-shared --enable-mathmore --enable-c++11 

make

make install
```

===

There are more functions loaded with shspe:



**gr_  group**

functions to play with graphs
```
grhelp()
```

**joingraphs**

join several graphs to one multigraph

**MPad  group**

MPadGetByName()
MPadCreate()
MPadPrintIn()

for use with TCounter



**TCounter  group**

not interesting now



**cuts  group**

uses **cuts.root** file for storing the cuts
```
cutload()
cutsave()
cutls()
cutrm()
cutcp()
```
