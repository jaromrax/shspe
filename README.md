
**shspe**
=====

root macro to SHow SPEctra

1.1 download shspe from git
-------
this is a standard procedure, (make and) go to some directory and then:
```
 git init
 git remote add origin https://github.com/jaromrax/shspe
 git pull origin master
```





2.2 prerequisites for compilation of root: 
----------
basically, better check root.cern.ch website.

```
aptitude install dpkg-cross
aptitude install libxext-dev
aptitude install libxpm-dev
aptitude install libxft-dev
```

2.3 root compilation
-----
  (example for 64bit)


I recommend to create three directories in $HOME - one containing root, the other for source files + compilation 
and the last for macros and compiled .so files:
```
mkdir ~/root
mkdir ~/root.inst
mkdir ~/root_macros
```



Append to your  ```$HOME/.bashrc``` paths to your ~/root/:  
```
export ROOTSYS=$HOME/root
export PATH=$ROOTSYS/bin:~/root_macros:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$ROOTSYS/lib/root:$LD_LIBRARY_PATH
```

Download root:  wget https://root.cern.ch/download/root_v5.34.34.source.tar.gz

Unpack to ~/root.inst and enter there

First you need to *configure* the Makefiles, if this is successful, use *make* and *make install*. To use more CPU cores and compare times you can do e.g. *time make -j4*



```
 ./configure linuxx8664gcc  --prefix=$HOME/root --etcdir=$HOME/root/etc --enable-opengl --enable-mysql --enable-minuit2 --enable-xml --enable-python --enable-roofit --enable-fftw3 --enable-gsl-shared --enable-mathmore --enable-c++11 

make

make install
```
Root should reside in $HOME/root/bin and should be reachable from commandline: *root*.

**Comment for Ubuntu xenial 16.04:**
*use root 5.34.36, edit ./root/tmva/src/RuleFitParams.cxx and change isnan(fstarVal) to std::isnan(fstarVal);  root6 has still problem as of 2016/05*



3.1 shspe compilation
-----

you may install the *~/.rootrc* and *~/.rootlogon* - it is my choice, you may prefer something differerent
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
shspe .so library should be installed in root macro directory (verify)



4.1 run root
----------
```
root
````
and inside root  shspe. It is a function so don't miss parentheses.
```
root [0] shspe()
```
**4.1.1  AUTOLOAD**

At this point - in the directory I want to work - I create *autoload_libs.C* with a content like this:
```
{
gROOT->ProcessLine("shspe()");
}
```
and the next start of root, the shspe() opens automaticaly.

**4.1.2 .REMOTE_DATA_DIR**

If the data are on different PATH than your working dir: create a /hidden/ file:
``` .REMOTE_DATA_DIR ``` . shspe() on *Openfile* will show the content also of this destination.

**4.1.3 OPEN FILE**

First thing you can do now is open a root file with histograms. Root files that are on ```.REMOTE_DATA_DIR``` path are displayed with a tilde ~. The actual opened file name written to /hidden/ ```.CURRENTFILE```. This could be usefull if you needed to deduce the run number in some script or something. But this can also create difficulties when two shspe() sessions are open in the same directory.

**4.1.4. OPEN FILE details**

The  shspe() can also open:
* mysql reference files with a name like *tablename.mysql* /host-user-password/ that refers to database nemaed *test*
* *mmap.histo file* that shares memory with histograms from digitizer acquisistion


**4.1.5 FILE SAVE** 

You can save
* canvases + histograms on screen click ```SaveCanvas``` - you can input you filename into the text field and save multiple format, with a time tag. Withou a name it stores canvas as 1.root, 2.root,... 9.root that can be recovered later with ```LoadCanvas```
* all histograms click ```SaveAllSpectra``` - filename is always based on datetime
* copy histogram from file to memory ```Spectrum2Memory``` and go to memory later and compare histograms from different files there.



4.2 Fitting
------------

**4.2.1 Prepare fit**

**4.2.2 Fit procedure**

**4.2.3 Save fit**

Several files are created ```zfitresults``` the extensions are

*```.eff``` - for calibration of efficiency
*```.ecal``` - just energy and channel
*```.tmp``` - same as eff but only the last fit - can be parsed by script
*```final.root``` - all objects of results
*```tmp.root``` - as root, but the last fit


5.1 Other functions
------------

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

6.1 some specific  details
------------
creates .CURRENTFILE on open

checks .REMOTE_DATA_DIR on open

searches for a script  *shspe.pk_mysql* setup file on savefit



*edited in http://dillinger.io/   and elpa-markup-more*

