/*******************************************************************************
 *
 *  Filename    : RecoResult.hh
 *  Description : Master class for reconstruction results
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __RECORESULTS_H__
#define __RECORESULTS_H__

#include "TLorentzVector.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Fit_Result.h"

class RecoResult {
public:
   RecoResult();
   virtual ~RecoResult ();

   RecoResult& operator=( const RecoResult& );

   double TstarMass() const;
   double ChiSquare() const;
   TLorentzVector Lepton()        const;
   TLorentzVector Neutrino()      const;
   TLorentzVector LeptonicW()     const;
   TLorentzVector LeptonicBJet()  const;
   TLorentzVector LeptonicTop()   const;
   TLorentzVector LeptonicGluon() const;
   TLorentzVector LeptonicTstar() const;
   TLorentzVector HadronicJet1()  const;
   TLorentzVector HadronicJet2()  const;
   TLorentzVector HadronicW()     const;
   TLorentzVector HadronicBJet()  const;
   TLorentzVector HadronicTop()   const;
   TLorentzVector HadronicGluon() const;
   TLorentzVector HadronicTstar() const;

   // Tranlator functions
   RecoResult( // For Chi Square sorting Method
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
   );

   // For HitFit Method
   void MakeFromHitFit( const hitfit::Fit_Result& );
   static TLorentzVector ToRootFormat( const hitfit::Fourvec& );

private:
   double _tstarMass;
   double _chiSquare;
   TLorentzVector _lepton       ;
   TLorentzVector _neutrino     ;
   TLorentzVector _leptonicW    ;
   TLorentzVector _leptonicBJet ;
   TLorentzVector _leptonicTop  ;
   TLorentzVector _leptonicGluon;
   TLorentzVector _leptonicTstar;
   TLorentzVector _hadronicJet1 ;
   TLorentzVector _hadronicJet2 ;
   TLorentzVector _hadronicW    ;
   TLorentzVector _hadronicBJet ;
   TLorentzVector _hadronicTop  ;
   TLorentzVector _hadronicGluon;
   TLorentzVector _hadronicTstar;
};

#endif /* end of include guard: __RECORESULTS_H__ */
