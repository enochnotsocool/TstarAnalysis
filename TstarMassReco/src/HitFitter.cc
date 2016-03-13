/*******************************************************************************
 *
 *  Filename    : HitFitter.cc
 *  Description : Implementations of HitFitter class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/HitFitter.h"

#include "TopQuarkAnalysis/TopHitFit/interface/fourvec.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Defaults_Text.h"

#include <iostream>
#include <algorithm>
using namespace hitfit;
using namespace std;
//------------------------------------------------------------------------------
//   Constant variables
//------------------------------------------------------------------------------
#define DEFAULT_W_MASS    80.4
#define DEFAULT_TOP_MASS   0.0
static const edm::FileInPath defaultFitter( "TstarAnalysis/TstarMassReco/data/top_gluon.txt" );
static const edm::FileInPath defaultElecFile( "TopQuarkAnalysis/TopHitFit/data/resolution/tqafElectronResolution.txt" );
static const edm::FileInPath defaultMuonFile( "TopQuarkAnalysis/TopHitFit/data/resolution/tqafMuonResolution.txt" );
static const edm::FileInPath defaultLJetFile( "TopQuarkAnalysis/TopHitFit/data/resolution/tqafUdscJetResolution.txt" );
static const edm::FileInPath defaultBJetFile( "TopQuarkAnalysis/TopHitFit/data/resolution/tqafBJetResolution.txt" );

#define MAX_HIT_FIT_JETS  6

//------------------------------------------------------------------------------
//   Constructor and Destructor
//------------------------------------------------------------------------------
HitFitter::HitFitter( const edm::ParameterSet& iConfig ):
   _debug( iConfig.getUntrackedParameter<int>( "Debug" , 0 ) )
{
   if( _debug ) { cout << "Getting Top_Fit variables.... " << endl ; }
   const edm::FileInPath fitterFile = iConfig.getUntrackedParameter<edm::FileInPath>( "fitterConfig" , defaultFitter );
   double fitter_lepWMass = iConfig.getUntrackedParameter<double>( "fittingLeptonicWMass" ,  DEFAULT_W_MASS );
   double fitter_hadWMass = iConfig.getUntrackedParameter<double>( "fittingHadronicWMass" ,  DEFAULT_W_MASS );
   double fitter_topMass  = iConfig.getUntrackedParameter<double>( "fittingTopMass" ,  DEFAULT_TOP_MASS );
   _top_fitter = new Top_Fit( Defaults_Text(fitterFile.fullPath()) , fitter_lepWMass , fitter_hadWMass , fitter_topMass );

   if( _debug ) { cout << "Getting Resolutions..." << endl; }
   const edm::FileInPath elecResolutionFile = iConfig.getUntrackedParameter<edm::FileInPath>( "electronResolution" , defaultElecFile );
   const edm::FileInPath muonResolutionFile = iConfig.getUntrackedParameter<edm::FileInPath>( "muonResolution" , defaultMuonFile );
   const edm::FileInPath ljetResolutionFile = iConfig.getUntrackedParameter<edm::FileInPath>( "lightJetResolution" , defaultLJetFile );
   const edm::FileInPath bjetResolutionFile = iConfig.getUntrackedParameter<edm::FileInPath>( "bJetResolution" , defaultBJetFile );
   _electronResolution = new EtaDepResolution( elecResolutionFile.fullPath() );
   _muonResolution     = new EtaDepResolution( muonResolutionFile.fullPath() );
   _lightJetResolution = new EtaDepResolution( ljetResolutionFile.fullPath() );
   _bJetResolution     = new EtaDepResolution( bjetResolutionFile.fullPath() );
   _met_KtResolution   = new Resolution("0,0,12");

}

HitFitter::~HitFitter()
{
   if( _debug ) { cout << "Clearing analysis wide objects" << endl; }
   delete _top_fitter;
   delete _electronResolution;
   delete _muonResolution;
   delete _lightJetResolution;
   delete _bJetResolution;
   delete _met_KtResolution;
}

//------------------------------------------------------------------------------
//   Main control - Runing HitFit permutations
//------------------------------------------------------------------------------
void HitFitter::SetMET( const double met, const double metphi )
{
   _met = met;
   _metPhi = metphi;
}

void HitFitter::SetLepton(
   const double pt,
   const double eta,
   const double phi,
   const double energy,
   const bool   leptontype )
{
   _lepton.SetPtEtaPhiE( pt, eta, phi,energy );
   _leptontype = leptontype;
}

void HitFitter::AddBTagJet( const double pt, const double eta, const double phi, const double energy )
{
   TLorentzVector jet;
   jet.SetPtEtaPhiE( pt, eta, phi, energy );
   _btagJetList.push_back( jet );
}

void HitFitter::AddLightJet( const double pt, const double eta, const double phi, const double energy )
{
   TLorentzVector jet;
   jet.SetPtEtaPhiE( pt, eta, phi, energy );
   _lightJetList.push_back( jet );
}

void HitFitter::RunPermutations()
{
   // Dummy variables for storing fitting results;
   bool   solveNeutrino;
   double fittedWHadronMass;
   double fittedTopMass;
   double fittedTopMass2;
   double sumTopMass;
   double chiSquare;
   Column_Vector pullx;
   Column_Vector pully;

   // Dummy event only handling MET and Lepton
   // Jets are handled in the permutation loop
   if( _debug ) { cout << " Setting up template event " << endl; }
   Lepjets_Event hitFitEventTemplate( 0,0 );

   //----- Setting up objects -----
   AddHitFitMET( hitFitEventTemplate , _met, _metPhi );
   AddHitFitLepton( hitFitEventTemplate, _lepton, _leptontype  );

   //----- Setting up Jet Labels  -------------------------------------------------
   const unsigned numberOfJets = _btagJetList.size() + _lightJetList.size();
   const unsigned hitfitJetCount = (numberOfJets < MAX_HIT_FIT_JETS ) ? numberOfJets : MAX_HIT_FIT_JETS;
   vector<int> jet_type_list( hitfitJetCount , hitfit::unknown_label ) ;
   jet_type_list[0] = hitfit::lepb_label;
   jet_type_list[1] = hitfit::hadb_label;
   jet_type_list[2] = hitfit::hadw1_label;
   jet_type_list[3] = hitfit::hadw2_label;
   jet_type_list[4] = hitfit::gluon1_label;
   jet_type_list[5] = hitfit::gluon2_label;

   //----- Permute jet labels  ---------------------------------------------------
   do {
      if( _debug > 2) {
         cout << "\t Jet Type:" << flush;
         for( unsigned i = 0 ; i < jet_type_list.size() ; ++i ){
            cout << ", " << jet_type_list[i] << flush;
         }
         cout << endl;
      }
      if( !CheckBTagOrder( jet_type_list ) ) { continue; }

      Lepjets_Event hitFitEvent = hitFitEventTemplate;
      unsigned i = 0 ;
      for( const auto jet : _btagJetList ){
         AddHitFitJet( hitFitEvent, jet, jet_type_list[i]); ++i; }
      for( const auto jet : _lightJetList ){
         AddHitFitJet( hitFitEvent, jet, jet_type_list[i]); ++i;
         if( i >= hitfitJetCount ){ break; }
      }
      hitFitEvent.set_jet_types( jet_type_list );

      // Running Hit Fit
      chiSquare = _top_fitter->fit_one_perm(
            hitFitEvent,
            solveNeutrino,
            fittedWHadronMass,
            fittedTopMass,
            fittedTopMass2,
            sumTopMass,
            pullx,
            pully );

      // Store all results for now
      _resultList.push_back( Fit_Result(
               chiSquare,
               hitFitEvent,
               pullx,
               pully,
               fittedWHadronMass,
               fittedTopMass,
               fittedTopMass2,
               sumTopMass ));

   } while ( next_permutation( jet_type_list.begin() , jet_type_list.end() ) );
}

void HitFitter::ClearAll()
{
   if( _debug ) { cout << "Deleting all per-event results" << endl; }
   _resultList.clear();
   _btagJetList.clear();
   _lightJetList.clear();
}

const hitfit::Fit_Result& HitFitter::BestResult() const
{
   unsigned min_index  = -1;
   double min_ChiSquare = 100000000000.;
   double this_ChiSquare;

   if( _debug ) { cout << "Getting Best Results out of" << _resultList.size() << endl; }
   for( unsigned i = 0; i < _resultList.size() ; ++i ){
      this_ChiSquare = _resultList[i].chisq() ;
      if( _debug ) {
         if( this_ChiSquare <= 0. ){
            cout << "Found negative results at " << i << endl;
         } else if( this_ChiSquare > 100000000000. ){
            cout << "Found Huge chi square at " << i << endl;
         }
      }
      if( this_ChiSquare < min_ChiSquare && this_ChiSquare > 0.0 ){
         if( _debug > 2 ) { cout << "Best Result at" << i << endl; }
         min_index = i;
         min_ChiSquare = this_ChiSquare;
      }
   }
   if( min_index < _resultList.size() ) {
      if( _debug ) { cout << "Returning index "  << min_index << endl;  }
      return _resultList[min_index];
   } else {
      cerr << "Warning! Legal Results found! Returning first in list" << endl;
      return _resultList[0];
   }
}

//------------------------------------------------------------------------------
//   Translator private functions
//------------------------------------------------------------------------------
void HitFitter::AddHitFitMET( Lepjets_Event& hitFitEvent, const double met, const double metphi ) const
{
   const double px = met*cos(metphi);
   const double py = met*sin(metphi);
   hitFitEvent.met()   = Fourvec( px, py , 0 , met );
   hitFitEvent.kt_res() = *( _met_KtResolution );
}

void HitFitter::AddHitFitLepton( Lepjets_Event& hitFitEvent , const TLorentzVector& lep , const bool leptontype ) const
{
   double lep_eta = lep.Eta() ; // Required! For type conversion
   Fourvec lep_p4( lep.Px() , lep.Py() , lep.Pz() , lep.Energy() );
   Vector_Resolution lep_resolution = (leptontype == ISMUON) ?
      _muonResolution->GetResolution( lep_eta ) : _electronResolution->GetResolution( lep_eta );
   hitFitEvent.add_lep( Lepjets_Event_Lep(lep_p4 , 0 , lep_resolution) ); //label = 0 for primary lepton
}

void HitFitter::AddHitFitJet( Lepjets_Event& hitFitEvent, const TLorentzVector& jet , const int type ) const
{
   double jet_eta = jet.Eta();
   Fourvec jetp4( jet.Px() , jet.Py() , jet.Pz(), jet.Energy() );
   Vector_Resolution jet_resolution ;
   if( type == hitfit::hadb_label || type == hitfit::lepb_label ){
      jet_resolution = _bJetResolution->GetResolution( jet_eta );
   } else {
      jet_resolution = _lightJetResolution->GetResolution( jet_eta );
   }
   hitFitEvent.add_jet( Lepjets_Event_Jet(jetp4 , type , jet_resolution) );
}

bool HitFitter::CheckBTagOrder( const std::vector<int>& jet_type_list ) const
{
   if( jet_type_list[0] == hitfit::hadb_label ) return true;
   if( jet_type_list[0] == hitfit::lepb_label ) return true;
   return false;
}
