//
// Author: Jaromir Mrazek -  2008/08
//   started with scripts and help of people from root team (Antcheva 1/12/2006)
//



#include "kibbler_fit.C"

#include "shspe.h"
#include "kibbler_gdir.C"

//#include "cuts_manip.h"


//we move to  gui.h .......to have visibility...#include "kibbler_graphs.C"


ClassImp(MyMainFrame)  // this is a macro !!!!


void MyMainFrame::DoSelect(){   Printf("Slot DoSelect()");}
void MyMainFrame::DoExit(){   Printf("Slot DoExit()");}






int CURRENT_SLOT=1;  //   get_free_slot ....







//=============================================================
//================================================
//  CUTS  GROUP
//================================================
//=============================================================

const char* CUTFILE_GLOBAL="cuts.root";

/*
 *      save the defined cut into    cuts.root
 */
void cutsave(TCutG *cut, const char* name){
  TDirectory *dir=gDirectory;
  TCutG *newcut=(TCutG*)cut->Clone( name ); // better to clone before (other dir...)
  TFile *nf=new TFile( CUTFILE_GLOBAL , "UPDATE");
  newcut->Write();                         
  nf->ls();
  nf->Close();
  dir->cd();
  //  dir->ls();
}//cutsave--------------






void cutload(){
  gROOT->GetListOfSpecials()->ls();// ORIGINAL
  TDirectory *dir=gDirectory;
  TFile *nf=new TFile( CUTFILE_GLOBAL, "READ"); // WAS UPDATE, touched cuts.root..
  // UNUSED int n=gDirectory->GetNkeys();
  if (gDirectory->GetListOfKeys()){
      TObject *o;
      int max=gDirectory->GetList()->GetEntries();
      max=gDirectory->GetListOfKeys()->GetEntries();
      for (int iii=0 ; iii<max ; iii++ ){
	TString sa1=gDirectory->GetListOfKeys()->At(iii)->GetName();
	  gDirectory->GetObject( sa1.Data() , o );
	  TString sa2=o->ClassName();
	  // important check - else it makes double entries...
	  if ((sa2.Index("TCutG")==0)&&(gROOT->GetListOfSpecials()->FindObject(o)==NULL)) {
	    //	  if ((sa2.Index("TCutG")==0)&&(gDirectory->FindObject(o)==NULL)) {
	    //	    gDirectory->Add( (TH1F*)o );
	    gROOT->GetListOfSpecials()->Add( (TCutG*)o );
	  }// TCutG
      }
  }//gDirectory->GetListOfKeys()

  nf->ls();
  nf->Close();
  dir->cd();
  gROOT->GetListOfSpecials()->ls();
}//cutload--------------
/*  remove   rename.............
 * gDirectory->rmdir("cutt7d") !!!
 *  cutt7p->Clone("cutt7pV");cutt7pV->Write()
 */






void cutrm(const char* name, int version=0){
  if (version==0){
    printf("make a backup and use the version number  ;1 ;2%s\n","");
    return;
  }
  TDirectory *dir=gDirectory;
    TFile *nf=new TFile(CUTFILE_GLOBAL, "UPDATE");
    char name2[100];
    sprintf( name2 , "%s;%d", name, version );
    printf("deleting  %s\n", name2  );
    gDirectory->rmdir( name2 );
    nf->ls();
    nf->Close();
  dir->cd();
}


void cutcp(const char* name, const char* newname){
  TDirectory *dir=gDirectory;

  TFile *nf=new TFile(CUTFILE_GLOBAL, "UPDATE");
  TObject *o;
  gDirectory->GetObject( name , o );
  TCutG* cut=(TCutG*)o;
  TCutG* newcut=(TCutG*)cut->Clone( newname );
  newcut->Write();
    //  newcut->Print();  newcut->Draw("pawl");
  nf->ls();
  nf->Close();
  dir->cd();

}


void cutls(){
  TDirectory *dir=gDirectory;

  TFile *nf=new TFile(CUTFILE_GLOBAL, "");
  nf->ls();
  nf->Close();
  dir->cd();

}





//===========================================================
//               save obj to file 
//==============================================================




void saveobj2file(char *filenam,  TH1* xobj, int bkup=0){
  //  printf("obj 2 file ###########\n",1);
  TDirectory *oldir=gDirectory;
  TString s,nn;
  s.Append(filenam);
  nn.Append( xobj->GetName() );
  if( (bkup==1)&& (nn.Index("_")!=nn.Length()-1)){  nn.Append( "_" ); }
  printf("obj 2 file:   file=%s  ,  newname=%s\n", s.Data() , nn.Data() );
  //  printf("obj 2 file:   filenam=%s,   s=%s\n", filenam, s.Data());
  TH1 *htm=(TH1*)xobj->Clone( nn.Data() );
  htm->SetDirectory(0);//rm from the directory.see http://root.cern.ch/root/html/TH1.html
  if (s.Length()>0){
  TFile *f=new TFile(s.Data(),"update");
  if (f==NULL){
    printf("manualy:\n TFile *fxxx=new TFile( \"%s\", \"recreate\") \n", filenam);
    printf("      ->Write() \n");
    return;
  }
  htm->Write();
  printf("   %s ...written\n", htm->GetName() );
  //  f->ls();
  f->Close();
  oldir->cd();
  return;
  }
}// saveobj2file















//######################################################################
//     savecanvas
//######################################################################
void savecanvas(const char *filenam, int slot=0){

  //  FillWhite();

  TString s;
  s.Append(filenam);
  //  printf("savecanvas: filenam=%s,   s=%s\n", filenam, s.Data());
  if (s.Length()>0){

    if ( s.Length()-s.Index(".root")==5){
      s.Replace( s.Index(".root")  ,  5 , "");
    }// remove    .root
    printf("Canvas name (.root removed) == %s\n", s.Data() );
    char appcha[15]; 
  TString s_e=s;
  TString s_p=s;
  TString s_r=s;
  TString s_png=s;
  if (slot==0){  //  SLOT==0 means   no  slot....
  sprintf( appcha,".%s", "eps" );   s_e.Append(  appcha );
  sprintf( appcha,".%s", "ps" );    s_p.Append(  appcha  );
  sprintf( appcha,".%s", "root" );  s_r.Append(  appcha  );
  sprintf( appcha,".%s", "jpg" );   s_png.Append(appcha );
  }else{
  sprintf( appcha,"_%d.%s", slot, "eps" );   s_e.Append(  appcha );
  sprintf( appcha,"_%d.%s", slot, "ps" );    s_p.Append(  appcha  );
  sprintf( appcha,"_%d.%s", slot, "root" );  s_r.Append(  appcha  );
  sprintf( appcha,"_%d.%s", slot, "jpg" );  s_png.Append(appcha );
  }
 
  if (gROOT->GetListOfCanvases()->At(0) ==NULL){printf("%d canvases\n",0);return;}
  TCanvas *cmy=gPad->GetCanvas();
  TPad *lowpad=NULL; float lowpadp=1e+3,lowpadtmp;// this will contain the lowest TPad

 //-print on the canvas:
 /* if (cmy->FindObject("ut_C_label")!=NULL){
    cmy->FindObject("ut_C_label")->Delete();
    }*/
  //--------------- First play with deleting all our previous TText
  //                and find a TPAD at bottom


  TObjArray *toa=new TObjArray(); // list to delete

  int imx,i,ii,iimx;  TString dupname;

  imx=cmy->GetListOfPrimitives()->GetEntries(); // TText IN TCanvas
  for (i=0;i<imx;i++){
    dupname=cmy->GetListOfPrimitives()->At(i)->GetName();
    if (dupname.CompareTo("ut_C_label")==0) {// smaz vsechny
	//	cmy->GetListOfPrimitives()->At(i)->Delete();
	//	i--;
	    printf("found utclab %d\n", 1 );
	    toa->Add( cmy->GetListOfPrimitives()->At(i) );
      }	
  }//for loop

  int num_pads=0;
  imx=cmy->GetListOfPrimitives()->GetEntries();// IN TPads inside....
  for (i=0;i<imx;i++){
	dupname=cmy->GetListOfPrimitives()->At(i)->ClassName();
        if (dupname.CompareTo("TPad")==0){ // YES - TPad!
	  num_pads++;
          TPad *tp=(TPad*)cmy->GetListOfPrimitives()->At(i);
	   lowpadtmp=tp->GetYlowNDC();
	   if (lowpadtmp<lowpadp){ lowpadp=lowpadtmp;lowpad=tp;  }//lowpad
	  printf("found tpad %d at low-pos:%7.4f (min=%7.4f)\n", i , lowpadtmp, lowpadp );
	  iimx=tp->GetListOfPrimitives()->GetEntries();
		printf(" %d primitives inside TPad\n", iimx );
	    for (ii=0;ii<iimx;ii++){
	      dupname=tp->GetListOfPrimitives()->At(ii)->GetName();
	      if (dupname.CompareTo("ut_C_label")==0){
		printf("found label inside TPad %d - adding to the list\n", ii );
                toa->Add( tp->GetListOfPrimitives()->At(ii) );
	      }// is ut_C_label
	    }//for loop	    
	  }// is TPad
  }//for loop
  int toaen=toa->GetEntries();
  printf("To delete %d TText object(s)\n The list follows:\n", toaen );
  toa->ls();
  for (int iu=toaen-1;iu>=0;iu--){
    printf("    %d - deleting from the list\n", iu );
    toa->At(iu)->Delete();
  }
  // now - all our TText  are deleted.....

  printf("Deleted  %d TText object(s)\n", toaen );




  //------------------------ now prepare text to PRINT on canvas
  //   cannot detect whether it is on top or hidden...:(
  if (lowpad!=NULL){  
    printf("Dive into TPad%s\n", "" );lowpad->cd(); 
  }else{ // switch to lowest TPad. else TCanvas
    printf("Remain in the Canvas%s\n",  "" ); 
  }
  if (num_pads==0){// zkusim prekreslit to do tpadu a tim to zachranit
    cmy->Divide(1,1);
    TPad*one=(TPad*)cmy->GetPad(1);
    one->cd();
    for (int j=0;j<cmy->GetListOfPrimitives()->GetEntries();j++){
 	if (  strstr(cmy->GetListOfPrimitives()->At(j)->ClassName(), "TH" )!=NULL ){
	       TH1*ooo=(TH1*) cmy->GetListOfPrimitives()->At(j)->Clone();
	  ooo->Draw();
	}//TH
   }//for

  }//numpads==0


  TString sla;
  TDatime *dt=new TDatime() ;  // kTRUE previously...mess
  sla.Append( dt->AsString() );//append date
  sla.Append( " : " );//append fname
  sla.Append( s_p.Data() );//append fname
  if (gFile!=NULL){
    sla.Append( " : " );//append openfile
    sla.Append( gFile->GetName()  );//append openfile
  }
  TText *tt=new TText(0.01,0.01,sla.Data());
  tt->SetName("ut_C_label");
  tt->SetNDC(kTRUE);
  tt->SetX(0.013);  tt->SetY(0.013);
  tt->SetTextSize(0.027);
  tt->SetTextColor(15); tt->SetTextFont(42);  
  tt->Draw();
  printf("Draw label: %s\n\n", sla.Data() ); 

  printf(".!  lpr      %s\n\n",   s_p.Data()   ); 
  printf(".!  lpr      %s\n\n",   s_e.Data()   ); 
  printf(".!  gqview   %s&\n\n",  s_png.Data()   ); 


  //  change canvas name !  to let it open later without destroying new...
  TString canna,cannaori;
  cannaori.Append( cmy->GetName() ); //keep original name
  canna.Append( cmy->GetName() );
  s.Append("_save");
  canna.Append(s.Data() );  // not filenam .... but  s.Data() to avoid  .root
  cmy->SetName( canna.Data() );
 
  cmy->SaveAs(s_e);
  cmy->SaveAs(s_p);
  cmy->SaveAs(s_r);
  cmy->SaveAs(s_png);
  cmy->SetName( cannaori.Data() ); // put the original name
  }else{printf("use: savecanvas(\"filename_without_extension\")\n");}  

  printf("leaving savecanvas%s\n", "" );

}//  SAVECANCAS -----------------------------------------------------



















//######################################################################
//    Desktop setup    savecanvas  load configuration from a saved canvas
//######################################################################
int  Desktop_setup_savecanvas(const char *filenam){
  TFile *oldfile=gFile;
  if (oldfile!=NULL){
    printf("Veeeerrry  tricky. You must be careful with two files%s\n","");
  }


  TDirectory *oldir=gDirectory;

  if (gROOT->GetListOfCanvases()->At(0) ==NULL){printf("%d canvases\n",0);return 0;}
  TCanvas *cmy=gPad->GetCanvas();
  cmy->Clear();
  printf("CMY %ld\n", (int64_t)cmy);
  //  FillWhite();
  TObject *o;
  /*
   *  OPEN file and get all pads and canvases
   */
  int filexists=gSystem->AccessPathName( filenam ); // FILE EXISTS   TEST
  printf("DEBUG %s == %d\n",filenam,  filexists );

  if (filexists==0){// file for desktop setup canvas exists
  TFile *f=new TFile(filenam);
  gDirectory->GetObject(  gDirectory->GetListOfKeys()->At(0)->GetName() ,o);
  // 1st name is usually----------------------------------------------------
  // -  tcanvas
  // -  tpad
  if ( strcmp("TCanvas",o->ClassName()) ==0){
    cmy->cd();
  printf("CMY %ld\n", (int64_t)cmy);
    TPad *pads;
    TCanvas *cizi = (TCanvas*)o;
    int imax=cizi->GetListOfPrimitives()->GetEntries();
    printf("%d entries found(Desktop_setup_savecanvas)\n", imax  );
    int pad_exnum=0;
    for (int i=0;i<imax;i++){
      printf("Primitive %d has %s class\n", i,cizi->GetListOfPrimitives()->At(i)->ClassName() );
      //when it is TPad...create clone and  draw it.........
      if (strcmp("TPad",cizi->GetListOfPrimitives()->At(i)->ClassName())==0){
	printf("  ...drawing Primitive %d %s\n", i,cizi->GetListOfPrimitives()->At(i)->GetName() );
	pad_exnum++;
	pads=(TPad*) cizi->GetListOfPrimitives()->At(i)->Clone();
	// here I draw the clone of the original  TPAD
	pads->Draw();
	//and now it would be good to place an analogic histogram
	//
	// cizi->GetListOfPrimitives()->At(i)->Draw() ;
      }// is TPad

    }// all  entries found... i=0;i<imax;i++    

    //this doesnot  help...    
    if (pad_exnum==0){ 
      printf("NO TPADS .... you should have  used 26_SaveCanv\n%s" , "");
      /*
      TCanvas *ccizi=(TCanvas*)cizi->GetCanvas();
      ccizi->Divide(1,1);
      ccizi->cd(1);
      TPad *cizip=(TPad*)gPad;
      for (int j=0;j<cizi->GetListOfPrimitives()->GetEntries();j++){
	printf("NO Tpad: trying %s\n",cizi->GetListOfPrimitives()->At(j)->ClassName() );
	if (strstr(cizi->GetListOfPrimitives()->At(j)->ClassName(),"TH")>0){
	printf("NO Tpad: DRAW! %s\n",cizi->GetListOfPrimitives()->At(j)->ClassName() );
	TH1*ooo=(TH1*) cizi->GetListOfPrimitives()->At(j)->Clone();
	ooo->Draw();
	//	cmy->Modified();cmy->Update();  
	}
      }
    cmy->cd();
    cizip->Draw();
      */
    }//no tpads...


     if (oldfile!=NULL){
       //         printf("Veeeerrry  . You must be careful with two files%s\n","");
         printf(" ..... saving oldfile ...be careful with two files%s\n","");
	 oldfile->cd();
     }
       f->Close();
       cmy->Modified();cmy->Update();  
       //  this also works when there are 2 files 
       oldir->cd();
  printf("CMY %ld\n", (int64_t)cmy);
  
       /*
	*
	*/
    //------------------------- in this moment - former TPads are cleanly loaded with the original histos
    
    imax=cmy->GetListOfPrimitives()->GetEntries();
    printf("%d PAD entries found in local canvas ... parsing all...\n", imax  );
    int icizi; //  if there is TFrame at 1st place, we must split i and icizi
    for (int i=0;i<imax;i++){
      icizi=i;
      printf("%02d) cmy has %s class // cizi has %s class\n", i,
	     cmy->GetListOfPrimitives()->At(i)->ClassName(),
	     cizi->GetListOfPrimitives()->At(icizi)->ClassName() );

      //one problem. if there is only one TPad, the first can be TFrame.... i==0 here, we try:
      if (strcmp("TFrame",cizi->GetListOfPrimitives()->At(icizi)->ClassName())==0){
	for (icizi=0;icizi<cizi->GetListOfPrimitives()->GetEntries();icizi++){
	  if (strcmp("TPad",cizi->GetListOfPrimitives()->At(icizi)->ClassName())==0){break;}
	}// find icizi
      }//if it is tframe ... i==0 here


      if (strcmp("TPad",cizi->GetListOfPrimitives()->At(icizi)->ClassName())==0){
        TString padname = cizi->GetListOfPrimitives()->At(icizi)->GetName();

	printf("  ... My TPad primitive #%02d has name %s\n", i, padname.Data()  );
	// browse inside cizi to find the histos
	TPad *p=(TPad*)cizi->GetListOfPrimitives()->At(icizi);
	int jmax=p->GetListOfPrimitives()->GetEntries();
	for (int ii=0;ii<jmax;ii++){
	  // printf("  ...      My SubPrimitive %d %s class=%s\n", 
	  //	 ii,p->GetListOfPrimitives()->At(ii)->GetName(),p->GetListOfPrimitives()->At(ii)->ClassName() );
	  if (  (strcmp("TH1F",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
 (strcmp("TH2F",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
 (strcmp("TH1D",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
 (strcmp("TH2D",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
 (strcmp("TGraph",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
 (strcmp("TGraphErrors",p->GetListOfPrimitives()->At(ii)->ClassName())==0) 
		){
	    printf("     ... class THxx ok, original histo=%s #%02d ",p->GetListOfPrimitives()->At(ii)->GetName() ,i+1 );
	    // find this object at my gDirectory....! and in files
	    //   name;cycle
	    // Get - look into memory only (if not  ;123)
	    //	      TObject *dro=gDirectory->FindObject( p->GetListOfPrimitives()->At(ii)->GetName() );
	      TObject *dro=gDirectory->Get( p->GetListOfPrimitives()->At(ii)->GetName() );
	      int classify=0;
	      if (dro!=NULL){  classify=1;} // TH12
	      if (
		  (strcmp("TGraph",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ||
		  (strcmp("TGraphErrors",p->GetListOfPrimitives()->At(ii)->ClassName())==0) 
		  ){
		dro=gROOT->GetListOfSpecials()->FindObject( p->GetListOfPrimitives()->At(ii)->GetName() );
		if (dro!=NULL){  classify=2;} // GRAPH
	      }
	      // cd  into the proper pad
	      //cmy->cd(i+1);
	      TPad *p2=(TPad*)cmy->FindObject( padname.Data()  ); 
	      if (p2!=NULL){ 
		printf("TPad %s found.\n",padname.Data());p2->cd();
	      }else{
		printf("TPad named %s not found guess cd.\n",padname.Data());  cmy->cd(i+1);
	      }//p2
	      /*
	       *  draw the found object:
	       */
	      if (classify==2 ){
		printf("graph found localy. ok. %s found localy as %ld. ok.\n",  
		       p->GetListOfPrimitives()->At(ii)->GetName(),
		       (int64_t)gROOT->GetListOfSpecials()->FindObject(p->GetListOfPrimitives()->At(ii)->GetName() ) ); 
		TGraphErrors *obh=(TGraphErrors*)gROOT->GetListOfSpecials()->FindObject(p->GetListOfPrimitives()->At(ii)->GetName() );
		//		TGraphErrors *dro2=(TGraphErrors*)dro;
		obh->SetTitle( p->GetListOfPrimitives()->At(ii)->GetName() ); // We put the title == graph name
		obh->Draw("pawl");
       		dro=NULL;
	      }

	      if ( classify==1 ){   
		printf("histo found localy. ok. %s found localy. ok.\n",  p->GetListOfPrimitives()->At(ii)->GetName() ); 
		/*
		 * // replicate ZOOM
		 */
		double x1,x2,y1,y2;
		int    ix1,ix2,iy1,iy2;
		//	if (  (strcmp("TH1",p->GetListOfPrimitives()->At(ii)->ClassName())==0) ){
		TH1 *obh=(TH1*)p->GetListOfPrimitives()->At(ii);
		TH1 *dro2=(TH1*)dro;
		printf("preparing to get X zooms :  class %s\n", p->GetListOfPrimitives()->At(ii)->ClassName()   );
		//		x1=obh->GetXaxis()->GetXmin();
		//		x2=obh->GetXaxis()->GetXmax();
		ix1=obh->GetXaxis()->GetFirst(); //bin (1)
		ix2=obh->GetXaxis()->GetLast();  //bin 8192

		x1=obh->GetXaxis()->GetBinLowEdge( ix1 ); //bin (1)
		x2=obh->GetXaxis()->GetBinUpEdge(  ix2 );  //bin 8192
		//		y1=obh->GetYaxis()->GetXmin();
		//		y2=obh->GetYaxis()->GetXmax();
		//		dro2->GetYaxis()->SetRangeUser( y1,y2 );
		if ( (strcmp("TH2F",p->GetListOfPrimitives()->At(ii)->ClassName())==0) || 
		     (strcmp("TH2D",p->GetListOfPrimitives()->At(ii)->ClassName())==0)   ){
		  printf("2D treatment\n");
		  iy1=obh->GetYaxis()->GetFirst(); //bin (1)
		  iy2=obh->GetYaxis()->GetLast();  //bin 8192

		  y1=obh->GetYaxis()->GetBinLowEdge( iy1 ); //bin (1)
		  y2=obh->GetYaxis()->GetBinUpEdge(  iy2 );  //bin 8192

		  printf("2D zoom apply :  X %f %f,   Y %f %f\n",  x1,x2,   y1,y2 );
		  dro2->GetXaxis()->SetRangeUser( x1,x2 );
		  dro2->GetYaxis()->SetRangeUser( y1,y2 );
		  
		  dro2->Draw("col");
		  dro2->GetXaxis()->SetRangeUser( x1,x2 );
		  dro2->GetYaxis()->SetRangeUser( y1,y2 );
		}else{
		  printf("1D treatment\n");
		  printf("1D zoom apply :  X %f %f,   Y %f %f\n",  x1,x2,   y1,y2 );
		  dro2->GetXaxis()->SetRangeUser( x1,x2 );
		  dro2->Draw();
		  dro2->GetXaxis()->SetRangeUser( x1,x2 );
		}
		//	}// zooms for th1f  only
		//	else{ 		
		  //dro->Draw();   
		//	}
		// 
	      }
	      if ( classify==0 ){   
		printf("histo not found localy:  %s not found localy: clear\n", p->GetListOfPrimitives()->At(ii)->GetName()  );gPad->Clear();
	      }
	  }// if class TH12fd
	}// for primit inside tpad
      }// is TPad

    }// for  all  entries  in local canvas
  

  }else{ // TCanvas classname ......
    printf("Not Canvas..sorry%s\n","");
    f->Close();
   oldir->cd();
   return 1; //?
  }//canvas/not canvas


  oldir->cd();
  }//f!=NULL => file dava smysl.....
  else{
    return 1;// file canvas root doesnot exist
  }
  return 0; //ok, it exists
 
}//  Desktop setup  SAVECANCAS -----------------------------------------------------




















/*
 *  CONNECT THE MOUSE TO EVENT   .........   do it more transparent....
 */
void MyMainFrame::exec3event(Int_t event, Int_t x, Int_t y, TObject *selected)
{
  //UNUSED   TCanvas *c = (TCanvas *) gTQSender;
   //dont react on hover, pull.......
   /*   1 ...left down
    *  11    left up
    *  2,12  middle donw, up
    *  3     right down
    *
    *  1,11,61,11 == doubleclick left
    *  2,12,62,12 == doubleclick middle
    *  3,63,13 == doubleclick right
    */
   if ( (event!=51)&&(event!=52)&&(event!=53)&&(event!=21)   ){  //  21=pull
     //          printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", 
     //		 c->GetName(),  event, x, y, selected->IsA()->GetName() );
   TString ss= selected->IsA()->GetName();


   if (event==11){
     //  printf("left click (show energy)%s\n", "");
   }// event 11

   if (ss.CompareTo("TGraphErrors")==0){//==============ONLY ON TGraph  MARKS=============

     TGraphErrors *g=(TGraphErrors*)selected;//  g->Print();
     if (strcmp(g->GetName(),"MARKS")==0){//=====ONLY MARKS =======================


     if ((event==11)&&(g->GetN()==2)){  // LEFTCLICK - SUM DATA
       printf("SUMMING THE REGION OF MARKS \n");
     }//11---------LEFT CLICK 



     if (event==12){  // middle click => remove the point    (if TGraph) ******  MIDDLE CLICK REMOVE
       Double_t xp  = gPad->PadtoX(gPad->AbsPixeltoX(x));
       Double_t yp  = gPad->PadtoY(gPad->AbsPixeltoY(y));
       int npoints=g->GetN();  int closest=0; 
       double clo=sqrt( pow(xp-g->GetX()[0],2)+pow(yp-g->GetY()[0],2) );
       double clomax=clo; int i;
       for  (i=0;i<npoints;i++){
	 clo=sqrt( pow(xp-g->GetX()[i],2)+pow(yp-g->GetY()[i],2) );
	 if (clo<clomax) { clomax=clo; closest=i;}
       }
       printf("removing point #%d  near: %f : %f\n", closest, xp,yp );
       //       if (closest==0){ g-}
       if (g->GetN()>1){ 
	 g->RemovePoint( closest ); 
	 g->Sort(); // WE MUST SORT HERE but not when 2D!!
       }else{ 
	 g->Delete();
       }
       
       RefreshAll();
     }//12----------MIDDLECLICK

     //----- no tgraph can be after.....
     }//it is MARKS   tgraph
   }//it is TGraph--------------===============================================

   else if(gPad->FindObject("MARKS")!=NULL ){   // *******************************  MIDDLE CLICK INSERT
   /*
    *   IF NOT on TGraph... but MARKS still exist
    */  // normaly  11
     if ((event==12)&&(  
        (strcmp(selected->IsA()->GetName(),"TFrame")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH1F")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH1D")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH2F")==0 )
	 )
	 ){  // LEFT CLICK - SUM DATA
       TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
       printf("ADDING TO MARKS, now GetN() will = %d \n",
          g->GetN()+1 );
       //       Double_t xp  = gPad->PadtoX(gPad->AbsPixeltoX(x));
       //       Double_t yp  = gPad->PadtoY(gPad->AbsPixeltoY(y));
       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma
       /*
       TString *fentry=new TString( fEntry->GetText() );  // DEPRECIATED:---------
       if ( fentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(fentry->Data() )!=0){// match
	   defaultsigma=atoi( fentry->Data() );
	 }// is a number
       }//fentry  exists---------------possibility to change defaultsigma
       */
       TString *fentry=new TString( fEntrySIG->GetText() );  // DEPRECIATED:---------
       if ( fentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(fentry->Data() )!=0){// match
	   defaultsigma=atoi( fentry->Data() );
	 }// is a number
       }//fentry  exists---------------possibility to change defaultsigma

       //g->Print();
       g->InsertPoint();
       //g->Print();
       g->GetEX()[ g->GetN()-1 ] = defaultsigma;
       g->GetEY()[ g->GetN()-1 ] = 0.0;
       g->Draw("PL");
       //       g->Sort(&TGraph::CompareRadius); // Sort will be extra
       //  g->Sort();

       //=================listpoints
       for (int i=0; i<g->GetN(); i++){
	 printf("%2d   %7.3f  %7.3f\n",
		i,
		g->GetX()[i],
		g->GetY()[i]
		);
       }
       if (g->GetN()==2){
	  printf("&& (sum>%7.3f) && (sum<%7.3f)    %7.3f  %7.3f \n",
		 g->GetX()[0],g->GetX()[1],
		 g->GetY()[0],g->GetY()[1]
		);
       }
       //=====================list
       RefreshAll();

     }//11---------LEFT CLICK 
   }//ELIF -  NOT TGRAPH and    "MARKS on gPad exists"
   else if(gPad->FindObject("MARKS")==NULL){ // **************************** MIDDLE CLICK STARTUP
      if ((event==12)&&(
        (strcmp(selected->IsA()->GetName(),"TFrame")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH1F")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH1D")==0 )||
        (strcmp(selected->IsA()->GetName(),"TH2F")==0 )
	 )
	 ){  // MIDDLE CLICK - 
	//	fSELSetMarks; 


       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *fentry=new TString( fEntrySIG->GetText() );
      if ( fentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(fentry->Data() )!=0){// match
	   defaultsigma=atoi( fentry->Data() );
	 }// not a number
       }//fentry  exists---------------possibility to change defaultsigma


	double xw[1],yw[1],xwe[1],ywe[1]; 
	xw[0]=gPad->PadtoX(gPad->AbsPixeltoX(x)); 
	yw[0]=gPad->PadtoY(gPad->AbsPixeltoY(y));
	xwe[0]=defaultsigma;  // defined in class, assigned in constructor....
	  ywe[0]=0.0;
        TGraphErrors *m=new TGraphErrors(1,xw,yw,xwe,ywe);m->SetName("MARKS");
	m->SetMarkerStyle(2); m->SetMarkerSize(4);
	m->SetMarkerColor(2); m->SetLineStyle(2); m->SetLineColor(2); m->Draw("PL");  
       RefreshAll();
      }// MIDDLE CLICK
   }// ELIF -  NOT TGRAPH and    "MARKS on gPad NOT exist"

   }// more THEN just mouse hover....
}//  execute event..........









void  MyMainFrame::FillMainMenu(){
   char tmp[40];

   //NOT HERE....... fListBoxOF->AddEntry("testik", 1);

     if (PAGE==1){ 
   sprintf(tmp, "%i_***Next/Page***", SELNextPage);  fListBox->AddEntry(tmp, SELNextPage);
   sprintf(tmp, "%i_SetMarks", SELSetMarks);  fListBox->AddEntry(tmp, SELSetMarks);
   sprintf(tmp, "%i_SortMarks/GCut", SELGetMarks);  fListBox->AddEntry(tmp, SELGetMarks);
   sprintf(tmp, "%i_DelMarks", SELDelMarks);  fListBox->AddEntry(tmp, SELDelMarks);

   sprintf(tmp, "%i----------",SELbar1 );     fListBox->AddEntry(tmp, SELbar1);
   sprintf(tmp, "%i_FindPeaks", SELFindPks);  fListBox->AddEntry(tmp, SELFindPks);
   sprintf(tmp, "%i_FindBg", SELFindBg);      fListBox->AddEntry(tmp, SELFindBg);
   sprintf(tmp, "%i_DelPeaks", SELDelPks);    fListBox->AddEntry(tmp, SELDelPks);

   //   sprintf(tmp, "%i_FitGaus", SELFitGaus);    fListBox->AddEntry(tmp, SELFitGaus);
  
   sprintf(tmp, "%i----------",SELbar2 );     fListBox->AddEntry(tmp, SELbar2);
   sprintf(tmp, "%i_StartFIT", SELFBX);       fListBox->AddEntry(tmp, SELFBX );
   sprintf(tmp, "%i_UpdateMARKS", SELUpdate);      fListBox->AddEntry(tmp, SELUpdate);
   sprintf(tmp, "%i_SaveFit", SELComSig);      fListBox->AddEntry(tmp, SELComSig);
   sprintf(tmp, "%i_", SELFit);            fListBox->AddEntry(tmp, SELFit);
   sprintf(tmp, "%i_HelpFit", SELDelFBX);         fListBox->AddEntry(tmp, SELDelFBX);
   sprintf(tmp, "%i----------",SELbar3a );      fListBox->AddEntry(tmp, SELbar3a);

   sprintf(tmp, "%i_Clone2Rint", SELClone2Rint);         fListBox->AddEntry(tmp, SELClone2Rint);
   }// PAGE==1

   if (PAGE==2){
   sprintf(tmp, "%i_***Next/Page***", SELNextPage);fListBox->AddEntry(tmp, 1);
   // sprintf(tmp, "%i----------",SELbar3 );       fListBox->AddEntry(tmp,     2);
   sprintf(tmp, "DateTime,Calib"); fListBox->AddEntry(tmp,   SELDateTime  -SELGrid+2);
   sprintf(tmp, "Grid");               fListBox->AddEntry(tmp,   SELGrid      -SELGrid+2);
   sprintf(tmp, "Logy");               fListBox->AddEntry(tmp,   SELLogy      -SELGrid+2);
   sprintf(tmp, "Logz");               fListBox->AddEntry(tmp,   SELLogz      -SELGrid+2);
   //   sprintf(tmp, "%i_Div,LoadCanvas", SELDivide);   fListBox->AddEntry(tmp,   SELDivide -SELGrid+2);
   //   sprintf(tmp, "%i_LoadCanvas", SELDivide);   fListBox->AddEntry(tmp,   SELDivide -SELGrid+2);
   sprintf(tmp, "----------",SELbar4 );     fListBox->AddEntry(tmp,   SELbar4      -SELGrid+2  );

   sprintf(tmp, "LoadCanvas  " );    fListBox->AddEntry(tmp,   SELDivide    -SELGrid+2  );
   sprintf(tmp, "SaveCanvas  " );    fListBox->AddEntry(tmp,   SELSaveCanvas -SELGrid+2  );

   sprintf(tmp, "SaveAllSpectra  " );   fListBox->AddEntry(tmp,   SELSaveSpectra-SELGrid+2  );

   sprintf(tmp, "DivCanvas,RangeAll");fListBox->AddEntry(tmp,   SELDivCanv-SELGrid+2  );
   sprintf(tmp, "Unzoom  " );         fListBox->AddEntry(tmp,   SELUnzoom    -SELGrid+2  );
   //   sprintf(tmp, "%i----------",SELbar5 );          fListBox->AddEntry(tmp,   SELbar5      -SELGrid+2  );
   sprintf(tmp, "------Spectrum2Memory" );  fListBox->AddEntry(tmp, SELClone2Rint2-SELGrid+2 );

   sprintf(tmp, "RefreshAll  " );    fListBox->AddEntry(tmp,   SELRefresh   -SELGrid+2  );
   sprintf(tmp, "----------", SELbar5  );   fListBox->AddEntry(tmp,   SELbar5      -SELGrid+2  );
   sprintf(tmp, "Clear    " );         fListBox->AddEntry(tmp,   SELClear     -SELGrid+2  );
   sprintf(tmp, "ClearAll " );      fListBox->AddEntry(tmp,   SELClearAll  -SELGrid+2  );



   // sprintf(tmp, "%i_ZoomAll  "  ,SELZoomAll );  fListBox->AddEntry(tmp,   SELZoomAll-SELGrid+2  );

   } // PAGE2

   //   fListBox->Resize(100,270); // 2.cislo = vyska 
   //  240 vyska
   fListBox->Resize(100,listbox_vsize); // 2.cislo = vyska 
   //not def here   fListBox2->Resize(100,listbox2_vsize); // 2.cislo = vyska 

   Layout();
}// FillMainMenu  ...  fills main menu pages.












/******************************************************************
******************************************************************
******************************************************************
******************************************************************
 *             CONSTRUCTOR 
 *******************************************************************
 */




MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h, int page) :
   TGMainFrame(p, w, h)
{
  //   defaultsigma=5; ///   ???   FIRST GUESS TO SIGMA   Tbroomfit uses this 

  fit=0;          ///   there is a test ==NULL nad delete after...
  flistbox_selected=-1;
  // triple clikc

  time_t curtime;  struct tm *loctime;  
  curtime = time (NULL);  loctime = localtime (&curtime);
  lastcmd=0; lastlastcmd=0; 
  flistbox_selected_when=0;
  lastcmdtime=(int)curtime; lastlastcmdtime=(int)curtime;
  printf("linux time == %d\n",  (int)curtime);
  time_start=curtime;  // time when shsp was started
  //  time_block=3600;     // basic time block for TH2F's - to increase...
  time_block=600;    // basic time block for TH2F's - to increase...


  //  delay_refresh=1500;
  //  delay_clear=5000;

  PAGE=1; // init page
  PAGE=2; // init page
  PAGE=page;

  nfileentr=0;
  primar_sigma=10.0;
  MarksOn=0; // marks are not ON
  fancy_rot_flag=0;  // rotation of different fancy things (logz, 
  fancy_grid=0;fancy_logy=0;fancy_logz=0;
  divide_mod_flag=0;

  gStyle->SetPalette(1);

  //morhac find peaks....
  spe=NULL;
  bg=NULL;
  bgf=NULL;

   // Create main frame####################################################

  /*
   *  listbox1   ....    MENU
   *
   */



    TGHorizontalFrame *hBframe = new TGHorizontalFrame(this);
    //NONO hBframe->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "MyMainFrame", this,
    //NONO               "exec3event(Int_t,Int_t,Int_t,TObject*)");
 

  fListBox = new TGListBox(this, 100);// TGWindow, id=89
   fSelected = new TList;

  fListBox2 = new TGListBox(hBframe, 101);// TGWindow, id
   fSelected2 = new TList;

  fListBoxOF = new TGListBox(hBframe, 122);// TGWindow, id=89
  //   fSelectedOF = new TList;

   FillMainMenu();
   //   fListBoxOF->Clear();
   fListBoxOF->RemoveEntries( 0, fListBoxOF->GetNumberOfEntries() ); // correct way...?

   ////  (fListBoxOF  nema expandX
    hBframe->AddFrame(fListBox2 ,  new TGLayoutHints(kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    hBframe->AddFrame(fListBoxOF,  new TGLayoutHints(kLHintsRight                 | kLHintsExpandY, 0, 0, 0, 0));
    //    hBframe->AddFrame(fListBox2 ,  new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 0, 0, 0, 0));
    //    hBframe->AddFrame(fListBoxOF,  new TGLayoutHints(kLHintsRight                 , 0, 0, 0, 0));

    // WORKS
    fListBox->Connect("Selected(Int_t, Int_t)", "MyMainFrame", this, "ClickResponse(Int_t, Int_t)");
    //
    //  copied from exec3event

    //  | kFixedHeight   makes it all stiff...
    AddFrame(fListBox, new TGLayoutHints(kLHintsLeft |
					 kLHintsExpandX, 
                                            5, 5, 5, 5));
    // |  kLHintsExpandY

    fListBoxOF->Connect("Selected(Int_t, Int_t)", "MyMainFrame", this, "ClickResponse(Int_t, Int_t)");
    //   AddFrame(fListBoxOF, new TGLayoutHints(kLHintsTop | kLHintsLeft |
    //                                        kLHintsExpandX | kLHintsExpandY, 
    //                                        5, 5, 5, 5));

    AddFrame(hBframe, new TGLayoutHints( kLHintsExpandX |  kLHintsExpandY , 5, 5, 0, 5));

    //    hBframe->GetContainer()->Connect("DoubleClicked(TGFrame*, Int_t)", "MyMainFrame",
    //			   this, "ClickResponseDC(TGFrame*, Int_t)");

   //--------------------------------------------------------------dalsi flistbox

  /*
   *  listbox2   ....    filey
   *
   */

    //  fListBox2 = new TGListBox(this, 101);// TGWindow, id
    //   fSelected2 = new TList;

   //  fListBox2->RemoveEntries( 0, fListBox2->GetNumberOfEntries-1 );

      fListBox2->AddEntry("***Open File***", 1);
      //     for (int  ii=2;ii<=19;ii++){ sprintf(tmp, "%d", ii  ); fListBox2->AddEntry(tmp, ii); }
      nfileentr=0; // zero histos in file

      fListBox2->Resize(100 , listbox2_vsize );

      fListBoxOF->Resize(1 , listbox2_vsize );  // BY DEFAULT     it is very small

      fListBox2->Connect("Selected(Int_t, Int_t)", "MyMainFrame", this, "ClickResponse(Int_t, Int_t)");

      fListBox2->Connect("DoubleClicked(Int_t)", "MyMainFrame", this, 
     		      "ClickResponse(Int_t)");

      /*
      // THOS DOESNOT WORK - UNFORTUNATELY......... 
      //http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=10972
      //
      printf("box2 container: %d <%s>\n", fListBox2->GetContainer(),  fListBox2->GetContainer()->ClassName()  );
      fListBox2->GetContainer()->Connect("DoubleClicked(TGFrame*, Int_t)", "MyMainFrame",
			   this, "ClickResponseDC(TGFrame*, Int_t)");
      */


    //   AddFrame(fListBox2, new TGLayoutHints(kLHintsTop | kLHintsLeft |
    //                                        kLHintsExpandX , 
    //					 5, 5, 5, 5));// padding, neexpanduj Y        

                              


   TGHorizontalFrame *hframe = new TGHorizontalFrame(this, 150, 20, kFixedWidth);

   //textentry
   fEntry = new TGTextEntry( hframe, ""  );
   hframe->AddFrame(fEntry,  new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));

   //---- finalize HFRAME
AddFrame(hframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
 
//========== new hframe_a   .... center?
   TGHorizontalFrame *hframe_a = new TGHorizontalFrame(this, 150, 20, kFixedWidth);

   //textentry  SIGMA
   defaultsigma=5.0; ///   ???   FIRST GUESS TO SIGMA   Tbroomfit uses this 
   char defaultsigmachar[10];
   sprintf(defaultsigmachar,"%f",defaultsigma );
   printf(" default sigma is %s  d=%f-------!!!!!1", defaultsigmachar,defaultsigma  );
   sprintf(defaultsigmachar,"5%s",""  ); // ????? WHY THIS ???
   fEntrySIG = new TGTextEntry( hframe_a, defaultsigmachar , 555  );// id nefunguje....
   fEntrySIG->SetToolTipText("sigma for gaussian fits");
   fEntrySIG->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe_a->AddFrame(fEntrySIG,  new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
 
   //button      W  up
   TGTextButton *show = new TGTextButton(hframe_a, "&W(up)",1);
   show->SetToolTipText("movement UP");
   show->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   //   hframe_a->AddFrame(show, new TGLayoutHints( kFixedWidth, 5, 5, 3, 4));
   hframe_a->AddFrame(show, new TGLayoutHints( kLHintsCenterX, 0,0,0,0));
   AddFrame(hframe_a, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));


   //   TGCheckButton *
     fChk1 = new TGCheckButton(hframe_a, "Multi", 71);
  fChk1->SetToolTipText("common moves");
  fChk1->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe_a->AddFrame(fChk1, new TGLayoutHints( kLHintsCenterX, 0,0,0,0));



   
//========== new hframe 
   // Create a horizontal frame containing button(s)
   TGHorizontalFrame *hframe2 = new TGHorizontalFrame(this, 150, 20, kFixedWidth);
   //button2
   TGTextButton *show2 = new TGTextButton(hframe2, "<<&A",2);
   show2->SetToolTipText("LEFT");
   show2->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe2->AddFrame(show2, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));
   //button3
   TGTextButton *show3 = new TGTextButton(hframe2, "&S(Dn)",3);
   show3->SetToolTipText("DOWN");
   show3->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe2->AddFrame(show3, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));
   //button4
   TGTextButton *show4 = new TGTextButton(hframe2, "&D>>",4);
   show4->SetToolTipText("RIGHT");
   show4->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe2->AddFrame(show4, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));
   //button5
  
   //finalize - adding ASD buttons to   object
AddFrame(hframe2, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));





//========== new hframe_zoom_up   .... center?
   TGHorizontalFrame *hframe_3a = new TGHorizontalFrame(this, 150, 20, kFixedWidth);

   //button      W  up
   TGTextButton *show9 = new TGTextButton(hframe_3a, "&E(UnzoomY)",5);
   show9->SetToolTipText("Unzoom Y");
   show9->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   //   hframe_a->AddFrame(show, new TGLayoutHints( kFixedWidth, 5, 5, 3, 4));
   hframe_3a->AddFrame(show9, new TGLayoutHints( kLHintsCenterX, 0,0,0,0));
   AddFrame(hframe_3a, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));


//========== new hframe          ZOOM UNZOOM
   // Create a horizontal frame containing button(s)
   TGHorizontalFrame *hframe3 = new TGHorizontalFrame(this, 150, 20, kFixedWidth);
   //button6
  TGTextButton *show6 = new TGTextButton(hframe3, "&ZooX",6);
   show6->SetToolTipText("ZOOM");
   show6->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe3->AddFrame(show6, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));

   //button6
     TGTextButton *show8 = new TGTextButton(hframe3, "ZoomY &C",8);
      show8->SetToolTipText("ZOOMY");
      show8->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
      hframe3->AddFrame(show8, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));

   //button7
  TGTextButton *show7 = new TGTextButton(hframe3, "UnZ&X",7);
   show7->SetToolTipText("Unzoom");
   show7->Connect("Pressed()", "MyMainFrame", this, "ClickResponse()");
   hframe3->AddFrame(show7, new TGLayoutHints(kLHintsExpandX , 5, 5, 3, 4));

  //finalize
  AddFrame(hframe3, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
  //--------------------------------------------------------------------

   // Set a name to the main frame   
   SetWindowName("List Box");  
   MapSubwindows();
   // Initialize the layout algorithm via Resize()
   
   //      fListBoxOF->Resize(40 ,listbox_vsize  );

   Resize(GetDefaultSize());
   // Map main frame
   MapWindow();   
   fListBox->Select(1);



  //===============   Prepare canvas =================
   int month;//   time_t curtime;  struct tm *loctime;  
                curtime = time (NULL);  loctime = localtime (&curtime);
		gRandom->SetSeed( 1.1* loctime->tm_sec*loctime->tm_min );
   month = int(gRandom->Uniform(22.) );
   switch(month){
     case 0:    MyCan=new TCanvas("A","A -  canvas"); break;
     case 1:    MyCan=new TCanvas("B","B -  canvas"); break;
     case 2:    MyCan=new TCanvas("C","C -  canvas"); break;
     case 3:    MyCan=new TCanvas("D","D -  canvas"); break;
     case 4:    MyCan=new TCanvas("E","E -  canvas"); break;
     case 5:    MyCan=new TCanvas("F","F -  canvas"); break;
     case 6:    MyCan=new TCanvas("G","G -  canvas"); break;
     case 7:    MyCan=new TCanvas("H","H -  canvas"); break;
     case 8:    MyCan=new TCanvas("I","I -  canvas"); break;
     case 9:    MyCan=new TCanvas("J","J -  canvas"); break;
     case 10:   MyCan=new TCanvas("K","K -  canvas"); break;
     case 11:   MyCan=new TCanvas("L","L -  canvas"); break;
     case 12:   MyCan=new TCanvas("M","M -  canvas"); break;
     case 13:   MyCan=new TCanvas("N","N -  canvas"); break;
     case 14:   MyCan=new TCanvas("O","O -  canvas"); break;
     case 15:   MyCan=new TCanvas("P","P -  canvas"); break;
     case 16:   MyCan=new TCanvas("Q","Q -  canvas"); break;
     case 17:   MyCan=new TCanvas("R","R -  canvas"); break;
     case 18:   MyCan=new TCanvas("S","S -  canvas"); break;
     case 19:   MyCan=new TCanvas("T","T -  canvas"); break;
     case 20:   MyCan=new TCanvas("U","U -  canvas"); break;
     case 21:   MyCan=new TCanvas("V","V -  canvas"); break;
     case 22:   MyCan=new TCanvas("W","W -  canvas"); break;
     case 23:   MyCan=new TCanvas("X","X -  canvas"); break;
     case 24:   MyCan=new TCanvas("Y","Y -  canvas"); break;
     case 25:   MyCan=new TCanvas("Z","Z -  canvas"); break;
   }//switch
   MyCan->Draw();
   MyCan->Update();
   //  This is from  exec3.C............
   GPAD=(TCanvas*)MyCan;
   /*
    *   NAMING ALSO THE CONTROL PANEL
    */
 TString sr=GPAD->GetName();
 TString sr2="";
 sr2.Append( sr.Data()  );
 SetWindowName( sr2.Data()   );   // CONTROL PANEL?
 sr.Append("-shspe-canvas");
 GPAD->SetTitle( sr.Data() );

 GPAD->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "MyMainFrame", this ,
               "exec3event(Int_t,Int_t,Int_t,TObject*)");


 //------------------------------------------------------
 // lets   first  openfile......
 TString *fentry=new TString( fEntry->GetText() );  // normaly ""
 fOpenFile(fentry,fListBox2,  atoi(fEntrySIG->GetText()) );    // 1st fopenfile ---<<<<<<<<< read current directory
 TString GPADTITLE= GPAD->GetTitle();
 GPADTITLE.Append("##"); 
 GPADTITLE.Append( gDirectory->GetName()  ); 
 GPAD->SetTitle( GPADTITLE.Data()  );
 //FUTURE::: a.Replace( a.Index("##")+2, a.Length()-a.Index("##")+2 , "safdasfDS" )
 printf("very first openfile done%s\n","");


 // I know that 0 is canvas;   1 is TPad
 GPAD->Divide( 1 , 1 ,0.002,0.002, 0);  
 GPAD->Modified();GPAD->Update();
 GPAD->cd(1);
 // maybe this was a problem with labels not redrawn.......
 // GPAD->GetPad(1)->SetFillColor( 0 );
 // GPAD->GetPad(1)->SetFillStyle( 2 );
 //-----------------------------------------------------------
	GPAD->GetPad(1)->SetBorderSize(1);// TEST TO HAVE BETTER MODE
	GPAD->GetPad(1)->SetBorderMode(1);// TEST TO HAVE BETTER MODE
	GPAD->GetPad(1)->Modified();// force the color
	GPAD->GetPad(1)->Update();// force the color printf("very first TPad created%s\n","");
}// TGMAINFRAME......................................






MyMainFrame::~MyMainFrame()
{
   // Clean up main frame...
   Cleanup();
   //   if (fSelected) {
   //      fSelected->Delete();
   //      delete fSelected;
   //   }
}






/*
void  MyMainFrame::TimerRefresh(){
    RefreshAll();
}//timer refresh

*/


MyMainFrame*  shspe(int page=2){
   // Popup the GUI...
  //   new MyMainFrame(0, 200, 200);
  //     MyMainFrame *m= new MyMainFrame(0,100,100);

  da.Set(1995,1,1,0,0,0);  
  gStyle->SetTimeOffset( da.Convert(kTRUE) );

  //  printf( "%s\n",  da.GetTimeOffset() );
  printf(" trying to start  shspe - mmframe %s\n","");
  //  mmframe= new MyMainFrame(0,120,180);  // TGWindow==0,  w,   h 
  mmframe= new MyMainFrame(0,mmframe_w,mmframe_h, page  );  // TGWindow==0,  w,   h 

  cutload();
  //  printf(" trying to add TH1%s\n","");

  //      mmframe->AddTH1();
      return mmframe;
}//shspe---------------------------


MyMainFrame*  shspe(const char* name, int page=1){
  //  TFile *f=new TFile( name ); //UNUSED VARIABLE
  new TFile( name );
  shspe( page );
  return NULL;
}



MyMainFrame*  shspe(const char* name, const char* histo, int page=1){
  //UNUSED VARIABLE  TFile *f=new TFile( name );
  new TFile( name );
  TH1 *h=(TH1*)gDirectory->Get( histo );
  //  fOpenFile(name ,fListBox2); 
  shspe(page);
  h->Draw();
  return NULL;
}





//#########################################################
//#########################################################
//
//   -    - - --       CONTROL MODULE  --  -- - - -  - - - 
//
//#########################################################
//#########################################################





/*****************************************************
 *
 *
 *      SUPPORT FUNCTIONS
 *             spiders
 *             movements...
 *
 */


	      /*
	       *
	       */            



/*
 * Movexy("Y", 0.18, 1.0 );    //     Movexy("X", -0.18, 1.0 );
 *
 *  if ( fChk1->GetState()==0 ){//   DETECTS  CHECKBOX  multi
 *     restrict == 1   means CURRENT TPAD  ONLY; UN-MULTIPLE
 *
 *  20120620: exclude type - e.g. TGraph will exclude all graphs
 *    //default args are in .h
 */
void  MyMainFrame::RecoverTH1fromGPAD(int &count,int64_t addr[], 
				      const char* onlyclass, int restrict, 
				      const char*exclude ){
  int countmax=MAXPRIMITIVES;  //was 26
  count=0;  addr[0]=0;
  TList *prim;
  prim=GPAD->GetListOfPrimitives();//evidently GPAD IS CURRENT PAD
  //NEEE-PODELA SE TO  if (restrict>0){  fChk1->SetState( kButtonEngaged ); } // NASILNICKY TO UDELAM RESTRICT
  // LOGIC IS DIFFERENT - Spider-----
  if ( (fChk1->GetState()==0)&&(restrict==0) ){// if not all
    //    printf("RecoverTH1fromGPAD  taking only from gPad\n");
    prim=gPad->GetListOfPrimitives();
  }

       // I CREATE <shadow> prim not to change gpad contents
       TList *primbar=new TList;
       for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
	 primbar->Add(prim->At(ii));
       }
       if ( (fChk1->GetState()==0)&&(restrict==0) ){// if not all
	 primbar->Add( gPad ); // TPad is added to the list when ONE only
       }
      //  primbar->ls();

       int primbarlast=primbar->LastIndex();
       // 
       // Copy all TPads into the list first...
       if ( fChk1->GetState()!=0 ){// checkstate ==0 =>all
	 //	 printf("RecoverTH1fromGPAD  taking ALL  GPAD - BUTTON STATE==%d\n",  (int)fChk1->GetState());
//--------------------
 for (int ii=0; ii<=primbarlast ;ii++ ){
      TString sn=primbar->At(ii)->ClassName();

      //TPAD
   if ( (sn.Index("TPad")==0)){
     TPad* tempad=(TPad*)primbar->At(ii);
     int lastiii=tempad->GetListOfPrimitives()->LastIndex();
     for (int iii=0;iii<lastiii;iii++){
       primbar->AddLast(  tempad->GetListOfPrimitives()->At(iii) );
     }
   }//TPad index...
   //primbar->ls();
 }// ii<last
 //--------------------------------
       }// checkstate ==0 =>all




 // OK
       // printf("INDEXES:  %d............................RecoverTH1fromGPAD\n",primbar->LastIndex());
 for (int ii=0; ii<=primbar->LastIndex() ;ii++ ){
   TString sn=primbar->At(ii)->ClassName();

   //    printf( "  refr:%2d/%2d (count=%3d/3%d) %s\n", ii,primbar->LastIndex(), count,countmax, sn.Data()  );
 
    //ALLOWED CLASSES !!!!!!!!!!
    if ( (sn.Index("TH1")==0)||(sn.Index("TH1F")==0)||(sn.Index("TH1D")==0)||(sn.Index("TH2F")==0)
       ||(sn.Index("TMultiGraph")==0)||(sn.Index("TGraph")==0)||(sn.Index("TGraphErrors")==0
       ||(sn.Index("TPad")==0)) ){  // there is TPad :< there 
      TH1 *tp=((TH1*)primbar->At(ii));

      if ( (sn.Index("TMultiGraph")==0)&&(sn.Index(exclude)!=0) ) {
        TMultiGraph* m=(TMultiGraph*)primbar->At(ii);
        tp=(TH1*)m->GetHistogram();
	if ( strstr(onlyclass,"TH")!=0){ sn="TH2"; } // I do the same trick as in TgraphErrors to get TH
	printf("     ...tmultigraph - histo ==%ld\n", (int64_t)tp );
      }//TMultiGraph
      if ( (sn.Index("TGraphErrors")==0)&&(sn.Index(exclude)!=0) ) {
        TGraphErrors* m=(TGraphErrors*)primbar->At(ii);
        tp=(TH1*)m->GetHistogram();
	if ( strstr(onlyclass,"TH")!=0){ sn="TH2"; }
	//printf("     ...tgrapherrors - histo ==%ld\n", (int64_t)tp );
      }//TMultiGraph
      else{
      if ( (sn.Index("TGraph")==0)&&(sn.Index(exclude)!=0)  ) {
        TGraph* m=(TGraph*)primbar->At(ii);
        tp=(TH1*)m->GetHistogram();
	if ( strstr(onlyclass,"TH")!=0){ sn="TH2"; }
	//printf("     ...tgraph - histo ==%ld\n", (int64_t)tp );
      }//TMultiGraph
      }//else - tgraph

      if (count<countmax){   
	if  (strlen(onlyclass)>0) {// I WANT ONLY ONE CLASS (like for the case of TCutg)
	  if ( sn.Index(onlyclass)==0) {
	    addr[count++]= (int64_t)tp;  
	    //	    printf("   /%s/ found, (%ld) added to the list :onlyclass %s\n",tp->GetName(), (int64_t)tp, sn.Data() );
	  }//OnlyClass
	  else{
	    //	    printf("   /%s/ found, (%ld) not added         :class=%s; demand=%s\n",tp->GetName(), (int64_t)tp, sn.Data(),onlyclass );
	  }
	}else{// onlyclass=="" from now
	  addr[count]= (int64_t)tp;  
	  //	  printf("   /%s/ found, (%ld) added to the list:allclasses[%d]\n", tp->GetName(), (int64_t)tp, count );
	  count++;
	}
      }else{// count is max
	printf("Increase the field above %d. I cannot add more primit.now.\n", count);
      }
    }//if TH1 or some other TH
 }// for   ii (prim  
 //delete primbar;
 //printf("         %d things were added to the list RecoverTH1fromGPAD. Return.\n",count );
}//RecoverTH1fromGPAD







void  MyMainFrame::RefreshAll(){ 
  GPAD->Modified();GPAD->Update();  

    TList *prim=GPAD->GetListOfPrimitives();
 for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
    TString sn=prim->At(ii)->ClassName();
    //   printf( "refr:%2d  %s\n", ii,  sn.Data()  );

    if ( sn.Index("TPad")==0 ){  // there is TPad :< there 
      TPad *tpod=(TPad*)prim->At(ii);
      //printf(" Refresh-found active gPad that is inside the GPAD %s\n" , "");
      for (int j=0;j<tpod->GetListOfPrimitives()->LastIndex();j++){
	/*	printf("--in tpad: %s\n",
	      tpod->GetListOfPrimitives()->At(j)->ClassName()
	      );*/
	if ( strcmp("TMultiGraph",tpod->GetListOfPrimitives()->At(j)->ClassName()) ==0){
	  printf("found tmultigraph in  sub tpad%s\n", "" );
      TString sn2=tpod->GetListOfPrimitives()->At(j)->GetName();
      sn2.ReplaceAll("_MG",".mysql");
 	char commandrm[200];
 	char grname[200];
	char grname2[200];  
	
	sprintf(grname,"%s.dat",  sn2.Data() );
	sprintf(commandrm,"sqmylite -r %s %d %d  >%s", 
		sn2.Data(), 0, atoi(fEntrySIG->GetText()),grname);
	system(commandrm);
	printf("%s\n", commandrm);
	sprintf(grname2,"%s.dat.cols",  sn2.Data() );
	sprintf(commandrm,"cat %s | head -1 | wc -w > %s ", 
		grname, grname2 );
	system(commandrm); // # columns
	printf("%s\n", commandrm);

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
	   if (outputCol>1){ joingraphsX(grname,res->GetTitle() );}
	 }// for all columns ------ of mysql output
	 //	TGraphErrors *r=(TGraphErrors*)gr_engineX(grname,0,1,-1,-1); 
	 //	 return;

	}//it was tmultigraph under tpad
	
      }//for all subobjects---looking for tmultigraph
      ((TCanvas*)prim->At(ii))->Modified();
      ((TCanvas*)prim->At(ii))->Update();
    }//if TPad

    // if ( sn.Index("TMultiGraph")==0 ){  // there is single multigraph
    //   printf("multigraph found in refreshall \n%s","");
    //   TString sn2=prim->At(ii)->GetName();
    //   sn2.ReplaceAll("_MG",".mysql");
    // 	char commandrm[200];
    // 	char grname[200];
    // 	char grname2[200];  
	
    // 	sprintf(grname,"%s.dat",  sn.Data() );
    // 	sprintf(commandrm,"sqmylite -r %s %d %d  >%s", 
    // 		sn.Data(), 0, atoi(fEntrySIG->GetText()),grname);
    // 	system(commandrm);
    // 	printf("%s\n", commandrm);
    // 	sprintf(grname2,"%s.dat.cols",  sn.Data() );
    // 	sprintf(commandrm,"cat %s | head -1 | wc -w > %s ", 
    // 		grname, grname2 );
    // 	system(commandrm); // # columns
    // 	printf("%s\n", commandrm);

    // 	 ifstream myReadFile;
    // 	 int outputCol=2;
    // 	 myReadFile.open(grname2);
    // 	 if (myReadFile.is_open()) {myReadFile >> outputCol;}
    // 	 myReadFile.close();
    // 	 outputCol--;
    // 	 for (int i=0;i<outputCol;i++){
    // 	   TGraphErrors *res=(TGraphErrors*)gr_engineX(grname,0,i+1,-1,-1); 
    // 	   res->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
    // 	   res->GetHistogram()->GetXaxis()->SetTimeFormat("#splitline{%d.%m}{%H:%M}");
    // 	   gDirectory->Add( res );
    // 	   printf("fileMYSQL seems opened CMD:/%s/\n",commandrm);
    // 	   if (outputCol>1){ joingraphsX(grname,res->GetTitle() );}
    // 	 }// for all columns ------ of mysql output
    // 	 //	TGraphErrors *r=(TGraphErrors*)gr_engineX(grname,0,1,-1,-1); 
    // 	 return;
    // }//MULTIGRAPH THERE
 }// for   ii (prim
 //============one extra thing is mysql multigraph========
}// REFRESH   ALL =========================





void  MyMainFrame::Movexy(const char *XY, double factor, double mvzm)//XY 0.2 +-1.0
{
  TH1* histo;  int64_t addr[MAXPRIMITIVES];addr[0]=0;  int count=1;
  //  RecoverTH1fromGPAD( count, addr ,"T");// this was a problem - selecting Tpad
  RecoverTH1fromGPAD( count, addr ,"TH");
  histo=(TH1*)addr[0];
  //  printf("from tpad %d recovered %ld histo addresses\n", count, addr[0] );


  if (histo!=NULL){  
    for (int ii=0;ii<count;ii++){
      //      printf("movexying (%d) :  %d/%d\n" ,addr[ii], ii,count);
      Movexy( (TH1*)addr[ii], XY, factor, mvzm); 
    }
  }
  //gPad->Modified();
  //  gPad->Update();
  // V MOVEXY JE TO  
 RefreshAll();
}// movexy




void  MyMainFrame::Movexy(TH1 *h, const char *XY, double factor, double mvzm)//XY 0.2 +-1.0
 {
   TString ss=XY;
   double x1=0.,x2=0.,dx, ox1=0.,ox2=0., factorl=factor;
   int    predbin=0.;
   //   nowbin,f1,l1, finbin,

   if (ss.CompareTo("X")==0){
     //      TString sc=h->ClassName();
      TString sn=h->GetName();
      //      printf("movexy: h(name)= %s (class)=%s\n", 
      //	     sn.Data(), sc.Data() );

      int direc;
      if (factorl>0){ direc=1;}
      if (factorl<0){ direc=-1;}
      if (  (sn.Index("_mysql_dat")>0) ){
	printf("MYSQL FOUND\n%s","");
	sn.ReplaceAll("_mysql_datb",".mysql");
	sn.ReplaceAll("_mysql_datc",".mysql");
	sn.ReplaceAll("_mysql_datd",".mysql");
	sn.ReplaceAll("_mysql_date",".mysql");
	sn.ReplaceAll("_mysql_datf",".mysql");
	sn.ReplaceAll("_mysql_datg",".mysql");
	sn.ReplaceAll("_mysql_dath",".mysql");
	sn.ReplaceAll("_mysql_dat",".mysql");
 	char commandrm[200];
 	char grname[200];
	char grname2[200];  
	
	sprintf(grname,"%s.dat",  sn.Data() );
	sprintf(commandrm,"sqmylite -r %s %d %d  >%s", 
		sn.Data(), direc, atoi(fEntrySIG->GetText()),grname);
	system(commandrm);
	printf("%s\n", commandrm);
	sprintf(grname2,"%s.dat.cols",  sn.Data() );
	sprintf(commandrm,"cat %s | head -1 | wc -w > %s ", 
		grname, grname2 );
	system(commandrm); // # columns
	printf("%s\n", commandrm);

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
	   if (outputCol>1){ joingraphsX(grname,res->GetTitle() );}
	 }// for all columns ------ of mysql output
	 //	TGraphErrors *r=(TGraphErrors*)gr_engineX(grname,0,1,-1,-1); 
	 return;
      }//(sn.Index("_mysql_dat")>0




      if (  (sn.Index("_sqlite_dat")>0) ){
	printf("SQLITE FOUND\n%s","");
	sn.ReplaceAll("_sqlite_dat",".mysql");
 	char commandrm[200];
 	char grname[200];
	
	sprintf(grname,"%s.dat",  sn.Data() );
	sprintf(commandrm,"sqmylite -r %s %d >%s", 
		sn.Data(), direc ,grname);
	system(commandrm);

	TGraphErrors *r=(TGraphErrors*)gr_engineX(grname,0,1,-1,-1); 
	r->GetN();
	 return;
      }//(sn.Index("_sqlite_dat")>0





     x1=h->GetXaxis()->GetBinCenter( h->GetXaxis()->GetFirst() );
     x2=h->GetXaxis()->GetBinCenter( h->GetXaxis()->GetLast() );
     ox1=x1;ox2=x2;
     //     nowbin=h->GetXaxis()->FindBin( x2 ) - h->GetXaxis()->FindBin( x1 );
     //debug          
     //  printf("\nOrig   :   %f  %f        fac=%f   mvzm=%f /%s\n", x1,x2,  factorl, mvzm, h->GetName() );
	    //	    if ((nowbin<4) && (x2-x1>ox2-ox1) ){factorl=factorl*2;}
     dx=(x2-x1)*factorl;   x1=x1+dx; x2=x2+ mvzm*dx;
     if (x2>h->GetXaxis()->GetXmax() ){ x2=h->GetXaxis()->GetXmax(); }
     if (x1<h->GetXaxis()->GetXmin() ){ x1=h->GetXaxis()->GetXmin(); }
     //     f1=h->GetXaxis()->GetFirst(); l1=h->GetXaxis()->GetLast();
     predbin=h->GetXaxis()->FindBin( x2 ) - h->GetXaxis()->FindBin( x1 );
 }
   if (ss.CompareTo("Y")==0){
     // ox1=x1;ox2=x2;
     x1=h->GetYaxis()->GetBinCenter( h->GetYaxis()->GetFirst() );
     x2=h->GetYaxis()->GetBinCenter( h->GetYaxis()->GetLast() );
      ox1=x1;ox2=x2;
      //    nowbin=h->GetYaxis()->FindBin( x2 ) - h->GetYaxis()->FindBin( x1 );
    //debug
    //            printf("\n movexy...Orig: %f  %f       fac=%f   mvzm=%f\n", x1,x2,  factorl, mvzm);
	    //	    if ((nowbin<4) && (x2-x1>ox2-ox1) ){factorl=factorl*2;}
     dx=(x2-x1)*factorl;   x1=x1+dx; x2=x2+mvzm*dx;
       if (x2>h->GetYaxis()->GetXmax() ){ x2=h->GetYaxis()->GetXmax(); }
     if (x1<h->GetYaxis()->GetXmin() ){ x1=h->GetYaxis()->GetXmin(); }
     //     f1=h->GetYaxis()->GetFirst(); l1=h->GetYaxis()->GetLast();
     predbin=h->GetYaxis()->FindBin( x2 ) - h->GetYaxis()->FindBin( x1 );
   }

   //   if (f2-f1<=64 && l2-l1<=64){
   //   printf( "Wange %s :  %f : %f      %d bins\n"  , XY, x1,x2 ,  l1-f1);
   

   //if ( (factorl>0.) && (mvzm<0.) && (l1-f1<15)  ){}else{
   if (predbin<05 &&  (x2-x1 < ox2-ox1) ){}else{ // MUST BE move by 15%
     h->SetAxisRange(x1,x2, XY);
     //     finbin=h->GetYaxis()->FindBin( x2 ) - h->GetYaxis()->FindBin( x1 );
     /* if (  (finbin<5) && (finbin<nowbin) ){//BACK
       h->SetAxisRange(ox1,ox2, XY);
       }*/
   }//predbin<8
     //   }
	  //   }else{ printf(" No move - 64 bins\n",""); }
   /*    if (ss.CompareTo("X")==0){
     f2=h->GetXaxis()->GetFirst(); l2=h->GetXaxis()->GetLast();
    }
   if (ss.CompareTo("Y")==0){
     f2=h->GetYaxis()->GetFirst(); l2=h->GetYaxis()->GetLast();
     } 
   if (f2==f1 && l1==l2){printf("PROBLEM  binu = %d\n", l1-f1);}
   printf( "range %s :  %f : %f   nowbin=%d   predictedbin=%d\n"  , XY, x1,x2,  nowbin, predbin );
   */
   //debug   printf( "range %s :  %f : %f   nowbin=%d   predictedbin=%d  fin=%d\n"  , XY, x1,x2,  nowbin, predbin, finbin );
   // TOO HEAVY IF MULTIPLE
   // RefreshAll();   //   GPAD->GetCanvas()->Modified();GPAD->GetCanvas()->Update();
}// MOVE





void MyMainFrame::SpiderAllTPADs(int &count, int64_t addr[]){
  int countmax=count;
  count=0;
  //  int addr[500];  // number of tpads max....MAXPRIMITIVES
  addr[count++]=(int64_t)GPAD;
  
  TList *prim=GPAD->GetListOfPrimitives();

 for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
    TString sn=prim->At(ii)->ClassName();
    //       printf( "refr:%2d  %s\n", ii,  sn.Data()  );
    if ( (sn.Index("TPad")==0)   ){  // there is TPad :< there 
      TCanvas *tp=((TCanvas*)prim->At(ii));
      if (count<countmax){        addr[count++]= (int64_t)tp;  }else{
	printf("Increase the field above %d\n", count);
      }
    }//if TPad
 }// for   ii (prim

}//  SpiderAllTPADs



void MyMainFrame::SpiderAllTH1s(int &count, int64_t addr[]){ // WHY THIS???? I HAVE TPAD spider!
  int countmax=count;
  count=0;
  //  int addr[500];  // number of tpads max....
  //  addr[count++]=(int)G;
  
  TList *prim=GPAD->GetListOfPrimitives();

 for (int ii=0; ii<=prim->LastIndex() ;ii++ ){
    TString sn=prim->At(ii)->ClassName();
    printf( "     - spidered primitives:%2d  %s\n", ii,  sn.Data()  );
    if ( (sn.Index("TH1")==0)||(sn.Index("TH1F")==0)||(sn.Index("TH1D")==0)||(sn.Index("TH2F")==0) ){  // there is TPad :< there 
      TH1 *tp=((TH1*)prim->At(ii));
      if (count<countmax){        addr[count++]= (int64_t)tp;  }else{
	printf("Increase the field above %d\n", count);
      }
    }//if TH1
 }// for   ii (prim

}//  SpiderAllTH1   // WHY THIS? I have tpad spider



/*************************************************************
 *
 *
 *            GUI  FUNCTIONS
 *
 *************************************************************
 *
 */

void MyMainFrame::fSELDelPks(int id,TString *fentry){ 
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  RecoverTH1fromGPAD( count, addr  ,"", 1);
  histo=(TH1*)addr[0];
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );

  if (histo!=NULL){  
            if (bg   != NULL){ delete bg  ;bg=NULL;}
            if (spe != NULL){ delete spe ; spe=NULL;}
            if (bgf != NULL){ delete bgf ; bgf=NULL;}
        printf("del spe,bgf------>>>   %ld %ld\n", (int64_t)spe,  (int64_t)bgf  );
	  TObject *o=histo->GetListOfFunctions()->FindObject("TPolyMarker");
          histo->GetListOfFunctions()->Remove( o );
 RefreshAll();
  }//histo
}//delete morhac peaks


void  MyMainFrame::fSELFindPks(int id,TString *fentry){ 
  //  int sigma=5;
  //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
  
       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *xfentry=new TString( fEntrySIG->GetText() );
      if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(xfentry->Data() )!=0){// match
	   defaultsigma=atoi( xfentry->Data() );
	 }// not a number
      }//xfentry  exists---------------possibility to change defaultsigma


  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  RecoverTH1fromGPAD( count, addr ,"" ,1);
  histo=(TH1*)addr[0];
  if (histo!=NULL){  

     int npeaks=102;
       if (spe != NULL){ 
	  TObject *o=histo->GetListOfFunctions()->FindObject("TPolyMarker");
          histo->GetListOfFunctions()->Remove( o ); delete spe ; spe=NULL;
       } // clear peaks 
        spe = new TSpectrum(2*npeaks);
	//	Int_t nfound = spe->Search(histo, sigma ,  "",  0.1); //sigma=5??

	//UNUSED VAR	Int_t nfound = spe->Search(histo,defaultsigma,"nobackground", 0.1);

	printf("Found npeaks = %d\n" , spe->GetNPeaks());
	//not	histo->Draw("same");

	TList *functions = histo->GetListOfFunctions();
	TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
	if (pm !=NULL){
	//	pm->SetMarkerColor(3);
	  Double_t *ax,*ay, axe[200], aye[200]; 
	  for (int i=0;i<200;i++){axe[i]=defaultsigma;aye[i]=0.;}
	int nmarks=pm->GetN();	 pm->SetMarkerColor(3);
	if (nmarks<200){ 	
	  ax=(Double_t*)pm->GetX();ay=(Double_t*)pm->GetY(); 
	  
	  TGraphErrors *m= new TGraphErrors(nmarks,ax,ay,axe,aye);
	   m->SetMarkerStyle(2);
	   m->SetMarkerSize(4);
	   m->SetMarkerColor(2);  // chceme RED
	   m->SetLineStyle(2);
	   m->SetLineColor(2);
	   m->SetName("MARKS"); 
	   //	   m->Sort(&TGraph::CompareRadius);
	   m->Sort();   //  FindPeaks....sort here is OK....
	   m->Draw("PL"); 
	   //	   m->SetName("MARKS");
	}else{
	  printf("Too many (%d) marks \n",  pm->GetN());
	}// 

	}// pm!=NULL
	RefreshAll();
  }// not NULL
} // findpeaks



void MyMainFrame::fSELFindBg(int id,TString *fentry){
  //  int sigma=10;

       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *xfentry=new TString( fEntrySIG->GetText() );
      if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(xfentry->Data() )!=0){// match
	   defaultsigma=atoi( xfentry->Data() );
	 }// not a number
      }//xfentry  exists---------------possibility to change defaultsigma


      //  if ( fentry->CompareTo("")!=0 ){ sigma=atoi( fentry->Data() ); }
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1; addr[0]=0;
  RecoverTH1fromGPAD( count, addr ,"" ,1 );
  histo=(TH1*)addr[0];
  if (histo!=NULL){  
       int npeaks=102;   //   if (spe != NULL){ delete spe ; spe=NULL;} // ???
      if (bg   != NULL){ delete bg  ;bg=NULL;}
      if (bgf  != NULL){ delete bgf ;bgf=NULL;}
      if (spe == NULL){spe = new TSpectrum(2*npeaks);}
      //morhac - tspectrum BG
      bg= spe->Background( histo , defaultsigma ,  "same" );  
      bg->SetLineColor(3);
      //UNUSED int warn=0;
	RefreshAll();  
  }//histo
}//find bg




void MyMainFrame::fSELSetMarks(int id,TString *fentry){

  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );

       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *xfentry=new TString( fEntrySIG->GetText() );
      if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(xfentry->Data() )!=0){// match
	   defaultsigma=atoi( xfentry->Data() );
	 }// not a number
      }//xfentry  exists---------------possibility to change defaultsigma


   TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
   if (g!=NULL){ g->Delete(); }
   //DELMARKS....
 double px1=0.1; double py1=0.4; double px2=0.9;double py2=0.6;  int pnts=2; // 01
 double x[5],y[5],xe[5],ye[5], dx, dy;
 //  ---   difference between  canvas ==GPAD  and a Pad
 x[0]=((TFrame*)gPad->GetFrame())->GetX1();
 x[1]=((TFrame*)gPad->GetFrame())->GetX2();
 y[0]=((TFrame*)gPad->GetFrame())->GetY1();
 y[1]=((TFrame*)gPad->GetFrame())->GetY2();
 dx=x[1]-x[0]; dy=y[1]-y[0];
 x[1]=x[0]+dx*px2;
 x[0]+=dx*px1;
 y[1]=y[0]+dy*py2;
 y[0]+=dy*py1;
 //  now backto absolute values.....
 px1= x[0];  px2= x[1];  py1= y[0];  py2= y[1]; 
 TGraph *m;
 //------- real coordinates.....
 x[0]=px1;
 x[1]=px2;
 x[2]=px2+(px2-px1);
 y[0]=py1;
 y[1]=py2;
 y[2]=py2;
 xe[0]=defaultsigma; xe[1]=defaultsigma; xe[2]=defaultsigma;
 ye[0]=0.0; ye[1]=0.0; ye[2]=0.0;

 cout<<" marker put at coordinates(x1,x2, y1,y2) "<<x[0]<<" "<<x[1]<<"    "<<y[0]<<" "<<y[1]<<endl;
 if (pnts==2) m=new TGraphErrors(2,x,y,xe,ye);
 if (pnts==3) m=new TGraphErrors(3,x,y,xe,ye);
 m->SetName("MARKS");
// m->SetMarkerStyle(27);
 m->SetMarkerStyle(2);
 m->SetMarkerSize(4);
 m->SetMarkerColor(2);//MARKS chceme RED
 m->SetLineStyle(2);
 m->SetLineColor(2);
 m->Draw("PL");    // SETMARKS - to 'specials'
 // gROOT->GetListOfSpecials()->Add(m);
 RefreshAll(); // GPAD->GetCanvas()->Modified();GPAD->GetCanvas()->Update();
}// SetMarks



void MyMainFrame::fSELGetMarks(int id,TString *fentry){  // SortMarks/GCut
   printf("item %2d:%s - fSELGetMarks ====  SortMarks/GCut \n",id,fentry->Data());  
  int sort_gcut=0;
   TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
   if (g!=NULL){ //if graph exists, we continue.....
     TH1* histo;  int64_t addr[MAXPRIMITIVES];addr[0]=0;  int count=1;
     //     RecoverTH1fromGPAD( count, addr , "TH" ,1 );//was?? // I SEARCH TH2 or ????
     // what I want:
     // I can search for whatever object, also for TH2 of TGraphErrors....
     // sometimes I need to check TGraph/TH2 (move,zoom)
     // but sometimes I need only TH2F like here
     RecoverTH1fromGPAD( count, addr , "TH" ,0 , "TGraph" ); // I SEARCH TH2 or ????
     histo=(TH1*)addr[0];
     if (histo!=NULL){
       TString s=histo->ClassName();
       printf("...searching TH2 in classname <%s>\n", s.Data() );
       if (s.Contains("TH2")>0){ sort_gcut=1;}
     }//1.histo 
     else{
       printf("...histo is NULL ?? %ld\n", (int64_t)addr[0] );
     }

     if (sort_gcut==0){ g->Sort(); }
     if ( (sort_gcut==1)&&(g->GetN()>2) ){ // TCutG for BIDIM
       int cont=0; char contn[15];
       TString nm;TString nm_c;
       /*
	*  be sure that you create a unique TCutG: even a present Cut00_.._c is wrong
	*/
       do {
       sprintf(contn, "Cut%02d_", cont );
       nm=contn;  nm.Append( histo->GetName() );
       nm_c=nm; nm_c.Append("_c");// ---why no to leave it normal...
       printf("trying name for TCutG: <%s>\n", nm.Data() );
       cont++;
       }while ( (gDirectory->FindObject( nm.Data() )!=NULL )|| 
		(gDirectory->FindObject( nm_c.Data() )!=NULL )||(cont>99) );

       TCutG *c=new TCutG( nm.Data(), g->GetN()+1,  g->GetX(), g->GetY() );
       c->SetPoint(g->GetN(),   g->GetX()[0], g->GetY()[0]  ); // connect to beginning
       c->Draw("L");
       c->Print();
       RefreshAll();// WITHOUT THIS- IT NEVER MAKES gDir add......

       printf("wait for sum of totals....%s\n","");//-------------------------------SUM
       int nbinsx,nbinsy,ii,jj;
       double cont1=0.0, cont2=0.0, contcur;
       nbinsx=histo->GetNbinsX();
       nbinsy=histo->GetNbinsY();
       for (ii=0;ii<nbinsx;ii++){
       for (jj=0;jj<nbinsy;jj++){
	 cont1=cont1+histo->GetBinContent(ii,jj);
	 if ( c->IsInside( histo->GetXaxis()->GetBinCenter(ii)   , 
				histo->GetYaxis()->GetBinCenter(jj)  )>0 ){
	   contcur=histo->GetBinContent(ii,jj);
           cont2=cont2+contcur;	
	   //	   if  (sr.CompareTo("_px")==0) {p_px->AddBinContent(ii,contcur);}
	   //	   if  (sr.CompareTo("_py")==0) {p_py->AddBinContent(jj,contcur);}
	 }//is inside
       }}//forfor
       printf(" HISTO TOTAL: %15f       CONTOUR : %f\n\n", cont1, cont2);//---------SUM
       printf("                   AREA INTEGRAL OF CONTOUR : %f\n\n",c->Integral() );//---------SUM



      //       nm.Append("_TCutG_from_"); nm.Append( gFile->GetName() ); //c->SetTile( nm.Data() );
       gROOT->GetListOfSpecials()->Add( c );//?? yes or no??
       //       printf("...adding TCutG into gDirectory: name= <%s> class=<%s>\n", 
       printf("...adding TCutG into gROOT->GetListOfSpecials(): name= <%s> class=<%s>\n", 
	      c->GetName(), c->ClassName()  );
       printf("  cutsave(%s , \"name\" )\n", 
	      c->GetName()   );

       gSystem->Sleep(200);
       gDirectory->Add( c );
     }//TCutG
   }
   RefreshAll();

}//SELGetMarks

void MyMainFrame::fSELDelMarks(int id,TString *fentry){
   TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
   if (g!=NULL){ g->Delete(); }
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
   RefreshAll();
}//delmarks






void MyMainFrame::fSELFBX(int id,TString *fentry){ 
  // GET WINDOW LIMITS...
  double x[5],y[5]; //unused , dx, dy;
  int iw=id;iw++;iw--;//unused ir
 x[0]=((TFrame*)gPad->GetFrame())->GetX1();
 x[1]=((TFrame*)gPad->GetFrame())->GetX2();
 y[0]=((TFrame*)gPad->GetFrame())->GetY1();
 y[1]=((TFrame*)gPad->GetFrame())->GetY2();
 printf("...FBX x0 x1 y0 y1:  %f %f %f %f\n", x[0],x[1],y[0],y[1] );
 // GETINFO ABOUT N PEAKS AND BACKGROUND
 int npeaks;
 double peak[100];  //funny 50 peaks
 double sigm[100];  //funny 
 for (int i=0;i<100;i++){sigm[i]=defaultsigma;peak[i]=(i+1)/102.0*(x[1]-x[0]) +x[0];}
 TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
 if (g!=NULL){// tgraph not NULL ............
 printf("...FBX address of MARKS= %ld\n",  (int64_t)g ); g->Print();
    //----------------------------------------
    if (g==NULL){
      npeaks=0;
    }else{
      npeaks=g->GetN(); if (npeaks>50){npeaks=50;} 
      g->Sort(); 
      for (int ii=0;ii<npeaks;ii++){
  	peak[ii]=g->GetX()[ii];   // prepare position value from MARKS
  	sigm[ii]=g->GetEX()[ii];   // prepare sigma value from MARKS
	printf("from MARKS: p=%f  s=%f\n", peak[ii], sigm[ii] );
      }
    }// MARKS  EXIST
     printf("...FBX  %d peaks\n",  npeaks );

   //-----------------------------------------
   //  GET HISTO 
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  //  RecoverTH1fromGPAD( count, addr , "" ,1 ); .//WAS PROBLEM
  RecoverTH1fromGPAD( count, addr , "TH1" ,0 );//OK
  histo=(TH1*)addr[0];
  //  printf("from tpad %d recovered %ld histo addresses\n", count, (int64_t)addr[0] );
  printf("...FBX  %d peaks\n",  npeaks );



  //   TBROOOOOOOOOOOOOOMMMMMM________________________________________
  if (histo!=NULL){  
    //    Tbroomfit *fit=new Tbroomfit( x[0], x[1],  (TH1F*)histo, npeaks, peak, "p1" );  
    // DEFINED  IN   kibbler_gui.h            Tbroomfit *fit;
    printf("...FBX  deleting fit %ld \n",  (int64_t)fit );
    if (fit!=NULL){ delete fit;}
    printf("...FBX  brooooooom  -----------------------NPEAKS==%d-------\n",   npeaks );
    if (npeaks>0){
     fit=new Tbroomfit( x[0], x[1],  (TH1*)histo, npeaks,   peak,sigm,   fentry->Data()  );  
    }else{
    printf("...  oops  -------------------------------NPEAKS==%d ! ----\n",   npeaks );
    }
  }//if ...  (histo!=NULL)
 }// tgraph not NULL ............
}// fselfbx____________________________________________________________ 






//  say ..... update back to graph....MARKS.......
void MyMainFrame::fSELUpdate(int id,TString *fentry){ 
  int npeaks;
  int iii=id;iii++;iii--;
  //  printf("item %2d:%s\n",id,fentry->Data());  
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
  if (fit!=NULL){
  //   //   printf ("Mean transported to GUI is %f\n",  fit->getMean(0) );

     TGraphErrors *g=(TGraphErrors*)gPad->FindObject("MARKS");
  //   //----------------------------------------
     if (g==NULL){
     }else{
       npeaks=g->GetN();
       if (npeaks<fit->getNpeaks() ){ npeaks=fit->getNpeaks(); }
       for (int i=0;i<npeaks;i++){
     	g->GetX()[i]= fit->getMean(i); // assign back the peak
     	g->GetEX()[i]= fit->getSigma(i); // assign back the peak
       }// UPDATE PEAK POSITIONS
       RefreshAll();
     }//  g==NULL   else ... g not NUL, means we put back the x values....

    fit->printResult();
    //    fit->saveResult("temporary_fitresults.root");   // curious...what will happen
    fit->saveResult("zfitresults_temporary.root");   // curious...what will happen
     
  }// fit not NULL

}//fselupdate....  










//  SAVE THE FIT SOMEWHERE -------------------------
//  SAVE THE FIT SOMEWHERE -------------------------
void  MyMainFrame::fSELComSig(int id,TString *fentry){ 
  //  printf("item %2d:%s\n",id,fentry->Data());  
  TString filename="zfitresults_final.root";// SAVE SAVE SAVE
  TString filename_eff="zfitresults.eff";
  TString filename_cal="zfitresults.ecal";
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );


   if (fit!=NULL){

     fit->printResult(); // PRINT--------------
    TString *name=fit->saveResult( filename.Data() );   //SAVE SAVE SAVE


    TCanvas *c=(TCanvas*)gROOT->GetListOfCanvases()->FindObject("fitresult");
    if (c!=NULL){
      printf("saving also the canvas%s\n","");
      //DESCRIPTION ----
      TString desc;
        TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
        RecoverTH1fromGPAD( count, addr , "" ,1);
        histo=(TH1*)addr[0];
	//       printf(" canvas - new description == %s\n", desc.Data()  );
      desc.Append( "file:" );
      if (gFile!=NULL){ desc.Append( gFile->GetName() );}else{desc.Append("nofile" );}
      //       printf(" canvas - new description == %s\n", desc.Data()  );
      desc.Append( "    histo:" );
      desc.Append( histo->GetName() );
      //       printf(" canvas - new description == %s\n", desc.Data()  );
      desc.Append( "    range:" );
      double x[5];
      //      double y[5]; //unused , dx, dy;
      x[0]=((TFrame*)gPad->GetFrame())->GetX1();
      x[1]=((TFrame*)gPad->GetFrame())->GetX2();
      //      y[0]=((TFrame*)gPad->GetFrame())->GetY1();
      //      y[1]=((TFrame*)gPad->GetFrame())->GetY2();
      desc+=x[0];
      desc.Append( "->" );
      desc+=x[1];
      desc.Append( "    CmdBox:" );
      //       printf(" canvas - new description == %s\n", desc.Data()  );
      desc.Append( fentry->Data()  );

      printf(" canvas - new description == %s\n", desc.Data()  );


      TDirectory *curr=(TDirectory*)gDirectory;// to return back


      TFile f( filename ,"UPDATE") ; 
      //      c->SetTitle( name->Data() );
      c->SetTitle( desc.Data() );
      c->Write( name->Data() );  // CANVAS-------
      f.Close();
      //SAVED  TO  permanent root file........................
      printf(" canvas (successfully) saved....%s\n","");


      printf("Canvas saved now, trying accessParams: %s\n", "");
      //      fit->printResult(); 
      //
      //   also bgarea.....
      double array[5][8];  //peaks,    k  dk  a da
      for (int i=0; i<fit->getNpeaks(); i++){

      fit->accessParams(i+1, array[i] ); 
      printf("#%d  %f   %f        %f  %f\n", i,array[i][0], array[i][1],  array[i][2],  array[i][3]  );
      }// i




      FILE *fo;
      fo=fopen( filename_eff.Data() ,"a") ; 
      for (int i=0; i<fit->getNpeaks(); i++){
      fit->accessParams(i+1, array[i] ); 
      //      printf("#%d  %f   %f        %f  %f\n", i,array[i][0], array[i][1],  array[i][2],  array[i][3]  );
      fprintf(fo,
         "%8.3f   %6.3f  %f  %f\n", array[i][0], array[i][1],  array[i][2],  array[i][3]  );
      }// i
      fclose( fo );

      /*
       * CALIBRATION FILE
       */
      //      ->GetBinWidth();         // coef a
      //      ->GetXaxis()->GetXmin()  // coef b
      //      run15_s9418_5_adc19->SetBins( 4096, -9.3, -9.3+0.96665*4096 )
      //      run15_s9418_5_adc19->GetNbinsX() 
      double cal_a=histo->GetBinWidth(1);
      double cal_b=histo->GetXaxis()->GetXmin();
      //UNUSED   double cal_chan=histo->GetNbinsX();
      // let us try.... no hope to get non-calibrated values anyway...
      //nonsense   ...histo->SetBins( cal_chan , 0.  ,  cal_chan );
      fo=fopen( filename_cal.Data() ,"a") ; 
      fprintf( fo, "%s\n", "");
      for (int i=0; i<fit->getNpeaks(); i++){
      fit->accessParams(i+1, array[i] ); 
      //      printf("#%d  %f   %f        %f  %f\n", i,array[i][0], array[i][1],  array[i][2],  array[i][3]  );
      //  E = k*a+b:   k=(E-b)/a
      fprintf(fo,
	    "%8.3f %6.3f   %8.3f %6.3f\n", (array[i][0]-cal_b)/cal_a, array[i][1]/cal_a,array[i][0],array[i][1]);
      }// i
      fclose( fo );
      //nonsense ...histo->SetBins( cal_chan , cal_b  ,  cal_b + cal_a*cal_chan );


      printf("saved %s\n","");
      curr->cd();
    }//C not NULL
   }//fit not null 
}// SELComSig







void fSELFit(int id,TString *fentry){ printf("item %2d:%s\n",id,fentry->Data());  }     
void MyMainFrame::fSELDelFBX(int id,TString *fentry){ 
  printf("item %2d:%s\n",id,fentry->Data());  

       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *xfentry=new TString( fEntrySIG->GetText() );
      if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(xfentry->Data() )!=0){// match
	   defaultsigma=atoi( xfentry->Data() );
	 }// not a number
      }//xfentry  exists---------------possibility to change defaultsigma

  printf("HELP-----------------------------------%s-\n","");
  printf("initial (default) sigma <now==%f> is important\n", defaultsigma );
  printf("   it is set by <number> in fentry box when mark is set%s\n", "" );
  printf("    change in input line pressing help or findpeaks%s\n", "");
  printf("--input line------%s-\n","");
  printf("p1+gg:scom    pn:scom,tcom   ...  definition of p.d.f. and: conditions%s\n","");
  //  printf("\n","");
  printf("--peak forms------%s-\n","");
  printf("g   ... gaussian%s\n","");
  printf("n   ... novosibirsk skewed shape(tails%s)\n","");
  printf("b   ... BreitWiegner shape (Lorentzian%s)\n","");
  printf("c   ... Crystal Ball shape (one common nalpha,n)%s\n","");
  //  printf("\n","");
  printf("pn  ... no background,  p0 ... constant background etc%s.\n","");
  printf("--modes----------%s-\n","");
  printf("chi2  ...  doesnot work good now%s\n","");
  //  printf("\n","");
  //  printf("\n","");
  printf("--conditions-----------%s\n","");
  printf("scom  ... common sigma%s\n","");
  printf("tcom  ... common tails (for novosibirsk)%s\n","");
  printf("p1fix ... peak 1 position fixed%s\n","");
  printf("s1fix ... sigma 1 fixed%s\n","");
  //  printf("\n","");
  //  printf("\n","");
  //  printf("\n","");
  printf("HELP------------------------------------%s\n","");
}  




void MyMainFrame::fSELClone2Rint(int id,TString *fentry){ 
  printf("item %2d:%s\n",id,fentry->Data());  

       //  CHCI POUZE POKUD JE TO CISLO ...... == apriori sigma 
      TString *xfentry=new TString( fEntrySIG->GetText() );
      if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]+$").Match(xfentry->Data() )!=0){// match
	   defaultsigma=atoi( xfentry->Data() );
	 }// not a number
      }//xfentry  exists---------------possibility to change defaultsigma
      
      //fListBox2->GetSelected();

TObject*o=gDirectory->FindObject( fListBox2->GetSelectedEntry()->GetTitle() );
 char chtr[500]; 
 char ch[500]; 
 sprintf( chtr, "%s" ,  gDirectory->GetName() ); // trim
 chtr[6]='\0';
 printf("Trimstring = /%s/\n",chtr);
 sprintf( ch, "%s_%s", o->GetName() ,  chtr );
 printf("Finstring = /%s/\n",ch);
 TObject *o2=o->Clone( ch );
 printf("Cloned %d\n",o2);
 //I do direct clone ...  TObject *o2=gDirectory->FindObject( ch );

 // not nice ...sprintf( ch, "%s_{(from %s)}", o2->GetTitle(), gFile->GetName()  );

 if (  strstr( o2->ClassName(),"TH1") || strstr( o2->ClassName(),"TH2")  ){
   TH1 *hhh=(TH1*)o2;
   // not nice..  hhh->SetTitle( ch );  //no
   gROOT->GetDirectory("Rint:/")->Add( hhh );
 }else{
   // can fall?
 gROOT->GetDirectory("Rint:/")->Add( o2 );
 }


 /*  printf("...clone the %s to Rint:/   ... memory: name:%s /title:%s\n", 
	 fListBox2->GetSelectedEntry()->GetTitle(),
	 o2->GetName(),
	 o2->GetTitle()
	 );
 */
}//----------------------------------Clone2Rint

  

/*
 *  THIS IS Date Time    and   Calibration (if the text field
 *
 *
 *
 */
void MyMainFrame::fSELDateTime(int id,TString *fentry){ 
  printf("item %2d:%s\n",id,fentry->Data());    
  // DECIDE IF CALIBRATE OR DATE?TIME................
       TString sr=fEntry->GetText(); 
       if  (sr.CompareTo("")==0) {// DATETIME.......
       TString dtform[10];
       dtform[3]="#splitline{%d.%m}{%H:%M}";
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  //  RecoverTH1fromGPAD( count, addr ,"" ,1);// crash in graph
  //  RecoverTH1fromGPAD( count, addr ,"" ,0); // I want also TGraph->Histo CRASHING MULTI

  RecoverTH1fromGPAD( count, addr ,"TH" ,0); // I want also TGraph->Histo

  for (int icount=0;icount<count;icount++){
  histo=(TH1*)addr[icount];
  //  printf("from tpad %d recovered histo addresses (%ld,...) [DATE]\n", count,  (int64_t)addr[0] );
  if (histo!=NULL){  
    
    // already declared......     TString dtform[10];
       //       dtform[0]="";
       dtform[0]="#splitline{%d.%m}{%H:%M}";
       //       dtform[1]="%d.%m";
       //       dtform[1]="%d.%m.%Y";
       //       dtform[2]="%d.%m.%Y/%H:%M";
       //       dtform[3]="#splitline{%d.%m}{%H:%M}";
       //       dtform[3]="#splitline{%d.%m.%Y}{%H:%M:%S}";
       //  dtform[4]="%d.%m/%H:%M:%S";
       //   dtform[5]="%d/%H:%M:%S";
       dtform[1]="%H:%M:%S";
       dtform[2]="%M:%S";
       //      dtform[0]="%d.%m";
       //       dtform[1]="%d.%m.%Y";
       //       dtform[2]="%d.%m.%Y/%H:%M";
       //       dtform[3]="#splitline{%d.%m}{%H:%M}";
       //       dtform[3]="#splitline{%d.%m.%Y}{%H:%M:%S}";
       dtform[3]="";
       dtform[4]="";
       dtform[5]="";
       dtform[6]="";
       dtform[7]="";
       dtform[8]="";
       dtform[9]="";
	 
       int SW=0;
       if ( histo->GetXaxis()->GetTimeDisplay()==0)  {SW=1;} 
       TString saq=histo->GetXaxis()->GetTimeFormat();
       TString ssq;
       int ioi;
       for (ioi=0;ioi<=1;ioi++){ // one less then the maximum possibilities...
        ssq=saq(0,dtform[ioi].Length()+2);
        if ( ssq.CompareTo( dtform[ioi]+"%F" )==0 ) {SW=ioi+2;}
       }
       printf("SW detected==%d\n", SW );
       if ( SW == 0 ){ //reset  >>>>
           histo->GetXaxis()->SetTimeFormat("");
	   histo->GetXaxis()->SetTimeDisplay(0);
	   histo->GetXaxis()->SetTimeOffset( da.Convert(kTRUE) );
       }else
       if (SW == 1 ){ //
       histo->GetXaxis()->SetTimeFormat( dtform[0] );  
       histo->GetXaxis()->SetTimeDisplay(1);  
       histo->GetXaxis()->SetTimeOffset( da.Convert(kTRUE) );
       printf( " SW=%d, histo = %s\n" , SW,   histo->GetName()  );
       }else{
       printf( " SW=%d, histo = %s - format SW-1\n" , SW,   histo->GetName()  );
       histo->GetXaxis()->SetTimeFormat(dtform[SW-1] );
       histo->GetXaxis()->SetTimeOffset( da.Convert(kTRUE) );	 
       }//else 1

       if (SW == 9 ){ //Exit to SW 0
       histo->GetXaxis()->SetTimeFormat("");histo->GetXaxis()->SetTimeDisplay(0);
       }//9  
  }//histo !=NULL // TIME FORMAT histo not  NULL
  }// for icount=0 icnout<count
       }else{//  calibrating, not doing datetime.......................
	   TString acoef=sr(0,sr.Index(","));
	   TString bcoef=sr(sr.Index(",")+1, sr.Length()-sr.Index(",")-1);
	   double ac=acoef.Atof(),bc=bcoef.Atof();
	   printf("calibrating with %s = %lf %lf\n", sr.Data() , ac, bc );
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  RecoverTH1fromGPAD( count, addr ,"TH1" ,0 );// I added TH, there was a problem with pads; 0 instd 1 and it works...
  histo=(TH1*)addr[0];
  //  printf("from tpad %d recovered %ld histo addresses\n", count,  (int64_t)addr[0] );
  if (histo!=NULL){  
	   int nbins=histo->GetNbinsX();
	   histo->SetBins( nbins, bc, ac*nbins+bc  );
  }
	   fEntry->SetText("");
       }
       RefreshAll();
}//SELDateTime






void MyMainFrame::fSELGrid(int id,TString *fentry){ 
  printf("item %2d:%s\n",id,fentry->Data());  
   int count=MAXPRIMITIVES;  int64_t addr[MAXPRIMITIVES];
  SpiderAllTPADs( count, addr );
  printf("count =  %d \n", count );
  for (int i=0;i<count;i++){
      TCanvas *tp=((TCanvas*)addr[i]);
      //	tp->SetLogy( 1-tp->GetLogy() );
	tp->SetGridy( 1-tp->GetGridy() );
	tp->SetGridx( 1-tp->GetGridx() );
	//      if (onof ==1 ){
	//	tp->SetGridy(1);tp->SetGridy(1);
	//      }else{
	//	tp->SetGridx(0);tp->SetGridy(0);
	//      }    
  }// i<count  
  //GPAD->Modified();GPAD->Update();    // MOVE TO MENU 
  RefreshAll(); 
}

void MyMainFrame::fSELLogy(int id,TString *fentry){ 
  // int onof=1;
  int count=MAXPRIMITIVES;  int64_t addr[MAXPRIMITIVES];
  //  SpiderAllTPADs( count, addr ); //Maybe this takes TPads and recover takes th1?
  // QUESTION: - 1 gPAD  LOGY - what is the point notto operate with gPad->SetLogy
  RecoverTH1fromGPAD( count, addr ,"TPad" );// this is sensitive to checkbox....multi
  printf("To Logy ...  # primitives =  %d \n", count );
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
  for (int i=0;i<count;i++){
      TCanvas *tp=((TCanvas*)addr[i]);
	tp->SetLogy( 1-tp->GetLogy() );
	//	tp->SetGridy( 1-tp->GetGridy() );
	//	tp->SetGridx( 1-tp->GetGridx() );
	//      if (onof ==1 ){
	//	tp->SetGridy(1);tp->SetGridy(1);
	//      }else{
	//	tp->SetGridx(0);tp->SetGridy(0);
	//      }    
  }// i<count  
   // MOVE TO MENU 
 RefreshAll();
}






void MyMainFrame::fSELUnzoom(int id,TString *fentry){  
  TH1* histo;  int64_t addr[MAXPRIMITIVES];  int count=1;addr[0]=0;
  RecoverTH1fromGPAD( count, addr ,"TH" );// this is sensitive to checkbox....multi
  //  printf("from tpad %d histos recovered \n", count );
  if (id==999)printf("  unused id, fentry %d %s\n", id, fentry->Data()  );
  for (int i=0;i<count;i++){
  histo=(TH1*)addr[i];
  if (histo!=NULL){  
    histo->GetXaxis()->UnZoom(); 
    TString cl=histo->ClassName();
    if (cl.Contains("TH2")>0){
      printf("making also Y unzoom%s\n","");
      histo->GetYaxis()->UnZoom(); 
    }//TH2
  }//histo != null
  }//for all addr[]
  RefreshAll();
}






/*           DivCanv,RangeAll
 *           1/  range all (if text)            
 *           2/  divide
 */

//void MyMainFrame::fSELUnzoomAll(int id,TString *fentry){ 
void MyMainFrame::fSELDivCanv(int id,TString *fentry){ 
  printf("item %2d:%s.... sel unzoom all\n",id,fentry->Data());  
  // Zrejme variantni moznost vyhledani TH1 vsude............... NEVER USE UNZOOMALL
  
  int count=MAXPRIMITIVES;  int64_t addr[MAXPRIMITIVES];
  SpiderAllTPADs( count, addr );  // AFTER THIS I HAVE ALL TPADs
  // nevim proc allth1s  SpiderAllTH1s( count, addr );   //  SPIDER ALL THE  TREE DOWN FROM THE MAIN  GPAD
  printf("count =  %d \n", count );
  //  TPad* tempo=(TPad*)gPad;

  //  if (strlen(fEntry->GetText()) >0 ){// RANGE ALL
  //    printf("TEXT MEANS RANGE ALL\n%s" , "");

  if ( (strlen(fEntry->GetText()) >0)&&( strstr(fEntry->GetText(),"-")!=NULL)   ){// RANGE ALL
    printf("'-' inside text  MEANS RANGE ALL\n%s" , "");



  ///////////////////////////////////////////////////////////RANGEALL START
  char sr[100];  int range_ok=0;
  sprintf( sr, "%s", fEntry->GetText() );     /////////
  int Range=0, Range0=0; //UNUSED VAR, posmin=0;
      char *pch;
      if ( (strlen(sr)!=0)  ){
         printf("sent text is  ../%s/.. we expect range like 500-1200\n",  sr  );
       if (  strstr(sr,"-")!=NULL  ){
	 //         printf("set text to ""  ../%s/\n",  sr  );
	 fEntry->SetText("");
	 //         printf("atoi: ""  ../%s/\n",  sr  );
	 Range=atoi(  strstr( sr,"-" )+1 );
	 //         printf("get pch: ""  ../%s/\n",  sr  );
	 pch=strstr( sr, "-" );
	 pch[ 0 ]='\0';
	 Range0=atoi( sr );
	 printf("  range  from %d  to  %d  ( 0 forces unzoom )  pch=%s\n",  Range0, Range, pch );
	 if (  (Range>Range0)&&(Range0>=0) ){ range_ok=1;}
       }//  "-"  is inside, good news
       else{printf("...Problem....%s\n","");return;}
      }//  strlen > 0


  for (int i=0;i<count;i++){
    TPad *pp=(TPad*)addr[i];
    pp->cd();
      TH1* histo;  int64_t addr2[MAXPRIMITIVES];addr2[0]=0;  int count2=1;
      RecoverTH1fromGPAD( count2, addr2 ,"TH" ,0);// restrict==1  means current pad only.HERE 0
      histo=(TH1*)addr2[0];
      //      printf("from tpad %d recovered %ld histo addresses\n", count2,  (int64_t)addr2[0] );

      for (int ii=0;ii<count2;ii++){
	TH1 *tp=((TH1*)addr2[ii]);
	//	tp->Print();
	if (range_ok==0){//UNZOOM
	tp->GetXaxis()->UnZoom();
	//Y also
	TString cl=tp->ClassName();
	if (cl.Contains("TH2")>0){
	  printf("making also Y unzoom%s\n","");
	  tp->GetYaxis()->UnZoom(); 
	}// i<count  
	}//UNZOOM;   RANGE
	else{
	  tp->GetXaxis()->SetRangeUser(Range0,Range);
	}//RANGE
      }//for ii ---- all histos in one pad
  }//all pads   for i
  ///////////////////////////////////////////////////////////RANGEALL END
  }// if (strlen(fEntry->GetText()) >0 )
  else{// IF NOT RANGE ALL===>>  PUT selDIVIDE =============
    //not divide anymore....    fSELDivide(id, fentry );
    int targetpads=0;
    if (  (strlen(fEntry->GetText())>0) && (atoi(fEntry->GetText())>0) ){
      targetpads= atoi(fEntry->GetText()) ;
      printf( "I WANT %d PADS...\n", targetpads );
    }else{
      printf("click for pads %d \n",targetpads);
    }
   //    TCanvas *GPAD; // THIS IS KEY 1 to identify histogram
   //      GPAD=
      GPAD->GetCanvas()->Clear();
      GPAD->GetCanvas()->SetFillColor( 16 );
      //      	GPAD->GetPad(ii)->Modified();GPAD->GetPad(ii)->Update();
      TPad *p;
      float  dxy=0.002;
      GPAD->Clear();
      //printf("=== divide_mod_flag %d\n", divide_mod_flag   );
      fEntry->SetText(""); // I want to clear
      switch( targetpads ){
      case 1:
	divide_mod_flag=13; break;
      case 2:
	divide_mod_flag=0; break;
      case 3:
	divide_mod_flag=6; break;
      case 4:
	divide_mod_flag=4; break;
      case 6:
	divide_mod_flag=8; break;
      case 9:
	divide_mod_flag=9; break;
      case 12:
	divide_mod_flag=10; break;
      case 16:
	divide_mod_flag=11; break;
      }
	 switch(divide_mod_flag){
	 case 0:
	     divide_mod_flag++;
	     GPAD->Divide( 1 ,2  ,0.002,0.002, 0);
	     GPAD->cd(1); 

	     break;
	 case 1:
	     divide_mod_flag++;
	     GPAD->Divide( 2 ,1  ,0.002,0.002, 0);
	     GPAD->cd(1);

	     break;
	 case 2: 
	     divide_mod_flag++;
	     //	     GPAD->Divide( 1 ,2  ,0.002,0.002, 0);
	     GPAD->cd(0);
	     p=new TPad("c1","c",    dxy,    dxy,1.0-dxy, 0.5-dxy,  4);p->Draw();//dolni
	     p->SetNumber(1  );
	     p=new TPad("c2","c",    dxy,0.5+dxy,0.5-dxy, 1.0-dxy,  4);p->Draw();
	     p->SetNumber(2  );
	     p=new TPad("c3","c",0.5+dxy,0.5+dxy,1.0-dxy, 1.0-dxy,  4);p->Draw();
	     p->SetNumber(3  );
	     GPAD->cd(1);

	     break;
	 case 3:
	     divide_mod_flag++;
	     //  GPAD->Divide( 2 ,1  ,0.002,0.002, 0);
	     GPAD->cd(0);
	     p=new TPad("c1","c",    dxy,0.5+dxy,1.0-dxy, 1.0-dxy,  4);p->Draw();//horni
	     p->SetNumber(1  );
	     p=new TPad("c2","c",0.5+dxy,0.0+dxy,1.0-dxy, 0.5-dxy,  4);p->Draw();
	     p->SetNumber(2  );
	     p=new TPad("c3","c",    dxy,0.0+dxy,0.5-dxy, 0.5-dxy,  4);p->Draw();
	     p->SetNumber(3  );
	     GPAD->cd(1);

	     break;
	 case 4:
	     divide_mod_flag++;
	     GPAD->Divide( 2 ,2  ,0.002,0.002, 0);
	     GPAD->cd(1);

	     break;
	 case 5:
	     divide_mod_flag++;
	     GPAD->Divide( 3 , 1 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 6:
	     divide_mod_flag++;
	     GPAD->Divide( 1 , 3 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 7:
	     divide_mod_flag++;
	     GPAD->Divide( 3 , 2 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 8:
	     divide_mod_flag++;
	     GPAD->Divide( 2 , 3 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 9:
	     divide_mod_flag++;
	     GPAD->Divide( 3 , 3 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 10:
	     divide_mod_flag++;
	     GPAD->Divide( 4 , 3 ,0.002,0.002, 0);	     GPAD->cd(1);

	     break;
	 case 11:
	     divide_mod_flag++;
	     GPAD->Divide( 4 , 4 ,0.002,0.002, 0);	     GPAD->cd(1);
	     break;

	 case 12:
	     divide_mod_flag++;
    	     GPAD->Divide( 5 , 4 ,0.002,0.002, 0);	     GPAD->cd(1);
	     //	     GPAD->Divide( 2 , 2 ,0.002,0.002, 0);	     GPAD->cd(1);
	     break;

	 case 13:
	     divide_mod_flag=0; 
	     GPAD->cd(0);
	     p=new TPad("c1","c",    dxy,    dxy,1.0-dxy, 1.0-dxy,  4);p->Draw();//dolni
	     //try to remove this  GPAD->GetCanvas()->SetFillColor( 0 );
	     p->SetNumber(1  );GPAD->cd(1);
	     break;
	 }//switch


	 int ii=1; 
	 //	 int cols[22]={0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
	 int cols[22]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      while ( GPAD->GetPad(ii)!= NULL){
	//	printf("%2d  # %d\n", ii,   GPAD->GetPad(ii)->GetNumber()  );
	GPAD->GetPad(ii)->SetFillColor( cols[ii] );
	//no good GPAD->GetPad(ii)->Draw();// force the color
	//	GPAD->GetPad(ii)->Update();// force the color
	GPAD->GetPad(ii)->SetBorderSize(1);// TEST TO HAVE BETTER MODE
	GPAD->GetPad(ii)->SetBorderMode(1);// TEST TO HAVE BETTER MODE
	GPAD->GetPad(ii)->Modified();// force the color
	GPAD->GetPad(ii)->Update();// force the color
	ii++;
	if (ii>22) break;
      }// while pad exist      

      //	printf("leaving if else %s\n", ""   );

  }
  //  if (tempo!=NULL)tempo->cd();
  //	printf("refreshall %s\n", ""   );

  RefreshAll();           /////  /////////

}// fSELDivCanv//fSELUnzoomAll - but now does Divide Canvas   DivCanv,RangeAll









void MyMainFrame::fSELLogz(int id,TString *fentry){ 
  printf("item %2d:%s logz\n",id,fentry->Data());  
  int count=MAXPRIMITIVES;  int64_t addr[MAXPRIMITIVES];
  //????   SpiderAllTPADs( count, addr );
  RecoverTH1fromGPAD( count, addr ,"TPad" );// this is sensitive to checkbox....multi
  printf("To Logz ...  # primitives =  %d \n", count );
  for (int i=0;i<count;i++){
      TCanvas *tp=((TCanvas*)addr[i]);
	tp->SetLogz( 1-tp->GetLogz() );
	//	tp->SetGridy( 1-tp->GetGridy() );
	//	tp->SetGridx( 1-tp->GetGridx() );
	//      if (onof ==1 ){
	//	tp->SetGridy(1);tp->SetGridy(1);
	//      }else{
	//	tp->SetGridx(0);tp->SetGridy(0);
	//      }    
  }// i<count  
   // MOVE TO MENU 
 RefreshAll();
} 

   
// loadcanvas  -   seldivide
//                   divide is deprecated, loadcanvas remains
//
void MyMainFrame::fSELDivide(int id,TString *fentry){ 
   printf("item %2d:%s=fentry  - divide ... deprecated, loadcanvas NOW\n",id,fentry->Data());
   //   char sr[100];sprintf( sr, "%s", fEntry->GetText() );
   // no    TString sr=fEntry->GetText();  
   TString *xfentry=new TString( fEntry->GetText() );  
   int gofile=0;
   if ( ( xfentry->CompareTo("")!=0)  ){ gofile=1;}
   switch(  atoi( xfentry->Data() ) ){

       case 1: CURRENT_SLOT=1; gofile=0;break;
       case 2: CURRENT_SLOT=2; gofile=0;break;
       case 3: CURRENT_SLOT=3; gofile=0;break;
       case 4: CURRENT_SLOT=4; gofile=0;break;
       case 5: CURRENT_SLOT=5; gofile=0;break;
       case 6: CURRENT_SLOT=6; gofile=0;break;
       case 7: CURRENT_SLOT=7; gofile=0;break;
       case 8: CURRENT_SLOT=8; gofile=0;break;
       case 9: CURRENT_SLOT=9; gofile=0;break;
       }
   fEntry->SetText("");
       if ( gofile==1  ){ // HARD WAY - LOAD ANY FILENAME
	 Desktop_setup_savecanvas(  (const char*)xfentry->Data()  );
	 fEntry->SetText("");

	  //       }else  if ( fChk1->GetState()==1 ){  // MULTI I IS  CHECKED
       }else  if ( 1==1 ){  // ALWAYS USE LOADCANV _ CYCLE
	 char filename[100];
	 if (CURRENT_SLOT<1){CURRENT_SLOT=1;} 
	 if (CURRENT_SLOT>9){CURRENT_SLOT=1;} 
	 sprintf( filename, "canvas_%d.root", CURRENT_SLOT );
	   printf("###### %s ##### BEFORE DECIMA \n", filename);

	 while ((Desktop_setup_savecanvas( filename )!=0)&&(CURRENT_SLOT>1)){
	   CURRENT_SLOT++;
 	   sprintf( filename, "canvas_%d.root", CURRENT_SLOT );	 
	   printf("###### %s ##### not exist \n", filename);

	   if (CURRENT_SLOT>9){
	     CURRENT_SLOT=1;
	     sprintf( filename, "canvas_%d.root", CURRENT_SLOT );
	     Desktop_setup_savecanvas( filename );
	     break;}
	 }

	 printf("###### %s ##### AFTER DECIMA \n", filename);

	 //-------------------SHOW #
	   //	 TPad *p=new TPad("SHOW","SHOW", 0.3,0.3,0.7,0.7, kGray ,?);
	 TPad *p=new TPad("SHOW","SHOW", 0.3,0.3,0.7,0.7, kGray  );
	 gPad->GetCanvas()->cd();
	 p->Draw();
	 p->cd();
	 TBox *b1=new TBox(0.05,0.05,0.95,0.95);
	 b1->SetLineColor(1);b1->SetLineWidth(1);b1->SetFillStyle(0);
	 b1->Draw();
	 TBox *b2=new TBox(0.01,0.01,0.99,0.99);
	 b2->SetLineColor(1);b2->SetLineWidth(2);b2->SetFillStyle(0);
	 b2->Draw();
	 TText *t=new TText(0.5,0.5 , filename );
	 t->SetTextAlign(22);
	 t->SetTextFont( 20 ); //10* font + precision
	 t->SetTextSize( 0.8/7 ); //10* font + precision
	 t->Draw();
	 p->Modified();p->Update();
	 gSystem->Sleep(500);
	 b1->Delete();	 b2->Delete();
	 t->Delete();
	 p->Delete();
	 //-------------------SHOW #
	 gPad->GetCanvas()->cd(1);

	 printf("###### %s ##### tried to load\n", filename);
	 CURRENT_SLOT++; 
       }else{// Div/LoadCFG        NEVER USE DIVIDE


       }// Div/LoadCFG


       RefreshAll(); // GPAD->GetCanvas()->Modified();GPAD->GetCanvas()->Update();

}//fSELDivide...........................................................









void MyMainFrame::fSELClear(int id,TString *fentry){ 
   printf("item %2d:%s clear (one)\n",id,fentry->Data());  
  TH1* histo;  int64_t addr[MAXPRIMITIVES];addr[0]=0;  int count=1;
  // i use exclude tgraph;  restrict davam:0= muze to byt tgraph->Histo?
  //ABY MI fungovalo clear     (x) multi, musim tohle zmenit........
  RecoverTH1fromGPAD( count, addr ,"TH" ,0 , "TGraph");  // was 1+crash; either ,"TH" ,0   OR  "",1
  for (int icount=0;icount<count;icount++){
      histo=(TH1*)addr[icount];
      //      printf("%d)from tpad %d recovered histo addresses (%ld,...) [DATE]\n", icount,count, (int64_t)addr[0] );
      printf("Clearing histogram /%s/\n",  histo->GetName()  );
      if (histo!=NULL){       histo->Reset();  } 
  }//for loop
    //---------previous---------------------
  /*
  histo=(TH1*)addr[0];
  printf("from tpad %d recovered %d histo addresses\n", count, addr[0] );
  printf("Clearing histogram /%s/\n",  histo->GetName()  );
  if (histo!=NULL){       histo->Reset();  }
  */
  //  gPad->Modified();gPad->Update();  
  RefreshAll();
}   


void  MyMainFrame::fSELRefresh(int id,TString *fentry){
  printf("item %2d:%s refresh (all)\n",id,fentry->Data());  
  RefreshAll();
} 



void MyMainFrame::fSELSaveSpectra(int id,TString *fentry){ 
  printf("item %2d:%s - fSELSaveSpectra \n",id,fentry->Data()); 
  
      //======= TIME AND DATE ===============
  time_t curtime;  struct tm *loctime;  
  curtime = time (NULL);  loctime = localtime (&curtime);

  char ch[100];
  sprintf(ch,"save%04d%02d%02d_%02d%02d%02d.root",
 	   1900+loctime->tm_year,   1+loctime->tm_mon,  
	   loctime->tm_mday,  loctime->tm_hour,   loctime->tm_min,  loctime->tm_sec);
  printf("****file %s *********\n", ch );
  int max; max=gDirectory->GetList()->GetEntries();
  int i; char clasna[100];
 for (i=0;i<max;i++){
   sprintf( clasna, "%s",  gDirectory->GetList()->At(i)->ClassName() );
   if ( (strstr(clasna,"TH1")!=0)||(strstr(clasna,"TH2")!=0) ){ 
   printf("saving item #%03d : %s\n", i,gDirectory->GetList()->At(i)->GetName() );	
   saveobj2file( (char*)ch, (TH1*)gDirectory->GetList()->At(i) ,1 );
   }//TH*
 }//for i
 printf("****file %s *********\n", ch );// DONE
}//--------save spectra--------------------





/*
 *  I like the idea  of 9 files that will cycle...
 *              9 slots :    canvas_1.root ....
 */

int get_free_slot(const char* name){
  	 const char path[500]=".";
	 char regexp[100];  //real
	 char to_cmp[100];  //compare to this
	 int slot[10]={0,0,0,  0,0,0,  0,0,0,  0};

	 sprintf( regexp, "^%s_\\d\\.root$", name );
	 DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
	 pdir = opendir (path); // "." will refer to the current directory
	 struct dirent *pent = NULL;
	 if (pdir == NULL) // if pdir wasn't initialised correctly
	   { // print an error message and exit the program
	     printf ("\nERROR! pdir could not be initialised correctly");
	     return 0; // exit the function
	   } // end if 
	 while (  (pent = readdir (pdir))   )  {
	     if (pent == NULL) { 
		 printf ("\nERROR! pent could not be initialised correctly");
		 return 0; // exit the function
	       }
	     TString s=pent->d_name;
	     //	     if (  (TPRegexp("^canvas_\d\\.root$").Match(s.Data())!=0)  ){//
	     if (  (TPRegexp( regexp  ).Match(s.Data())!=0)  ){//
	       for (int i=1;i<10;i++){
		 sprintf( to_cmp, "%s_%d.root", name, i );
		 if (strcmp(s.Data(),to_cmp)==0){  slot[i]=1;} 
	       }// for  compare
	     }// regexp is ok
	   }//while pent
	 
	 // finally, let's close the directory
	 closedir (pdir);
	 for (int i=1;i<10;i++){
	   if (slot[i]==0){ return i;}
	 }
	 return 0;
}//get_free_slot





void MyMainFrame::fSELSaveCanvas(int id,TString *fentry){ 
  printf("item %2d:%s - savecanvas\n",id,fentry->Data());  
  //  here I check the filename.................................................
  int i;
  TString *xfentry=new TString( fEntry->GetText() ); 
  if ( xfentry->CompareTo("")!=0 ){ 
	 if  (TPRegexp("^[\\d]*$").Match(xfentry->Data() )!=0){// match A NUMBER
	   i=atoi( xfentry->Data() );
	   savecanvas(  "canvas.root" , i );
	 }// was a number 
	 else{ // is not a number => filename
	   savecanvas(  xfentry->Data() );
	 }// it was filename
	 fEntry->SetText("");   //CLEAN
  }// there was something in fEntry ======
  else{ // there is NOTHING  in fEntry ======
     i=get_free_slot("canvas");
     if (i==0){ printf("ALL SLOTS <canvas_i.root> ARE FULL - REMOVE SOME PLEASE%s\n",""); return;}
     savecanvas( "canvas.root" , i );
  }// there was nothing i fentry

}// FUNCTION END ----------===========----------





void  MyMainFrame::fSELClearAll(int id,TString *fentry){ 
  printf("item %2d:%s clear all\n",id,fentry->Data()); 

	         TH1 *o;
	 for (int iii=0 ; iii<gDirectory->GetList()->GetEntries() ; iii++ ){ //why 1000? - just so
	   TString sa1=gDirectory->GetList()->At(iii)->GetName();
	   o=(TH1*)gDirectory->FindObject( sa1.Data() );
	   TString sa2=o->ClassName();
	   if (  (sa2.Index("TH1")==0) || (sa2.Index("TH2")==0) ){
	     o->Reset();
	   }// TH  OBJECT 
	}//iii (mem)
	 //  gPad->Modified();gPad->Update();  
	 RefreshAll();
}











//=====================================================================
//
//
//
//
//   HANDLE ALL THE EVENTS
//
//
//
//
//
//
//
//=====================================================================

//--------BUTTONS--------------
void MyMainFrame::ClickResponse()  // this isconnected to  BUTTONS
{  Int_t id;    TGButton *btn = (TGButton *) gTQSender;   id = btn->WidgetId();
   fSelected->Clear();
   HandleEvents(id); // common target for all buttons/lists
}// ClickResponse





// THIS IS 1parameter - doubleclick for tlistbox2
// http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=10972&p=55443#p55443
//void  MyMainFrame::ClickResponseDC(TGFrame *frm, Int_t id)   // THIS IS CONNECTED TO TLIST2
//
// Open File
//
void  MyMainFrame::ClickResponse( Int_t id)   // THIS IS CONNECTED TO TLIST2
{
  int i=id;i++;
  //2013
  //  printf(" DOUBLECLICKED - item=%d /%s/\n",  id ,fListBox2->GetSelectedEntry()->GetTitle() );
  //  fEntry->SetText( fListBox2->GetSelectedEntry()->GetTitle()  );
  return;

  //COPIED FROM id=101 void MyMainFrame::HandleEvents(Int_t id)
  //       if (newfentry->CompareTo("")==0){   //  make  list of  files..........
	 const char *path=".";
	 // first off, we need to create a pointer to a directory
	 DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
	 pdir = opendir (path); // "." will refer to the current directory
	 struct dirent *pent = NULL;
	 if (pdir == NULL) // if pdir wasn't initialised correctly
	   { // print an error message and exit the program
	     printf ("\nERROR! pdir could not be initialised correctly");
	     return; // exit the function
	   } // end if 
	 /*
	  *   insert all .root files into this
	  */
	 int next=0;
	 fListBoxOF->AddEntry( "***cancel***", next++ );
	 //	 TString *s=new TString("***cancel***");fSelectedOF->Add( (TObject*)s );
	 fListBoxOF->AddEntry( "*SaveToNewFile*", next++ );
	 fListBoxOF->AddEntry( "*Memory DirAQ*", next++ );
	 //	 s=new TString("*SaveToNewFile***");fSelectedOF->Add( (TObject*)s );
	 while (   (pent = readdir (pdir))   ) // while there is still something in the directory to list
	   {
	     if (pent == NULL) // if pent has not been initialised correctly
	       { // print an error message, and exit the program
		 printf ("\nERROR! pent could not be initialised correctly");
		 return; // exit the function
	       }
	     // otherwise, it was initialised correctly. let's print it on the console:
	     //printf ("%s\n", pent->d_name);
	     TString s=pent->d_name;
	     if (TPRegexp("\\.root").Match(s.Data())!=0){
	       fListBoxOF->AddEntry(pent->d_name, next++ );
	     }
	     if (TPRegexp("\\.mysql").Match(s.Data())!=0){
	       fListBoxOF->AddEntry(pent->d_name, next++ );
	     }
	     if (TPRegexp("\\.sqlite").Match(s.Data())!=0){
	       fListBoxOF->AddEntry(pent->d_name, next++ );
	     }
	   }
	 
	 // finally, let's close the directory
	 closedir (pdir); 

	 fListBoxOF->SortByName();
	 /*
	  *  RESIZE THE DIRECTORY BOCX TO MAXIMUM,  flistbox2 to minimum
	  */
	 printf("RESIZING fListBoxOF%s\n","");
	 fListBox2->Resize(5,listbox2_vsize);  // was flistbox and worked...
	 fListBoxOF->Resize(140 , listbox2_vsize );  // BY DEFAULT  
	 //	 Resize(GetDefaultSize());
	 Layout();//important


  //   fDisplayFromList2( fListBox2->GetSelected() ,
  //	                        fListBox2->GetSelectedEntry()->GetTitle()  );  


}//Clickresponse 1    === doubleclick on flistbox2





//--------LISTS--------------
void  MyMainFrame::ClickResponse(Int_t w, Int_t i)   // THIS IS CONNECTED TO TLIST
{     time_t curtime;  // struct tm *loctime;  // needed for automatic refresh
  curtime = time (NULL);  //loctime = localtime (&curtime);
   flistbox_selected=-1;  // by default - nothing was selected
   flistbox2_selected=-1;  // by default - nothing was selected
   if (i==1234){ printf("%s","");}
  //   100 flistbox  -  big menu
  if (w==101){  
      flistbox2_selected=fListBox2->GetSelected();
      //      printf("selected2 %d\n", flistbox2_selected);
  }
  if (w==100){  
      flistbox_selected=fListBox->GetSelected();
      //      printf("selected  %d\n", flistbox_selected);
      // page2  begins  @  16  (16 means +14)
      if ( (PAGE==2) && (flistbox_selected>1)  ){ 
	flistbox_selected=flistbox_selected+14;  
      } 
      lastlastcmd=lastcmd;lastcmd=flistbox_selected;
      lastlastcmdtime=lastcmdtime;lastcmdtime=flistbox_selected_when;
      flistbox_selected_when=curtime;

      //     ****  here- any time histogram will be extended   ****      
      //      TDatime da(2000,01,01,0,0,0);
      curtime=curtime - da.Convert(kTRUE); 
      //     shspetime1 shspetime2 shspetime3 shspetime4 shspetime5
  } // listbox
  HandleEvents(w);  
}// ClickResponse2













//This will collect all the events-------------------------------------

void MyMainFrame::HandleEvents(Int_t id)
{    
  //  printf("entered  handle events...  id==%d, \n",  id );
  TString *fentry=new TString( fEntry->GetText() );
  flistbox_selected=-1;  // by default - nothing was selected
  flistbox2_selected=-1;  // by default - nothing was selected



  /*
   *    OF   box  pressed.....
   * 
   *   reset the size of directory window
   */
  if (id==122){   // TLIST OF  was clicked -  MEANS OPEN NEW FILE   /  OR MEANS  SAV ETO NEW/OLD FILE
    TString savename="zhistos.root";
    int save=0;

    int ii=fListBoxOF->GetSelected();
    TString fileselect=fListBoxOF->GetSelectedEntry()->GetTitle();

    printf("clicked in BoxOF......selected==%d , <%s>\n",ii,fileselect.Data()  );
    int OFaction=0; 
    /*   0   cancel  ----- BY DEFAULT
     *   1   OPEN file
     *   2   save new file selected
     *   3   other file selected
     */

    if (fListBoxOF->GetNumberOfEntries()>0){// MORE entries THEN just " CANCEL "
     TString fileselect1=fListBoxOF->GetEntry(1)->GetTitle(); 
     printf("   ...entry #2 ==<%s> - check of filesave mode...\n", fileselect1.Data()  );   
     if ((fileselect1.Contains("*SaveToNewFile")>0)&&(ii>1)) { 
       save++;printf("SAVING BOX file <%s> demanded BUT default file== <%s> is used\n",
		     fileselect.Data(), savename.Data()  );
       //NOT USED UNTIL DOUBLECLICKED...OFaction=2; // At least save new file, maybe otherfile (next time?)
     }// SAVENEW EXISTS
    }//MORE ENTRIES THAN ***CANCEL***


    if (fileselect.Contains("*Memory")>0){ //directly NEW FILE demanded
      printf("getting back to Rint://   <%s>\n", savename.Data()  );
      //      OFaction=2; // SURELY NEW FILE ... ???
      gROOT->cd();
    } //


    if (fileselect.Contains("*SaveToNewFile")>0){ //directly NEW FILE demanded
      save++;printf("SAVING TO A NEW FILE <%s>\n", savename.Data()  );
      OFaction=2; // SURELY NEW FILE
    } //directly NEW FILE demanded

    //    if ((ii>0)&&(save==0)){ OFaction=1;}// OPEN - 
    if ((ii>1)){ OFaction=1;}// not cancel, nor svatonew

    if (ii==0){ OFaction=0;}// cancel
    /////#######################################################  DECIDE WHAT TO DO NOW
    printf("decision part %d:\n", OFaction );
    //   sleep(1);
    if (OFaction==0){// cancel //------------------------------------------------
    }//0 cancel

    if (OFaction==1){// OPEN //------------------------------------------------
      if ( gFile!=NULL ){ 
	printf("...closing file %ld <%s>\n",  (int64_t)gFile, gFile->GetName() );
      	// gFile->Close(); // BIG PROBLEMS TO CLOSE
 	printf("...file closed (virtualy) %ld \n", (int64_t)gFile );
     } // if file opened

      //      sleep(1);
      //      printf("%s","... file closed\n");
      //    TString *fn= (TString*)fSelectedOF->At( ii) ;
      TString *fn=new TString(  fileselect.Data() ); 
      //     printf("...appending  <%s>\n", fileselect.Data()     );
      //     fn->Append( fileselect.Data() );
     //     sleep(1);

 ifstream myReadFile;


 // at startup........this appears================ REMOTE DATADIR
 printf("going to open REMOTE_DATA_DIR %s\n", "file"); // NOT HERE???????
 myReadFile.open("REMOTE_DATA_DIR");
 printf("done  to open REMOTE_DATA_DIR %s\n", "file ");
 char output[300];
 if (myReadFile.is_open()) {
  while (!myReadFile.eof()) {    myReadFile >> output;    cout<<output; }
 }
 myReadFile.close();//============================ REMOTE DATADIR


      if (fn->BeginsWith("~")){
	fn->ReplaceAll("~","/mnt/hgfs/AA_share/DATA/20121029_elast_p_3He/");
      }
      
      printf("File OPENINIG  <%s>\n",  fn->Data()    );
      fOpenFile(  fn   , fListBox2  , atoi(fEntrySIG->GetText() ) );  // 2nd fopenfile<< click in id=122 listboxOF
  //   sleep(1);
      printf("File OPENED  <%s> (OFaction)\n",  fn->Data()    );
    }//1 OPEN  // OFaction==1


    //    // LETS CHANGE ANYTME   ***listboxOF clicked****
      TString GPADTITLE= GPAD->GetTitle();
      GPADTITLE.Replace(GPADTITLE.Index("##")+2, GPADTITLE.Length()-GPADTITLE.Index("##")+2 , gDirectory->GetName()  );
      GPAD->SetTitle( GPADTITLE.Data()  );




    if (1==2){// SAVE NEW FILE //----------------------------
//  if (OFaction==2){// SAVE NEW FILE //----------------------------
       TDirectory *curr=(TDirectory*)gDirectory;// to return back 

       // MUSI tam byt    fentry.  Jinak to blbne s ***openfile***
       printf( "...before fSAVEFromList2 call: <%s>\n",  fEntry->GetText()  );
       fSAVEFromList2( 2 , fEntry->GetText() ,  savename.Data() );  // TRY 2, id is obsolete...

       // problem :  get entry gives   ** openfile *** .........  putin
       //       printf( "...before fSAVEFromList2 call: <%s>\n",fListBox2->GetSelectedEntry()->GetTitle() );
       //       fSAVEFromList2( 2 , fListBox2->GetSelectedEntry()->GetTitle() ,  savename.Data() );  // TRY 2, id is obsolete...
       printf("saved %s\n",""); curr->cd();
    }//2 SAVE NEW


    if (OFaction==3){// SAVE TO EXISTING  //-----------------
    }//3


    //    printf("decision ok %d, now Close OF:\n", OFaction );


    // IT CRASHES  SOMEWHERE  HERE==================================
    //    printf("???flbOF (%d entries). Removing entries from 1 to %d\n", 
    //	   fListBoxOF->GetNumberOfEntries(), fListBoxOF->GetNumberOfEntries() );


    fListBoxOF->RemoveEntries( 0, fListBoxOF->GetNumberOfEntries() ); // remove all entries
    //    printf("???Removing entries from fSelectedOF %s\n",  "");
    //    fSelectedOF->RemoveAll();
    //    printf("???Removed All() from fSelectedOF %s\n",  "");
    //    fSelected2->RemoveAll(); //  ????? TRY THIS  NO EFFECT AT ALL.....
    //    Layout();// this and sleep and seem more stable...
    // printf("???resize to 140x%d  flistbox2\n", listbox2_vsize );
    fListBox2->Resize(140,listbox2_vsize);  // was flistbox and work
    //printf("???resize to 5x%d flistboxOF\n", listbox2_vsize );
    fListBoxOF->Resize(5 , listbox2_vsize );  // BY DEFAULT  MAKE SMALL

    //   sleep(1);
    /*
    printf("???resize getdefaultssize  %d\n", listbox_vsize );
    Resize(GetDefaultSize());

    printf("???mapsub %s\n",  "(.... critical place......)");
       MapSubwindows();
    printf("???resize %s\n",  "(.... critical place......)");
   Resize(GetDefaultSize());
    printf("???mapwin %s\n",  "(.... critical place......)");
   MapWindow();   
    printf("???layout %s\n",  "(.... critical place......)");
    */
    Layout();
  }//entry  122############################ OF  listbox ####





  /*  ----------------------------------------------------------------------------
   *
   *    100 -  big listbox  -  fListBox
   *
   */
  if (id==100){   // TLIST 1  was clicked
    flistbox_selected=fListBox->GetSelected();  
    //    printf("TLIST1 was clicked %s\n" , "");
    if ( (PAGE==2) && (flistbox_selected>1)  ){ 
      flistbox_selected=flistbox_selected+16;  // SELECT OFFSET !  
    }     
    //
    //  SET GET  DEL     MARKS....
    //
    if (flistbox_selected== SELNextPage){  
      PAGE++;      if (PAGE>2){PAGE=1;} 
      printf("Next PAGE %d\n",  PAGE );
      fListBox->RemoveAll();
       FillMainMenu();
    }//entry 1
    //  if (flistbox_selected== SELNextPage
  if (flistbox_selected== SELSetMarks   ){ fSELSetMarks(flistbox_selected,fentry);  }
  if (flistbox_selected== SELGetMarks   ){ fSELGetMarks(flistbox_selected,fentry);  }
  if (flistbox_selected== SELDelMarks   ){ fSELDelMarks(flistbox_selected,fentry);  }
  if (flistbox_selected== SELFindPks    ){ fSELFindPks(flistbox_selected,fentry);  }
  if (flistbox_selected== SELFindBg     ){ fSELFindBg(flistbox_selected,fentry);  }
  if (flistbox_selected==SELDelPks      ){ fSELDelPks(flistbox_selected,fentry);  }
  if (flistbox_selected== SELFBX        ){ fSELFBX(flistbox_selected,fentry);  } 
  if (flistbox_selected== SELUpdate     ){ fSELUpdate(flistbox_selected,fentry);  }  
  if (flistbox_selected== SELComSig     ){ fSELComSig(flistbox_selected,fentry);  }
  if (flistbox_selected== SELFit        ){ fSELFit(flistbox_selected,fentry);  }       
  if (flistbox_selected== SELDelFBX     ){ fSELDelFBX(flistbox_selected,fentry);  }       
  if (flistbox_selected== SELClone2Rint     ){ fSELClone2Rint(flistbox_selected,fentry);  }       


  if (flistbox_selected== SELGrid       ){ fSELGrid(flistbox_selected,fentry);  }
  if (flistbox_selected== SELDateTime   ){ fSELDateTime(flistbox_selected,fentry);  }  
  if (flistbox_selected== SELLogy       ){ fSELLogy(flistbox_selected,fentry);  } 
  if (flistbox_selected== SELLogz       ){ fSELLogz(flistbox_selected,fentry);  } 
  if (flistbox_selected== SELDivide     ){ fSELDivide(flistbox_selected,fentry);  }
  if (flistbox_selected== SELClear      ){ fSELClear(flistbox_selected,fentry);  }
  if (flistbox_selected== SELRefresh    ){ fSELRefresh(flistbox_selected,fentry);  }
  if (flistbox_selected== SELUnzoom     ){ fSELUnzoom(flistbox_selected,fentry);  }
  if (flistbox_selected==SELSaveSpectra ){ fSELSaveSpectra(flistbox_selected,fentry);  }
  if (flistbox_selected== SELSaveCanvas ){ fSELSaveCanvas(flistbox_selected,fentry);  } 
  if (flistbox_selected== SELClearAll   ){ fSELClearAll(flistbox_selected,fentry);  }
  //  if (flistbox_selected== SELUnzoomAll  ){ fSELUnzoomAll(flistbox_selected,fentry);  }
  if (flistbox_selected== SELDivCanv  ){ fSELDivCanv(flistbox_selected,fentry);  }
  if (flistbox_selected== SELClone2Rint2     ){ fSELClone2Rint(flistbox_selected,fentry);  }       

  }// TLIST    1   id==89



  /*  ----------------------------------------------------------------------------
   *
   *    101 -  small listbox2  -  fListBox2
   *
   */
   if (id==101){ //  TList2 was clicked
     //     printf("TLIST2 was clicked, getselected==#%d/%d <%s>\n",
     //	    fListBox2->GetSelected(),fListBox2->GetNumberOfEntries(), 
     //	    fListBox2->GetSelectedEntry()->GetTitle() ); 
     /*
      *
      *------------------------------------------  OPEN FILE ------->>>>>>>>>>>>
      *
      */
     if (fListBox2->GetSelected()==1 ){ //openfile clicked
       //  printf("testing .... file when getselected==1\n%s", "");
       /*
	*   I WILL NOT SUPPORT file in FENTRY, because problems of doubleclick.....
	* 
	*/
       /*
       FILE *ftst=fopen( fentry->Data(), "r");    //  check the existence of the filename in fentry
       if (ftst!=NULL){ 
	 printf("...FILE <%s> does exist\n",fentry->Data());
       }else{
	 printf("...FILE <%s> does NOT exist\n",fentry->Data());
       }
       */


       /*
	*       file exists     or       ""
	*/
       //not like this       if (fentry->Data()==""){printf(" fentry je NIC%s\n","");}
       /*
       TString *newfentry=fentry;
       if  (ftst==NULL) { newfentry->Clear();}else{fEntry->Clear();}  // MAKE IT NO STRING
       if ( (ftst!=NULL)||(newfentry->CompareTo("")==0) ){  //if empty -> open anyway (==refresh from memory)
        if  (ftst!=NULL) {fclose(ftst);}
	printf("going to open <%s> \n", newfentry->Data()  );
        fOpenFile(newfentry,fListBox2); 

        fOpenFile("",fListBox2); 
	printf("         opened <%s>, going to resize \n", newfentry->Data()  );
	//        fEntry->Clear();//always clear the text field...
	//         fListBoxOF->Resize(1 , listbox_vsize );  // BY DEFAULT 
	fListBox2->Resize(100,listbox_vsize); 
	printf("      resized, going to layout %s\n",  "" );
        Layout();// important !  makes it wider
       }// no file, ok, refresh from memory
       */
       TString *newfentry=fentry;newfentry->Clear();
       
       fOpenFile(newfentry,fListBox2, atoi(fEntrySIG->GetText()));
    // 3rd fopenfile----------------<<<<<<<<< FROM ***openfile*** fentry



    // LETS CHANGE ANYTME   **openfil****** clicked
      TString GPADTITLE= GPAD->GetTitle();
      GPADTITLE.Replace(GPADTITLE.Index("##")+2, GPADTITLE.Length()-GPADTITLE.Index("##")+2 , gDirectory->GetName()  );
      GPAD->SetTitle( GPADTITLE.Data()  );



      /***************************************************
       *  CONTENTS OF DIRECTORY            HERE
       *
       */
	 char path[500]=".";
	 DIR *pdir = NULL; // remember, it's good practice to NULL!
	 pdir = opendir (path); // "." will refer current directory
	 struct dirent *pent = NULL;
	 if (pdir == NULL) // if pdir wasn't initialised correctly
	   { // print an error message and exit the program
	     printf ("\nERROR! pdir could not be init. correctly");
	     return; // exit the function
	   } // end if 
	 /*
	  *   insert all .root files into this
	  */
	 int next=0;
	 // *** necessary to keep SORTing
	 fListBoxOF->AddEntry( "***cancel ***", next++ );
	  fListBoxOF->AddEntry( "*S        *", next++ ); // SORRY - until doubleclicked solves....
	  //	  fListBoxOF->AddEntry( "*SaveToNewFile *", next++ ); // SORRY - until doubleclicked solves....
	 fListBoxOF->AddEntry( "*Memory *", next++ ); //   #HERE 
	 while (  (pent = readdir (pdir))   ) // while in dir
	   {
	     if (pent == NULL) // if pent hasnotbeeninit. correctly
	       { 
		 printf ("\nERROR! pent could not init. correctly");
		 return; // exit the function
	       }
	     TString s=pent->d_name;
	     char totname[500];
	     sprintf(totname,"%s",  pent->d_name ); 
	     //HERE IS WHAT TO ADD TO LIST2 ????????
	     //  the funcion fopenfile opens the files
	     if (TPRegexp("\\.root$").Match(s.Data())!=0){
	       fListBoxOF->AddEntry( totname, next++ );
	     }
	     if (TPRegexp("\\.mysql$").Match(s.Data())!=0){
	       fListBoxOF->AddEntry( totname, next++ );
	     }
	     if (TPRegexp("\\.sqlite$").Match(s.Data())!=0){
	       fListBoxOF->AddEntry( totname, next++ );
	     }
	   }//while pent
	 
	 // finally, let's close the directory
	 closedir (pdir);
	 //-----------------------stage 2 start---------------
	 //THIS PART I SEE WHEN I DO OPENFILE: ::::
	 ifstream myReadFile;
	 printf("going to open file  REMOTE_DATA_DIR to append additional files from remote repo%s\n", "");
	 myReadFile.open("REMOTE_DATA_DIR");
	 char output[300];
	 if (myReadFile.is_open()) {
	   while (!myReadFile.eof()) {    
	     myReadFile >> output;    cout<<output; 
	   }
	 }else{
	   sprintf(output,"/tmp/shspe");//SOLVES THE CRASH ****openfile*** problem(there is anotherREMOTE_DATA_DIR)
	   output[0]='\0';//
	 }
	 myReadFile.close();

	 //------------------file path is clear now-----------------
	 if ( strlen(output)>0){
	   sprintf(path, "%s",  output );
	   pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
	   pdir = opendir (path); // "." will refer to the current directory
	   pent = NULL;
	   if (pdir == NULL) // if pdir wasn't initialised correctly
	     {     printf ("\nERROR! pdir could not be initialised correctly");  return;  } 
	   while (  (pent = readdir (pdir))   ) // while there is still something in the directory to list
	     {
	       if (pent == NULL) 
		 { 
		   printf ("\nERROR! pent not init.correctly");
		   return; // exit the function
		 }
	       
	       TString s=pent->d_name;
	       char totname[500];
	       sprintf(totname,"%s%s", "~" , pent->d_name ); 
	       printf("%03d : %s\n" , next, totname );
	       if (TPRegexp("\\.root$").Match(s.Data())!=0){
		 fListBoxOF->AddEntry( totname, next++ );
	       }
	       if (TPRegexp("\\.sqlite$").Match(s.Data())!=0){
		 fListBoxOF->AddEntry( totname, next++ );
	       }
	       if (TPRegexp("\\.mysql$").Match(s.Data())!=0){
		 fListBoxOF->AddEntry( totname, next++ );
	       }
	     }//while pent
	   closedir (pdir);
	   //------------------------stage 2  end -------------------
	 }//   strlen(output)>0   Solves the crash and avoids to /tmp
	 //this part was the case of REMOTE_DATA_DIR
	 
	 //	 printf("! after remote\n%s","");

	 fListBoxOF->SortByName();
	 /*
	  *  RESIZE THE DIRECTORY BOCX TO MAXIMUM, flistbox2 to min
	  */
	 printf("RESIZING fListBoxOF%s\n","");
	 fListBox2->Resize(1,listbox2_vsize);  // was flistbox and worked...
	 fListBoxOF->Resize(140 , listbox2_vsize );  // BY DEFAULT  
	 //	 Resize(GetDefaultSize());
	 Layout();//important


	 //newfentry.....       }// PURE --OpenFile-- , no text..........
	 //       printf("NO FILE like  <%s> WAS FOUND\n", fentry->Data() );
     }// getselected == 1 .................

     //     printf("! after getselected==1\n%s","");

     /*
      *
      *   NEXT LINES------------------------------ OPENFILE NOT clicked ----------- >>>>>>>>>>>>
      * 
      */
      if (fListBox2->GetSelected()>1 ){ 
	//   printf("testing .... file when getselected!=1\n%s", "");
	//             fOpenFile(fentry,fListBox2); 
	     fDisplayFromList2( fListBox2->GetSelected() ,
				//				"" );
	                        fListBox2->GetSelectedEntry()->GetTitle(), fChk1->GetState()  );

      }// some item from flistbox2 ==display
   }//if id 101 (LISTBOX 2 => read histo









   ////////////////////////////////////////////////////////////////////////////////
   // HANDLE BUTTONS HERE - Part of HandleEvents,  the rest are buttons //
   //////////////////////////////////////////////////////////////////////////////
   
   if (id==71){ // W - fit or proceed CMD
     printf("Checkbox %d clicked state=%d\n", 
	    id,  fChk1->GetState()  );
     //if ( fChk1->GetState()==1 ){printf("All TPads are on\n","");}
     //     fChk1->GetState();
   }// id==1 (check) 

   if (id==1){ // W - fit or proceed CMD
       Movexy("Y", 0.18, 1.0 );
   }// id==1 (W) 
   // ----------------    MOVE AND ZOOM/UNZOOM
   if (id==2){ // A left
      Movexy("X", -0.18, 1.0 );
   }
   if (id==3){ // S down
       Movexy("Y", -0.18, 1.0 );
   }
   if (id==4){ // D right
        Movexy("X",  0.18, 1.0 );
   }
   if (id==5){ // E  unzoomy  not ok
     //     Movexy("Y", -0.1, 1.0 );   // was -0.1, 1.0
     //     Movexy("Y", 0.2, -1.0 );   // was -0.1, 1.0
     Movexy("Y", -0.25, -1.0 );   // was -0.1, 1.0
   }
   if (id==6){ // Z zoomx
        Movexy("X", 0.2, -1.0 );
   }
   if (id==7){ // X unzoomx
        Movexy("X", -0.25, -1.0 );
   }
   if (id==8){ // C zooY  zoomy    ok
       Movexy("Y", 0.2, -1.0 );
   }
   if (id==9){ //   NO 9 
     //        Movexy("Y", -0.25, -1.0 );
   }
  

   //     printf("exited handle events %s\n","");
}// HandleEvents = process ======================================================




//  doesnot link functions to .so  #include "kibbler_graphs.C"

//  SEE    http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=7936

/*
By default ACLiC generate the dictionary for __only__ the symbol defined in the script and _any_ include header with the same name. So when your script is named testMacro, it generates the dictionary for the classes defined in testMacro.C and testMacro.h. When your script is named analysisMacro, it generates the dictionary for the classes defined in analysisMacro.C and analysisMacro.h. 

In both case, the error are of the style: " undefined reference to `sciGeometry::ShowMembers(TMemberInspector&, char*)'" which indicates that you have a ClassDef macro for the class sciGeometry (this is a good thing) but you have not (well ACLiC did not) generate the dictionary for the class sciGeometry. To solve the problem once and for all, just add the following lines at the end of your .C file:
CODE: SELECT ALL
#ifdef __MAKECINT__
#pragma link C++ class sciGeometry+;
#pragma link C++ class amplificationManager+;
#endif
which will insure that ACLiC generates the dictionary for your classes no matter what the name of the script and header files are. 

 */










void grhelp2(){
  printf("GRHELP FROM h/C  %s\n","");
}

//====================================================================
  // 
 //
//
  //  L O A D G R A P H 
  // 
  // 
  // 
  // -------------------------------------------------------------------
//


//=============================================================
//           HELP
//=============================================================
void grhelp ()
{
 cout<<"====================================================GRHELP\n";
 cout<<"---call:"<<endl;
 cout<<"     .L loadgraph.C;"<<endl;
 // cout<<" o) x  y "<<endl;
 cout<<" o) x  y      TGraph *ccc=(TGraph*) gr2(\"filename\")"<<endl;
 cout<<" o) x  y dy   TGraphErrors *cr = (TGraphErrors*)gr3(\"fname\")"<<endl;
 cout<<" o) x dx y dx TGraphErrors *gr4 = (TGraphErrors*)gr4(\"fname\")"<<endl;
 cout<<" o) TGraphErrors *gr4 = (TGraphErrors*)gr_engine(\"fn\",0,2,1,3)"<<endl;
 cout<<" ----- cut or graph MANIPULATION---  TGraph, TCutG.. "<<endl;
 cout<<" o)  gr_printval( ccc )                   ...  prints values"<<endl;
 cout<<" o)  gr_transfy(TGraph *gg, slope, shift) ... shifts the y values\n";
 cout<<" o)  gr_transfx(TGraph *gg, slope, shift) ... shifts the x values\n";
 cout<<" o)  gr_sort(TGraph *gg) ... sorts the graph by x values\n";
 cout<<" o)  TGraphErrors *gg= (TGraphErrors*)gr_revert(gg) ... revert x,y\n";
 cout<<" o)  gr4->ysubx( gr4 )       ...  y'=y-x, y-errors reevaluated\n";
 cout<<" o)  TGraphErrors *crr =(TGraphErrors*)gr_fitpol(cr,\"pol1\"[,range])\n";
 cout<<"  --------this returns differences in crr; similar to clbr  \n";
 cout<<"         and gives fancy formulae.\n";
 cout<<"   \n";
 cout<<" x) ========================= Typical use for calibrations:\n";
 cout<<"   TGraphErrors *gg1 = (TGraphErrors*)gr3(\"fname\")\n";
 cout<<"   TGraphErrors *gg2= (TGraphErrors*)gr_revert(gg1)\n";
 cout<<"   TGraphErrors *gg3 =(TGraphErrors*)gr_fitpol(gg2,\"pol1\")\n";
 cout<<"   gPad->Clear()\n";
 cout<<"   gg3->SetMarkerStyle(22);    gg3->Draw(\"pawl\")\n";
 cout<<"   ####  or use instead ###  gr_stdcalproc(\"fname\")\n";
 cout<<"   \n";
 cout<<"   \n";
 cout<<"   fbx_savefit( (TF1*)gg2->GetFunction(\"pol1\") , \"fname.res\" )\n";
 cout<<"   \n";
 cout<<"   \n";
 cout<<"===================== OTHER TRICKS...============-\n"; 

 //??gr_calibrate( TH1F  *hh,  double slope, double bias)
 cout<<"#make calibrated X axis      ---------------------"<<endl;
 cout<<"      gr_calibrate(  hh,   slope,  bias)"<<endl;
 cout<<"#make canvas place for graphs ---------------------"<<endl;
 cout<<"    TH2F *hh =new TH2F(\"hh\",\"hh\",100,0,250,100,0,5000)"<<endl;
 cout<<"    hh->SetStats(0);hh->Draw()"<<endl;
 cout<<""<<endl;
 cout<<"#convert CUT to TGraph -------------------------"<<endl;
 cout<<"  TGraph *g=(TGraph*)gr_cut2gr(CUTG)"<<endl;
 cout<<"  g->Draw(\"pl\")"<<endl;
 cout<<""<<endl;
 cout<<"#convert TGraph to function --- histogram creation ----------"<<endl;
 cout<<"  gr_mkfun(g, 23,231)        ...(graph shoul be sorted)"<<endl;
 cout<<"  TH1F *h1=new TH1F(\"h1\",\"h1\",1000,25,220)"<<endl;
 cout<<"  h1->FillRandom(\"fif_graffun\",10000)"<<endl;
 cout<<""<<endl;
 cout<<"#convert TGraph to function (Spline) --- histogram creation-----------"<<endl;
 cout<<"  TSpline3 *sp=(TSpline3*)gr_mksplfun(g, 23,231)"<<endl;
 cout<<"  sp->SetLineColor(4);sp->Draw(\"same\");"<<endl;
 cout<<"  TH1F *h1=new TH1F(\"h1\",\"h1\",1000,25,220)"<<endl;
 cout<<"  h1->FillRandom(\"fif_splinefun\",10000)"<<endl;
 cout<<"#convert TGraph to  HISTO.(+read 1 ascii spe)----------------- "<<endl;
 cout<<"  TGraphErrors *gr4 = (TGraphErrors*)gr_engine(\"ctp.dat\",0,0,0,0)  "<<endl;
 cout<<"  #for (int i=0;i<gr4->GetN();i++){ gr4->SetPoint( i, 1.0*i,   1.0*gr4->GetY()[i] );} "<<endl;
 cout<<"  TH1F *h=new TH1F(\"h\",\"\",8192,0,8192);for(int i=0;i<gr4->GetN();i++){ h->SetBinContent(i+1,gr4->GetY()[i]);}"<<endl;
 cout<<"#HISTO 2 asci   "<<endl;
 cout<<" for (int i=1;i<CLhisto00->GetXaxis()->GetNbins();i++){ printf(\"%d\\n\",CLhisto00->GetBinContent(i) );} > spe.asc "<<endl;
 // cout<<"   "<<endl;
 cout<<"#convert TGraph to  HISTO.2----------------------------------- "<<endl;
 cout<<"  TH1F *h=new TH1F(\"h\",\"\",4096,0,6e+6);for (int i=0; i<ccc->GetN(); i++){  h->Fill(ccc->GetY()[i]);} "<<endl;
 cout<<"#creat HISTO from file with one column-------------------------------- "<<endl;
 cout<<"  TGraphErrors *onecol = (TGraphErrors*)gr_engine(\"histo1.dat\",-1,0,-1,-1)"<<endl;
 cout<<"  TH1F *h=new TH1F(\"h\",\"\",4096,0,6e+6);for (int i=0; i<onecol->GetN(); i++){  h->Fill(onecol->GetY()[i]);} "<<endl;
 cout<<"#ZERO ERRORS:-----------------------------"<<endl;
 cout<<"  for (int i=0;i<gr4->GetN();i++){ gr4->SetPointError(i,0.,0.);}"<<endl;
 cout<<""<<endl;
 cout<<"#TGraph from ntuple (aka from TTree)-------------------------------"<<endl;
 cout<<"    c01->Draw(\"d1:d2\",\"\",\"col\",100)"<<endl;
 cout<<"    TGraph *g=new TGraph( c01->GetSelectedRows() , c01->GetV2(), c01->GetV1() )"<<endl;
 cout<<"    g->Draw(\"pa\")"<<endl;
 cout<<""<<endl;

}





void gr_calibrate( TH1F  *hh,  double slope, double bias){
  //  H1b1->SetBins(8192,-71.98, -71.98 + 0.427sn6*8192 );

  int  bins= hh->GetNbinsX();
  hh->SetBins( bins  , bias , bias +  slope * bins );


}// gr_calibrate




void gr_help() {grhelp();}





//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//=============================================================
//            ENGINE READ 4 COLUMNS   give -1 if  to be  0
//=============================================================
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//  version 2010 04 07 - can read quite a mess...
//      taken 20100813 from Root/03_programs/53_simul_SRIM/losslib2.h
// 


int64_t gr_engine (const char* name, int rx, int ry, int rdx, int rdy)
{
 double x[64000],y[64000],dy[64000],dx[64000],  bu;
 int i,j;
 FILE * pFile;
 int MAXLINES=64000;
 // char mystring[1500];// one line
 TString oneline, title=name, token;

  printf("gr_engine: going to open filename=%s\n", name );

  pFile=fopen( name ,"r" ); 
  if (pFile==NULL) {
    printf("cannot open %s,STOPping\n", name ); 
    return 0;
  } // error
  //  printf("file opened\n%s","");
  //.......................readout HERE.......i
  i=0;
  int lastlen;// remove spaces
  while ((i<MAXLINES)&&( feof(pFile)==0) ){
    //    printf("  reading line %d...\n", i );
    if ( oneline.Gets (pFile, kTRUE) ){//chop true ... continue if not eof
      //      printf("     string==%s\n",  oneline.Data() ); 

    //purify .................. start
    do {
      lastlen=oneline.Length();
      if (oneline.Index(" ")==0){oneline=oneline(1,oneline.Length()-1);}
      lastlen=oneline.Length();
      if (oneline.Index(" ")==lastlen){oneline=oneline(0,oneline.Length()-1);}
       oneline.ReplaceAll("\t"," ");
       oneline.ReplaceAll("  "," ");
       oneline.ReplaceAll("  "," ");
       //       printf(" ---    string==<%s> (%d)\n",  oneline.Data(), lastlen ); 
    }while( lastlen!=oneline.Length());
    //    printf("     string==<%s> (%d)\n",  oneline.Data(), lastlen ); 
    //purify .................. stop
    //printf("S=%s\n", oneline.Data() );
   //........ parse oneline
    if (  (oneline.Index("#")==0) || 
	  (oneline.Index("@")==0) ||
	  (oneline.Index("END")==0) 
 ){ // starts with # - COMMENT HERE
      //       printf( "COMM:%s\n", oneline.Data() );
      if (title.Length()==0){ title=oneline( 1,oneline.Length()-1  );}
     //     title=oneline( oneline.Length(),oneline.Length()-1  );
  }else{// DATA HERE
      TObjArray *tar; 
     if (oneline.Length()>1){
       if (oneline.Index("#")>0){oneline.Remove(oneline.Index("#") );}// #
       if (oneline.Index("@")>0){oneline.Remove(oneline.Index("@") );}// #
       if (oneline.Index("END")>0){oneline.Remove(oneline.Index("END") );}// #
       //       oneline.ReplaceAll(oneline.Index("\t"),1," ");
       oneline.ReplaceAll("\t"," ");
      x[i]=0; y[i] =0;dx[i]=0;dy[i]=0; j=0;  //  go through the columns 
      // printf( "NotCM:%s\n", oneline.Data() );

      /*
       * r?? contains a column to use for x,dx,y,dy:
       */
      tar= oneline.Tokenize(" ");
      //      printf("entries==%d\n", tar->GetEntries() );
      while( (j<=rx && rx>=0) || (j<=ry && ry>=0) || (j<=rdy && rdy>=0)|| (j<=rdx && rdx>=0)){  
	if (j<tar->GetEntries()){
         token= ((TObjString*)(tar->At(j)))->GetString();
	 //	 printf(" %3d %3d  token <%s>\n", i,j,  token.Data() );
	 bu= token.Atof(); 
	 if (i<5){ 	 printf("  token <%s> = %f\n", token.Data(), bu ); }
         if (rx==j)  { x[i] =bu;}
         if (ry==j)  { y[i] =bu;}
         if (rdx==j) { dx[i] =bu;}
         if (rdy==j) { dy[i] =bu;} 
	}//j<entries
         j++;
      }//small while 

     //tar->Delete();  // abandon tar 
      i++;// skip to a next datum if point
     }// if oneline  lenght>1 go thru all
     //back the loop
   }//DATA HERE end
    //     printf("%s","data line ended\n");


    }//if oneline.Gets false
   
   //........ parse oneline
  }//while
  //.......................readout HERE end....i
  //.......................close, create the TGraphErrors
  fclose( pFile);
  // TITLE HERE----------------
  //  title=name; DONE earlier
  title.ReplaceAll(".","_"); 
  title.ReplaceAll(" ","_"); 
  printf("%d elements read. title= /%s/\n", i-1, title.Data() );
 TGraphErrors *g=new TGraphErrors(i,x,y,dx,dy);
 g->SetMarkerStyle(22);
 g->SetTitle( title.Data()  );
  g->SetName( title.Data()  );
// g->Print();// DONT PRINT TABLES

 //========================ADDED 2013: AUTO!push to getlistofspecials (4multigraph)
 gROOT->GetListOfSpecials()->Add( g );
 printf("added to GetListOfSpecials:   %s \n",  g->GetName()  );
 //=========================


 return (int64_t)g;
 //-----------------------------------------------------
}// end of the new 201004 version of gr_engine






//=============================================================
//            print  Graph, cutg   values
//=============================================================
void gr_printval (TGraph *gg)
{
 Int_t i,imax;
 imax=gg->GetN();
 for (i=0;i<imax;i++){
    cout<<gg->GetX()[i]<<"   "<<gg->GetY()[i]<<endl;
 }
 cout<<"   "<<imax<<" values printed"<<endl;
}

void gr_saveval (TGraph *gg, char *filename)
{
 Int_t i,imax;
 //unused  TGraphErrors *ge;
 //unused TGraph *gr;
 if ( strcmp(gg->Class_Name(),"TGraph")== 0){ }
 if ( strcmp(gg->Class_Name(),"TGraphErrors")== 0){ }

 imax=gg->GetN();

 ofstream fo;
 fo.open(filename,ios::out);
 cout<<"# saved "<<" Graph name="<<gg->GetName()<<"  to file="<<filename<<endl;
 for (i=0;i<imax;i++){
    cout<<gg->GetX()[i]<<"   "<<gg->GetY()[i]<<endl;
    fo<<gg->GetX()[i]<<"     "<<gg->GetY()[i]<<endl;
 }
 cout<<"   "<<imax<<" values printed and saved to "<<filename<<endl;

 fo.close();
}






//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//             one point and in x ... channel number
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//    gr1
TGraphErrors* gr1 (char *name){
   cout<<"going to create graph 1 col."<<name<<"\n";
   TGraphErrors  *gr=(TGraphErrors*) gr_engine(name,-1,0,-1,-1);
   for (int i=0;i<gr->GetN();i++){
     gr->GetX()[i]=1.0*i;
   }
    TString n=name;n.ReplaceAll(".","_"); gr->SetName(n);   gDirectory->Add( gr );
   return gr;
}// 

//=============================================================
//            two points in file
//=============================================================
TGraphErrors*  gr2(char* name)
{
  int64_t res;
  printf("going to create graph 2 col. from  \"%s\"  file\n",name);
  //   res=qgr_engine(name,0,1,-1,-1);
   res=gr_engine(name,0,1,-1,-1);
   if (res==0){ return 0;}
   TGraphErrors  *gr=(TGraphErrors*)res;
   //   printf("gr inside gr2=%d\n", (int)gr);
   //   gr->Draw("pawl");
   //    TString n=name;n.ReplaceAll(".","_"); IN ENGINE 
   //    gr->SetName(n);
    gDirectory->Add( gr );
   return gr;
   // NEW VERSION COMPILES....
   //   cout<<"going to create graph 2 col."<<name<<"\n";
   //   TGraph  *gr=(TGraph*) gr_engine(name,0,1,-1,-1);
   //   //   gr->Draw("pawl");
   //   return gr;
}


//=============================================================
//            three points in file
//=============================================================
TGraphErrors*  gr3 (const char* name)
{
   cout<<"going to create graph 3 col."<<name<<"\n";
   TGraphErrors  *gr=(TGraphErrors*) gr_engine(name,0,1,-1,2);
   printf(" TGraphErrors *gg2= (TGraphErrors*)gr_revert(gg1)  \n%d\n" ,1);
   //    TString n=name;n.ReplaceAll(".","_"); IN ENGINE 
   //    gr->SetName(n);
    gDirectory->Add( gr );
   return gr;
}


TGraphErrors*  gr4 (char* name)
{
   cout<<"going to create graph 4 col. - x y dx dy"<<name<<"\n";
   TGraphErrors  *gr=(TGraphErrors*) gr_engine(name,0,1,2,3);
   //   TString n=name;n.ReplaceAll(".","_");  IN ENGINE 
   //   gr->SetName(n);
   gDirectory->Add( gr );
   return gr;
}

TGraphErrors*  gr4a (char* name)
{
   cout<<"going to create graph 4 col. - x dx y dy"<<name<<"\n";
   TGraphErrors  *gr=(TGraphErrors*) gr_engine(name,0,2,1,3);
   //   TString n=name;n.ReplaceAll(".","_");    IN ENGINE 
   //   gr->SetName(n); 
   gDirectory->Add( gr );
   return gr;
}



//=============================================================
//        SHIFT  graph
//=============================================================
int gr_transfy (TGraphErrors *gg,double slope, double diff)
{
 Int_t i,imax;
 //--------------------- graph: and NEW GRAPH
 Double_t x[MAXPNTS],y[MAXPNTS];//,dx[MAXPNTS],dy[MAXPNTS];
 double ytemp,ytemp2;
 imax=gg->GetN();
 for (i=0;i<imax;i++){
    x[i]= gg->GetX()[i];
    ytemp=gg->GetY()[i];
    ytemp2=slope*ytemp+diff;
    //  WHA IS THIS?????   comparison of the fit ???? 
    //    y[i]= ytemp-ytemp2; 
    y[i]= ytemp2;
    //    printf("%.9f - %.9f = %.9f\n",  ytemp, ytemp2,  y[i] );
    gg->SetPoint(i,x[i],y[i]);
 }
// cout<<"   "<<imax<<" "<<endl;
// TGraph *gn=new TGraph(imax,x,y,dx,dy);
// return gn;
 return 1;
}


int gr_transfx (TGraphErrors *gg,float slope, float diff)
{
 Int_t i,imax;
 //--------------------- graph: and NEW GRAPH
 Double_t x[MAXPNTS],y[MAXPNTS]; //,dx[MAXPNTS],dy[MAXPNTS];
 imax=gg->GetN();
 for (i=0;i<imax;i++){
   //    x[i]= slope*gg->GetX()[i]+diff;
    x[i]= slope*gg->GetX()[i]+diff;
    y[i]= gg->GetY()[i];
    gg->SetPoint(i,x[i],y[i]);
    //    cout<<i<<" "<<x[i]<<"  "<<y[i]<<endl;
 }
// cout<<"   "<<imax<<" "<<endl;
// TGraph *gn=new TGraph(imax,x,y,dx,dy);
// return gn;
 return 1;
}




TGraphErrors* gr_revert (TGraphErrors *gg)
{
 Int_t i,imax;
 //--------------------- graph: and NEW GRAPH
 Double_t x[MAXPNTS],y[MAXPNTS],dx[MAXPNTS],dy[MAXPNTS];
 imax=gg->GetN();
 for (i=0;i<imax;i++){
    y[i]= gg->GetX()[i];
   dy[i]= gg->GetEX()[i];
    x[i]= gg->GetY()[i];
   dx[i]= gg->GetEY()[i];
 }
 cout<<"   "<<imax<<" x<->y (and dx,dy) values reverted to the new graph"<<endl;
 TGraphErrors *gn=new TGraphErrors(imax,x,y,dx,dy);
 printf(" TGraphErrors *gg3 =(TGraphErrors*)gr_fitpol(gg2,\"pol1\") %s\n", " ");
 printf(" TGraphErrors *gg3 =(TGraphErrors*)gr_fitpol(gg2,\"pol2\") \n%d\n", 1);
 return gn;
}




//=============================================================
//               gr_fitpol
//     fit with function (pol0,pol1 etc), print output in human form
//=============================================================







//----------------------------------------------------------
//      gr_fitpol    GraphErrors  Ranges !!!!!! (VAL-FIT) ERRORS!?!?
//
//      returns:
//              Y_value   -   fitted_value :  units on y-axis remain
//
//----------------------------------------------------------
// ---if using arrays=>>>  CAST!
// ---- grrf[i]=(TGraphErrors*)gr_fitpol((TGraphErrors*)grr[i],"pol1");

TGraphErrors* gr_fitpol (TGraphErrors *gg, const char *cpol, Float_t x1, Float_t x2)
{
 Int_t i,imax,npar;
 char txt[1000],val[50],txta[1000];

//  gg->Fit(cpol,"","",x1,x2);
// BEFORE 070910  gg->Fit(cpol,"","",x1,x2); 
// after 070910 use minuit for poly "F"
// 0 dont plot
  gg->Fit(cpol,"F0","",x1,x2);
 TF1 *fit=(TF1*)gg->GetFunction(cpol);
double p1=fit->GetParameter(1); // take 1st degree coef. 
 printf("par1=%f\n", p1 );
 //--------------------- graph: and NEW GRAPH ---->>> transform errors from x to y
 Double_t x[MAXPNTS],y[MAXPNTS],dx[MAXPNTS],dy[MAXPNTS];
 imax=gg->GetN();
 for (i=0;i<imax;i++){
    x[i]= gg->GetX()[i];
//    dx[i]=gg->GetEX()[i];
    dx[i]=0;
//-----------------------  get difference----
    //NE TED!!potom  y[i]= gg->GetY()[i] - fit->Eval( gg->GetX()[i] );
  y[i]= gg->GetY()[i] ;
//-------------------------- JUST TO SEE MAGNIFICATION _ DONT PUT ERRORS FIT+dY TOGETHER
//-------------------------- PUT TOGETHER dX and dY via linear coef
  double qa= pow(gg->GetEY()[i],2.0);
  double qb=pow(p1*gg->GetEX()[i],2.0);
    dy[i]=sqrt( qa+qb );
 }
 cout<<"  new graph with "<<imax<<" values made"<<endl;

 TGraphErrors *gn=new TGraphErrors(imax,x,y,dx,dy);
 //#######  NOW the real, valid fit! ####
  gn->Fit(cpol,"F0","",x1,x2);




  // TF1 *fit=(TF1*)gn->GetFunction(cpol);
 fit=(TF1*)gn->GetFunction(cpol);
 Double_t chi=fit->GetChisquare();
 printf("Chi2 == %f\n", chi);
 //------------get parameters --- not needed...
 strcpy(txt,"Double_t         (Double_t x){ double r;\n  if (x>");
 strcpy(txta,"Double_t         (Double_t *x,Double_t *p) { double r;\n if (x[0]>");
 sprintf(val,"(%e)",x1);
 strcat(txt,val);
 strcat(txta,val);
 strcat(txt," && x<");
 strcat(txta," && x[0]<");
 sprintf(val,"(%e)",x2);
 strcat(txt,val);
 strcat(txta,val);
 strcat(txt,") \n  r=");
 strcat(txta,") \n  r=");


 Double_t p[9];
 //Double_t p[9] ;
 npar=fit->GetNpar();
 for (i=0;i<npar;i++){
   p[i] =fit->GetParameter(i);
   //   dp[i]=fit->GetParError(i);
//   cout<<p[i]<<" .... "<<i+1<<"/"<<npar<<"    "<<chi<<endl;
   sprintf(val,"(%e)",p[i]);    strcat(txt,val); strcat(txta,val);
   for (int j=0;j<i;j++){
      strcat(txt,"*x");
      strcat(txta,"*x[0]");
   }
   if (i<npar-1) {strcat(txt,"+"); strcat(txta,"+");}
 } 
 //-------------------------------  print values in nice form
 strcat(txt,";\n return r;\n}");
 strcat(txta,";\n return r;\n}");
 cout<<txt<<"\n\n"<<txta<<endl;
 cout<<"TF1 *=new TF1(\"\",,"<<x1<<","<<x2<<",0);"<<endl;

//----------- For fast rebin
 if (npar==2){
  sprintf(txt,"->SetBins(16384,%e, %e+(%e)*16384)",p[0],p[0],p[1]);
  cout<<" for rebin with 16384 bins ( ->GetNbinsX() )... \n   "<<txt<<endl;
 }


 //--------------------- graph: and NEW GRAPH
 // Double_t x[MAXPNTS],y[MAXPNTS],dx[MAXPNTS],dy[MAXPNTS];
 imax=gn->GetN();
 for (i=0;i<imax;i++){
   //OK    x[i]= gn->GetX()[i];
   //     //    dx[i]=gn->GetEX()[i];
   gn->SetPointError(i, 0.0, gn->GetEY()[i] );
   gn->SetPoint(i,gn->GetX()[i] ,    gn->GetY()[i] - fit->Eval( gn->GetX()[i] )    );
//-----------------------  get difference----
//    y[i]= gn->GetY()[i] - fit->Eval( gn->GetX()[i] );
//-------------------------- JUST TO SEE MAGNIFICATION _ DONT PUT ERRORS FIT+dY TOGETHER
//-------------------------- PUT TOGETHER dX and dY
//    dy[i]=gn->GetEY()[i];
//    dy[i]=sqrt( (gn->GetEY()[i])**2+(fit->Derivative( x[i] )*gn->GetEX()[i])**2  );
//already done earlier...     dy[i]=sqrt( (gn->GetEY()[i])**2+(gg->GetEX()[i])**2  );
 }
 cout<<"  new graph with "<<imax<<" values made"<<endl;
 // again - new?
 // TGraphErrors *gn=new TGraphErrors(imax,x,y,dx,dy);
 printf(" gPad->Clear();\n  gg3->SetMarkerStyle(22);    gg3->Draw(\"pawl\")  \n%d\n" , 1 );
 return gn;
}



//---------------------------------------------- GraphErrors no range
TGraphErrors* gr_fitpol (TGraphErrors *gg, const char *cpol)
{
TGraphErrors *tg;
tg=(TGraphErrors*)gr_fitpol(gg,cpol,  gg->GetX()[0],    gg->GetX()[gg->GetN()-1]   );
return  tg;
}



void  gr_stdcalproc(const char *fname){

  TGraphErrors *gg1 =(TGraphErrors*)gr3(fname);
  TGraphErrors *gg2= (TGraphErrors*)gr_revert(gg1);
  TGraphErrors *gg3 =(TGraphErrors*)gr_fitpol(gg2,"pol1");
  gPad->Clear();
  gg3->SetMarkerStyle(22);    gg3->Draw("pawl");

}














/*****************************************************************************
 *  USEFUL FOR JOINING TGRAPHS INTO MULTIGRAPH........................
 *
 */
//========================================================================
void joingraphs(const char* myname, const char* g1 , int autocolors=1 ){

TMultiGraph *mg;

 if (  ( gROOT->GetListOfSpecials()->FindObject(myname) )  ||
       ((gPad!=NULL)&&(gPad->FindObject(myname) ))  ){
   mg=(TMultiGraph*)gROOT->GetListOfSpecials()->FindObject( myname );
   if (mg==NULL){mg=(TMultiGraph*)gPad->FindObject( myname );}
   printf("TMultiGraph %s found...\n",myname );
 }else{
   printf("TMultiGraph %s created\n",myname );
  mg=new TMultiGraph();
  mg->SetNameTitle(myname,myname);
  gROOT->GetListOfSpecials()->Add( mg );
 }
 TGraphErrors *o;
 o=(TGraphErrors*)gROOT->GetListOfSpecials()->FindObject( g1 );
 if (o==NULL){ o=(TGraphErrors*)gDirectory->FindObject( g1 ); }
if (o==NULL){
  printf("graph %ld NOT found...\n", (int64_t)g1 );
 }else{

  printf("gettin entries %s\n", "");
  int ent=0;
  if ( mg->GetListOfGraphs()!=NULL){
  ent=mg->GetListOfGraphs()->GetEntries();
  }
  //  ent=1;
  printf("gettin entries %d\n", ent);

  if (autocolors==1){
    printf("setting autocolor %d\n",  ent);
    o->SetLineColor(ent+1);
    o->SetMarkerColor(ent+1);
  }else{
    printf("NO autocolor (graphs=%d)\n",  ent);
  }
  printf("Graph %s added\n", o->GetName()  );
  mg->Add(  (TGraphErrors*)o  , "PL"  )  ;
 }//graph found?

 //  gROOT->GetListOfSpecials()->Add(  gROOT->GetListOfSpecials()->FindObject( g1 )   );
 //// for (int i=0;i<imax;i++){  mg->Add( gg[i],"lp");  }

}////========== void joingraphs(const char* myname, const char* g1 ){ ================













//==================================================================MPad
//==================================================================MPad
//==================================================================MPad
//==================================================================MPad
/***********************************************************************
 *  GetPadByName      returns  pointer to pad   OR  NULL -------general func
 *
 */
//===================================TPad* GetPadByName(const char* name){
TPad* MPadGetByName(const char* name){
   TPad *MyGPad=NULL;

  if (gROOT->GetListOfCanvases()->GetEntries()==0){
    /*
    new TCanvas();
    gPad->SetTitle( name );
    gPad->SetName( name );
    MyGPad=gPad;
    printf("(GetPadByName) %s was defined %d\n" , name, MyGPad  );
    */
    printf("(MPadGetByName) %s NOT found \n" , name  );
    return NULL;

  }else{
    for (int i=0;i<gROOT->GetListOfCanvases()->GetEntries();i++){
      TPad *cpad=(TPad*)(gROOT->GetListOfCanvases()->At(i));
      //      printf("canvas %d/%d\n" , i, gROOT->GetListOfCanvases()->GetEntries() );
      //1st Just tpad0.....

	  if (strcmp(cpad->GetName(),name)==0){
	    MyGPad=(TPad*)cpad;
	    //	    printf("%s was found here in (ROOT): %d\n" , name,  MyGPad  );
	  }//if name is good

      
      for (int j=0;j<cpad->GetListOfPrimitives()->GetEntries();j++){
	if ( strcmp(cpad->GetListOfPrimitives()->At(j)->ClassName(),"TPad")==0){
	  //	  printf("TPad %s looked j classname  %d/%d\n" ,  cpad->GetListOfPrimitives()->At(j)->GetName() , j, cpad->GetListOfPrimitives()->GetEntries());

	  if (strcmp(cpad->GetListOfPrimitives()->At(j)->GetName(),name)==0){
	    MyGPad=(TPad*)cpad->GetListOfPrimitives()->At(j);
	    //	    printf("%s was found here in j==%d: %d\n" , name, j, MyGPad  );
	  }//if name is good

	}// is TPad
      }// for j
    }//for i
  }//else-there were pads....
  if (MyGPad==NULL){

    printf("(MPadGetByName) %s NOT found\n" , name  );
    return NULL;
    /*     new TCanvas();
    gPad->SetTitle( name );
    gPad->SetName( name );
    MyGPad=(TPad*)gPad;   
    printf("%s was NOT found anywhere in pads, we define new %d\n" , name, (int)MyGPad  );
    */
  }//
  return MyGPad;
}// GetPadName ===============================================================






/****************************************************************
 *             Prints a text into PAD         -------general func
 *
 *
 */
//==========void PrintInPad(const char* label, const char* value){

void MPadPrintIn(const char* label, const char* value){
   TPad *orig=NULL;
   if (gROOT->GetListOfCanvases()->GetEntries()>0){orig=(TPad*)gPad;}
   TPad *cpad=MPadGetByName( label );

 if (cpad!=NULL){
   cpad->Clear();
   TText *t1=new TText(0.5 ,0.5,     value );   // position 0.5,0.5; size 0.5
   t1->SetTextFont(43);  // 42 was helvetica prec 2,   3 is precision 3!!!!pixels!!!
   int w=cpad->GetWw();
   //       w=cpad->GetWindowWidth();
   int h=cpad->GetWh();
   t1->SetTextAlign(22);//  CENTER === 2*10 +2
   //   t1->SetY(0.1)
   // EMPIRICAL FORMULA FOR HELVETICA FONT //   
   double size=0.4*w/h/strlen(value)*7;
   // OK    size=0.4*w/strlen(value)/15;//  this is quite reasonable....
   size=0.4*w/strlen(value)/15;//  this is quite reasonable....
   if (size>1.26){ size=1.26;} // from empirical test of height (350 pix)

   // HACK 43 PIXELS ======================
   //   t1->SetTextSize( size ); 
   if (  w/strlen(value)/1.1 < h/1.2 ){
     t1->SetTextSize( w/strlen(value)/1.1  ); 
   }else{
     t1->SetTextSize( h/1.2  ); 
   }
   //   printf("bounding box  %d:%d  :%f\n", w ,h,  size);
   t1->Draw(); 
   if (orig!=NULL){orig->cd();}
 }//cpad not NULL
}//DisplayInPad==================================================================


void MPadPrintIn(const char* label, int slot, const char* value){
  char ch[100];
  sprintf(ch, "%s_%d", label, slot);
  MPadPrintIn( ch, value );
}



void MPadCreate(const char* LABEL, int columns, int rows){

  /*
   *  HERE I (CONDITIONALY) CREATE SET OF PADS ..............................
   */
  TCanvas *cc;
  if (MPadGetByName( LABEL )==NULL){
    cc=new TCanvas( LABEL,LABEL);
    cc->Divide(columns , rows, 0.002,0.002 );
    cc->GetCanvas()->SetFillColor(9);cc->GetCanvas()->SetFillStyle(1);
    cc->Draw();
  }else{
    TPad *pp=(TPad*)MPadGetByName( LABEL );cc=pp->GetCanvas();
  }

for (int j=0;j<columns;j++){
for (int i=0;i<rows;i++){
  char aa[15];  sprintf(aa,"%s_%d",LABEL,i+1);
 char bb[15]; 
 sprintf(bb, "kuku %03d", i  );
 // sprintf(bb, "0.%d%d%d bar", i, (int)gRandom->Uniform(9),(int)gRandom->Uniform(9)  );
 MPadPrintIn( aa,bb );
 }
 }//for loop -----------------
  
MPadGetByName( LABEL )->Update();

}//MPadCreate(==================================================================





















//=====================================================================COUNTERS OBJECTS
//=====================================================================COUNTERS OBJECTS
//=====================================================================COUNTERS OBJECTS


//-------------------------- class definition ----------------- 1
class TCounter{

 public:
// protected:
  // int is limited to  4e6 !
  int DEBUG;
  double markVO,  markV; // Value (integer)
  double markTO, markT;  // time
  double currT, currV;   // currents , waiting to 
 
  TCounter();
  ~TCounter();
  void Register(double weight=1.0);
  double GetRate();
  double GetTime();
  void SetDebug(int deb);
};



TCounter::TCounter(  )
{ 
  printf("...creating counter%s\n","");
  DEBUG=1;
  double now=GetTime();
  markVO=0.0;
  markTO=now - 1.0;
  markV=0.0;
  markT=now;  
  currT=now;
  currV=0.;
}

TCounter::~TCounter() { printf("...destroing counter%s\n","");}



// This makes it   in   seconds.............good for high rates
//              large error at low rates
double TCounter::GetTime(){

  //  TString sla;
  TDatime *dt=new TDatime();  // kTRUE previously...mess
  // sla.Append( dt->AsString() );//append date
  //  sla.Append( " : " );//append fname
  //  sla.Append( s_p.Data() );//append fname
  //  sla.Append( " : END" );//append openfile
  //  printf("%s\n",  sla.Data()  );
  return   1.0* dt->Get();
}// GetTime


double TCounter::GetRate(){
  return (markV-markVO)/(markT-markTO);
}

void TCounter::Register(double weight){
  double now=GetTime();
  currV=currV+weight;
  currT=now;
  if (currT - markT >= 1.0 ){
    markVO=markV;
    markTO=markT;
    markV=currV;
    markT=currT;
  }//if 1 sec
  if (DEBUG>0){ printf( "%09.3f ... %f/%f\n", GetRate() , currT, currV );}
  return;
}


void TCounter::SetDebug(int deb){
  DEBUG=deb;
}


void test(){
  int i,imax=2;
  TCounter *c[2];
  for (i=0;i<imax;i++){
   c[i]=new TCounter;
  }


  for (i=0;i<imax;i++){
    delete c[i];
  }

}





//-------------------------- class definition ----------------- 2
class TCounterMulti{

 public:
// protected:
  // int is limited to  4e6 !
  double markTO, markT;  // time
  double currT, currV;   // currents , waiting to 

  static const int max=10;
  int N;
  TCounter *mcounter[max];
  int i;


  TCounterMulti( int counters );
  ~TCounterMulti();
  void Register(int counter, double weight=1.0);
  void GetRate();
  double GetTime(); // same
  void Display();
};



double TCounterMulti::GetTime(){

  TDatime *dt=new TDatime();  // kTRUE previously...mess
  return   1.0* dt->Get();
}// GetTime


TCounterMulti::TCounterMulti( int counters  ){ 
  markT=GetTime();
  markTO=markT-1;
  currT=markT;

  N=max;
  if (N>counters){ N=counters;}
  printf("...creating multicounter, # of counters is %d\n", N);
  
  for (i=0;i<N;i++){
    printf(" %d)", i );
    mcounter[i]=new TCounter();
    //    mcounter[i]->SetDebug(0);
  }
  MPadCreate("COUNTERS",1,N);
}//------------------constructor


TCounterMulti::~TCounterMulti() { 
  printf("...destroing multicounter%s\n","");
    for (i=0;i<N;i++){
      delete mcounter[i];
  }
}//-------------------------destructor


void TCounterMulti::GetRate(){
    for (i=0;i<N;i++){
      //   printf("%d/%d - \n", i , N );
      //     if ((i>=0)&&(i<N) ) {
       printf("%9.2f ",mcounter[i]->GetRate( ) );
       //     }   
    }
    printf("%s","\n");
}//---------------------------

void TCounterMulti::Register(int counter, double weight){
  if ((counter>=0)&&(counter<N) ) {
    mcounter[counter]->Register( weight );
  }else{
    printf(" counter not in the predefined range (0-%d)\n", N );
  }
  currT=GetTime();
  if (currT>=markT+1){  markT=currT;   Display(); }// once per second autodisplay (max.)
  return;
}//----------------------------



void TCounterMulti::Display(){
  char c[40]; 
  //---taken from MySql  multipads2-----------
  TCanvas *cc;
  if (MPadGetByName( "COUNTERS" )==NULL){
    cc=new TCanvas( "COUNTERS","COUNTERS" );
    cc->Divide(1 ,N, 0.002,0.002 );
    cc->GetCanvas()->SetFillColor(9);cc->GetCanvas()->SetFillStyle(1);
    cc->Draw();
  }else{
    TPad *pp=(TPad*)MPadGetByName( "COUNTERS" );cc=pp->GetCanvas();
  }
  //---------------------------------------------

  // ===   IT CAN SEEM STOPPED === NO WAY TO SEE THE notcomming COUNTS ?????==
  // GetRate();   
    for (i=0;i<N;i++){
      //   printf("%d/%d - \n", i , N );
      //     if ((i>=0)&&(i<N) ) {
      sprintf(c, "%09.1f",mcounter[i]->GetRate( ) );
      MPadPrintIn("COUNTERS", i+1,  c );
       //     }   
    }
    printf("%s","\n");
}//---------------------------




//=====================================================================COUNTERS OBJECTS
//=====================================================================COUNTERS OBJECTS
//=====================================================================COUNTERS OBJECTS







