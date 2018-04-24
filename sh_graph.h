#ifndef __SH_GRAPH_H__
#define __SH_GRAPH_H__
//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "TROOT.h"
#include "TGraphErrors.h"
//#include std
#include <iostream>
#include <fstream>

#include "TObjString.h"

#include "TMultiGraph.h"


#include "TMath.h"  // ?TMath
//=========== stndalone .L C+
#include "TH1F.h"
#include "TAxis.h"
#include "TPad.h"
//------------------- declare everything here:
extern   const int MAXPNTS;
//static const int MAXPNTS=15000; defined in C file

void grhelp ();
void gr_calibrate( TH1F  *hh,  double slope, double bias);


int64_t gr_engine (const char* name, int rx, int ry, int rdx, int rdy);
void gr_printval (TGraph *gg);
void gr_saveval (TGraph *gg, char *filename);
TGraphErrors* gr1 (char *name);
TGraphErrors*  gr2(char* name);
TGraphErrors*  gr3 (const char* name);
TGraphErrors*  gr4 (char* name);
TGraphErrors*  gr4a (char* name);
int gr_transfy (TGraphErrors *gg,double slope, double diff);
int gr_transfx (TGraphErrors *gg,float slope, float diff);
TGraphErrors* gr_revert (TGraphErrors *gg);
TGraphErrors* gr_fitpol (TGraphErrors *gg, const char *cpol, Float_t x1, Float_t x2);

void  gr_stdcalproc(const char *fname);

// COPY - they are the same now
void joingraphs(const char* myname, const char* g1 , int autocolors );
void joingraphsX(const char* myname, const char* g1 , int autocolors );

  
#endif
