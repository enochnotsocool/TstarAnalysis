/*******************************************************************************
 *
 *  Filename    : RecoResult.cc
 *  Description : Implementation of RecoResult class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/RecoResult.hh"

#include "TopQuarkAnalysis/TopHitFit/interface/Top_Decaykin.h"
#include "DataFormats/Math/interface/LorentzVector.h"

//------------------------------------------------------------------------------
//   Constructor and Desctructor
//------------------------------------------------------------------------------
RecoResult::RecoResult() {}
RecoResult::~RecoResult() {}
RecoResult& RecoResult::operator=( const RecoResult& x )
{
   _tstarMass = x._tstarMass;
   _chiSquare = x._chiSquare;
   _lepton        = x._lepton;
   _neutrino      = x._neutrino;
   _leptonicW     = x._leptonicW;
   _leptonicBJet  = x._leptonicBJet;
   _leptonicTop   = x._leptonicTop;
   _leptonicGluon = x._leptonicGluon;
   _leptonicTstar = x._leptonicTstar;
   _hadronicJet1  = x._hadronicJet1;
   _hadronicJet2  = x._hadronicJet2;
   _hadronicW     = x._hadronicW;
   _hadronicBJet  = x._hadronicBJet;
   _hadronicTop   = x._hadronicTop;
   _hadronicGluon = x._hadronicGluon;
   _hadronicTstar = x._hadronicTstar;

   return *this;
}

//------------------------------------------------------------------------------
//   Access Members
//------------------------------------------------------------------------------
double RecoResult::TstarMass() const { return _tstarMass; }
double RecoResult::ChiSquare() const { return _chiSquare; }
TLorentzVector RecoResult::Lepton()        const { return _lepton; }
TLorentzVector RecoResult::Neutrino()      const { return _neutrino; }
TLorentzVector RecoResult::LeptonicW()     const { return _leptonicW; }
TLorentzVector RecoResult::LeptonicBJet()  const { return _leptonicBJet; }
TLorentzVector RecoResult::LeptonicTop()   const { return _leptonicTop; }
TLorentzVector RecoResult::LeptonicGluon() const { return _leptonicGluon; }
TLorentzVector RecoResult::LeptonicTstar() const { return _leptonicTstar; }
TLorentzVector RecoResult::HadronicJet1()  const { return _hadronicJet1; }
TLorentzVector RecoResult::HadronicJet2()  const { return _hadronicJet2; }
TLorentzVector RecoResult::HadronicW()     const { return _hadronicW; }
TLorentzVector RecoResult::HadronicBJet()  const { return _hadronicBJet; }
TLorentzVector RecoResult::HadronicTop()   const { return _hadronicTop; }
TLorentzVector RecoResult::HadronicGluon() const { return _hadronicGluon; }
TLorentzVector RecoResult::HadronicTstar() const { return _hadronicTstar; }


//------------------------------------------------------------------------------
//   Translator functions
//------------------------------------------------------------------------------
TLorentzVector RecoResult::ToRootFormat( const hitfit::Fourvec& x )
{
   TLorentzVector ans;
   const math::XYZTLorentzVector y(x);
   ans.SetPtEtaPhiE( y.pt() , y.eta(), y.phi(), y.energy() );
   return ans;
}

void RecoResult::MakeFromHitFit( const hitfit::Fit_Result& x )
{
   _tstarMass  = x.mt();
   _chiSquare  = x.chisq() ;
   _lepton      = RecoResult::ToRootFormat( x.ev().lep(0).p() );
   _neutrino    = RecoResult::ToRootFormat( x.ev().met() );
   _leptonicW   = RecoResult::ToRootFormat( hitfit::Top_Decaykin::lepw( x.ev() ) );
   _leptonicTop = RecoResult::ToRootFormat( hitfit::Top_Decaykin::lept( x.ev() ) );
   _hadronicW   = RecoResult::ToRootFormat( hitfit::Top_Decaykin::hadw( x.ev() ) );
   _hadronicTop = RecoResult::ToRootFormat( hitfit::Top_Decaykin::hadt( x.ev() ) );
}

RecoResult::RecoResult(
      const double TstarMass,
      const double ChiSquare,
      const TLorentzVector& Lepton,
      const TLorentzVector& Neutrino,
      const TLorentzVector& LeptonicBJet,
      const TLorentzVector& LeptonicGluon,
      const TLorentzVector& HadronicJet1,
      const TLorentzVector& HadronicJet2,
      const TLorentzVector& HadronicBJet,
      const TLorentzVector& HadronicGluon
) {
   _tstarMass = TstarMass;
   _chiSquare = ChiSquare;
   _lepton        = Lepton;
   _neutrino      = Neutrino;
   _leptonicW     = _lepton + _neutrino;
   _leptonicBJet  = LeptonicBJet;
   _leptonicTop   = _leptonicW + _leptonicBJet ;
   _leptonicGluon = LeptonicGluon;
   _leptonicTstar = _leptonicTop + _leptonicGluon;
   _hadronicJet1  = HadronicJet1;
   _hadronicJet2  = HadronicJet2;
   _hadronicW     = _hadronicJet1 + _hadronicJet2;
   _hadronicBJet  = HadronicBJet;
   _hadronicTop   = _hadronicW + _hadronicBJet ;
   _hadronicGluon = HadronicGluon;
   _hadronicTstar = _hadronicTop + _hadronicGluon;
}
