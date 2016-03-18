/*******************************************************************************
 *
 *  Filename    : simple_read.cc
 *  Description : Testing the reading capability of the output
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"
#include "TstarAnalysis/TstarMassReco/interface/HitFitResult.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   TFile* file = TFile::Open("signal.root");
   fwlite::Event event( file );

   fwlite::Handle<ChiSquareResult>  chiSqHandle;
   fwlite::Handle<HitFitResult>     hitfitHandle;
   TH1D* myplot = new TH1D("myplot","myplot",50,0,3000);
   TH1D* hitfit = new TH1D("hitfit","hitfit",50,0,3000);

   unsigned i = 0 ;
   for( event.toBegin() ; ! event.atEnd() ; ++event , ++i ){
      cout << "At Event [" << i << "]" << endl;
      chiSqHandle.getByLabel(  event , "tstarMassReco", "ChiSquareResult", "TstarMassReco" );
//      hitfitHandle.getByLabel( event , "tstarMassReco", "HitFitResult"   , "TstarMassReco" );
      myplot->Fill( chiSqHandle->TstarMass() );
//      hitfit->Fill( hitfitHandle->TstarMass() );
      cout << chiSqHandle->TstarMass() << " "
         << chiSqHandle->ChiSquare() << endl;
   }

   TCanvas* c = new TCanvas("c","c",400,600);
   myplot->Draw();
   hitfit->Draw("SAME");
   c->SaveAs("test.png");

   hitfit->SetLineColor( kRed );

   return 0;
}
