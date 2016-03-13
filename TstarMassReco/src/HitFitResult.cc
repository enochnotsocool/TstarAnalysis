/*******************************************************************************
 *
 *  Filename    : HitFitResult.cc
 *  Description : implementation of minimal hit fit results
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TopQuarkAnalysis/TopHitFit/interface/Top_Decaykin.h"
#include "TstarAnalysis/TstarMassReco/interface/HitFitResult.h"
#include "DataFormats/Math/interface/LorentzVector.h"

HitFitResult::HitFitResult(){}
HitFitResult::~HitFitResult(){}

double HitFitResult::TstarMass() const  { return _tstarMass; }
double HitFitResult::TstarSigma() const { return _tstarSigma; }
double HitFitResult::ChiSquare() const  { return _chiSquare; }

const TLorentzVector& HitFitResult::Lepton() const      { return _lepton ; }
const TLorentzVector& HitFitResult::Neutrino() const    { return _neutrino ; }
const TLorentzVector& HitFitResult::LeptonicW() const   { return _leptonicW ; }
const TLorentzVector& HitFitResult::LeptonicTop() const { return _leptonicTop ; }
const TLorentzVector& HitFitResult::HadronicW() const   { return _hadronicW ; }
const TLorentzVector& HitFitResult::HadronicTop() const { return _hadronicTop ; }

void HitFitResult::MakeResult( const hitfit::Fit_Result& x )
{
   _tstarMass  = x.mt();
   _tstarSigma = x.sigmt() ;
   _chiSquare  = x.chisq() ;
   _lepton      = HitFitResult::ToRootFormat( x.ev().lep(0).p() );
   _neutrino    = HitFitResult::ToRootFormat( x.ev().met() );
   _leptonicW   = HitFitResult::ToRootFormat( hitfit::Top_Decaykin::lepw( x.ev() ) );
   _leptonicTop = HitFitResult::ToRootFormat( hitfit::Top_Decaykin::lept( x.ev() ) );
   _hadronicW   = HitFitResult::ToRootFormat( hitfit::Top_Decaykin::hadw( x.ev() ) );
   _hadronicTop = HitFitResult::ToRootFormat( hitfit::Top_Decaykin::hadt( x.ev() ) );
}

TLorentzVector HitFitResult::ToRootFormat( const hitfit::Fourvec& x )
{
   TLorentzVector ans;
   const math::XYZTLorentzVector y(x);
   ans.SetPtEtaPhiE( y.pt() , y.eta(), y.phi(), y.energy() );
   return ans;
}
