#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"

#include "RooGaussian.h"
#include "RooNovosibirsk.h"
#include "RooChebychev.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"

#include "RooConstVar.h"
#include "RooAddPdf.h"
#include "RooWorkspace.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooDataHist.h"

//#include "RooRealVar.h"
//#include "RooDataSet.h"
//#include "RooGaussian.h"
//#include "RooConstVar.h"
//#include "TCanvas.h"
//#include "TAxis.h"
//#include "RooPlot.h"
#include "RooHist.h"

#include "TPRegexp.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TFile.h"


#include "RooCustomizer.h"
#include "RooFitResult.h"

// to get canvas list
#include "TROOT.h"

using namespace std; 
using namespace RooFit;   // THIS LOADS  ROOFIT  RooFit RooFit   RooFit   RooFit  

/*
 *
 *   GOAL IS CHANGED:
 *   I WANT TO HAVE A CLASS THAT GETS RANGE, MODEL AND MAKES THE FIT
 *
 */

/*
 *  CAN DRAW IF IT RECOGNIZES PEAK  COMPONENT
 *
 */
int IsPeak(const char *ch){
  int out=0;
  if (strcmp(ch,"RooGaussian")==0)out=1;
  if (strcmp(ch,"RooNovosibirsk")==0)out=1;
  if (strcmp(ch,"RooBreitWigner")==0)out=1;
  if (strcmp(ch,"RooCBShape")==0)out=1;
  return out;
}







/*
 *  better guess   area,  guess backround (from find...)....
 */

class  Tbroomfit{
 public:
  //  COMMAND BOX CONTENT
  TString s;
  // npeaks --  basic  information....
  int npeaks;

  double min,max;
  //
  //references to all variables ..........  mean sigma area tail nalpha n
  //
  RooRealVar *msat[14][6]; //  POINTERS TO ALL variables..........but not used????

  double  msat_values[14][6];
  double  msat_errors[14][6];

  RooFitResult *fitresult;

  /*
   *   parameter's  values
   */
  double getMean(int i){      return msat_values[0][i] ;   }
  double getSigma(int i){     return msat_values[1][i] ;   }
  double getArea(int i){      return msat_values[2][i] ;   }
  double getTail(int i){      return msat_values[3][i] ;   }
  double getNalpha(int i){    return msat_values[4][i] ;   }
  double getN(int i){         return msat_values[5][i] ;   }
  int    getNpeaks(){         return npeaks ;   }

  void   printResult(){      
    if (fitresult!=NULL){ 
      printf("%s\n",
	     "==================================================+++" );
      fitresult->Print("v")  ;   
      fitresult->floatParsFinal().Print("s") ;
      printf("%s\nfit range: %5.1f %5.1f\n",
	     "==================================================+++",
               min, max );
      //RooRealVar* par1_fitresult = (RooRealVar*) fitresult->floatParsFinal()->find("par1") 
      //par1_fitresult->GetAsymErrorHi() ; // etc... 
    }
  }//printResult



  void  accessParams(int n, double array[] ){
    /*
    RooArgList fl=fitresult->floatParsFinal();
    RooArgList cl=fitresult->constPars();
    fl.Print();
    cl.Print();
    int fn=fl.getSize();
    int cn=cl.getSize();
    */    //    mean,area
    RooRealVar* par;
    RooRealVar* pas;
    if (n==1){
                     par = (RooRealVar*) fitresult->floatParsFinal().find("mean1");
    if (par==NULL){  par = (RooRealVar*) fitresult->constPars().find("mean1"); }
                     pas = (RooRealVar*) fitresult->floatParsFinal().find("area1");
    if (pas==NULL){  pas = (RooRealVar*) fitresult->constPars().find("area1"); }
    //    printf( "   mean1 == %f +- %f\n", par->getVal() ,  par->getError()  );
    array[0]= par->getVal();     array[1]= par->getError();
    array[2]= pas->getVal();     array[3]= pas->getError();
    }
    if (npeaks<2) return;

    if (n==2){
                     par = (RooRealVar*) fitresult->floatParsFinal().find("mean2");
    if (par==NULL){  par = (RooRealVar*) fitresult->constPars().find("mean2"); }
                     pas = (RooRealVar*) fitresult->floatParsFinal().find("area2");
    if (pas==NULL){  pas = (RooRealVar*) fitresult->constPars().find("area2"); }
    //    printf( "   mean2 == %f +- %f\n", par->getVal() ,  par->getError()  );
    array[0]= par->getVal();     array[1]= par->getError();
     array[2]= pas->getVal();     array[3]= pas->getError();
   }
    if (npeaks<3) return;
    
    if (n==3){
                     par = (RooRealVar*) fitresult->floatParsFinal().find("mean3");
    if (par==NULL){  par = (RooRealVar*) fitresult->constPars().find("mean3"); }
                     pas = (RooRealVar*) fitresult->floatParsFinal().find("area3");
    if (pas==NULL){  pas = (RooRealVar*) fitresult->constPars().find("area3"); }
    //    printf( "   mean2 == %f +- %f\n", par->getVal() ,  par->getError()  );
    array[0]= par->getVal();     array[1]= par->getError();
    array[2]= pas->getVal();     array[3]= pas->getError();
    }
    if (npeaks<4) return;
    
    if (n==4){
                     par = (RooRealVar*) fitresult->floatParsFinal().find("mean4");
    if (par==NULL){  par = (RooRealVar*) fitresult->constPars().find("mean4"); }
                     pas = (RooRealVar*) fitresult->floatParsFinal().find("area4");
    if (pas==NULL){  pas = (RooRealVar*) fitresult->constPars().find("area4"); }
    //    printf( "   mean2 == %f +- %f\n", par->getVal() ,  par->getError()  );
    array[0]= par->getVal();     array[1]= par->getError();
     array[2]= pas->getVal();     array[3]= pas->getError();
   }
    if (npeaks<5) return;
    
    if (n==5){
                     par = (RooRealVar*) fitresult->floatParsFinal().find("mean5");
    if (par==NULL){  par = (RooRealVar*) fitresult->constPars().find("mean5"); }
                     pas = (RooRealVar*) fitresult->floatParsFinal().find("area5");
    if (pas==NULL){  pas = (RooRealVar*) fitresult->constPars().find("area5"); }
    //    printf( "   mean2 == %f +- %f\n", par->getVal() ,  par->getError()  );
    array[0]= par->getVal();     array[1]= par->getError();
     array[2]= pas->getVal();     array[3]= pas->getError();
   }
    if (npeaks<6) return;
    


    return;
  }//----------access





  TString*  saveResult(const char *filename){
    //    printf("        current dir == %s\n" , gDirectory->GetPath());
   TDirectory *curr=(TDirectory*)gDirectory;
      //======= TIME AND DATE ===============
    time_t curtime;  struct tm *loctime;  
    char ch[200];
    char chroof[200];
    curtime = time (NULL);  loctime = localtime (&curtime);

    //    TString sr=s.Data();     // commandbox contents
    //    if ( sr.CompareTo("")==0 ){

	 sprintf(ch,"fit%04d%02d%02d_%02d%02d%02d",
 	   1900+loctime->tm_year,   1+loctime->tm_mon,  
	   loctime->tm_mday,  loctime->tm_hour,   loctime->tm_min,  loctime->tm_sec);
	 sprintf(chroof,"roo%04d%02d%02d_%02d%02d%02d",
 	   1900+loctime->tm_year,   1+loctime->tm_mon,  
	   loctime->tm_mday,  loctime->tm_hour,   loctime->tm_min,  loctime->tm_sec);

      	 TString *stamp=new TString(ch); //tpad fit HERE

	 //       }else{
	 //	 sprintf(ch,"shspe%04d%02d%02d_%02d%02d%02d_%s_.root",
	 // 	   1900+loctime->tm_year,   1+loctime->tm_mon,  
	 //		 loctime->tm_mday,  loctime->tm_hour,   loctime->tm_min,  loctime->tm_sec,
	 //		 sr.Data()  );
	 //    }
    printf("! ... saving RooFitResult into /%s/\n",  filename  );
    printf("i ... names:  /%s/ \n" , chroof );
    
    TFile f(filename,"UPDATE") ; 
    fitresult->Write( chroof ) ; //write RooFitResult    fitresult je GLOBALNI
    f.Close() ;
      curr->cd();
      printf("i ... returned to current dir == %s\n" , gDirectory->GetPath());
    return stamp;
  }//saveresult...




  /***********************************************************************
   ***********************************************************************
   *   CONSTRUCTOR   MAKES ALLLLLLLL
   *******************************************************************
   *************************************************
   *****************************
   *****
   */
  Tbroomfit(double xlow, double xhi, TH1 *h2,  int npeak, double *peak, double *sigm, const char *chpol="p0"){
    int iq=0;
    printf("constructor - %d   %ld", iq++,  (int64_t)h2 );
    h2->Print();
    /*
     *   get global area, ranges for sigma, x
     */
    npeaks=npeak; // class defined int
    //    double areah2=h2->Integral( int(xlow), int(xhi) ); // WRONG - BINS
    min= h2->GetXaxis()->GetFirst();
    printf("constructor - %d   %f", iq++, min  );
    max= h2->GetXaxis()->GetLast();
    printf("constructor - %d   %f", iq++, max  );
    double areah2=h2->Integral( min, max );
    printf("constructor - %d   %f", iq++,  areah2 );
    min=xlow;
    max=xhi;


    double sigmamin=(max-min)/300;
    double sigmamax=(max-min)/4;
    double areamin=0;
    double areamax=2*areah2;
    printf("x:(%f,%f)  s:(%f,%f)  a:(%f,%f) \n", min,max,sigmamin, sigmamax,areamin, areamax );
  /*
   *   definition of  variables..............
   *
   */    
    RooRealVar       x("x",    "x",   min, max);

    int MAXPEAKS=6;  // later from 5 to 6 ???


    printf("RooFit: npeaks=%d\n",  npeaks );
    // ABOVE:  RooRealVar *msat[14][5]; //  POINTERS TO ALL variables
    // 0  m     Mean
    // 1  s     Sigma
    // 2  a     Area
    // 3  t     Tail
    // 4  [0] nalpha
    // 5  [0] n1

for (int ii=0;ii<14;ii++){
 for (int jj=0;jj<MAXPEAKS;jj++){
   msat[ii][jj]=NULL; 
   msat_values[ii][jj]=0.0;
 } //for for
 }// for for 

 printf("delete fitresult, why crash?\n%s","");
 fitresult=NULL;
 printf("delete fitresult, no crash?\n%s","");


    RooRealVar    mean1("mean1", "mean",  1*(max-min)/(npeaks+1)+min,    min,max);msat[0][0]=&mean1;
    RooRealVar    mean2("mean2", "mean",  2*(max-min)/(npeaks+1)+min,    min,max);msat[0][1]=&mean2;
    RooRealVar    mean3("mean3", "mean",  3*(max-min)/(npeaks+1)+min,    min,max);msat[0][2]=&mean3;
    RooRealVar    mean4("mean4", "mean",  4*(max-min)/(npeaks+1)+min,    min,max);msat[0][3]=&mean4;
    RooRealVar    mean5("mean5", "mean",  5*(max-min)/(npeaks+1)+min,    min,max);msat[0][4]=&mean5;
    RooRealVar    mean6("mean6", "mean",  6*(max-min)/(npeaks+1)+min,    min,max);msat[0][5]=&mean6;


    RooRealVar   sigma1("sigma1","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][0]=&sigma1;
    RooRealVar   sigma2("sigma2","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][1]=&sigma2;
    RooRealVar   sigma3("sigma3","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][2]=&sigma3;
    RooRealVar   sigma4("sigma4","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][3]=&sigma4;
    RooRealVar   sigma5("sigma5","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][4]=&sigma5;
    RooRealVar   sigma6("sigma6","sigma", (max-min)/10,       sigmamin,  sigmamax );msat[1][5]=&sigma6;


    RooRealVar    area1("area1", "area",      areah2/npeaks,       areamin, areamax  );msat[2][0]=&area1;
    RooRealVar    area2("area2", "area",      areah2/npeaks,       areamin, areamax  );msat[2][1]=&area2; 
    RooRealVar    area3("area3", "area",      areah2/npeaks,       areamin, areamax  );msat[2][2]=&area3; 
    RooRealVar    area4("area4", "area",      areah2/npeaks,       areamin, areamax  );msat[2][3]=&area4; 
    RooRealVar    area5("area5", "area",      areah2/npeaks,       areamin, areamax  );msat[2][4]=&area5; 
    RooRealVar    area6("area6", "area",      areah2/npeaks,       areamin, areamax  );msat[2][5]=&area6; 

    RooRealVar   bgarea("bgarea", "bgarea", areah2/5, 0, 2*areah2);  



    double  tailstart=-1.0;//  tune the tails....
    double  tailmin=-1e+4;
    double  tailmax=1e+4;

    RooRealVar    tail1("tail1", "tail",      tailstart,       tailmin, tailmax  );msat[3][0]=&tail1;
    RooRealVar    tail2("tail2", "tail",      tailstart,       tailmin, tailmax  );msat[3][1]=&tail2;
    RooRealVar    tail3("tail3", "tail",      tailstart,       tailmin, tailmax  );msat[3][2]=&tail3;
    RooRealVar    tail4("tail4", "tail",      tailstart,       tailmin, tailmax  );msat[3][3]=&tail4;
    RooRealVar    tail5("tail5", "tail",      tailstart,       tailmin, tailmax  );msat[3][4]=&tail5;
    RooRealVar    tail6("tail6", "tail",      tailstart,       tailmin, tailmax  );msat[3][5]=&tail6;

    // for CBShape
    RooRealVar    nalpha1("nalpha1", "nalpha",      1.3, 0, 100  );msat[4][0]=&nalpha1;

    RooRealVar    n1("n1",          "n",         5.1, 0, 100  );  msat[5][0]=&n1;






    /*
     *   initial values  for  peak positions................
     */
    if (npeaks>=1) {mean1=peak[0];sigma1=sigm[0];}
    if (npeaks>=2) {mean2=peak[1];sigma2=sigm[1];}
    if (npeaks>=3) {mean3=peak[2];sigma3=sigm[2];}
    if (npeaks>=4) {mean4=peak[3];sigma4=sigm[3];}
    if (npeaks>=5) {mean5=peak[4];sigma5=sigm[4];}
    if (npeaks>=6) {mean6=peak[5];sigma6=sigm[5];}


    /*
     *    RooAbsPdf -> RooGaussian
     *                 RooNovosibirsk
     *                 RooLandau
     */
     RooAbsPdf *pk[6];                 // MAXIMUM PEAKS ==5    6 NOW!!             
     RooAbsPdf *pk_dicto[14][6];        // ALL DICTIONARY OF PEAKS..........
     //  Abstract Class.... carrefuly

    RooGaussian gauss1("gauss1","gauss(x,mean,sigma)", x, mean1, sigma1);pk_dicto[0][0]=&gauss1;
    RooGaussian gauss2("gauss2","gauss(x,mean,sigma)", x, mean2, sigma2);pk_dicto[0][1]=&gauss2;
    RooGaussian gauss3("gauss3","gauss(x,mean,sigma)", x, mean3, sigma3);pk_dicto[0][2]=&gauss3;
    RooGaussian gauss4("gauss4","gauss(x,mean,sigma)", x, mean4, sigma4);pk_dicto[0][3]=&gauss4;
    RooGaussian gauss5("gauss5","gauss(x,mean,sigma)", x, mean5, sigma5);pk_dicto[0][4]=&gauss5;
    RooGaussian gauss6("gauss6","gauss(x,mean,sigma)", x, mean6, sigma6);pk_dicto[0][5]=&gauss6;

    RooNovosibirsk ns1("ns1","novosib(x,mean,sigma,tail)", x, mean1,sigma1, tail1 );pk_dicto[1][0]=&ns1;
    RooNovosibirsk ns2("ns2","novosib(x,mean,sigma,tail)", x, mean2,sigma2, tail2 );pk_dicto[1][1]=&ns2;
    RooNovosibirsk ns3("ns3","novosib(x,mean,sigma,tail)", x, mean3,sigma3, tail3 );pk_dicto[1][2]=&ns3;
    RooNovosibirsk ns4("ns4","novosib(x,mean,sigma,tail)", x, mean4,sigma4, tail4 );pk_dicto[1][3]=&ns4;
    RooNovosibirsk ns5("ns5","novosib(x,mean,sigma,tail)", x, mean5,sigma5, tail5 );pk_dicto[1][4]=&ns5;
 
    // BreitWiegner  is  Lorentzian...?
    RooBreitWigner bw1("bw1","BreitWigner(x,mean,sigma)", x, mean1, sigma1 );pk_dicto[2][0]=&bw1;
    RooBreitWigner bw2("bw2","BreitWigner(x,mean,sigma)", x, mean2, sigma2 );pk_dicto[2][1]=&bw2;
    RooBreitWigner bw3("bw3","BreitWigner(x,mean,sigma)", x, mean3, sigma3 );pk_dicto[2][2]=&bw3;
    RooBreitWigner bw4("bw4","BreitWigner(x,mean,sigma)", x, mean4, sigma4 );pk_dicto[2][3]=&bw4;
    RooBreitWigner bw5("bw5","BreitWigner(x,mean,sigma)", x, mean5, sigma5 );pk_dicto[2][4]=&bw5;

    RooCBShape cb1("cb1","CBShape(x,mean,sigma)", x, mean1, sigma1, nalpha1, n1 );pk_dicto[3][0]=&cb1;
    RooCBShape cb2("cb2","CBShape(x,mean,sigma)", x, mean2, sigma2, nalpha1, n1 );pk_dicto[3][1]=&cb2;
    RooCBShape cb3("cb3","CBShape(x,mean,sigma)", x, mean3, sigma3, nalpha1, n1 );pk_dicto[3][2]=&cb3;
    RooCBShape cb4("cb4","CBShape(x,mean,sigma)", x, mean4, sigma4, nalpha1, n1 );pk_dicto[3][3]=&cb4;
    RooCBShape cb5("cb5","CBShape(x,mean,sigma)", x, mean5, sigma5, nalpha1, n1 );pk_dicto[3][4]=&cb5;
    RooCBShape cb6("cb6","CBShape(x,mean,sigma)", x, mean6, sigma6, nalpha1, n1 );pk_dicto[3][5]=&cb6;



    /*
     *    PEAK TYPES   BACKGROUND TYPE .........   COMMAND BOX  OPTIONS ......
     */
    /****************************************************************************
     *  PLAY  WITH  THE DEFINITION  COMMANDLINE...................... POLYNOM + PEAKS
     */
    // CALSS DECLARED TString s;
  s=chpol;
  /*
   *   peaks+bg== ALL BEFORE  ; or :          (after ...  it is a conditions/options)
   */
  TString command;
  int comstart=s.Index(":");   if (comstart<0){ comstart=s.Index(";");}
  if  (comstart<0){ command="";}else{
    command=s(comstart+1, s.Length()-comstart -1 ); // without ;
    s=s(0,comstart); // without ;
    printf("COMMANDLINE : %s\n",  command.Data()  );
    if (TPRegexp("scom").Match(command)!=0){
      
    }// COMMANDS - 
  }// there is some command
  /*************************************************
   *  PLAY WITH peaks+bg..................    s
   */
  s.Append("+"); s.Prepend("+");  s.ReplaceAll(" ","+");
  s.ReplaceAll("++++","+"); s.ReplaceAll("+++","+"); s.ReplaceAll("++","+");s.ReplaceAll("++","+");
  printf ("   regextp =  %s\n",  s.Data()  );
  if (TPRegexp("\\+p[\\dn]\\+").Match(s)==0){ // no match
     printf("NO polynomial demanded =>: %s\n",  "appending  pn command"  ); s.Append("pn+");
  }
  TString spk=s;   TString sbg=s;
  TPRegexp("\\+p[\\dn]\\+").Substitute(spk,"+");  // remove   +p.+   
  TPRegexp(".+(p[\\dn]).+").Substitute(sbg,"$1"); // remove   all but +p+   

  printf ("PEAKS=%s      BG=%s\n",  spk.Data() ,  sbg.Data()  );
  spk.ReplaceAll("+","");   //  VARIANT 1 ------- EACH  LETTER MEANS ONE PEAK 






  /************************************************************************
   *          PREPARE PEAKS  FOLLOWING THE COMMAND BOX................
   */
    //default PEAK types
    pk[0]=&gauss1;
    pk[1]=&gauss2;
    pk[2]=&gauss3;
    pk[3]=&gauss4;
    pk[4]=&gauss5;
    pk[5]=&gauss6;


  int maxi=spk.Length();
  if (maxi>npeaks){maxi=npeaks;}
  for (int i=0;i<maxi;i++){
    if (spk[i]=='n'){
      pk[i]=pk_dicto[1][i];//novosibirsk
      printf("PEAK #%d ... Novosibirsk\n", i );
    }else if(spk[i]=='b'){
      pk[i]=pk_dicto[2][i];//BreitWiegner
      printf("PEAK #%d ... BreitWigner\n", i );
    }else if(spk[i]=='c'){
      pk[i]=pk_dicto[3][i];//CBShape
      printf("PEAK #%d ... CBShape\n", i );
    }else if(spk[i]=='y'){
    }else if(spk[i]=='z'){
    }else{
      pk[i]=pk_dicto[0][i]; //gauss
      printf("PEAK #%d ... Gaussian\n", i );
    }// ELSE CHAIN
  }//i to maxi


  for (int i=0;i<npeaks;i++){ printf("Peak %d   at  %f  s=%f:  PRINT:\n  " ,  i, peak[i], sigm[i]  );pk[i]->Print();}



 /******************************************************** BACKGROUND pn-p4
     *   a0 == level - also skew
     *   a1 == p2
     *   a2 == p3
     */
 // Build Chebychev polynomial p.d.f.  
 // RooRealVar a0("a0","a0", 0.) ;
  RooRealVar a0("a0","a0",    0., -10, 10) ;
  RooRealVar a1("a1","a1",    0., -10, 10) ;
  RooRealVar a2("a2","a2",    0., -10, 10) ;
  RooRealVar a3("a3","a3",    0., -10, 10) ;
  RooArgSet setcheb;
  if ( sbg=="pn" ){ setcheb.add(a0);  a0=0.; a0.setConstant(kTRUE);bgarea=0.; bgarea.setConstant(kTRUE);}
  if ( sbg=="p0" ){ setcheb.add(a0);  a0=0.; a0.setConstant(kTRUE); }
  if ( sbg=="p1" ){ setcheb.add(a0); }
  if ( sbg=="p2" ){ setcheb.add(a1); setcheb.add(a0); }
  if ( sbg=="p3" ){ setcheb.add(a2); setcheb.add(a1); setcheb.add(a0); }
  if ( sbg=="p4" ){ setcheb.add(a3);setcheb.add(a2); setcheb.add(a1); setcheb.add(a0); }
  //  RooChebychev bkg("bkg","Background",x,RooArgSet(a0,a1,a2,a3) ) ;
  RooChebychev bkg("bkg","Background",x, setcheb ) ;

 


  /**********************************************************************
   * MODEL
   */
 
 
 RooArgList rl;  
 if (npeaks>0)rl.add( *pk[0] );  
 if (npeaks>1)rl.add( *pk[1] );  
 if (npeaks>2)rl.add( *pk[2] );  
 if (npeaks>3)rl.add( *pk[3] );  
 if (npeaks>4)rl.add( *pk[4] );  
 if (npeaks>5)rl.add( *pk[5] );  
 rl.add( bkg ); 
 RooArgSet rs;
 if (npeaks>0)rs.add( area1 );  
 if (npeaks>1)rs.add( area2 );  
 if (npeaks>2)rs.add( area3 );  
 if (npeaks>3)rs.add( area4 );  
 if (npeaks>4)rs.add( area5 );  
 if (npeaks>5)rs.add( area6 );  
 rs.add( bgarea );
 RooAddPdf modelV("model","model", rl, rs );


 /*
  *  WITH CUSTOMIZER - I can change parameters inside. But
  *             - then all is a clone and I dont know how to reach it
  */
   RooCustomizer cust( modelV ,"cust"); 
   /*
    *  Possibility to fix all sigma  or tails....
    */ 
   if (TPRegexp("scom").Match(command)!=0){//----------------------SCOM
     printf("all sigma have common values.....\n%s", ""); 
     if (npeaks>1)cust.replaceArg(sigma2,sigma1) ;
     if (npeaks>2)cust.replaceArg(sigma3,sigma1) ;
     if (npeaks>3)cust.replaceArg(sigma4,sigma1) ;
     if (npeaks>4)cust.replaceArg(sigma5,sigma1) ;
     if (npeaks>5)cust.replaceArg(sigma6,sigma1) ;
    }
   if (TPRegexp("tcom").Match(command)!=0){//----------------------TCOM
     printf("all tails have common values.....\n%s", ""); 
     if (npeaks>1)cust.replaceArg(tail2,tail1) ;
     if (npeaks>2)cust.replaceArg(tail3,tail1) ;
     if (npeaks>3)cust.replaceArg(tail4,tail1) ;
     if (npeaks>4)cust.replaceArg(tail5,tail1) ;
     if (npeaks>5)cust.replaceArg(tail6,tail1) ;
    }
   /*   if (TPRegexp("tcom").Match(command)!=0){//----------------------TCOM Neni dalsi ACOM,NCOM pro CB...
     printf("all tails have common values.....\n%s", ""); 
     if (npeaks>1)cust.replaceArg(tail2,tail1) ;
     if (npeaks>2)cust.replaceArg(tail3,tail1) ;
     if (npeaks>3)cust.replaceArg(tail4,tail1) ;
     if (npeaks>4)cust.replaceArg(tail5,tail1) ;
    }
   */
   if  (TPRegexp("p1fix").Match(command)!=0){//----------------------
     mean1.setConstant();printf("position 1 set constant%s\n","");
   }
   if  (TPRegexp("p2fix").Match(command)!=0){//----------------------
     mean2.setConstant();printf("position 2 set constant%s\n","");
   }
   if  (TPRegexp("p3fix").Match(command)!=0){//----------------------
     mean3.setConstant();printf("position 3 set constant%s\n","");
   }
   if  (TPRegexp("p4fix").Match(command)!=0){//----------------------
     mean4.setConstant();printf("position 4 set constant%s\n","");
   }
   if  (TPRegexp("p5fix").Match(command)!=0){//----------------------
     mean5.setConstant();printf("position 5 set constant%s\n","");
   }
   if  (TPRegexp("p6fix").Match(command)!=0){//----------------------
     mean6.setConstant();printf("position 6 set constant%s\n","");
   }
   if  (TPRegexp("s1fix").Match(command)!=0){//----------------------
     sigma1.setConstant();printf("sigma 1 set constant%s\n","");
   }
   if  (TPRegexp("s2fix").Match(command)!=0){//----------------------
     sigma2.setConstant();printf("sigma 2 set constant%s\n","");
   }
   if  (TPRegexp("s3fix").Match(command)!=0){//----------------------
     sigma3.setConstant();printf("sigma 3 set constant%s\n","");
   }
   if  (TPRegexp("s4fix").Match(command)!=0){//----------------------
     sigma4.setConstant();printf("sigma 4 set constant%s\n","");
   }
   if  (TPRegexp("s5fix").Match(command)!=0){//----------------------
     sigma5.setConstant();printf("sigma 5 set constant%s\n","");
   }
   if  (TPRegexp("s6fix").Match(command)!=0){//----------------------
     sigma6.setConstant();printf("sigma 6 set constant%s\n","");
   }


   RooAbsPdf* model = (RooAbsPdf*) cust.build(kTRUE) ; //build a clone...comment on changes...
   //   model->Print("t") ;
   //delete model ; // eventualy delete the model...





 /*
  *  DISPLAY RESULTS            >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  */
   TPad *orig_gpad=(TPad*)gPad;


   TCanvas *c;
   c=(TCanvas*)gROOT->GetListOfCanvases()->FindObject("fitresult");
   if (c==NULL){
     printf("making new canvas\n%s","");
     c=new TCanvas("fitresult",h2->GetName(),1000,700);  
   }else{
     printf("using old canvas\n%s","");
     c->SetTitle( h2->GetName() );
   }
   c->Clear();
     printf(" canvas cleared\n%s","");
   c->Divide(1,2) ;
     printf(" canvas divided\n%s","");
  c->Modified();c->Update(); 

 RooDataHist datah("datah","datah with x",x,h2);
 RooPlot* xframe = x.frame();
 datah.plotOn(xframe,  DrawOption("logy") );

 // return;
   if (TPRegexp("chi2").Match(command)!=0){//----------------------CHI2
     //from lorenzo moneta
     //     TH1 * h1 = datah.createHistogram(x);
     //     TF1 * f = model->asTF(RooArgList(x) , parameters ); //??? 
     //     h2->Fit(f);
     //It will work but you need to create a THNSparse and fit it 
     //or use directly the ROOT::Fit::BinData class to create a ROOT::Fit::Chi2Function to minimize.
     // THIS CANNOT DO ZERO BINS
     fitresult = model->chi2FitTo( datah , Save()  );  
   }else{
     //   FIT    FIT    FIT    FIT    FIT    FIT    FIT     FIT    FIT   FIT   
      fitresult = model->fitTo( datah , Save()  );   
   }

   fitresult->SetTitle( h2->GetName() ); // I PUT histogram name to global fitresult
   
   // will be done by printResult ... fitresult->Print("v") ;
  //duplicite  fitresult->floatParsFinal().Print("s") ;
  // later - after  parsfinale .... : printResult();
    //    model->Print();  // not interesting........
    model->plotOn(xframe, LineColor(kRed),   DrawOption("l0z") );

  //,Minos(kFALSE)

 /*
  *  Posledni nakreslena vec je vychodiskem pro xframe->resid...?
  *   NA PORADI ZALEZI....
  */

    //unused RooHist* hresid = xframe->residHist() ;
 RooHist* hpull =  xframe->pullHist() ;

 // RooPlot* xframe2 = x.frame(Title("Residual Distribution")) ;
 // xframe2->addPlotable(hresid,"P") ;

  // Construct a histogram with the pulls of the data w.r.t the curve
 RooPlot* xframe3 = x.frame(Title("Pull Distribution")) ;
 xframe3->addPlotable(hpull,"P") ;

  /*
   *  plot components at the end....                     PLOT >>>>>>>>>>>>>>>>
   */

 int colorseq[10]={kRed,kGreen,kBlue,kYellow,kCyan,kMagenta,kViolet,kAzure,kGray,kOrange};

 // RooArgSet* model_params = model->getParameters(x); // this returns all parameters
 RooArgSet* model_params = model->getComponents();
 TIterator* iter = model_params->createIterator() ;
 RooAbsArg* arg ; int icomp=0, ipeak=0;
 //  printf("ENTERING COMPONENT ITERATOR x%dx.....................\n",  icomp );
  while((arg=(RooAbsArg*)iter->Next())) {
    //    printf("printing COMPONENT %d\n",  icomp );
    //    arg->Print();    
    //    printf("NAME==%s\n", arg->Class_Name()  ); //This returns only RooAbsArg
    //    printf("NAME==%s\n", arg->ClassName()  ); //This RooGaussian RooChebychev
    if ( IsPeak( arg->ClassName() )==1 ){
      pk[ipeak]=(RooAbsPdf*)arg; //?
      //      pk[ipeak]->Print();
      ipeak++;
      //      printf("adresses ... %d - %d - %d\n", pk[0], pk[1], pk[2]  );
    }// yes peak.
    icomp++; 
  }//iterations over all components


    model->plotOn(xframe, Components(bkg), LineColor(kRed), LineStyle(kDashed),  DrawOption("l0z") );
    for (int i=0;i<npeaks;i++){
      //      printf("plotting  %d. peak, color %d\n", i,  colorseq[i+1]  );
      //      printf("adresses ... %d - %d - %d\n", pk[0], pk[1], pk[2]  );
      //      pk[i]->Print();
      model->plotOn(xframe, Components( RooArgSet(*pk[i],bkg) ), LineColor(colorseq[i+1]), LineStyle(kDashed),
		    DrawOption("l0z") );
      //		    DrawOption("pz"),DataError(RooAbsData::SumW2) );???  pz removes complains...warnings
      //      model.plotOn(xframe, Components( RooArgSet(*pk[i],bkg) ), LineColor(colorseq[i+1]), LineStyle(kDashed));
    }
 

    // WE SET THE 1st PAD in "fitresult" to LOGY....  1
    //  .....  if the original window is LOGY.....   :)
    //
    //    printf("########### ORIGPAD LOGY==%d #########3\n",  orig_gpad->GetLogy()  );
    c->cd(1); xframe->Draw();  gPad->SetLogy(  orig_gpad->GetLogy()  );
 // c->cd(2); xframe2->Draw();  
 c->cd(2); xframe3->Draw();  
  c->Modified();c->Update(); 

 orig_gpad->cd();


 // printf("msat reference to peak 0 0 = %d,  (%f)\n",  msat[0][0] ,  msat[0][0]->getVal()  );
 for (int ii=0;ii<14;ii++){
 for (int jj=0;jj<MAXPEAKS;jj++){
   if ( msat[ii][jj]!=NULL){
     msat_values[ii][jj]=msat[ii][jj]->getVal();
   }//if
 } //for for
 }// for for   
 printf("at the total end of the constructor....%s\n","");
 // done in pirntResult .. fitresult->floatParsFinal().Print("s") ;
 printResult();
  }; // constructor


}; //CLASS



