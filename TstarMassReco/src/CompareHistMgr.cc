/*******************************************************************************
 *
 *  Filename    : CompareHistMgr.cc
 *  Description : Implementation of comparison manager
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/CompareHistMgr.hh"

using namespace std;
using fwlite::Event;
using fwlite::Handle;

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------
CompareHistMgr::CompareHistMgr(
   const string& name,
   const string& latex_name,
   const string& model,
   const string& product,
   const string& process
):
   _name(name),
   _latex_name(latex_name),
   _module_label(model),
   _product_label(product),
   _process_label(process)
{
   AddHist( "TstarMass" , "t^{*} Mass"            , "GeV/c^{2}", 120, 0, 3000 );
   AddHist( "ChiSq"     , "#chi^{2} of Method"    , ""         , 120, 0, 3000 );
   AddHist( "LepTopMass", "Leptonic Top Mass"     , "GeV/c^{2}", 100, 0, 500  );
   AddHist( "HadTopMass", "Hadronic Top Mass"     , "GeV/c^{2}", 100, 0, 500  );
   AddHist( "HadWMass"  , "Hadronic W Boson Mass" , "GeV/c^{2}", 40,  0, 200  );
   AddHist( "LepPtDiff"  , "Difference in Lepton p_{T}",         "GeV/c", 80,  -200, 200 );
   AddHist( "LepBPtDiff" , "Difference in Leptonic b-tag p_{T}", "GeV/c", 80,  -200, 200 );
   AddHist( "LepGPtDiff" , "Difference in Leptonic gluon p_{T}", "GeV/c", 80,  -200, 200 );
   AddHist( "HadBPtDiff" , "Difference in Hadronic b-tag p_{T}", "GeV/c", 80,  -200, 200 );
   AddHist( "HadGPtDiff" , "Difference in Hadronic gluon p_{T}", "GeV/c", 80,  -200, 200 );
   AddHist( "NeuPz"     , "Neutrino P_{z}"        , "GeV/c",     40,  0, 400 );
   _event_count  = 0 ;
   _lep_match    = 0 ;
   _lep_b_match  = 0 ;
   _lep_g_match  = 0 ;
   _had_j1_match = 0 ;
   _had_j2_match = 0 ;
   _had_b_match  = 0 ;
   _had_g_match  = 0 ;
}

CompareHistMgr::~CompareHistMgr()
{
   for( auto& hist : _hist_list ){
      delete hist;
   }
}

//------------------------------------------------------------------------------
//   Main control flow
//------------------------------------------------------------------------------
void CompareHistMgr::AddEvent( const fwlite::Event& ev )
{
   _result_handle.getByLabel( ev, _module_label.c_str(), _product_label.c_str(), _process_label.c_str() );
   const RecoResult& result = *(_result_handle);
   Hist( "TstarMass"  )->Fill( result.TstarMass() );
   Hist( "ChiSq" )->Fill( result.ChiSquare() );
   Hist( "LepTopMass" )->Fill( result.LeptonicTop().M()   );
   Hist( "HadTopMass" )->Fill( result.HadronicTop().M()   );
   Hist( "HadWMass"   )->Fill( result.HadronicW().M()   );
   Hist( "LepPtDiff"  )->Fill( result.Lepton().FittedP4().Pt()        - result.Lepton().ObservedP4().Pt() );
   Hist( "LepBPtDiff" )->Fill( result.LeptonicBJet().FittedP4().Pt()  - result.LeptonicBJet().ObservedP4().Pt() );
   Hist( "LepGPtDiff" )->Fill( result.LeptonicGluon().FittedP4().Pt() - result.LeptonicGluon().ObservedP4().Pt() );
   Hist( "HadBPtDiff" )->Fill( result.HadronicBJet().FittedP4().Pt()  - result.HadronicBJet().ObservedP4().Pt() );
   Hist( "HadGPtDiff" )->Fill( result.HadronicGluon().FittedP4().Pt() - result.HadronicGluon().ObservedP4().Pt() );
   Hist( "NeuPz"      )->Fill( result.Neutrino().FittedP4().Pz()   );

   // Match counting
   ++_event_count;
   if( result.Lepton().FitMatchTruth() )      { ++_lep_match; }
   if( result.LeptonicBJet().FitMatchTruth() ){ ++_lep_b_match; }
   if( result.LeptonicGluon().FitMatchTruth() ){ ++_lep_g_match; }
   if( result.HadronicJet1().FitMatchTruth() ) { ++_had_j1_match; }
   if( result.HadronicJet2().FitMatchTruth() ) { ++_had_j2_match; }
   if( result.HadronicBJet().FitMatchTruth() ) { ++_had_b_match; }
   if( result.HadronicGluon().FitMatchTruth() ) { ++_had_g_match; }
}

//------------------------------------------------------------------------------
//   Access functions
//------------------------------------------------------------------------------
TH1D* CompareHistMgr::Hist( const std::string& hist_name )
{
   for( auto& hist : _hist_list ){
      if( Name()+hist_name == hist->GetName() ){
         return hist;
      }
   }
   return NULL;
}

vector<string> CompareHistMgr::AvailableHistList() const
{
   vector<string> ans;
   for( const auto& hist : _hist_list ){
      string name = hist->GetName();
      name.erase( 0 , Name().length() );
      ans.push_back( name );
   }
   return ans;
}

void CompareHistMgr::SetColor( const Color_t x )
{
   for( auto& hist : _hist_list ){
      hist->SetLineColor( x );
   }
}

//------------------------------------------------------------------------------
//   Match rate computations
//------------------------------------------------------------------------------
unsigned CompareHistMgr::EventCount() const { return _event_count; }
double CompareHistMgr::LepMatchRate() const  { return (double)_lep_match/double(_event_count); }
double CompareHistMgr::LepBMatchRate() const { return (double)_lep_b_match/double(_event_count); }
double CompareHistMgr::LepGMatchRate() const { return (double)_lep_g_match/double(_event_count); }
double CompareHistMgr::HadJ1MatchRate() const{ return (double)_had_j1_match/double(_event_count); }
double CompareHistMgr::HadJ2MatchRate() const{ return (double)_had_j2_match/double(_event_count); }
double CompareHistMgr::HadBMatchRate() const { return (double)_had_b_match/double(_event_count); }
double CompareHistMgr::HadGMatchRate() const { return (double)_had_g_match/double(_event_count); }


//------------------------------------------------------------------------------
//   Helper function implementation
//------------------------------------------------------------------------------
void CompareHistMgr::AddHist(
   const string& label ,
   const string& x_label,
   const string& x_unit,
   const int bin_count,
   const double x_min,
   const double x_max )
{
   char y_title[1024];
   sprintf( y_title , "Yield [1/%.2lf %s]" ,
      (x_max - x_min)/bin_count,
      x_unit.c_str()
   );

   string x_title = x_label;
   if( x_unit != "" ){
      x_title += " [" + x_unit + "]" ;
   }
   const string hist_name = Name() + label ;
   const string hist_title = label +";" + x_title + ";" + y_title ;
   _hist_list.push_back( new TH1D( hist_name.c_str(), hist_title.c_str(), bin_count, x_min, x_max ) );
   _hist_list.back()->SetStats(0);
}
