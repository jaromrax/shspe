#include "TSystem.h"


void shspe_ls(){ 
  
  TString sr=".";
  if (gFile!=NULL){ sr=gFile->GetName() ;}
  //histofile=new TFile( sr.Data() );
  /*
     printf("\n\n|######### LIST OF root FILES ####################################|\n%s" ,  "" ); 
    	 printf("|=================================================================|\n%s" , ""  ); 
	 //	 printf("|                         |%s" , ""  ); 
	 gROOT->ProcessLine(".! pwd");
    	 printf("|=================================================================|\n%s" , ""  ); 
	 printf("|-------------------->                                            |\n%s" , ""  ); 
	 gROOT->ProcessLine(".! ls -lh *.root 2>/dev/null | cut -d\" \" -f 5-1111");
	 printf("|-------------------->                                            |\n%s" , ""  ); 
	 //  	 printf("|-- possible canvases   (20_DivLoadCanvas) -----------------------|\n%s" , ""  ); 
	 //  	 printf("|-------------------->                                            |\n%s" , ""  ); 
	 //	 gROOT->ProcessLine(".! ls -lh shspe*c.root *canv*.root 2>/dev/null | cut -d\" \" -f 5-1111");
	 // 	 printf("|-------------------->                                            |\n%s" , ""  ); 
  	 printf("|--                     (ASCII spectra - one channel - *.asc) ----|\n%s" , ""  ); 
  	 printf("|-------------------->                                            |\n%s" , ""  ); 
	 gROOT->ProcessLine(".! ls -lh *.dat *.asc 2>/dev/null | cut -d\" \" -f 5-1111");
  	 printf("|--                     (ASCII spectra - two channels- *.asc2)----|\n%s" , ""  ); 
  	 printf("|-------------------->                                            |\n%s" , ""  ); 
	 gROOT->ProcessLine(".! ls -lh *.dat *.asc 2>/dev/null | cut -d\" \" -f 5-1111");
 	 printf("|-------------------->                                            |\n%s" , ""  ); 
  	 printf("|--                     (TGraph x,y     *.gr2, *.gr3) ------------|\n%s" , ""  ); 
	 gROOT->ProcessLine(".! ls -lh *.gr2 2>/dev/null | cut -d\" \" -f 5-1111");
	 gROOT->ProcessLine(".! ls -lh *.gr3 2>/dev/null | cut -d\" \" -f 5-1111");
 	 printf("|-------------------->                                            |\n%s" , ""  ); 
  	 printf("|=================================================================|\n%s" , ""  ); 
	 // printf("|-----------------------------------------------------------------|\n%s" , sr.Data()  ); 
	 //	 printf("|------------------------|                                        |\n" , sr.Data()  ); 
	 //	 printf("########## Now enter the filename in the field. %s ################\n" , sr.Data()  ); 
	 //	 printf("=========================|                                        |\n" , sr.Data()  ); 
	 printf( "|My current gDirectory is %-40s|\n", gDirectory->GetName()  );
	 //intf("|-----------------------------------------------------------------|\n%s" , sr.Data()  ); 
         printf("|#################################################################|\n%s\n" , sr.Data()  ); */
}


void  RecoverTH1fromGPAD2(int &count,int64_t addr[]){
  int countmax=1;
  count=0;
       TList *prim=gPad->GetListOfPrimitives();
 for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
    TString sn=prim->At(ii)->ClassName();
    //       printf( "refr:%2d  %s\n", ii,  sn.Data()  );
    if ( (sn.Index("TH1")==0)||(sn.Index("TH1F")==0)||(sn.Index("TH1D")==0)||(sn.Index("TH2F")==0) ){  // there is TPad :< there 
      TH1 *tp=((TH1*)prim->At(ii));
      if (count<countmax){        addr[count++]= (int64_t)tp;  
	printf("TH1 found /%s/\n", tp->GetName() );
      }else{
	printf("Increase the field above %d\n", count);
      }
    }
 }
}//RECOVER TH from gPad



/*
 *  OCEKAVA  title == histoname
 */
void fSAVEFromList2(int id, const char* title, const char *savename){
  printf("...saving from list2 #%d  title==<%s>  to  savename<%s>\n", id, title, savename ); 
  if ( strstr(title,"Open File")||(strcmp(title,"")==0) ){
    printf("\n\nUSE DOUBLECLICK TO ENTER THE HISTO NAME TO fEntry....\n\n\n%s","");
    return;
  }
  if (id>1){
    //unused    TObject *obj=gDirectory->Get( title ); // probably NAME, not TITLE
    TH1 *h=(TH1*)gDirectory->FindObject(title ); 
    TString ntit=h->GetTitle();//get TITLE
    ntit.Append("_{ @");
    if (gFile!=NULL){ ntit.Append( gFile->GetName() );} 
    ntit.Append( " }" );
    printf("  new title for save ......... <%s>\n" , ntit.Data()  );
    TFile *fout=new TFile( savename, "UPDATE" );
    TH1* hc=(TH1*)h->Clone(); hc->SetTitle( ntit.Data()  ); hc->Write();
    //    h->SetDirectory("
    fout->Close();
  }//id>1
}



void fDisplayFromList2(int id, const char* title, int fchk1state=0){
  //  printf("...displaying from list2 #%d:<%s>\n", id, title );
  if (id>1){
    TObject *obj=gDirectory->Get( title );
    if (obj==NULL){obj=gROOT->GetListOfSpecials()->FindObject( title );}
    if (obj==NULL){ printf("object not found%s\n",""); }else{
      TString  trida=obj->ClassName();
      //      printf("  the class is == %s\n", trida.Data() );
         if ( strstr(trida.Data(),"TH")!=0){
	      TH1 *h=(TH1*)gDirectory->FindObject(title );
	      printf("entries  = %9.1f\n",  h->GetEntries()  );
	      printf("mean     = %9.1f\n",  h->GetMean()   );
	      printf("RMS      = %9.1f\n",  h->GetRMS()   );
	      printf("integral = %9.1f\n",  h->Integral()   );
          }
	  if (trida.CompareTo("TH2F")==0){TH2F *h=(TH2F*)gDirectory->FindObject(title ); h->Draw("col");}
	  if (trida.CompareTo("TH1F")==0){TH1F *h=(TH1F*)gDirectory->FindObject(title ); h->Draw();}
	  if (trida.CompareTo("TH1D")==0){TH1D *h=(TH1D*)gDirectory->FindObject(title ); h->Draw();}
	  if (trida.CompareTo("TH2D")==0){TH2D *h=(TH2D*)gDirectory->FindObject(title ); h->Draw("col");}

	  if (trida.CompareTo("TCutG")==0){
	    //TCutG *h=(TCutG*)gDirectory->FindObject(title ); 
	    TCutG *hc=(TCutG*)gROOT->GetListOfSpecials()->FindObject(title ); 
	    //GET CURRENT TH2 HERE
	    // when u do simple tree->Draw("a:b")
	    //      titles are conserved
	    char xtith[200];
	    char ytith[200];
	    char xtitc[200];
	    char ytitc[200];
	    TH1 *h2; // current bidim
	    int count=1;
	    int64_t addr[MAXPRIMITIVES];addr[0]=0;
	    RecoverTH1fromGPAD2( count, addr);
	    h2=(TH1*)addr[0];
	    
	    strcpy(xtith,h2->GetXaxis()->GetTitle());
	    strcpy(ytith,h2->GetYaxis()->GetTitle());
	    //	    printf("    titles x,y : /%s/,/%s/\n",  xtith, ytith );

	    if ( (strlen(xtith)<=0) || (strlen(ytith)<=0) ){
	      //	      printf("    no titles x,y : %s%s\n",  "", "" );
	      //derive axes from the title:
	      char ww[300];
	      strcpy(ww,h2->GetTitle() );
	      //	      printf("    ww head title : %s ... i check cutpart\n", ww );
	      //here i remove condition appedix
	      char *w1; w1=strstr( ww," ");
	      //	      printf("    w1 head title wspace : %s ... \n", w1 );

	      if ( w1!=NULL ){
		printf("    w1 > 3 - trim at 0 : %s ... \n", ww );
		w1[0]='\0'; //dangerous!?! no check
	      }// cleaned...
	      //	      printf("    ww head title wspace : %s ... \n", ww );
	      //	      printf("    head title after cut removal : %s\n", ww );
	      char *wx; wx=strstr( ww,":");
	      char wx2[100]; strcpy( wx2,&wx[1] );
	      // x-part of the title goes here
	      //	      printf("    head title  x part : %s\n", wx2 );
	      strcpy( xtith, wx2 );
	      //original title cut makes  y
	      wx[0]='\0'; 
	      //	      printf("    head title  y part : %s\n", ww );
	      strcpy(ytith,ww);
	    }// null titles

	    hc->SetLineColor(1); hc->SetLineStyle(1);hc->SetMarkerStyle(7);hc->SetLineWidth(2);
	    if ( strcmp(hc->GetVarX(),xtith)!=0){
	      printf("!!! x-axis doesnot match %s x %s\n",xtith,hc->GetVarX());
	      hc->SetLineColor(2); hc->SetLineStyle(2);hc->SetMarkerStyle(7);hc->SetLineWidth(2);
	    }
	    if ( strcmp(hc->GetVarY(),ytith)!=0){
	      printf("!!! y-axis doesnot match %s x %s\n",ytith,hc->GetVarY());
	      hc->SetLineColor(2); hc->SetLineStyle(2);hc->SetMarkerStyle(7);hc->SetLineWidth(2);
	    }
	    hc->Draw("LP");//works with TCutG
	  }


	  if (trida.CompareTo("TMultiGraph")==0){
	    TMultiGraph *h=(TMultiGraph*)gDirectory->FindObject(title ); gPad->Clear();h->Draw("plaw");
	    //	    printf("   .... TMultiGraph PLAW  (clear gpad before)%s\n","");
	  }
	  if (trida.CompareTo("TGraph")==0){
	    TGraph *h=(TGraph*)gDirectory->FindObject(title ); 
	    h->SetTitle( obj->GetName()  ); // We put the title == graph name
	    h->Draw("plaw");
	  }
	  if (trida.CompareTo("TGraphErrors")==0){
	    TGraphErrors *h=(TGraphErrors*)gDirectory->FindObject(title ); 
	    h->SetTitle( obj->GetName()  );
	    h->Draw("plaw");
	  }


	  /*
	  if ( (trida.CompareTo("TGraph")==0)||(trida.CompareTo("TGraphErrors")==0)){

	    TGraphErrors *gr=(TGraphErrors*)gDirectory->FindObject(title );
	    TH1* histo;  int addr[1];  int count=1; addr[0]=0;
	    RecoverTH1fromGPAD2( count, addr );
	    int make_own_h=1;
	    histo=(TH1*)addr[0];
	    if (histo!=NULL){
	      printf("histo belonging to tgraph was found: see the stats TGraphErrors:%s\n", "" );
	      gr->Print();
	      printf("histo belonging to tgraph was found: see the stats TH1:%s\n", "" );
	      histo->Print();
	      // CHECKING ONLY IN X ......  Y can be very difficult for 1D
	      if ( (gr->GetXaxis()->GetXmin()>histo->GetXaxis()->GetXmin())&&
		   (gr->GetXaxis()->GetXmax()<histo->GetXaxis()->GetXmax())//&&
		   //		   (gr->GetYaxis()->GetXmin()>histo->GetYaxis()->GetXmin())&&
		   //		   (gr->GetYaxis()->GetXmax()<histo->GetYaxis()->GetXmax())
		   ){
 		printf("Using a previous HISTO to plot TGraph...\n%s", "" );
		 gr->Draw("pl");
		 make_own_h=0;
	      }// graph fits inside.
	      else{
		printf(" previous HISTO is too narrow.... to plot TGraph...\n%s", "" );
		make_own_h=1;
	      }// graph doesnot fit
	    }// histo not null......
	    if (make_own_h==1){
	    // BETTER WE SHOULD FIND/CREATE histo for the tgraph....
	    TString grhis=title;
	    grhis.Append("_H");
	    TH2F *hgr=new TH2F(grhis.Data(),grhis.Data(),200, gr->GetXaxis()->GetXmin() , gr->GetXaxis()->GetXmax(),
			                                 200, gr->GetYaxis()->GetXmin() , gr->GetYaxis()->GetXmax() );
	    hgr->SetStats(kFALSE);
	    hgr->Draw();
	    gr->Draw("pl");
	    }// make own HISTO
	  }// CLASS == TGRAPH  OR  TGRAPHERROR
*/


	  if (gPad!=NULL){ gPad->Modified();gPad->Update();}
    }// object exists
    // printf("diplay%s\n","");
  }// if   id>1

   if ( fchk1state==1 ){
     TString s=gPad->GetName();
     printf("Jumping from %s to next ", s.Data()  );
     s.Replace(0,2,"");
     TCanvas *c=gPad->GetCanvas( );
     c->cd( s.Atoi()+1 );
     printf("  %d\n", s.Atoi()+1  );

   }//checked ALL


}// f display from list2












/*
 *
 *    fOPENFILE ------------------------------------------------------------------------------
 *           reacts on click on  ***openfile***
 */

void fOpenFile(TString *fentry, TGListBox *fListBox2){

    shspe_ls();

  int id_selected=fListBox2->GetSelected();
  printf("...<open file> pressed... (%s) selected==#%d/%d\n", 
	 fentry->Data(), id_selected ,fListBox2->GetNumberOfEntries());
  // handle what is in the gDirectory first
  int filename_present=0;
  if ( fentry->CompareTo("")!=0 && fentry->CompareTo(" ")!=0  ) filename_present=1;

  if ( (filename_present==0)&&(id_selected>1)){
    // will be obsolete......???????
    printf("display from list2%s\n", "" );
    fDisplayFromList2( id_selected , fListBox2->GetSelectedEntry()->GetTitle()  );
    return;
  }// id selected>0


  // filename present, proceed with closing an old file
  if ((gFile!=NULL)&&(filename_present==1)){// this closes file, even when we want to read it?
    printf("(fOpenFile:) Closing opened file (%s)\n", gFile->GetName() ); 
    //gFile->Close(); // BIG PROBLEMS TO CLOSE
    printf("(fOpenFile:) file Closed  (%s)\n", "virtually"); 
  }// close current file



  /*
   *  read all the strange  files............... or enter _root_ at the end
   */
  if (filename_present==1){  // ROOT
    printf( "Openning file named:#%s#\n" , fentry->Data()  );
    if (fentry->Index(".root")<0){ fentry->Append(".root");}
    TFile *histofile=new TFile( fentry->Data() );
    if (histofile != NULL){  // opening ROOT file and the FILE EXISTS
      printf("file seems opened %s\n","");
    } 
    filename_present=0;
  }//filename was present


  /*
   *   clear all entries and start from scratch......
   * 
   */
int max;
 printf("clear all %d entries at flistbox2 \n", fListBox2->GetNumberOfEntries() );
    printf("???flb2  (%d entries). Removing entries from 1 to %d\n", 
	   fListBox2->GetNumberOfEntries(), fListBox2->GetNumberOfEntries() );
    // fListBox2->RemoveEntries( 2, fListBox2->GetNumberOfEntries()-1  ); /// -WHAT???  zkusime -1
    // if  1,  ge-1  : 2 zustaly ale ne openfile
    // if  1   ge    : neni ani openfile
    //fListBox2->RemoveEntries( 1, fListBox2->GetNumberOfEntries()-1  ); /// impossible. 2 remain but not openfile
    //  fListBox2->RemoveEntries( 2, fListBox2->GetNumberOfEntries()-2  ); /// *open* a 2 dalsi
  fListBox2->RemoveEntries( 2, fListBox2->GetNumberOfEntries()  ); /// *open*  ONLY remains
//int nfileentr=0;// clear fileentr !! 
//TString fileentr[3000];


/***************************************************************************************************
 *   legalize primitives from gpad (htemp)
 *   ???  I assume that all graphs should be at    specials
 *
 */
  TList *prim=gPad->GetListOfPrimitives();
  for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
    TString sn=prim->At(ii)->ClassName();
    // ======================= TH ====================
    if ( sn.Index("TH")==0 ){
       TH1 *hih=(TH1*)prim->At(ii);TString sn_hih=hih->GetName();
       TH1 *hih2;
        if ( (sn_hih.CompareTo( "htemp" )==0) ){
	  if (gDirectory->FindObject("htemp_shspe") ){delete gDirectory->FindObject("htemp_shspe");}
	  hih2=(TH1*)hih->Clone("htemp_shspe");  // setname not enough...it clears
	  hih2->SetName("htemp_shspe");
	 printf("htemp histo found... adding to the list %s\n","");
	 printf("adding to gdir 1 %s\n", ""); gSystem->Sleep(200);
	 gDirectory->Add( hih2 );
	}//if htemp
    }//TH class

    /**********************   RIKAM SI - VYHAZIM VSECHNY GRAFY A MULTIGRAFY ********************

    // ======================= TGraph ====================
    if ( sn.Index("TGraph")==0 ){
       TGraph *hih=(TGraph*)prim->At(ii);
       TString sn_hih=hih->GetName(); 
       if  (TPRegexp("_g$$").Match(sn_hih)==0){// no match

       sn_hih.Append("_g");
       hih->SetName(sn_hih);
       //
       printf("adding %s to gdir 2 \n", sn_hih.Data() ); gSystem->Sleep(200);
       gDirectory->Add( hih );
       }//no match....
       else{
	 
       }// already _g exists.... we replace

    }// TGraph.....+



    // ======================= TMultiGraph ====================
    if ( sn.Index("TMultiGraph")==0 ){
       TMultiGraph *hih=(TMultiGraph*)prim->At(ii);
       TString sn_hih=hih->GetName(); 
       if  (TPRegexp("_g$$").Match(sn_hih)==0){// no match
	  //
	  //  MUSIM NOVE GRAFY.  ONLY - specials, vzdy ->gethistogram, plaw   a ono to nejak klapne
	  //  a vzdy s originalem, nikdy kopie.
	  //
       sn_hih.Append("_g");
       hih->SetName(sn_hih);
       //
       printf("adding %s to gdir 2 \n", sn_hih.Data() ); gSystem->Sleep(200);
       gDirectory->Add( hih );
       }//no match....
       else{
	 
       }// already _g exists.... we replace

    }// TMultiGraph.....+

*/
    /**********************   RIKAM SI - VYHAZIM VSECHNY GRAFY A MULTIGRAFY - to zabralo
          A CUTS ********************
      AT JE TO VE SPECIALS>... NE?        

    // ======================= TCutG ==================== in fOpen...
    // nothing happened with TCutG here... why???
    if ( sn.Index("TCutG")==0 ){
       TCutG *hih=(TCutG*)prim->At(ii);
       TString sn_hih=hih->GetName(); 
       if  (TPRegexp("_c$$").Match(sn_hih)==0){// no match

       sn_hih.Append("_c");
       hih->SetName(sn_hih);
       printf("adding to gdir 2 %s\n", ""); 
       gSystem->Sleep(200);
       gDirectory->Add( hih );
       }//no match....
       else{
       }// already _g exists.... we replace

    }// TCutG.....+
    */

  }//ii all primitives


  /*********************************************************************************************
   *            gDirectory->GetListOfKeys()->GetEntries()
   * KEYS
   * a pak 
   * OBJECTS
   *********************************************************************************************
   *
   */

TObject *o;
// KEYS: if KEYS ==> what happens if I copy to memory?
 max=0;
if (gDirectory->GetListOfKeys()){
             max=gDirectory->GetList()->GetEntries();
  printf(" items in gDirectory = %d  ..........  \n", max);
  max=gDirectory->GetListOfKeys()->GetEntries();
  printf(" KEYS in gDirectory = %d  ..........  \n", max);
  	for (int iii=0 ; iii<max ; iii++ ){
	  TString sa1=gDirectory->GetListOfKeys()->At(iii)->GetName();
	  gDirectory->GetObject( sa1.Data() , o );
	  TString sa2=o->ClassName();
	  //	  printf( "KEY: name-class:    %15s %15s(object)\n" ,  sa1.Data(),  sa2.Data()   );
	  /*     Get  da cokoli,  GetKey..only key, find object...only object
	   *
	   */
	  if ((sa2.Index("TH1F")==0)&&(gDirectory->FindObject(o)==NULL)) {
	    printf("adding explicitely %s\n", sa1.Data()  );
	    printf("adding to gdir 3 %s\n", ""); gSystem->Sleep(200);
	    gDirectory->Add( (TH1F*)o ); 
	  }
	  // this loaded TCutG into the list2
	  if ((sa2.Index("TCutG")==0)&&(gDirectory->FindObject(o)==NULL)) {
	    printf("adding explicitely %s\n", sa1.Data()  );
	    printf("adding to gdir 3 %s\n", ""); gSystem->Sleep(200);
	    gDirectory->Add( (TCutG*)o ); 
	  }

	  //  gDirectory->Add( (TCanvas*)o ); // problematic
	  //	  o=gDirectory->FindKey( hinu.Data() );
	  //gDirectory->Get(  sa1.Data() );
	  //	  TString ntit=o->GetTitle();
	  //	  ntit.Append( "_{   " );ntit.Append( fentry->Data() );ntit.Append( "}" );
	  //	  o->SetTitle( ntit.Data() );
	}//iii < max
 }//if list of KEYS
 else{  printf(" NO KEYS in gDirectory  %d   ..........  \n", 0); }





/*
 *   all from gdirectory goes hereto flist2.................
 *
 *
 */
//printf(" keys in gDirectory = %d  ..........  loading MEMory -> fListBox2\n", max);
max=gDirectory->GetList()->GetEntries();
printf("OBJECTS in gDirectory = %d  ..........  loading MEMory -> fListBox2\n", max);
 int activentry=2; // this is entry number in the listbox2 .. 
 //             . ...not correlated with the displayed position in the box..
 for (int iii=0 ; iii<max ; iii++ ){
 TString sa1=gDirectory->GetList()->At(iii)->GetName();
 if (sa1.Contains("/")>0){ printf("PROBLEM, I CANNOT HANDLE SLASH / in %s\n",sa1.Data() );}
 // printf("  %03d/  object: %s\n",  iii, sa1.Data()   );
 o=gDirectory->FindObject( sa1.Data() );
 TString sa2=o->ClassName();
 

 // printf("  %03d/  object: %s/%s\n",  iii, sa1.Data(), sa2.Data()   );
   if (  (sa2.Index("TH1")==0) || (sa2.Index("TH2")==0) || (sa2.Index("TGraph")==0)
   || (sa2.Index("TGraphErrors")==0) || (sa2.Index("TMultiGraph")==0) || (sa2.Index("TCutG")==0) ){	 //


     if (fListBox2->FindEntry( sa1.Data()) ==NULL ) {
         fListBox2->AddEntry(  sa1.Data() , activentry );  // from 2 ++	 //
	    printf("Entry added %15s, entry#%2d\n", sa1.Data(),activentry );
            activentry++;  
     }else{
            printf("....entry %15s already exists in the list\n",sa1.Data() );
     }
	//	fileentr[nfileentr]= sa1.Data();  
       //nfileentr++;	
   }//is TH1 th2 tgraph tcutg.....
 }//iii all entries gdir






 max=gROOT->GetListOfSpecials()->GetEntries();
 printf("List of specials - entries == %d\n",max);
 for (int iii=0 ; iii<max ; iii++ ){
   printf("making iii==%d <  max==%d\n", iii,  max );
  TString sa1=gROOT->GetListOfSpecials()->At(iii)->GetName();
  printf("NAME      === %s\n", sa1.Data()  );
  //??  o=gDirectory->FindObject( sa1.Data() );// nesmysl,uz to preci mam.
  //  o=gROOT->GetListOfSpecials()->FindObject( sa1.Data() ); // nesmysl,uz to preci mam.
  o=(TObject*)gROOT->GetListOfSpecials()->At(iii);
  TString sa2=o->ClassName();
  printf("CLASSNAME === %s\n", sa2.Data()  );

  // Now I have a class
    if (  (sa2.Index("TH1")==0) || (sa2.Index("TH2")==0) || (sa2.Index("TGraph")==0)
   || (sa2.Index("TGraphErrors")==0) || (sa2.Index("TMultiGraph")==0) || (sa2.Index("TCutG")==0) ){	 //


     if (fListBox2->FindEntry( sa1.Data()) ==NULL ) {
       	   printf("adding entry %15s, entry#%2d\n", sa1.Data(),activentry );
           fListBox2->AddEntry(  sa1.Data() , activentry );  // from 2 ++	 //
           activentry++;  
     }else{
           printf("....entry %15s already exists in the list\n",sa1.Data() );
     }
	//	fileentr[nfileentr]= sa1.Data();  
       //nfileentr++;	
    }//check classes..........
    printf("next iii=%d\n", iii);
 }//iii ----------  all specials -------


   printf("sorting %s\n", "");
 fListBox2->SortByName( kTRUE );
 printf("########## MEM is loaded %s ################end of openfile\n" , ""  );

}//---------------------------------END OF OPENFILE ##########################################
//############################################################################################
