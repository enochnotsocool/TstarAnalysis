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
#include "TstarAnalysis/TstarMassReco/interface/ChiSquareResult.h"
#include "TLorentzVector.h"
#include <vector>

class ChiSquareSolver{
public:
   ChiSquareSolver(const edm::ParameterSet&);
   virtual ~ChiSquareSolver ();

   friend class ChiSquareResult;

   void SetMET( const double, const double );
   void SetLepton  ( const double, const double, const double, const double );
   void AddBTagJet ( const double, const double, const double, const double );
   void AddLightJet( const double, const double, const double, const double );
   void RunPermutations();
   void ClearAll();

   const std::vector<ChiSquareResult>& ResultList() const { return _resultsList; }
   const ChiSquareResult& BestResult() const ;

private:
   std::vector<ChiSquareResult>  _resultsList;
   std::vector<TLorentzVector>   _jetVecList;
   TLorentzVector  _lepton;
   TLorentzVector  _neutrino[2]; // Two possible solution for neutrino
   std::vector<TLorentzVector>  _bjetList;
   std::vector<TLorentzVector>  _ljetList;
   double          _met;
   double          _metphi;
   const unsigned  _debug;

   void solveNeutrino();
   void makeJetVecList();
   const TLorentzVector& had_b()  const { return _jetVecList[0]; }
   const TLorentzVector& lep_b()  const { return _jetVecList[1]; }
   const TLorentzVector& had_g()  const { return _jetVecList[2]; }
   const TLorentzVector& had_q1() const { return _jetVecList[3]; }
   const TLorentzVector& had_q2() const { return _jetVecList[4]; }
   const TLorentzVector& lep_g()  const { return _jetVecList[5]; }
};

template<class T>
class ComparePT{
public:
   ComparePT(){}
   ~ComparePT(){}
   bool operator() ( const T& i , const T& j ) const{
      return i.Pt() > j.Pt();
   }
};

#endif // __CHISQUARESOLVER_H__
