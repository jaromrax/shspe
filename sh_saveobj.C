//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "sh_saveobj.h"

//------------------- CODE everything here:


//===========================================================
//               save obj to file 
//==============================================================

void saveobj2asc(const char *filenam,  TH1* xobj){
  if ( strlen(filenam)==0){return;}
  FILE * pFile;
  int c,i,max;
  TString newfilename = filenam;
  newfilename.ReplaceAll(".txt","");
  newfilename.Append("_");
  newfilename.Append(xobj->GetName() );
  newfilename.Append(".txt");
  pFile=fopen( newfilename.Data()  ,"w" );
  if (pFile!=NULL) {
    max=xobj->GetNbinsX();
    for (i=0;i<max;i++){
      c=xobj->GetBinContent(i);
      fprintf(pFile,"%d %d\n",i,c);
    } //>alfa_run50_de6.txt

    fclose(pFile);
    printf("file /%s/ saved\n", filenam );
  }
  
}

void saveobj2asc1col(const char *filenam,  TH1* xobj){
  if ( strlen(filenam)==0){return;}

  FILE * pFile;
  int c,i,max;
  TString newfilename = filenam;
  newfilename.ReplaceAll(".asc","");
  newfilename.Append("_");
  newfilename.Append(xobj->GetName() );
  newfilename.Append(".asc");
  pFile=fopen( newfilename.Data()  ,"w" );
  if (pFile!=NULL) {
    max=xobj->GetNbinsX();
    for (i=0;i<max;i++){
      c=xobj->GetBinContent(i);
      fprintf(pFile,"%d\n",c);
    } //>alfa_run50_de6.txt

    fclose(pFile);
    printf("file /%s/ saved\n", filenam );
  }
  
}

void saveobj2file(const char *filenam,  TH1* xobj, int bkup=0){
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

  printf("Canvas name (.root not removed) == %s\n", s.Data() );
  char appcha[15]; 
  TString s_e=s;
  TString s_p=s;
  TString s_r=s;//root file:  but now i want to save spectra there
  TString s_asc=s;//ascii file:  but now i want to save spectra there
  TString s_asc1=s;//ascii file:  but now i want to save spectra there
  TString s_png=s;
    
    if ( s.Index(".root")>0 ){
      s_e="";
      s_p="";
      s_png="";s_asc="";s_asc1="";
      s.Replace( s.Index(".root")  ,  5 , "");s_r=s;
    }
    if ( s.Index(".ps")>0 ){
      s_e="";
      s_r="";
      s_png="";s_asc="";s_asc1="";
      s.Replace( s.Index(".ps")  ,  3 , "");s_p=s;
    }
    if ( s.Index(".eps")>0 ){
      s_p="";
      s_r="";
      s_png="";s_asc="";s_asc1="";
      s.Replace( s.Index(".eps")  ,  4 , "");s_e=s;
    }
    if ( s.Index(".jpg")>0 ){
      s_e="";
      s_p="";
      s_r="";s_asc="";s_asc1="";
      s.Replace( s.Index(".jpg")  ,  4 , "");s_png=s;
    }
    //    if ( s.Length()-s.Index(".root")==5){
    //      s.Replace( s.Index(".root")  ,  5 , "");
    //    }// remove    .root

    if (slot==0){  //  SLOT==0 means   no  slot....
      if(strlen(s_e)>0){sprintf( appcha,".%s", "eps" );   s_e.Append(  appcha );}
      if(strlen(s_p)>0){sprintf( appcha,".%s", "ps" );    s_p.Append(  appcha  );}
      if(strlen(s_r)>0){sprintf( appcha,".%s", "root" );  s_r.Append(  appcha  );}
      if(strlen(s_asc)>0){sprintf( appcha,".%s", "txt" );   s_asc.Append(  appcha  );}
      if(strlen(s_asc1)>0){sprintf( appcha,".%s", "asc" );   s_asc1.Append(  appcha  );}
      if(strlen(s_png)>0){sprintf( appcha,".%s", "jpg" );   s_png.Append(appcha );}
  }else{
  sprintf( appcha,"_%d.%s", slot, "eps" );   s_e.Append(  appcha );
  sprintf( appcha,"_%d.%s", slot, "ps" );    s_p.Append(  appcha  );
  sprintf( appcha,"_%d.%s", slot, "root" );  s_r.Append(  appcha  );
  sprintf( appcha,"_%d.%s", slot, "txt" );  s_asc.Append(  appcha  );
  sprintf( appcha,"_%d.%s", slot, "asc" );  s_asc1.Append(  appcha  );
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

  //  printf(".!  cp       %s ~/automatic_print/ \n\n",   s_p.Data()   ); 
  printf(".!  lpr      %s\n\n",   s_p.Data()   ); 
  printf(".!  lpr      %s\n\n",   s_e.Data()   ); 
  printf(".!  geeqie   %s&\n\n",  s_png.Data()   ); 


  //  change canvas name !  to let it open later without destroying new...
  TString canna,cannaori;
  cannaori.Append( cmy->GetName() ); //keep original name
  canna.Append( cmy->GetName() );
  s.Append("_save");
  canna.Append(s.Data() );  // not filenam .... but  s.Data() to avoid  .root
  cmy->SetName( canna.Data() );
 
  if(strlen(s_e)>0){ cmy->SaveAs(s_e);}
  if(strlen(s_p)>0){ cmy->SaveAs(s_p);}
  if(strlen(s_png)>0){cmy->SaveAs(s_png);}

   if (slot==0){  //  SLOT==0 means   no  slot....
  // cmy->SaveAs(s_r); // not as TCanvas but :
  //--------------------------------------------------
  int canen=cmy->GetListOfPrimitives()->GetEntries();
  //  printf("DEBUG: %d\n", canen);
  for (int i=0;i<canen;i++){
    if ( strstr( cmy->GetListOfPrimitives()->At(i)->ClassName(),"TPad")!=NULL){
      TPad *p=(TPad*)cmy->GetListOfPrimitives()->At(i);
      int paden=p->GetListOfPrimitives()->GetEntries();
      //      printf("DEBUG: %d\n", canen);
        for (int j=0;j<paden;j++){
	  //	  printf("DEBUG: checking %d  class:%s\n", j, p->GetListOfPrimitives()->At(i)->ClassName()  );
	  if (strstr(p->GetListOfPrimitives()->At(j)->ClassName(),"TH")!=NULL){
	    //	    printf("DEBUG: saving %d to %s\n", j,  s_r.Data() );
	    //===========  BETTER TO MAKE SUBDIR : save_FNAME/hi.asc[1] ===========
	    saveobj2file(s_r.Data(),(TH1*)p->GetListOfPrimitives()->At(j) );
	    saveobj2asc(s_asc.Data(),(TH1*)p->GetListOfPrimitives()->At(j) );
	    saveobj2asc1col(s_asc1.Data(),(TH1*)p->GetListOfPrimitives()->At(j) );
	    //int c; for ((i=0;i<4096;i++)){ c=c022->GetBinContent(i);printf("%d %d\n",i,c);} >alfa_run50_de6.txt
	  } // strstr
	} //j paden
    } // strstr
  } //i canen
  //  printf("DEBUG: end canen   %d\n", canen);
   }else{ // SLOT==0
     cmy->SaveAs(s_r); // if SLOT != 0 => save TPad,not spectra
   } // SLOT==0
    
  //--------------------------------------------------

    
  cmy->SetName( cannaori.Data() ); // put the original name
  }else{printf("use: savecanvas(\"filename_without_extension\")\n");}  

  printf("leaving savecanvas%s\n", "" );

}//  SAVECANCAS -----------------------------------------------------













