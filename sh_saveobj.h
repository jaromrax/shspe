#ifndef __SH_SAVEOBJ_H__
#define __SH_SAVEOBJ_H__
//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "TROOT.h"
#include "TH1F.h"

//--------savecanvas:
#include "TPad.h"
#include "TCanvas.h"
#include "TText.h"
#include "TObjArray.h"

//------------------- declare everything here:

void saveobj2asc1col(const char *filenam,  TH1* xobj);  // 
void saveobj2asc(const char *filenam,  TH1* xobj);
void saveobj2file(const char *filenam,  TH1* xobj, int bkup); // ROOT FILE (update)



void savecanvas(const char *filenam, int slot);

#endif
