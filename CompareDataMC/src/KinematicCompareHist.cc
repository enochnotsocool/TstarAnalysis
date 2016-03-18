/*******************************************************************************
 *
 *  Filename    : KinematicCompareHist.cc
 *  Description : Defining histograms and fill methods
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/CompareDataMC/interface/KinematicCompareHist.hh"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"

using namespace std;

//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------
KinematicCompareHist::KinematicCompareHist( SampleMgr& sample , const double total_lumi ):
   _name( sample.Name() ),
   _sample_ref( &sample )
{
   _histlist.clear();
   // Defining Histograms
   // Format
   // Label , Title , Xaxis , X units, binCount, x_lower, x_upper
   AddHist( "LepPt"     , "Lepton Transverse Momentum"     , "L_{P_{T}}"          , "GeV/c"   , 50  , 0    , 500. );
   AddHist( "LepEta"    , "Lepton #eta"                    , "L_#eta"             , ""        , 50  , -2.5 , 2.5  );
   AddHist( "Jet1Pt"    , "Leading Jet P_{T}"              , "J_{1.P_{T}}"        , "GeV/c"   , 50  , 0    , 500. );
   AddHist( "Jet1Eta"   , "Leading Jet #eta"               , "J_{1.#eta}"         , ""        , 50  , -2.5 , 2.5  );
   AddHist( "JetNum"    , "Number of Jets"                 , "Number of Jets"     , ""        , 6   , 4    , 10   );
   AddHist( "MET"       , "Missing Transverse Energy"      , "Missing E_{T}"      , "GeV"     , 50  , 0    , 500. );
   AddHist( "METPhi"    , "Missing Transverse Energy"      , "Missing E_{T} #phi" , ""        , 6.8 , 0    , 6.28 );
   AddHist( "TstarMass" , "Mass of t+g system"             , "M_{t+g}"            , "GeV/c^2" , 50  , 0    , 3000 );
   AddHist( "ChiSq"     , "#chi^{2} of best configuration" , "#chi^{2}"           , ""        , 50  , 0    , 30000 );

   // Filling histogram with sample
   FillHistograms( sample , total_lumi );
}

void KinematicCompareHist::FillHistograms( SampleMgr& sample, const double total_lumi )
{
   fwlite::Handle<vector<pat::MET>>      metHandle;
   fwlite::Handle<vector<pat::Jet>>      jetHandle;
   fwlite::Handle<vector<pat::Muon>>     muonHandle;
   fwlite::Handle<vector<pat::Electron>> electronHandle;
   fwlite::Handle<LHEEventProduct>       lheHandle;
   fwlite::Handle<ChiSquareResult>       chisqHandle;

   double sample_weight;
   if( total_lumi > 0 ){
      sample_weight = sample.GetSampleWeight(total_lumi);
   } else {
      sample_weight = 1.0; // For running on data
   }

   unsigned i = 0;
   LOOP_EVENT( sample.Event() ){
      cout << "\rWorking on sample (" << sample.Name()
           << ") at Event: [" << ++i  << "/" << sample.Event().size() << "]"<< flush  ;

      metHandle.getByLabel(      sample.Event() , "slimmedMETs"    );
      jetHandle.getByLabel(      sample.Event() , "skimmedPatJets" );
      muonHandle.getByLabel(     sample.Event() , "skimmedPatMuons" );
      electronHandle.getByLabel( sample.Event() , "skimmedPatElectrons" );
      chisqHandle.getByLabel(    sample.Event() , "tstarMassReco" , "ChiSquareResult" , "TstarMassReco" );

      if( total_lumi > 0 ){
         lheHandle.getByLabel(      sample.Event() , "externalLHEProducer" );
      }

      double event_weight = 1.;
      if( total_lumi > 0 ){
         if( lheHandle.isValid() && lheHandle->hepeup().XWGTUP <= 0 ){
            event_weight = -1. ;
         } else {
            event_weight = 1. ;
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

      GetHist("JetNum")->Fill( jetHandle->size()        , total_weight );
      GetHist("Jet1Pt")->Fill( jetHandle->front().pt()  , total_weight );

      GetHist("MET")->Fill( metHandle->front().pt()     , total_weight );
      GetHist("METPhi")->Fill( metHandle->front().phi() , total_weight );

      GetHist("TstarMass")->Fill( chisqHandle->TstarMass() , total_weight );
      GetHist("ChiSq")->Fill( chisqHandle->ChiSquare()     , total_weight );
   }
   cout << "Done!" << endl;
}


//------------------------------------------------------------------------------
//   Helper/Debug functions
//------------------------------------------------------------------------------
KinematicCompareHist::~KinematicCompareHist()
{
   // for( auto& hist : _histlist ){ delete hist; }
   // _histlist.clear();
}

void KinematicCompareHist::AddHist(
   const string& label,
   const string& title,
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
   x_unit.c_str() );

   string x_title = x_label;
   if( x_unit != "" ){
      x_title += " [" + x_unit + "]" ;
   }
   const string hist_name = _name + label ;
   const string hist_title = title +";" + x_title + ";" + y_title ;

   _histlist.push_back( new TH1D( hist_name.c_str(), hist_title.c_str(), binCount, x_lower, x_upper ) );
}

TH1D* KinematicCompareHist::GetHist( const std::string& name )
{
   for( auto hist : _histlist ){
      if( (_name + name) == hist->GetName()){
         return hist;
      }
   }
   cout << 0 << flush ;
   return NULL ;
}

vector<string> KinematicCompareHist::AvailableHistList() const
{
   vector<string> ans;
   for( const auto& hist : _histlist ){
      string name = hist->GetName();
      name.erase( 0 , _name.length() );
      ans.push_back( name );
   }
   return ans;
}

void KinematicCompareHist::SetColor( const Color_t& x )
{
   for( auto hist : _histlist ){
      hist->SetFillColor( x );
      hist->SetLineColor( x );
   }
}
