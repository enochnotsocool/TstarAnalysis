/*******************************************************************************
 *
 *  Filename    : SampleHistMgr.cc
 *  Description : Implementation for sample Mgr with histograms
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/SampleHistMgr.hh"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------
SampleHistMgr::SampleHistMgr( const string& name ):
   SampleGroup( name )
{
   _histlist.clear();
   AddHist( "LepPt"     , "Lepton {P_{T}}"            , "GeV/c"   , 48 , 20   , 500. );
   AddHist( "LepEta"    , "Lepton #eta"               , ""        , 75 , -2.5 , 5.0  );
   AddHist( "Jet1Pt"    , "First Leading Jet P_{T}"   , "GeV/c"   , 60 , 30   , 1000. );
   AddHist( "Jet2Pt"    , "Second Leading Jet P_{T}"  , "GeV/c"   , 60 , 30   , 1000. );
   AddHist( "Jet1Eta"   , "First Leading Jet #eta"    , ""        , 75 , -2.5 , 5.0  );
   AddHist( "Jet2Eta"   , "Second Leading Jet #eta"   , ""        , 75 , -2.5 , 5.0  );
   AddHist( "JetNum"    , "Number of Jets"            , ""        , 5  , 5    , 12   );
   AddHist( "MET"       , "Missing E_{T}"             , "GeV"     , 50 , 0    , 500. );
   AddHist( "METPhi"    , "Missing E_{T} #phi"        , ""        , 96 , -3.2 , 6.4  );
   AddHist( "TstarMass" , "M_{t+g}"                   , "GeV/c^2" , 50 , 0    , 2000 );
   AddHist( "ChiSq"     , "#chi^{2}"                  , ""        , 50 , 0    , 10000 );
   AddHist( "Jet3Pt"    , "Third Jet P_{T}"           , "GeV/c"   , 60 , 30   , 700.  );
   AddHist( "Jet4Pt"    , "Fourth Jet P_{T}"          , "GeV/c"   , 60 , 30   , 600. );
   AddHist( "Jet5Pt"    , "Fifth Jet P_{T}"           , "GeV/c"   , 60 , 30   , 400. );
   AddHist( "Jet6Pt"    , "Sixth Jet P_{T}"           , "GeV/c"   , 60 , 30   , 400. );
   AddHist( "LepGluonPt", "Leptonic Gluon Jet P_{T}"  , "GeV/c"   , 60 , 30   , 1000. );
   AddHist( "HadGluonPt", "Hadronic Gluon Jet P_{T}"  , "GeV/c"   , 60 , 30   , 1000. );

   for( auto& sample : SampleList() ){
      FillHistograms(sample);
   }

}


void SampleHistMgr::FillHistograms( SampleMgr& sample )
{
   fwlite::Handle<vector<pat::MET>>      metHandle;
   fwlite::Handle<vector<pat::Jet>>      jetHandle;
   fwlite::Handle<vector<pat::Muon>>     muonHandle;
   fwlite::Handle<vector<pat::Electron>> electronHandle;
   fwlite::Handle<LHEEventProduct>       lheHandle;
   fwlite::Handle<ChiSquareResult>       chisqHandle;

   double sample_weight = 1.;
   if( !sample.IsRealData() ) {
      sample_weight = sample.GetSampleWeight(); // For running on data
   }

   unsigned i = 0;
   LOOP_EVENT( sample.Event() ){
      printf( "\rSample [%s|%s], Event[%u/%llu]..." ,
         Name().c_str(),
         sample.Name().c_str(),
         ++i ,
         sample.Event().size()
      );
      fflush(stdout);

      metHandle.getByLabel(      sample.Event() , "slimmedMETs"    );
      jetHandle.getByLabel(      sample.Event() , "skimmedPatJets" );
      muonHandle.getByLabel(     sample.Event() , "skimmedPatMuons" );
      electronHandle.getByLabel( sample.Event() , "skimmedPatElectrons" );
      chisqHandle.getByLabel(    sample.Event() , "tstarMassReco" , "ChiSquareResult" , "TstarMassReco" );

      double event_weight = 1.;
      if( !sample.IsRealData() ){
         lheHandle.getByLabel( sample.Event() , "externalLHEProducer" );
         if( lheHandle.isValid() && lheHandle->hepeup().XWGTUP <= 0 ){
            event_weight = -1. ;
         }
      }
      double total_weight = event_weight * sample_weight ;

      for( const auto& mu : *muonHandle.product() ){
         GetHist("LepPt")->Fill( mu.pt() , total_weight );
         GetHist("LepEta")->Fill( mu.eta() , total_weight );
      }
      for( const auto& el : *electronHandle.product() ){
         GetHist("LepPt")->Fill( el.pt() , total_weight );
         GetHist("LepEta")->Fill( el.eta(), total_weight );
      }

      GetHist("JetNum")->Fill( jetHandle->size()       , total_weight );
      GetHist("Jet1Pt")->Fill( jetHandle->at(0).pt()   , total_weight );
      GetHist("Jet1Eta")->Fill( jetHandle->at(0).eta() , total_weight );
      GetHist("Jet2Pt")->Fill( jetHandle->at(1).pt()   , total_weight );
      GetHist("Jet2Eta")->Fill( jetHandle->at(1).eta() , total_weight );
      GetHist("Jet3Pt")->Fill( jetHandle->at(2).pt()   , total_weight );
      GetHist("Jet4Pt")->Fill( jetHandle->at(3).pt()   , total_weight );
      GetHist("Jet5Pt")->Fill( jetHandle->at(4).pt()   , total_weight );
      GetHist("Jet6Pt")->Fill( jetHandle->at(5).pt()   , total_weight );

      GetHist("MET")->Fill( metHandle->front().pt()     , total_weight );
      GetHist("METPhi")->Fill( metHandle->front().phi() , total_weight );

      GetHist("TstarMass")->Fill( chisqHandle->TstarMass() , total_weight );
      GetHist("ChiSq")->Fill( chisqHandle->ChiSquare()     , total_weight );
      GetHist("LepGluonPt")->Fill( (chisqHandle->LeptonicTstar() - chisqHandle->LeptonicTop()).Pt() , total_weight  );
      GetHist("HadGluonPt")->Fill( (chisqHandle->HadronicTstar() - chisqHandle->HadronicTop()).Pt() , total_weight  );
   }
   cout << "Done!" << endl;

}

//------------------------------------------------------------------------------
//   House keeping functions
//------------------------------------------------------------------------------
SampleHistMgr::~SampleHistMgr()
{
   for( auto& hist : _histlist ){ delete hist; }
   _histlist.clear();
}

void SampleHistMgr::AddHist(
   const string& label,
   const string& x_label,
   const string& x_unit,
   const int  binCount,
   const double x_lower,
   const double x_upper
)
{
   char y_title[1024];
   sprintf( y_title , "Yield [1/%.2lf %s]" ,
      (x_upper - x_lower)/binCount,
      x_unit.c_str()
   );

   string x_title = x_label;
   if( x_unit != "" ){
      x_title += " [" + x_unit + "]" ;
   }
   const string hist_name = Name() + label ;
   const string hist_title = label +";" + x_title + ";" + y_title ;
   _histlist.push_back( new TH1D( hist_name.c_str(), hist_title.c_str(), binCount, x_lower, x_upper ) );
}

TH1D* SampleHistMgr::GetHist( const std::string& name )
{
   for( auto hist : _histlist ){
      if( (Name() + name) == hist->GetName()){
         return hist;
      }
   }
   cout << 0 << flush ;
   return NULL ;
}

vector<string> SampleHistMgr::AvailableHistList() const
{
   vector<string> ans;
   for( const auto& hist : _histlist ){
      string name = hist->GetName();
      name.erase( 0 , Name().length() );
      ans.push_back( name );
   }
   return ans;
}

void SampleHistMgr::SetColor( const Color_t& x )
{
   for( auto hist : _histlist ){
      hist->SetFillColor( x );
      hist->SetLineColor( x );
   }
}

void SampleHistMgr::Scale( const double x )
{
   for( auto hist: _histlist ){
      for( int i = 0 ; i < hist->GetSize() ; ++i ){
         double bincontent = hist->GetBinContent(i);
         double binerror   = hist->GetBinError(i);
         bincontent *= x ;
         binerror   *= x ;
         hist->SetBinContent(i,bincontent);
         hist->SetBinError(i,binerror);
      }
   }
}
