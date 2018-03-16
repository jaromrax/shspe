#ifndef __SH_CUTS_H__
#define __SH_CUTS_H__


#include "TCutG.h"
#include "TFile.h"
#include "TROOT.h"



const char* CUTFILE_GLOBAL="cuts.root";

void cutload();

void cutsave(TCutG *cut, const char* name);
void cutload();
void cutrm(const char* name, int version);
void cutcp(const char* name, const char* newname);
void cutls();

#endif
