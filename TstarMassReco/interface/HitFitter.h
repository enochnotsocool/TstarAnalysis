/*******************************************************************************
 *
 *  Filename    : HitFitter.cc
 *  Description : Class for communicating with HitFit package
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __HITFITTER_CC__
#define __HITFITTER_CC__

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "bpkFrameWork/ModifiedHitFit/interface/Lepjets_Event.h"
#include "bpkFrameWork/ModifiedHitFit/interface/Top_Fit.h"

#include "TopQuarkAnalysis/TopHitFit/interface/EtaDepResolution.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Top_Decaykin.h"
#include "TopQuarkAnalysis/TopHitFit/interface/Fit_Result.h"

#include "TLorentzVector.h"
#include <vector>

#define ISMUON true
#define ISELECTRON false

class HitFitter
{
public:
   HitFitter( const edm::ParameterSet& );
   virtual ~HitFitter ();

   void SetMET( const double, const double );
   void SetLepton( const double, const double, const double, const double, const bool isMuon);
   void AddBTagJet( const double, const double, const double, const double);
   void AddLightJet( const double, const double, const double, const double);

   hitfit::Fit_Result*  GetBest() { return _best_result; }
   void  RunPermutations();
   void  ClearAll();

private:
   double _met;
   double _metPhi;
   TLorentzVector  _lepton;
   bool   _leptontype;
   std::vector<TLorentzVector>  _btagJetList;
   std::vector<TLorentzVector>  _lightJetList;

   hitfit::Fit_Result*  _best_result;

   hitfit::Top_Fit*                _top_fitter;
   hitfit::Resolution*             _met_KtResolution;    // Constant for the time being
   hitfit::EtaDepResolution*       _electronResolution;  // Read from external file
   hitfit::EtaDepResolution*       _muonResolution;      // Read from external file
   hitfit::EtaDepResolution*       _lightJetResolution;  // Read from external file
   hitfit::EtaDepResolution*       _bJetResolution;      // Read from external file

   unsigned _debug;

   //----- Translator functions  --------------------------------------------------
   void AddHitFitMET( hitfit::Lepjets_Event&, const double, const double ) const;
   void AddHitFitLepton( hitfit::Lepjets_Event&, const TLorentzVector&, const bool) const;
   void AddHitFitJet( hitfit::Lepjets_Event&, const TLorentzVector& , const int) const;

   //----- B Tag checking functions  ----------------------------------------------
   bool CheckBTagOrder( const std::vector<int>& ) const;
};

#endif // __HITFITTER_CC__
