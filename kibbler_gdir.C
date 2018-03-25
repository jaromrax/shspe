#include "TSystem.h"
#include "THStack.h"
//#include "sh_graph.C" // I have this in main... SO :
// 








void shspe_ls(){ 
  TString sr=".";
  if (gFile!=NULL){ sr=gFile->GetName() ;}
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








/*
======================================================================
ON MULTI in shspe this will be checked and displayed in one win
   -  THStack("nostack") replacement
  : I looked in the title....  but more convenient would be to look
            inside GetListOfFunctions  TNamed
      TNamed *n=new TNamed("next","b0w00a2")
      b0w00_A1->GetListOfFunctions()->Add( new TNamed("next","b0w00_a2") )


 */
TH1F* extract_next(TH1F* h){
  char s[100];
  if (h==NULL)return NULL;
  TNamed *n=(TNamed*)h->GetListOfFunctions()->FindObject("next");
  if (n!=NULL){ // BEST way
    sprintf( s, "%s", n->GetTitle() );
    printf("i... TNamed 'next'==/%s/\n",s);
  }else{ // depreciated way...
    sprintf( s, "%s", h->GetTitle() );
    char* ss;
    ss=(char*)strstr( s, " next:" );
    if ( ss==NULL) {printf("...end\n%s","");return NULL;}
    char sss[100];
    strcpy( s, &ss[6]   ); //  _next:  has 6 chars
    //    printf("sss =%s\n", sss );
  }
  TH1F* nexth=(TH1F*)gDirectory->Get( s );
  if ( nexth!=NULL ){
    printf("nexth=%s\n", nexth->GetName() );
    //      printf("nexth=%s\n", nexth->GetTitle() );
    return (TH1F*)nexth;
  }
  return NULL;
}





void* extract_display_with(TH1F* h, int &count,int64_t addr[]){
  //int64_t addr[];
  count=0; addr[0]=0; // as in RecoverTH1
  char sn[100];
  char st[100];
  //TH1F *hs[20];
  printf("---- display with ----%s\n","");
  if (h==NULL)return NULL; 
  TList  *l=(TList*)h->GetListOfFunctions();
  int j=0;
  for (int i=0;i<l->GetEntries();i++){ 
    sprintf( sn, "%s", l->At(i)->GetName() );
    sprintf( st, "%s", l->At(i)->GetTitle() );
    printf("%d  %s %s\n",i, sn, st);
    if (strstr(sn,"display_with") ){
      printf("    ... %s... ",st);
      TH1F* nexth=(TH1F*)gDirectory->Get( st );
      if ( nexth!=NULL ){
	printf("  DISPLAY_WITH =%s\n", nexth->GetName() );
	//hs[j]=(TH1F*)nexth;
	addr[count++]= (int64_t)nexth;
	j=j+1;
      }
    }
  }
  return NULL;
}//------exctract with



//
// used by displ from list2 :  replacement of   ->Draw()
// - i think - if multi, this is called to display TH1F.....
//          main point is extract_next
//
// NEWLY - i want to remove recursive dependence
// and get multiple "display_with"
//
void* ro_getnext(TH1F* h){
  int mx,mn;
  int mx1=h->GetMaximum();
  int mn1=h->GetMinimum();
  //TH1F *hs[20];
  TH1* histo;
  int count=MAXPRIMITIVES;  int64_t addr[MAXPRIMITIVES];
  addr[0]=0;
  extract_display_with( h,count,addr ); // gives the paralel histos
  printf("D...return with count==%d\n", count );
  h->SetLineColor(1);
  h->SetLineWidth(2);
  h->Draw();
  double max=h->GetMaximum();
  for (int i=0;i<count;i++){
    histo=(TH1*)addr[i];
    if (histo!=NULL){
      printf("... display_with name/title %s %s\n",
	     histo->GetName(),histo->GetTitle() );
      histo->SetLineColor(i+2);
      histo->SetLineWidth(1);
      histo->Draw("same");
      if (histo->GetMaximum()>max){
	max=histo->GetMaximum();
      }
    }//not NULL
  }//for
  h->GetYaxis()->SetRangeUser(0., max );
  return NULL;
  // TH1F* h2,*h3,*h4, *h5, *h6;
  // h2=extract_next( h ); // gives the followin  histo
  // h->SetLineColor(1);h->SetLineWidth(1);
  // h->Draw();
  // if (h2!=NULL){
  //   h2->SetLineColor(2);h2->SetLineWidth(1);
    
  //   int mx2=h2->GetMaximum();
  //   int mn2=h2->GetMinimum();
  //   if (mn2>mn1){ mn=mn1;}else{mn=mn2;}
  //   if (mx2>mx1){ mx=mx2;}else{mx=mx1;}
  //   h->GetYaxis()->SetRangeUser(mn1, mx*1.1);
  //   h2->GetYaxis()->SetRangeUser(mn1, mx*1.1);

  //   h2->Draw("same");
  //     h3=extract_next( h2 );
  //     if (h3!=NULL){
  // 	h3->SetLineColor(3);h3->SetLineWidth(1);
  // 	h3->Draw("same");
  // 	h4=extract_next( h3 );
  // 	if (h4!=NULL){
  // 	  h4->SetLineColor(4);h4->SetLineWidth(1);
  // 	  h4->Draw("same");
  // 	  h5=extract_next( h4 );
  // 	  if (h5!=NULL){
  // 	    h5->SetLineColor(5);h5->SetLineWidth(1);
  // 	    h5->Draw("same");
  // 	    h6=extract_next( h5 );
  // 	    if (h6!=NULL){
  // 	      h6->SetLineColor(6);h6->SetLineWidth(1);
  // 	      h6->Draw("same");
  // 	    }
  // 	  }
  // 	}
  //     }
  // }//h2 NULL
  
}















//==============================================================
//  Display From List2 =====   Click and Display HISTO
//
//  GetTitle... vybiram podle title???
//
//  fchk1state  == Multi:checkbox
//
//   Nechapu 2015/11/13 proc gettitle uplne staci...aha.. title je vrealite name!
//
void fDisplayFromList2(int id, const char* title, int fchk1state=0){
  //  printf("...displaying from list2 #%d:<%s>\n", id, title );
  if (id>1){
    TObject *obj=gDirectory->Get( title );
    if (obj==NULL){obj=gROOT->GetListOfSpecials()->FindObject( title );}
    if (obj==NULL){ printf("object not found%s\n",""); }else{
      TString  trida=obj->ClassName();
      //      printf("  the class is == %s\n", trida.Data() );
      if (  (strstr(trida.Data(),"TH1")!=0)
	    ||(strstr(trida.Data(),"TH2")!=0)
	      ){
	      TH1 *h=(TH1*)gDirectory->FindObject(title );
	      printf("========== %s =====stat:\n",  title  );
	      printf("entries  = %9.1f\n",  h->GetEntries()  );
	      printf("mean     = %9.1f\n",  h->GetMean()   );
	      printf("RMS      = %9.1f\n",  h->GetRMS()   );
	      printf("integral = %9.1f\n",  h->Integral()   );
	      int nmax=h->GetListOfFunctions()->GetEntries();
	      for (int i=0;i<nmax;i++){
		TString trida2=h->GetListOfFunctions()->At(i)->ClassName();
		if (  strstr(trida2.Data(),"TNamed")!=0 ){
		  printf( "%10s : %s\n",h->GetListOfFunctions()->At(i)->GetName(),
			    h->GetListOfFunctions()->At(i)->GetTitle()    );
		}//it is TNamed
	      }// for all functions
          }
	  if (trida.CompareTo("TH2F")==0){TH2F *h=(TH2F*)gDirectory->FindObject(title ); h->Draw("col");}
	  if (trida.CompareTo("TH1F")==0){TH1F *h=(TH1F*)gDirectory->FindObject(title );
	    if (fchk1state!=0){ // if multi is checked
	      ro_getnext(h);
	    }else{
	      h->Draw();
	    }
	  }

	  // --- THStack appeared not good for movexy....
	  if (trida.CompareTo("THStack")==0){THStack *h=(THStack*)gROOT->GetListOfSpecials()->FindObject(title ); h->Draw("nostack");
	  }
	  
	  if (trida.CompareTo("TH1D")==0){TH1D *h=(TH1D*)gDirectory->FindObject(title ); h->Draw();}
	  if (trida.CompareTo("TH2D")==0){TH2D *h=(TH2D*)gDirectory->FindObject(title ); h->Draw("col");}


	  
	  if (trida.CompareTo("TCutG")==0){
	    TCutG *hc=(TCutG*)obj;//gDirectory->FindObject(title ); 
	    //	    TCutG *hc=(TCutG*)gROOT->GetListOfSpecials()->FindObject(title ); 
	    //GET CURRENT TH2 HERE
	    // when u do simple tree->Draw("a:b")
	    //      titles are conserved
	    char xtith[200];
	    char ytith[200];
	    //  char xtitc[200];
	    //  char ytitc[200];
	    TH1 *h2; // current bidim
	    int count=1;
	    int64_t addr[MAXPRIMITIVES];addr[0]=0;
	    RecoverTH1fromGPAD2( count, addr);
	    h2=(TH1*)addr[0];
	    printf("DEBUG TCutG %ld\n",(long int)h2);
	    if (h2==NULL){ return;}
	    if ( strstr(h2->ClassName(),"TH2F")==0){ return;}
	    
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
	    TCutG *hcbis=(TCutG*)hc->Clone("cuttmp");

	    if ( strcmp(hc->GetVarX(),xtith)!=0){
	      printf("!!! x-axis doesnot match %s x %s\n",xtith,hc->GetVarX());
	      hc->SetLineColor(2); hc->SetLineStyle(2);hc->SetMarkerStyle(7);hc->SetLineWidth(2);
	      TString *xtitS=new TString(xtith);
	      xtitS->ReplaceAll( hc->GetVarX(), "x" );
	      xtitS->ReplaceAll( hc->GetVarY(), "y" );
	      TFormula *newX=new TFormula( "transx", xtitS->Data() );
	      for (int ii=0;ii<hc->GetN();ii++){
		// xtith contains the transformation V022+V006
		double x=hc->GetX()[ii];
		double y=hc->GetY()[ii];
		hcbis->GetX()[ii]=	newX->Eval(x,y);
	      }
	    }
	    if ( strcmp(hc->GetVarY(),ytith)!=0){
	      printf("!!! y-axis doesnot match %s x %s\n",ytith,hc->GetVarY());
	      hc->SetLineColor(2); hc->SetLineStyle(2);hc->SetMarkerStyle(7);hc->SetLineWidth(2);
	    }
	    hc->Draw("LP");//works with TCutG
	    if (hcbis!=NULL)hcbis->Draw("LP");
	  }





	  if (trida.CompareTo("TMultiGraph")==0){
	    TMultiGraph *h=(TMultiGraph*)obj;   // ADD TO GLiOS ??
	    //gDirectory->FindObject(title ); 
	    gPad->Clear();h->Draw("plaw");
	    //	    printf("   .... TMultiGraph PLAW  (clear gpad before)%s\n","");
	  }
	  if (trida.CompareTo("TGraph")==0){
	    TGraph *h=(TGraph*)obj;
	    // nothere gROOT->GetListOfSpecials()->Add( h );
	    //gDirectory->FindObject(title ); 
	    h->SetTitle( obj->GetName()  ); // We put the title == graph name
	    h->Draw("plaw");
	  }

	  if (trida.CompareTo("TGraphErrors")==0){
	    TGraphErrors *h=(TGraphErrors*)obj;
	    //nothere gROOT->GetListOfSpecials()->Add( h ); // ADD TO GliOS ??
	    //gDirectory->FindObject(title ); 
	    h->SetTitle( obj->GetName()  );
	    h->Draw("plaw");
	  }





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












/*=======================================================================
 *=======================================================================
 *    fOPENFILE ---------------------------------------------------------
 *           reacts on click on  ***openfile***
 *
 *    .mysql / sqlite   file also
 *
 *  3 sections:
 *      primitives (canvas?)
 *      keys
 *      objects
 */
//                  npints is the result of fentrySIG - to say # graph points
void fOpenFile(TString *fentry, TGListBox *fListBox2, int npoints){

  shspe_ls();  // fills only the global string "sr"

  int id_selected=fListBox2->GetSelected();
  printf("...<open file> pressed... (%s) selected==#%d/%d\n", 
	 fentry->Data(), id_selected ,fListBox2->GetNumberOfEntries());
  // handle what is in the gDirectory first
  int filename_present=0;
  if (fentry->CompareTo("")!=0 && fentry->CompareTo(" ")!=0)filename_present=1;

  if ( (filename_present==0)&&(id_selected>1)){// not OpenFile
    // will be obsolete......???????
    printf("display from list2%s\n", "" );
    // this already displays the TH objects ==============
    fDisplayFromList2(id_selected , fListBox2->GetSelectedEntry()->GetTitle());
    return;
  }// id selected>0 .......................no openfile ends here



  
  // filename present, proceed with closing an old file
  // this closes file, even when we want to read it?
  if ((gFile!=NULL)&&(filename_present==1)){
    printf("(fOpenFile:) Closing opened file (%s)\n", gFile->GetName() ); 
    //gFile->Close(); // BIG PROBLEMS TO CLOSE
    printf("(fOpenFile:) file Closed (%s)\n", "not really"); 
  }// close current file

  /*
   *  read all the strange  files............... or enter _root_ at the end
   */
  //========================================================
  //  OPEN FILE  ROOT   MYSQL  SQLITE
  //========================================================
  if (filename_present==1){  // ROOT
    printf( "Opening file named:   #%s#\n" , fentry->Data()  );
    // I CAN OPEN ROOT FILES:
    // if (fentry->Index(".root")<0){ fentry->Append(".root");} 
    if (fentry->Index(".root")>0){
      TFile *histofile=new TFile( fentry->Data() );
      if (histofile != NULL){  // opening ROOT file  FILE EXISTS
	printf("file seems opened %s\n","");
      } 
    }// is .root file    
    
    /*   //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
    int npoints;
      TString *fentry=new TString( fEntrySIG->GetText() );
      if ( fentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(fentry->Data() )!=0){// match
	   npoints=atoi( fentry->Data() );
	 }// not a number
       }//fentry  exists---------------possibility to change defaultsigma
    */
    if (fentry->Index(".mysql")>0){
	char commandrm[200];
	char grname[200];  
	char grname2[200];  
	printf("n points = %d / %s\n", npoints, fentry->Data()  );
	sprintf(grname,"%s.dat",  fentry->Data() );
	sprintf(commandrm,"sqmylite -r %s 0 %d >%s", 
		fentry->Data(), npoints, grname);
	system(commandrm); // the graph generated now
	
	sprintf(grname2,"%s.dat.cols",  fentry->Data() );
	sprintf(commandrm,"cat %s | head -1 | wc -w > %s ", 
		grname, grname2 );
	system(commandrm); // # columns
	//	FILE *fco=new fopen( grname2 ,"r");

	 ifstream myReadFile;
	 int outputCol=2;
	 myReadFile.open(grname2);
	 if (myReadFile.is_open()) {myReadFile >> outputCol;}
	 myReadFile.close();
	 outputCol--;
	 for (int i=0;i<outputCol;i++){
	   
	   TGraphErrors *res=(TGraphErrors*)gr_engineX(grname,0,i+1,-1,-1); 
	   res->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
	   res->GetHistogram()->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
	   gDirectory->Add( res );
	   printf("fileMYSQL seems opened CMD:/%s/\n",commandrm);
	   if (outputCol>1){ joingraphsX(grname, res->GetTitle() );}
	 }// for all columns ------ of mysql output
    }// is .mysql file


    
    if (fentry->Index(".sqlite")>0){
	char grname2[200];  
	char commandrm[200];
	char grname[200];
	printf("n points = %d / %s\n", npoints, fentry->Data()  );
	sprintf(grname,"%s.dat",  fentry->Data() );
	sprintf(commandrm,"sqmylite -r %s 0 %d >%s", 
		fentry->Data(), npoints, grname);
	system(commandrm);

		
	sprintf(grname2,"%s.dat.cols",  fentry->Data() );
	sprintf(commandrm,"cat %s | head -1 | wc -w > %s ", 
		grname, grname2 );
	system(commandrm); // # columns


	ifstream myReadFile;
	 int outputCol=2;
	 myReadFile.open(grname2);
	 if (myReadFile.is_open()) {myReadFile >> outputCol;}
	 myReadFile.close();
	 outputCol--;
	 for (int i=0;i<outputCol;i++){
	   printf("sqlite: %d / %d from %s\n", i,  outputCol ,grname );
	   TGraphErrors *res=(TGraphErrors*)gr_engineX(grname,0,i+1,-1,-1); 
	   res->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
	   res->GetHistogram()->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
	   gDirectory->Add( res );
	   printf("fileSQLITE seems opened CMD:/%s/\n",commandrm);
	   //if (outputCol>1){ joingraphsX(grname,res->GetTitle() );}
	 }// for all columns ------ of mysql output

	
	 //TGraphErrors *res=(TGraphErrors*)gr_engineX(grname,0,1,-1,-1); 
	 //gDirectory->Add( res );
	 //printf("fileSQLite seems opened CMD:/%s/\n", commandrm);
    }// is .sqlite file

    
    //if (fentry->Index(".asc1$")>0){
    if (TPRegexp("\\.asc1$").Match(fentry->Data())!=0){
      printf(".... asc1 ... entering \n");
	char commandrm[200];
	char grname[200];
	TGraphErrors *onecol=(TGraphErrors*)gr_engineX( fentry->Data(), -1,0,-1,-1);
	//onecol->Print();
	printf("TGraph onecol\n");
	//	TH1F *h3=new TH1F(fentry->Data(),"Graph from ASC1", onecol->GetN(),0,onecol->GetN() );
	// I hate names with "." : i try to use this MOD
	TH1F *h3=new TH1F( onecol->GetName(),"Graph from ASC1", onecol->GetN(),0,onecol->GetN() );
	printf("new h3\n");
	for (int i=0; i<onecol->GetN(); i++){
	  h3->SetBinContent(i,onecol->GetY()[i]);
	  //printf("%d    %f\n",i,onecol->GetY()[i]);
	}
	gROOT->GetListOfSpecials()->Remove( onecol);
	//#delete onecol;
	delete onecol;
	//onecol->delete();
	//onecol->Delete();
	//	gDirectory->Add( onecol );
	// if (gDirectory->Get( fentry->Data() ) !=NULL){
	//   printf("ALREADY EXISTS h3\n%s","");
	//   //this would delete it 
	//   //delete gDirectory->Get( fentry->Data() );

	// }else{
	//   printf("NOT EXISTS h3\n%s","");
	//   gDirectory->Add( h3 );
	// }
	printf("ASC1 seems opened CMD:/%s/\n", "ok" );
    }// is .asc1 file

    

    if (TPRegexp("\\.asc$").Match(fentry->Data())!=0){
      //if (fentry->Index(".asc$")>0){
      printf(".... asc ... entering \n");
	char commandrm[200];
	char grname[200];
	TGraphErrors *onecol=(TGraphErrors*)gr_engineX( fentry->Data(), 0,1,-1,-1);
	printf("TGraph twocol\n");
	TH1F *h3=new TH1F(fentry->Data(),"Graph from ASC",8196,0,8196);
	printf("new h3\n");
	for (int i=0; i<onecol->GetN(); i++){  h3->SetBinContent(i,onecol->GetY()[i]);}
	gROOT->GetListOfSpecials()->Remove( onecol);
	//#delete onecol;
	//onecol->delete();
	//onecol->Delete();
	//	gDirectory->Add( onecol );
	gDirectory->Add( h3 );
	printf("ASC seems opened CMD:/%s/\n", "ok" );
    }// is .asc file



    
    
    filename_present=0;
  }// if filename was present


  
  
  /**************************************************
   *   clear all entries and start from scratch......
   */
  int max;
  // printf("clear all %d entries at flistbox2 \n", fListBox2->GetNumberOfEntries() );
  //   printf("???flb2  (%d entries). Removing entries from 1 to %d\n", 
  // fListBox2->GetNumberOfEntries(), fListBox2->GetNumberOfEntries() );
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
   */
  printf("============== PRIMITIVES  SECTION ==============%s","\n");
  TList *prim=gPad->GetListOfPrimitives();
  //---------------- PRMITIVES------------------
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

    
    // printf("D....    checking non  TH classes...................%s\n",sn.Data());
    // //--- new attempt to open graphs and stuff
    // if ( sn.Index("TGraph")==0 ){
    //   TGraph *hih=(TGraph*)prim->At(ii);
    //   TString sn_hih=hih->GetName();
    //   hih->SetName(sn_hih);
    //   if ( gROOT->GetListOfSpecials()->FindObject(hih)==NULL ){
    // 	gROOT->GetListOfSpecials()->Add( hih );
    // 	printf("%s added to GLiOSpecia\n", hih->GetName());
    //   }else{printf("%s NOT added\n", hih->GetName());}
    // }

    // if ( sn.Index("TGraphErrors")==0 ){
    //   TGraphErrors *hih=(TGraphErrors*)prim->At(ii);
    //   TString sn_hih=hih->GetName();
    //   hih->SetName(sn_hih);
    //   if ( gROOT->GetListOfSpecials()->FindObject(hih)==NULL ){
    // 	gROOT->GetListOfSpecials()->Add( hih );
    // 	printf("%s added to GLiOSpecia\n", hih->GetName());
    //   }else{printf("%s NOT added\n", hih->GetName());}
    // }

    // if ( sn.Index("TMultiGraph")==0 ){
    //   TMultiGraph *hih=(TMultiGraph*)prim->At(ii);
    //   TString sn_hih=hih->GetName();
    //   hih->SetName(sn_hih);
    //   if ( gROOT->GetListOfSpecials()->FindObject(hih)==NULL ){
    // 	gROOT->GetListOfSpecials()->Add( hih );
    // 	printf("%s added to GLiOSpecia\n", hih->GetName());
    //   }else{printf("%s NOT added\n", hih->GetName());}
    // }




    /**********************   RIKAM SI - VYHAZIM VSECHNY GRAFY A MULTIGRAFY *****
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
    /**********  RIKAM SI - VYHAZIM VSECHNY GRAFY A MULTIGRAFY - to zabralo
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
    
  }//-------------------------------ii all primitives
  
  
  /******************************************************************************
   *            gDirectory->GetListOfKeys()->GetEntries()
   * KEYS
   * a pak 
   * OBJECTS
   ******************************************************************************
   *
   */
  
  TObject *o;
  printf("============== KEYS  SECTION ==============%s","\n");
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
      // if ((sa2.Index("THStack")==0)&&(gDirectory->FindObject(o)==NULL)) {
      // 	printf("adding explicitely %s THStack\n", sa1.Data()  );
      // 	printf("adding to gdir 3 %s\n", ""); gSystem->Sleep(200);
      // 	gDirectory->Add( (THStack*)o ); 
      // }
      
      // this loaded TCutG into the list2
      if ((sa2.Index("TCutG")==0)&&(gDirectory->FindObject(o)==NULL)) {
	printf("adding explicitely %s\n", sa1.Data()  );
	printf("adding to gdir 3 %s\n", ""); gSystem->Sleep(200);
	gDirectory->Add( (TCutG*)o ); 
      }
      // this should load TGraph into the list2
      if ((sa2.Index("TGraph")==0)&&(gDirectory->FindObject(o)==NULL)) {
	/*TString sn_hih=o->GetName();
	  if  (TPRegexp("_g$$").Match(sn_hih)==0){// no match
	  sn_hih.Append("_g");TGraph *ggo=(TGraph*)o;
	  ggo->SetName(sn_hih);
	  }*/
	// add to gdir later - what if they erase in joingraphs NONO
	printf("adding to gDirectory %s\n", ""); gSystem->Sleep(20);
	gDirectory->Add( (TGraph*)o );
	TGraph *ggo=(TGraph*)o;
	ggo->SetMarkerStyle(6);  // dot  7=biger dot
	printf("adding explicitely %s -----------to GetListOfSpecials\n",
	       sa1.Data()  );
	if ( gROOT->GetListOfSpecials()->FindObject(ggo)==NULL ){
	  gROOT->GetListOfSpecials()->Add( ggo );
	  printf("%s added to GLiOSpecia\n", ggo->GetName());
	}else{printf("%s NOT added\n", ggo->GetName());}
	
	// GET LIS OF FUNCTIONS ?????????????
	//ggo->GetListOfFunctions()->ls();
	printf("D... look for function with TNamed...%s\n","");
	if ( (ggo->GetListOfFunctions()->GetEntries()>0)&& (strstr(ggo->GetListOfFunctions()->At(0)->ClassName(),"TNamed")!=NULL)  ){
	  TNamed *n=(TNamed*)ggo->GetListOfFunctions()->At(0);
	  printf("i... Class was TNamed, checking 1st element:  /%s/\n",n->GetTitle() );
	  joingraphsX( n->GetTitle(), ggo->GetName() ); // JOINGRAPHS  (X for mysql version; char *g1 !!)
	}
	
      }// IF  is TGraph.......
      
      

      //===================}// TGraph.....+

      //  gDirectory->Add( (TCanvas*)o ); // problematic
      //	  o=gDirectory->FindKey( hinu.Data() );
      //gDirectory->Get(  sa1.Data() );
      //	  TString ntit=o->GetTitle();
      //	  ntit.Append( "_{   " );ntit.Append( fentry->Data() );ntit.Append( "}" );
      //	  o->SetTitle( ntit.Data() );
    }//iii < max
  } else{     //if list of KEYS
    printf(" NO KEYS in gDirectory  %d   ..........  \n", 0);
  }
  




/* =========================================================
 *   all from gdirectory goes hereto flist2.................
 *
 */
//printf(" keys in gDirectory = %d  ..........  loading MEMory -> fListBox2\n", max);
  printf("============== OBJECTS  SECTION ==============%s","\n");

  max=gDirectory->GetList()->GetEntries();
  int newmax=max;
  if (max > 1000 ){
    printf("max entries is limited to 100 (in plae of %d)\n", max);
    newmax=1000;
  } // LIMIT ENTRIES 
  printf("OBJECTS in gDirectory = %d  ..........  loading MEMory -> fListBox2\n", max);
  int activentry=2; // this is entry number in the listbox2 .. 
  //             . ...not correlated with the displayed position in the box..
  for (int iii=0 ; iii<newmax ; iii++ ){
    TString sa1=gDirectory->GetList()->At(iii)->GetName();
    if (sa1.Contains("/")>0){ printf("PROBLEM, I CANNOT HANDLE SLASH / in %s\n",sa1.Data() );}
    // printf("  %03d/  object: %s\n",  iii, sa1.Data()   );
    o=gDirectory->FindObject( sa1.Data() );
    TString sa2=o->ClassName();
    
    
    // printf("  %03d/  object: %s/%s\n",  iii, sa1.Data(), sa2.Data()   );
    if (  (sa2.Index("TH1")==0) || (sa2.Index("TH2")==0)
	  //	  || (sa2.Index("THStack")==0)
	  || (sa2.Index("TGraph")==0)
	  || (sa2.Index("TGraphErrors")==0)
	  || (sa2.Index("TMultiGraph")==0) || (sa2.Index("TCutG")==0) ){	 //
      
      
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
  if (max > newmax ){
    printf("max entries is limited to 1000 (in place of %d)\n", max);
  } // LIMITED ENTRIES 
  
  



  max=gROOT->GetListOfSpecials()->GetEntries();
  printf("List of specials - entries == %d\n",max);
  for (int iii=0 ; iii<max ; iii++ ){
    //   printf("making iii==%d <  max==%d\n", iii,  max );
    TString sa1=gROOT->GetListOfSpecials()->At(iii)->GetName();
    printf("NAME      === %s\n", sa1.Data()  );
    //??  o=gDirectory->FindObject( sa1.Data() );// nesmysl,uz to preci mam.
    //  o=gROOT->GetListOfSpecials()->FindObject( sa1.Data() ); // nesmysl,uz to preci mam.
    o=(TObject*)gROOT->GetListOfSpecials()->At(iii);
    TString sa2=o->ClassName();
    //  printf("CLASSNAME === %s\n", sa2.Data()  );
    
    // Now I have a class
    if (  (sa2.Index("TH1")==0) || (sa2.Index("TH2")==0)
	  //	  || (sa2.Index("THStack")==0)
	  || (sa2.Index("TGraph")==0)
	  || (sa2.Index("TGraphErrors")==0) || (sa2.Index("TMultiGraph")==0) || (sa2.Index("TCutG")==0) ){	 //
      
      
      if (fListBox2->FindEntry( sa1.Data()) ==NULL ) {
	printf("adding entry %15s, entry#%2d\n", sa1.Data(),activentry );
	fListBox2->AddEntry(  sa1.Data() , activentry );  // from 2 ++	 //
	activentry++;  
      }else{
	//           printf("....entry %15s already exists in the list\n",sa1.Data() );
      }
      //	fileentr[nfileentr]= sa1.Data();  
      //nfileentr++;	
    }//check classes..........
    //    printf("next iii=%d\n", iii);
  }//iii ----------  all specials -------
  
  
  printf("sorting %s\n", "");
  fListBox2->SortByName( kTRUE );
  printf("########## MEM is loaded %s ################end of openfile\n" , ""  );
  
}//---------------------------------END OF OPENFILE ############################
//##############################################################################
