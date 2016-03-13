/*******************************************************************************
 *
 *  Filename    : ChiSquareSolver.h
 *  Description : Implementations for the ChiSquare Mass solver
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareSolver.h"
#include <algorithm>
#include <iostream>
using namespace std;
//------------------------------------------------------------------------------
//   Defining constants
//------------------------------------------------------------------------------
#define TOP_MASS    173.34
#define TOP_WIDTH   2.00
#define W_MASS      80.385
#define W_WIDTH     2.085
#define TSTAR_WIDTH 10
#define REQUIRED_BJETS_MATCHING 1
#define MAX_JETS_TO_RUN 6
static const ComparePT<TLorentzVector> comp;

//------------------------------------------------------------------------------
//   Constructor and destructor
//------------------------------------------------------------------------------
ChiSquareSolver::ChiSquareSolver(const edm::ParameterSet& iConfig):
   _debug( iConfig.getUntrackedParameter<int>( "Debug" , 0 ) )
{}

ChiSquareSolver::~ChiSquareSolver(){}

//------------------------------------------------------------------------------
//   Main Control Flow - Running Permutations
//------------------------------------------------------------------------------
void ChiSquareSolver::RunPermutations()
{
   double chiSquare;
   double tstarMass;
   TLorentzVector lep_w;
   TLorentzVector lep_t;
   TLorentzVector lep_tstar;
   TLorentzVector had_w;
   TLorentzVector had_t;
   TLorentzVector had_tstar;
   if( _debug ) { cout << "Running all chi square permutations" << endl; }
   do{
      do{ // Double Loop over b jets and light jets
         makeJetVecList();
         if( _debug > 2 ) {
            cout << "\t[ChiSq Jet Pt] " << flush;
            for( const auto& vec: _jetVecList ){
               cout << vec.Pt() << " " << flush;
            }
            cout << endl;
         }
         had_w     = had_q1() + had_q2();
         had_t     = had_w    + had_b();
         had_tstar = had_t    + had_g();

         for( unsigned i = 0 ; i < 2 ; ++i ){
            lep_w     = _lepton + _neutrino[i] ;
            lep_t     = lep_w   + lep_b();
            lep_tstar = lep_t   + lep_g();

            chiSquare =
            (( had_w.Mag()     - W_MASS         ) * ( had_w.Mag()     - W_MASS         )) / ( W_WIDTH     * W_WIDTH     )
            +(( had_t.Mag()     - TOP_MASS       ) * ( had_t.Mag()     - TOP_MASS       )) / ( TOP_WIDTH   * TOP_WIDTH   )
            +(( lep_t.Mag()     - TOP_MASS       ) * ( lep_t.Mag()     - TOP_MASS       )) / ( TOP_WIDTH   * TOP_WIDTH   )
            +(( lep_tstar.Mag() - had_tstar.Mag() ) * ( lep_tstar.Mag() - had_tstar.Mag() )) / ( TSTAR_WIDTH * TSTAR_WIDTH ) ;

            tstarMass = (lep_tstar.Mag() + had_tstar.Mag()) / 2.;
            _resultsList.push_back( ChiSquareResult(
               tstarMass,
               chiSquare,
               _lepton ,
               _neutrino[i],
               lep_w,
               lep_t,
               lep_tstar,
               had_w,
               had_t,
               had_tstar
            ) );
            if( _debug > 2 ) { cout << "\t " << tstarMass << " "<< chiSquare << endl; }
         }
      } while( next_permutation( _bjetList.begin() , _bjetList.end(), comp ) );
   } while( next_permutation( _ljetList.begin() , _ljetList.end(), comp ) );
}

const ChiSquareResult& ChiSquareSolver::BestResult() const
{
   if( _debug ) { cout << "Getting Best Results..." << endl; }
   unsigned index = -1;
   double min_chiSq = 100000000.;
   for( unsigned i = 0 ; i < _resultsList.size() ; ++i  ){
      if( _resultsList[i].ChiSquare() < min_chiSq ){
         min_chiSq = _resultsList[i].ChiSquare();
         index = i;
      }
   }

   if( _debug ){
      cout << "Found best results at " << index
      << " with chi^2 value: " << min_chiSq << endl;
   }
   return _resultsList[index];
}

//------------------------------------------------------------------------------
//   Setting functions
//------------------------------------------------------------------------------
void ChiSquareSolver::SetMET( const double pt, const double phi )
{
   _met = pt;
   _metphi = phi;
}

void ChiSquareSolver::SetLepton( const double pt, const double eta, const double phi, const double energy )
{
   _lepton.SetPtEtaPhiE( pt, eta, phi, energy );
   solveNeutrino();
}

void ChiSquareSolver::AddBTagJet( const double pt, const double eta, const double phi, const double energy )
{
   TLorentzVector jet;
   jet.SetPtEtaPhiE( pt, eta, phi, energy );
   if( _bjetList.size() < REQUIRED_BJETS_MATCHING ){
      _bjetList.push_back( jet );
   } else {
      _ljetList.push_back( jet );
   }
}
void ChiSquareSolver::AddLightJet( const double pt, const double eta, const double phi, const double energy )
{
   const unsigned target_lightjet_size = MAX_JETS_TO_RUN - _bjetList.size();
   TLorentzVector jet;
   jet.SetPtEtaPhiE( pt, eta, phi, energy );
   _ljetList.push_back( jet );
   // Sort by Pt
   stable_sort( _ljetList.begin() , _ljetList.end() , comp );
   // Truncating in size to avoid large run time.
   _ljetList.resize( target_lightjet_size );
}

void ChiSquareSolver::ClearAll()
{
   _bjetList.clear();
   _ljetList.clear();
   _jetVecList.clear();
   _resultsList.clear();
}


//------------------------------------------------------------------------------
//   Helper Private member functions
//------------------------------------------------------------------------------
void ChiSquareSolver::solveNeutrino()
{
   float _alpha_ , _beta_    , _gamma_          ;
   float _a_     , _b_       , _c_       , _d_  ;
   float _lx_    , _ly_      , _lz_      , _lE_ ;
   float _npx_   , _npy_     , _npz_     , _nE_ ;

   _npx_    = _met * cos( _metphi )    ;
   _npy_    = _met * sin( _metphi )    ;

   _lx_     = _lepton.Px()     ;
   _ly_     = _lepton.Py()     ;
   _lz_     = _lepton.Pz()     ;
   _lE_     = _lepton.Energy() ;

   _alpha_ = _npx_ + _lx_ ;
   _beta_  = _npy_ + _ly_ ;
   _gamma_ =   W_MASS*W_MASS  - _met*_met - _lE_*_lE_
   + _alpha_*_alpha_ + _beta_*_beta_ + _lz_*_lz_ ;

   _a_ =  4. *( _lE_*_lE_- _lz_*_lz_ );
   _b_ = -4. * _gamma_ * _lz_;
   _c_ =  4. * _lE_*_lE_ * _met*_met - _gamma_*_gamma_ ;
   _d_ = _b_ * _b_ - 4. * _a_ * _c_;

   if( _d_ < 0 ) {
      _npz_ = -1. * _b_ / ( 2.*_a_ ) ;
      _nE_  = sqrt( _npx_ * _npx_  + _npy_ * _npy_ + _npz_ * _npz_ ) ;
      _neutrino[0] = TLorentzVector( _npx_ , _npy_ , _npz_ , _nE_ ) ;
      _neutrino[1] = _neutrino[0];
   } else {
      _npz_ = ( -1. * _b_ + sqrt( _d_ ) )/ ( 2.*_a_ ) ;
      _nE_  = sqrt(_npx_ * _npx_  + _npy_ * _npy_ + _npz_ * _npz_ );
      _neutrino[0] = TLorentzVector( _npx_ , _npy_ , _npz_ , _nE_ ) ;
      _npz_ = ( -1. * _b_ - sqrt( _d_ ) )/ ( 2.*_a_ ) ;
      _nE_  = sqrt( _npx_ * _npx_  + _npy_ * _npy_ + _npz_ * _npz_) ;
      _neutrino[1] = TLorentzVector( _npx_ , _npy_ , _npz_ , _nE_ ) ;
   }
   if( _debug > 1 ){
      cout << "Lepton: " << _lepton.Pt() << endl;
      cout << "Neutrino:" << _neutrino[0].Pt() << endl;
   }
}

void ChiSquareSolver::makeJetVecList()
{
   // cout << "Making Jet List " << endl;
   _jetVecList.clear();
   for( const auto& bjet : _bjetList ){
      _jetVecList.push_back( bjet );
   }
   for( const auto& jet : _ljetList ){
      _jetVecList.push_back( jet );
   }
}
