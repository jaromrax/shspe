shspe
=====

root macro to SHow SPEctra

**download from git**

this is a standard procedure, (make and) go to some directory and then:
```
 git init
 git remote add origin https://github.com/jaromrax/shspe
 git pull origin master
```



root compilation
-----
**root compilation**  (example for 64bit)

Here is some simple guide to root compilation - before anything else - if necessary.

I recommend to create three directories in $HOME - one containing root, the other for source files + compilation 
and the last for eventual macros:
```
mkdir ~/root
mkdir ~/root.inst
mkdir ~/root_macros
```

Append to your  $HOME/.bashrc paths to ~/root/:  
```
export ROOTSYS=$HOME/root
export PATH=$ROOTSYS/bin:~/root_macros:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$ROOTSYS/lib/root:$LD_LIBRARY_PATH
```

First you need to configure the Makefiles, if this is successful, use *make* and *make install*. To use more CPU cores and compare times you can do e.g. *time make -j4*

Here, you probably need some prerequisites: *... to be added later ...*
```
 ./configure linuxx8664gcc  --prefix=$HOME/root --etcdir=$HOME/root/etc --enable-opengl --enable-mysql --enable-minuit2 --enable-xml --enable-python --enable-roofit --enable-fftw3 --enable-gsl-shared --enable-mathmore --enable-c++11 

make

make install
```
Root should reside in $HOME/root/bin and should be reachable from commandline: *root*.




**shspe compilation**

you may install the *~/.rootrc* and *~/.rootlogon* with
	init_scripts/install_initscripts 
```
cd 	init_scripts
./install_initscripts 
```
...this step can make some problems if you have already tuned these startup scripts, so check it before use.

and then go back to you shspe install directory (*cd ..*) and do
```
make 
make install
```
shspe .so library should be installed in root macro directory

===

run root
```
root
````
and inside root  shspe. It is a function so don't miss parentheses.
```
root [0] shspe()
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
