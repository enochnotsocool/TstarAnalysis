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
CompareHistMgr::CompareHistMgr(const string& name,const string& model, const string& product, const string& process):
   _name(name),
   _module_label(model),
   _product_label(product),
   _process_label(process)
{
   AddHist( "TstarMass" , "t^{*} Mass" , "GeV/c^{2}", 60, 0, 3000);
   AddHist( "LepTopMass", "Leptonic Top Mass"               , "GeV/c^{2}" , 35 , 0, 350 );
   AddHist( "LepTopPt"  , "Leptonic Top Tranverse Momentum" , "GeV/c"     , 35 , 0, 350 );
   AddHist( "LepTopEta" , "Leptonic Top #eta"               , ""          , 48 , -2.4,  2.4);
   AddHist( "HadTopMass", "Hadronic Top Mass"               , "GeV/c^{2}" , 35 , 0, 350 );
   AddHist( "HadTopPt"  , "Hadronic Top Tranverse Momentum" , "GeV/c"     , 35 , 0, 350 );
   AddHist( "HadTopEta" , "Hadronic Top #eta"               , ""          , 48 , -2.4,  2.4);
   AddHist( "HadWMass"  , "Hadronic W Boson Mass"               , "GeV/c^{2}" , 20 , 0, 200 );
   AddHist( "HadWPt"    , "Hadronic W Boson Tranverse Momentum" , "GeV/c"     , 35 , 0, 350 );
   AddHist( "HadWEta"   , "Hadronic W Boson #eta"               , ""          , 48 , -2.4,  2.4);
   AddHist( "NeuPz"     , "Neutrino P_{z}" , "GeV/c" , 20, 0, 1000 );
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
   Hist( "LepTopMass" )->Fill( result.LeptonicTop().M()   );
   Hist( "LepTopPt"   )->Fill( result.LeptonicTop().Pt()  );
   Hist( "LepTopEta"  )->Fill( result.LeptonicTop().Eta() );
   Hist( "HadTopMass" )->Fill( result.HadronicTop().M()   );
   Hist( "HadTopPt"   )->Fill( result.HadronicTop().Pt()  );
   Hist( "HadTopEta"  )->Fill( result.HadronicTop().Eta() );
   Hist( "HadWMass"   )->Fill( result.HadronicW().M()   );
   Hist( "HadWPt"     )->Fill( result.HadronicW().Pt()  );
   Hist( "HadWEta"    )->Fill( result.HadronicW().Eta() );
   Hist( "NeuPz"      )->Fill( result.Neutrino().Pz()   );
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
