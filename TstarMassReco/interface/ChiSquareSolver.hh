/*******************************************************************************
 *
 *  Filename    : ChiSquareSolver.h
 *  Description : Permutate jets for best Chi square results
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Note that the class is not generalized, but specializes for a two bjet
 *  output with at least one bjet and at most 2 bjets, with a total of
 *  6 outputs
 *
*******************************************************************************/

#ifndef __CHISQUARESOLVER_H__
#define __CHISQUARESOLVER_H__

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "TstarAnalysis/TstarMassReco/interface/RecoResult.hh"
#include "TstarAnalysis/TstarMassReco/interface/RecoUtils.hh"

#include "TLorentzVector.h"
#include <vector>

//------------------------------------------------------------------------------
//   Main Class prototype
//------------------------------------------------------------------------------
class ChiSquareSolver{
public:
   ChiSquareSolver(const edm::ParameterSet&);
   virtual ~ChiSquareSolver ();

   void SetMET( const pat::MET* );
   void SetLepton( const reco::RecoCandidate* );
   void AddJet   ( const pat::Jet* );
   void RunPermutations();
   void ClearAll();

   const std::vector<RecoResult>& ResultList() const { return _resultsList; }
   const RecoResult& BestResult() const ;
   void  AddResult(const double, const double, const unsigned);

private:
   // Original CMSSSW Objects
   const pat::MET*  _met;
   const reco::RecoCandidate*    _lepton;
   std::vector<const pat::Jet*>  _jetList;

   // Extended objects
   std::vector<RecoResult>       _resultsList;
   TLorentzVector  _neutrino[2]; // Two possible solution for neutrino

   // Operatation flags
   const unsigned  _debug;
   const unsigned  _max_jets;
   const unsigned  _req_b_jets;

   // Helper functions
   void solveNeutrino();
   bool CheckPermutation() const ;
   TLorentzVector had_b()  const { return ConvertToRoot( *_jetList[0]); }
   TLorentzVector lep_b()  const { return ConvertToRoot( *_jetList[1]); }
   TLorentzVector had_g()  const { return ConvertToRoot( *_jetList[2]); }
   TLorentzVector had_q1() const { return ConvertToRoot( *_jetList[3]); }
   TLorentzVector had_q2() const { return ConvertToRoot( *_jetList[4]); }
   TLorentzVector lep_g()  const { return ConvertToRoot( *_jetList[5]); }
};

#endif // __CHISQUARESOLVER_H__
