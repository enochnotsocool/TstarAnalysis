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

extern void ComparePlot( const string& plot_name, const vector<CompareHistMgr*> );    // See src/ComparePlot.cc
extern void MatchRatePlot( const string& plot_name, const vector<CompareHistMgr*> );  // See src/ComparePlot.cc

int main( int argc, char* argv[] ){

   fwlite::Event event( TFile::Open("test/edm_MassReco.root") );
   CompareHistMgr* ChiSq6jet0b  = new CompareHistMgr( "ChiSq6jet0b", "#chi^{2} (6 jet + 0 b-jets)", "ChiSq6jet0b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq6jet1b  = new CompareHistMgr( "ChiSq6jet1b", "#chi^{2} (6 jet + 1 b-jets)", "ChiSq6jet1b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq6jet2b  = new CompareHistMgr( "ChiSq6jet2b", "#chi^{2} (6 jet + 2 b-jets)", "ChiSq6jet2b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq8jet0b  = new CompareHistMgr( "ChiSq8jet0b", "#chi^{2} (8 jet + 0 b-jets)", "ChiSq8jet0b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq8jet1b  = new CompareHistMgr( "ChiSq8jet1b", "#chi^{2} (8 jet + 1 b-jets)", "ChiSq8jet1b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* ChiSq8jet2b  = new CompareHistMgr( "ChiSq8jet2b", "#chi^{2} (8 jet + 2 b-jets)", "ChiSq8jet2b", "ChiSquareResult", "HitFitCompare")  ;
   CompareHistMgr* HitFit6jet1b  = new CompareHistMgr( "HitFit6jet1b", "HitFit(6 jet + 1 b-jets, Not Top constrain)", "NoTopConstrain6j1b", "HitFitResult", "HitFitCompare")  ;
   CompareHistMgr* HitFit8jet1b  = new CompareHistMgr( "HitFit8jet1b", "HitFit(8 jet + 1 b-jets, Not Top constrain)", "NoTopConstrain8j1b", "HitFitResult", "HitFitCompare")  ;


   unsigned i = 0 ;
   for( event.toBegin() ; !event.atEnd() ; ++event ){
      cout << "\rAt Event [" << ++i << "]" << flush;
      ChiSq6jet0b->AddEvent(event);
      ChiSq6jet1b->AddEvent(event);
      ChiSq6jet2b->AddEvent(event);
      ChiSq8jet0b->AddEvent(event);
      ChiSq8jet1b->AddEvent(event);
      ChiSq8jet2b->AddEvent(event);
      HitFit6jet1b->AddEvent(event);
      HitFit8jet1b->AddEvent(event);
   }
   cout << endl;
   ComparePlot( "6j-bjets_effect" , {ChiSq6jet0b,ChiSq6jet1b,ChiSq6jet2b} );
   ComparePlot( "6j-v-8j_0bjet",  {ChiSq6jet0b,ChiSq8jet0b} );
   ComparePlot( "6j-v-8j_2bjet",  {ChiSq6jet2b,ChiSq8jet2b} );
   ComparePlot( "chisq-v-hitfit" , {ChiSq6jet1b,HitFit6jet1b,HitFit8jet1b} );
   MatchRatePlot( "chisq", {
      ChiSq6jet0b,
      ChiSq6jet1b,
      ChiSq6jet2b,
      ChiSq8jet0b,
      ChiSq8jet1b,
      ChiSq6jet2b }
   );
   MatchRatePlot( "chisq-v-hitfit-match", {
      ChiSq6jet1b,
      ChiSq8jet1b,
      HitFit6jet1b,
      HitFit8jet1b
      }
   ) ;


   return 0;
}
