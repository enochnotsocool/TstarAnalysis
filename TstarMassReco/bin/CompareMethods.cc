/*******************************************************************************
 *
 *  Filename    : CompareMethods
 *  Description : Make Plots for Comparision of reconstruction methods
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "DataFormats/FWLite/interface/Event.h"

#include "TstarAnalysis/TstarMassReco/interface/CompareHistMgr.hh"

#include "TFile.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TCanvas.h"

#include <iostream>
#include <vector>

using namespace std;
// TimeReport   9.416040     9.416040     9.416040  AddTopConstrain
// TimeReport   0.023562     0.023562     0.023562  ChiSq6jet0b
// TimeReport   0.001745     0.001745     0.001745  ChiSq6jet1b
// TimeReport   0.050019     0.050019     0.050019  ChiSq8jet1b
// TimeReport   6.300856     6.300856     6.300856  NoTopConstrain

int main( int argc, char* argv[] ){

   fwlite::Event event( TFile::Open("test/edm_MassReco.root") );
   CompareHistMgr* ChiSq6jet0b  = new CompareHistMgr( "ChiSq6jet0b",  "ChiSq6jet0b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq6jet1b  = new CompareHistMgr( "ChiSq6jet1b",  "ChiSq6jet1b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq8jet1b  = new CompareHistMgr( "ChiSq8jet1b",  "ChiSq8jet1b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* HitFitTopCon = new CompareHistMgr( "HitFitTopCon", "AddTopConstrain",  "HitFitResult", "HitFitCompare");
   CompareHistMgr* HitFitNoTop  = new CompareHistMgr( "HitFitNoTop",  "NoTopConstrain",   "HitFitResult", "HitFitCompare");

   unsigned i = 0 ;
   for( event.toBegin() ; !event.atEnd() ; ++event ){
      cout << "\rAt Event [" << ++i << "]" << flush;
      ChiSq6jet0b-> AddEvent(event);
      ChiSq6jet1b-> AddEvent(event);
      ChiSq8jet1b-> AddEvent(event);
      HitFitTopCon->AddEvent(event);
      HitFitNoTop-> AddEvent(event);
   }
   cout << endl;

   ChiSq6jet0b->SetColor(kBlue);
   ChiSq6jet1b->SetColor(kRed);
   ChiSq8jet1b->SetColor(kGreen);
   HitFitTopCon->SetColor(kBlack);
   HitFitNoTop->SetColor(kCyan);


   for( const auto& hist_name : ChiSq6jet0b->AvailableHistList() ){
      TCanvas* c = new TCanvas("c","c",650,500);
      TLegend* tl = new TLegend(0.6,0.5,0.9,0.9);
      ChiSq6jet0b-> Hist(hist_name)->Draw("HIST SAME");
      ChiSq6jet1b-> Hist(hist_name)->Draw("HIST SAME");
      ChiSq8jet1b-> Hist(hist_name)->Draw("HIST SAME");
      HitFitTopCon->Hist(hist_name)->Draw("HIST SAME");
      HitFitNoTop-> Hist(hist_name)->Draw("HIST SAME");
      tl->AddEntry( ChiSq6jet0b->Hist(hist_name) , "#chi^{2} (6 jets, no b-tag)" , "l");
      tl->AddEntry( ChiSq6jet1b->Hist(hist_name) , "#chi^{2} (6 jets, 1 b-tag)" , "l");
      tl->AddEntry( ChiSq8jet1b->Hist(hist_name) , "#chi^{2} (8 jets, 1 b-tag)" , "l");
      tl->AddEntry( HitFitNoTop->Hist(hist_name) , "HitFit (No Top Contrain)" , "l");
      tl->AddEntry( HitFitTopCon->Hist(hist_name) , "HitFit (Top Constrained)" , "l");
      tl->Draw();
      c->SaveAs( (hist_name+".png").c_str() );
      delete c;
   }

   return 0;
}
