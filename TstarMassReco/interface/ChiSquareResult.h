/*******************************************************************************
 *
 *  Filename    : ChiSquareResults.h
 *  Description : Storing results for Chi square sorting method
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __CHISQUARERESULTS_H__
#define __CHISQUARERESULTS_H__

#include "TLorentzVector.h"

class ChiSquareResult
{
public:
   ChiSquareResult();
   ChiSquareResult(
         double tstar,
         double chisq,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector& ,
         const TLorentzVector&
         );
   virtual ~ChiSquareResult();

   ChiSquareResult& operator=( const ChiSquareResult& );

   double ChiSquare() const;
   double TstarMass() const;

   const TLorentzVector& Lepton() const;
   const TLorentzVector& Neutrino() const;
   const TLorentzVector& HadronicW() const;
   const TLorentzVector& HadronicTop() const;
   const TLorentzVector& HadronicTstar() const;
   const TLorentzVector& LeptonicW() const;
   const TLorentzVector& LeptonicTop() const;
   const TLorentzVector& LeptonicTstar() const;

private:
   double _tstarMass;
   double _chiSquare;
   TLorentzVector _lepton;
   TLorentzVector _neutrino;
   TLorentzVector _hadronicW;
   TLorentzVector _hadronicTop;
   TLorentzVector _hadronicTstar;
   TLorentzVector _leptonicW;
   TLorentzVector _leptonicTop;
   TLorentzVector _leptonicTstar;

};

#endif // __CHISQUARERESULTS_H__
