/*******************************************************************************
 *
 *  Filename    : ChiSquareResult.cc
 *  Description : Implementations of ChiSquare Results
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"

ChiSquareResult::ChiSquareResult(
      double tstar                    ,
      double chisq                    ,
      const TLorentzVector& lep       ,
      const TLorentzVector& neu       ,
      const TLorentzVector& lep_w     ,
      const TLorentzVector& lep_t     ,
      const TLorentzVector& lep_tstar ,
      const TLorentzVector& had_w     ,
      const TLorentzVector& had_t     ,
      const TLorentzVector& had_tstar )
{
   _tstarMass = tstar ;
   _chiSquare = chisq ;
   _lepton = lep ;
   _neutrino = neu ;
   _hadronicW = had_w ;
   _hadronicTop = had_t ;
   _hadronicTstar = had_tstar ;
   _leptonicW = lep_w ;
   _leptonicTop = lep_t ;
   _leptonicTstar = lep_tstar;
}
ChiSquareResult::ChiSquareResult(){}
ChiSquareResult::~ChiSquareResult(){}

double ChiSquareResult::TstarMass() const { return _tstarMass ; }
double ChiSquareResult::ChiSquare() const { return _chiSquare ; }
const TLorentzVector& ChiSquareResult::Lepton() const        { return _lepton ; }
const TLorentzVector& ChiSquareResult::Neutrino() const      { return _neutrino ; }
const TLorentzVector& ChiSquareResult::HadronicW() const     { return _hadronicW ; }
const TLorentzVector& ChiSquareResult::HadronicTop() const   { return _hadronicTop ; }
const TLorentzVector& ChiSquareResult::HadronicTstar() const { return _hadronicTstar ; }
const TLorentzVector& ChiSquareResult::LeptonicW() const     { return _leptonicW ; }
const TLorentzVector& ChiSquareResult::LeptonicTop() const   { return _leptonicTop ; }
const TLorentzVector& ChiSquareResult::LeptonicTstar() const { return _leptonicTstar ; }


ChiSquareResult& ChiSquareResult::operator=( const ChiSquareResult& x )
{
   _tstarMass     = x._tstarMass     ;
   _chiSquare     = x._chiSquare     ;
   _lepton        = x._lepton        ;
   _neutrino      = x._neutrino      ;
   _hadronicW     = x._hadronicW     ;
   _hadronicTop   = x._hadronicTop   ;
   _hadronicTstar = x._hadronicTstar ;
   _leptonicW     = x._leptonicW     ;
   _leptonicTop   = x._leptonicTop   ;
   _leptonicTstar = x._leptonicTstar ;

   return *this;
}
