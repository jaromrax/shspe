//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "sh_graph.h"

//------------------- CODE everything here:

 const int MAXPNTS=15000;



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









