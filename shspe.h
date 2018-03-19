//
// Author: Jaromir Mrazek -  2008/08
//   started with 
//       a script of Ilka Antcheva   1/12/2006
//
//


#if !defined( __CINT__) || defined (__MAKECINT__)

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGTextView
#include "TGTextView.h"
#endif
#ifndef ROOT_TGClient
#include "TGClient.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGTextEntry
#include "TGTextEntry.h"
#endif
#ifndef ROOT_TGTextView
#include "TGTextView.h"
#endif

#include "Riostream.h"

#endif

/*
#include <TApplication.h>
#include <TGFrame.h>
//#include <TGWindow.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TGTextEntry.h>
#include "TGTextView.h"
*/

//==============================  includes for shspe: 
#include <TList.h>
#include <time.h>
#include "TF1.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include <iostream>
//#include <fstream.h> // deprecated
#include <fstream>
#include "TFile.h"
#include <TRandom.h>
#include <TCanvas.h>
#include "TROOT.h"
#include <stdio.h>
#include <iomanip>
#include<TDirectory.h>
#include "TButton.h"
#include "TCanvas.h"
#include "TPad.h"
//   gStyle Palette
#include "TStyle.h"

#include "TMath.h"
//---------------- Marks:

#include "TFrame.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCutG.h"

#include "TText.h"

//---- fitting etc
#include "TSpectrum.h"

//---------- FBX
//#include "TFbxshspe.h"
// -----     sleep(2)
//#include <time.h>
//#include <stdlib.h>
//#include <sys/time.h>
// sleep(1)-----unistd.h
#include <unistd.h>

//--------------------loadgraphs
#include <sstream>


//#include "Riostream.h"
//#include "Rtypes.h"
//#include "TObject.h"

#include "TPolyMarker.h"



// DIR CONTENT
#include <dirent.h>
#include <cstdio>

// mmap file.... i use err()
#include <err.h>
#include <sys/mman.h>
#include <fcntl.h>   //  open,  O_RW
/*
 *   import other  functions......
 */
//#include "kibbler_graphs.h"

//static const int MAXPNTS=15000;


static const int MAXPRIMITIVES=26; // in all multi things (selclear, logy, spider...movexy)


using namespace std;

/*
 * listbox_vsize 260 gets the whole list 1_..29_ --- listbox is the menu in my window
 *  mmframe= new MyMainFrame(0,120,180);  // TGWindow==0,  w,   h
 */
//static const int listbox_vsize=260;  to je ok
//static const int listbox_vsize=260;
static const int listbox_vsize=250;
static const int listbox2_vsize=300;
/*
 *  size of file-list.....,  was 120.
 */
//static const int listbox2_vsize=220;
/*
 * i do not see any influence
 */
static const int mmframe_w=120;
static const int mmframe_h=180;





 ////////////////////////////
 // HERE _ THE MENU BEGINS //
 ////////////////////////////

// TIME OFFSET _ ROOT CAN DO  1995, not earlier   :(
// doing offset by this avoids problems with 
//    graphs
TDatime da;

// FIXED   VARIABLE FOR THE MENU  ====> afrter the declaration
//MyMainFrame *mmframe; // My Main Frame 

int 
  SELNextPage=1,
  SELSetMarks=2,
  SELGetMarks=3,
  SELDelMarks=4,
  SELbar1=5,   //----------
  SELFindPks=6,
  SELFindBg=7,
  SELDelPks=8,
  SELbar2=9, //----------
//  SELFitGaus=8,
  SELFBX=10,
  SELUpdate=11,
  SELSaveFit=12,
//SELFit=13,
  SELDelFBX=14,
  SELbar3a=15, //----------
  SELClone2Rint=16,

// HERE PAGE2 BEGINS
  SELbar3=17,


  SELGrid=       18,
  SELDateTime=   19,
  SELLogy=       20,
  SELLogz=       21,
  SELLoadCanvas= 22,
  SELbar4=       23,
  SELClear=      24,
  SELRefresh=    25,
  SELUnzoom=     26,
  SELSaveSpectra=27,
  SELSaveCanvas= 28,
  SELbar5=       29,
  SELClearAll=   30,
  SELDivCanv=  31,
  SELClone2Rint2=32,
  SELbar5a=     33;
//  SELUnzoomAll=  31;
//  SELZoomAll=29;

//=======================================class for controls
//=======================================class for controls
//=======================================class for controls

//=======================================class for controls
//=======================================class for controls
//=======================================class for controls


//=======================================class for controls
//=======================================class for controls


class MyMainFrame : public TGMainFrame {

  //private:
public:
  TGTextButton *show11, *show12; //startstop
  TGTextButton *show13; //refresh
  
  int PAGE;  //  menu is in pages:   1(peaks) 2(grid,div) ?
  int flistbox_selected,flistbox2_selected, 
    flistbox_selected_when;// what was selected in fListBox. enables 2 page
  int lastcmd, lastlastcmd, lastcmdtime,lastlastcmdtime;
  int time_start,  time_block;

   TGListBox           *fListBox;
   TList               *fSelected;   

   TGListBox           *fListBox2;
   TList               *fSelected2;   

   TGListBox           *fListBoxOF;
   TList               *fSelectedOF;   

   TGCheckButton       *fCheckMulti;
   TGTextEntry         *fEntry; 
   TGTextEntry         *fEntrySIG; 

   TGCheckButton *fChk1; // chci zjistovat stav



   //I shadow in anyways in .C   TString *fentry;   // POZOR - PODOBNE jako fEntry

  TH1 *htmpt;     // main histo      -------- VSECHNO NULLuj na zacatku!
  TH1 *htmptclear; // this one will be cleared if needed.
  int ihtmptclearall; // tell if one 0  or all 1
  TSpectrum *spe; // morhac's Tspectrum
  TH1  *bg    ;   // background from TSpectrum
  TF1  *bgf; //backgr function

  //  TCutG *MyCUTG;
  TCanvas *MyCan;
  TCanvas *GPAD; // THIS IS KEY 1 to identify histogram
  TString htmptNAME; //THIS IS KEY 2 to identify histogram


  TFile *histofile;
  TString fileentr[3000];
  int nfileentr;




  //  TFBX *tb; // fitbox


//-----------   flags:
//  int FBXon;  //   flag signal that FBX
  double primar_sigma;
  int BiDim;
  int MarksOn;
  int fancy_rot_flag;  // fancy graphs (grid/logy/logz...)  DEPREC
  int fancy_grid,fancy_logy,fancy_logz;
  int divide_mod_flag; // CANVAS DIVISIONS rotate - rotate_flag



  double defaultsigma;  // first guess sigma....... 5?  10?


  /*
   *  BROOOOMFIT............  only one ?
   */
  Tbroomfit *fit;

  //   MyMainFrame();
  MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h, int page, int startstop);
    ~MyMainFrame();


   void FillMainMenu();
   void DoExit();
   void DoSelect();


  //  these two functions are connected directly to LIST and BUTTON
  //   they! know what was exactly pressed -> so - any paging must be
  //   translated inside
  void ClickResponse();
  void ClickResponse(Int_t w, Int_t i);
  //  void ClickResponseDC(TGFrame *frm, Int_t id);  //listbox2 - doubleclick
  void ClickResponse( Int_t id);  //listbox2 - doubleclick

  // this function takes id (tlist/button) and works it out
  void HandleEvents(Int_t id);
 
  //if derives from TObject - must use....
  //void ShowMembers();

  /*
   *
   *   GUI  FUNCTIONS .....................
   */

  // SERVICE / SUPPORT  FUNCTIONS
  void RecoverTH1fromGPAD(int &count,int64_t addr[], const char* onlyclass, int restrict=0, const char*exclude="XXX" );
  //  void RecoverTH1fromGPAD(int addr[]);
  void Movexy(TH1 *h,const char *XY, double factor, double mvzm);
  void Movexy(const char *XY, double factor, double mvzm);
  void SpiderAllTPADs(int &count, int64_t addr[] );
  void SpiderAllTH1s(int &count, int64_t addr[] );
  void RefreshAll();



  void fSELClear(int id,TString *fentry);
  void fSELClearAll(int id,TString *fentry);
  void  fSELRefresh(int id,TString *fentry);

  void fSELSaveSpectra(int id,TString *fentry);


  // GUI FUNCTIONS 

  //  void fSELDivide(int id,TString *fentry);
  void fMenuItemLoadcanvas(int id,TString *fentry);  
  void fMenuItemSavecanvas(int id,TString *fentry);


  void fSELGrid(int id,TString *fentry);
  void fSELLogy(int id,TString *fentry);
  void fSELLogz(int id,TString *fentry);
  



  // =========REMOVE THESE
  // void fSELUnzoom(int id,TString *fentry);
  //void fSELDivCanv(int id,TString *fentry);
  //////  void fSELUnzoomAll(int id,TString *fentry);


  //================ NEW NAMING SCHEME =======
  void fMenuItemUnzoom(int id,TString *fentry);
  void fUnzoom(int id,TString *fentry);


  void fSetRangeAll(int id,TString *fentry);
  void fDivCanvas(int id,TString *fentry);
  void fMenuItemDivCanvas(int id,TString *fentry);



  
  void fSELFindPks(int id,TString *fentry);
  void fSELDelPks(int id,TString *fentry);
  void fSELFindBg(int id,TString *fentry);

  void fSELSetMarks(int id,TString *fentry);
  void fSELDelMarks(int id,TString *fentry);
  void fSELGetMarks(int id,TString *fentry);

  void fSELFBX(int id,TString *fentry);    // MAKES FIT
  void fSELUpdate(int id,TString *fentry); // PUTS back to MARKS
  void  fSELSaveFit(int id,TString *fentry); // SAVE FIT & CANVAS
  void fSELDelFBX(int id,TString *fentry);//  HELP

  void  fSELDateTime(int id,TString *fentry); // 

  void fSELClone2Rint(int id,TString *fentry); // 

  void exec3event(Int_t event, Int_t x, Int_t y, TObject *selected);

  ClassDef(MyMainFrame, 0 )    // this is a macro!!!
};


//void MyMainFrame::ShowMembers(){}

MyMainFrame *mmframe; // My Main Frame 

//MyMainFrame*  shspe();


/*
 *
 *
 */
//this doesnot wotk   void grhelp ();

//this will work if the function is in gui.C  void grhelp2 ();

