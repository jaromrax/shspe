//==================================
//   part of shspe;
//     OR can be loaded separately with .L ++
//     #include in shspe as  #include "sh____.C"
//
//==================================

#include "sh_tnamed.h"



//------------------- CODE everything here:



/*

NOW 201803 - depreciated -----------   MOVED from kibbler_gdir
======================================================================
ON MULTI in shspe this will be checked and displayed in one win
   -  THStack("nostack") replacement
  : I looked in the title....  but more convenient would be to look
            inside GetListOfFunctions  TNamed
      TNamed *n=new TNamed("next","b0w00a2")
     b0w00_A1->GetListOfFunctions()->Add( new TNamed("next","b0w00_a2") )
 */
// TH1F* extract_next(TH1F* h){
//   char s[100];
//   if (h==NULL)return NULL;
//   TNamed *n=(TNamed*)h->GetListOfFunctions()->FindObject("next");
//   if (n!=NULL){ // BEST way
//     sprintf( s, "%s", n->GetTitle() );
//     printf("i... TNamed 'next'==/%s/\n",s);
//   }else{ // depreciated way...
//     sprintf( s, "%s", h->GetTitle() );
//     char* ss;
//     ss=(char*)strstr( s, " next:" );
//     if ( ss==NULL) {printf("...end\n%s","");return NULL;}
//     char sss[100];
//     strcpy( s, &ss[6]   ); //  _next:  has 6 chars
//     //    printf("sss =%s\n", sss );
//   }
//   TH1F* nexth=(TH1F*)gDirectory->Get( s );
//   if ( nexth!=NULL ){
//     printf("nexth=%s\n", nexth->GetName() );
//     //      printf("nexth=%s\n", nexth->GetTitle() );
//     return (TH1F*)nexth;
//   }
//   return NULL;
// }





void* extract_display_with(TH1F* h, int &count,int64_t addr[]){
  //int64_t addr[];
  count=0; addr[0]=0; // as in RecoverTH1
  char sn[100];
  char st[100];
  //TH1F *hs[20];
  //printf("---- display with ----%s\n","");
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
//void* ro_getnext(TH1F* h){
void* tnamed_draw(TH1F* h){
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
  h->GetYaxis()->SetRangeUser(0., max*1.1 );
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
  
} // TNAMED_DRAW=================end


void tnamed_printall(TH1 *h){
  
  int nmax=h->GetListOfFunctions()->GetEntries();
  for (int i=0;i<nmax;i++){
    TString trida2=h->GetListOfFunctions()->At(i)->ClassName();
    if (  strstr(trida2.Data(),"TNamed")!=0 ){
      printf( "%10s : %s\n",h->GetListOfFunctions()->At(i)->GetName(),
	      h->GetListOfFunctions()->At(i)->GetTitle()    );
    }//it is TNamed
  }// for all functions
}





void tnamed_hadd( TH1* h , TH1* h1 ){
  if ((h!=NULL)&&(h1!=NULL)){
    h->GetListOfFunctions()->Add( new TNamed("display_with", h1->GetName() ) );
    printf("%s histogram will be drawn with %s in 'multi'\n",
	   h1->GetName(),  h->GetName()  );
  }
}//-------------------------------------







TString* tnamed_get_calibline( TH1* histo ){
  TString* sr=new TString("");
  sr->Append(histo->GetListOfFunctions()->FindObject("calib1")->GetTitle());
  sr->Append(",");
  sr->Append(histo->GetListOfFunctions()->FindObject("calib0")->GetTitle());
  printf("D... tnamed calibration line=%s\n", sr->Data() );
  return sr;
}



void tnamed_set( TH1* h,const char* name , const char* title ){
  if ((h!=NULL)){
    TNamed *n=(TNamed*)h->GetListOfFunctions()->FindObject(name);
    if (n!=NULL){
      printf("D... TNamed %s already defined as %s. reset...\n", n->GetName(), n->GetTitle()  );
      n->SetTitle( title );
      n->Print();
    }else{
      h->GetListOfFunctions()->Add( new TNamed( name , title ) );
    }
  }
}
