//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "sh_menu.h"

//------------------- CODE everything here:









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



