#ifndef __SH_TNAMED_H__
#define __SH_TNAMED_H__
//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

/*
    TNamed is a very good tool to store information with TH1F
  ----- here is stored all     concerning this info treatment 
  

 */


#include "TROOT.h"
#include"TString.h"
//------------------- declare everything here:

void* extract_display_with(TH1F* h, int &count,int64_t addr[]);
//void* ro_getnext(TH1F* h){
void* tnamed_draw(TH1F* h);    //  Like Draw("unstack")
void tnamed_printall(TH1 *h);  //  Print All Names/Titles in GetLis..Functions()

void tnamed_hadd( TH1* h , TH1* h1 );    // more simple command to join histos

TString* tnamed_get_calibline( TH1* histo) ;

// CAN ALSO  reset if know! carefully if you want to like add histograms
//         with "display_with"
void tnamed_set( TH1* h, const char* name , const char* title ); // MANUALLY (calib0..)


#endif
