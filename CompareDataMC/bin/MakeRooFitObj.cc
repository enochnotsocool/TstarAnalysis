/*******************************************************************************
 *
 *  Filename    : MakeRooFitObj.cc
 *  Description : Makes RooFit objects to pass to HiggsCombine package
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooKeysPdf.h"
#include "RooPlot.h"

#include "TCanvas.h"

#include <iostream>
#include <vector>

using namespace std;

//------------------------------------------------------------------------------
//   Defining constants
//------------------------------------------------------------------------------
static const double total_lumi = 24.25;
static const double minmass = 0;
static const double maxmass = 3000;
static const double minweight = -10000;
static const double maxweight = 10000;
static RooRealVar x("x", "t+g Mass" , minmass , maxmass );
static RooRealVar w("w" , "event weight", minweight , maxweight );

//------------------------------------------------------------------------------
//   Helper funtions
//------------------------------------------------------------------------------
void FillDataSet( RooDataSet&, SampleMgr& , const double total_lumi );

//------------------------------------------------------------------------------
//   Begin control flow
//------------------------------------------------------------------------------
int main(int argc, char const *argv[]) {
   cout << "Hello World!" << endl;

   vector<SampleMgr> background;
   background.push_back( SampleMgr( "data/MuonSignal.json", "TTJets" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "SingleT_S" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "SingleT_T" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "SingleT_TW" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "SingleTbar_TW" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_100_200" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_200_400" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_400_600" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_600_800" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_800_1200" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_1200_2500" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WJets_2500_Inf" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "ZJets_100_200" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "ZJets_200_400" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "ZJets_400_600" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "ZJets_600_Inf" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WW" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "WZ" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "ZZ" ) );
   background.push_back( SampleMgr( "data/MuonSignal.json", "TTW_Lepton") );
   background.push_back( SampleMgr( "data/MuonSignal.json", "TTW_Quark") );
   background.push_back( SampleMgr( "data/MuonSignal.json", "TTZ_Lepton") );
   background.push_back( SampleMgr( "data/MuonSignal.json", "TTZ_Quark") );

   RooDataSet background_dataset( "bg", "background" , RooArgSet(x,w) , RooFit::WeightVar(w) );
   for( auto& sample : background ){
      FillDataSet( background_dataset, sample , total_lumi );
   }
   // Fitting back ground to fermi function
   RooRealVar a("a","a", 0 , 10000 );
   RooRealVar m("m","m", 0, 10000);
   RooRealVar b("b","b", 0 ,100000);
   RooGenericPdf* bg_fit = new RooGenericPdf(
      "bg_fit" , "bg_fit",
      "a/(1+exp(x-m)/b)",
      RooArgSet(a,x,m,b) );
   bg_fit->fitTo( background_dataset , RooFit::Save() , RooFit::SumW2Error(kTRUE) );

   // Making confirmation plots
   RooPlot* frame = x.frame();
   background_dataset.plotOn( frame );
   bg_fit->plotOn(frame);

   TCanvas* c1 = new TCanvas("c","c");
   frame->Draw();
   c1->SaveAs("test.png");
   delete c1;




   // Making signal MC objects
   SampleMgr signal( "data/MuonSignal_SignalMC.json" , "tstar_M1000" );
   RooDataSet signal_dataset( "sig", "signal" , RooArgSet(x,w), RooFit::WeightVar(w) );
   FillDataSet( signal_dataset, signal, total_lumi );
   RooKeysPdf* signal_fit = new RooKeysPdf( "signal_fit" , "signal_fit" , x , signal_dataset );

   RooPlot* frame2 = x.frame();
   signal_fit->plotOn( frame2 );

   TCanvas* c2 = new TCanvas("c2","c2");
   frame2->Draw();
   c2->SaveAs("test2.png");


   return 0;
}


//------------------------------------------------------------------------------
//   Helper function implementation
//------------------------------------------------------------------------------
void FillDataSet( RooDataSet& dataset , SampleMgr& sample , const double total_lumi )
{
   static fwlite::Handle<vector<pat::Jet>> jetHandle;
   static fwlite::Handle<LHEEventProduct>  lheHandle;
   const double sample_weight = total_lumi > 0 ? sample.GetSampleWeight( total_lumi ) : 1 ;
   LOOP_EVENT( sample.Event() ){
      jetHandle.getByLabel( sample.Event() , "skimmedPatJets" );
      lheHandle.getByLabel( sample.Event() , "externalLHEProducer" );

      double tstarMass = jetHandle->front().pt() ;
      double event_weight = 1.0 ;
      if( lheHandle.isValid() && lheHandle->hepeup().XWGTUP <= 0 ) { event_weight = -1.; }
      double weight = event_weight * sample_weight ;
      if( minmass <=tstarMass && tstarMass <= maxmass  &&
         minweight <= weight && weight <= maxweight ){
            x = tstarMass;
            dataset.add( RooArgSet(x) , weight );
      }
   }

}
