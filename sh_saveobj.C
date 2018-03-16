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



