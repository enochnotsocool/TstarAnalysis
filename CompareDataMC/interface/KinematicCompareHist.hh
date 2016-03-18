/*******************************************************************************
 *
 *  Filename    : KinematicCompareHist.hh
 *  Description : Simple Hist manager for kinematic comparison
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __KINEMATIC_COMPARE_HIST_HH__
#define __KINEMATIC_COMPARE_HIST_HH__

#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "TH1D.h"
#include <vector>
#include <string>

class KinematicCompareHist {
public:
   KinematicCompareHist( SampleMgr& , const double total_lumi );
   virtual ~KinematicCompareHist ();
   TH1D* GetHist( const std::string& );

   void SetColor( const Color_t& );
   SampleMgr* const Sample(){ return _sample_ref; }
   std::vector<std::string> AvailableHistList() const;

private:
   // storage class
   const std::string  _name; // For unique ID
   std::vector<TH1D*> _histlist;
   SampleMgr* const        _sample_ref;

   // Private function
   void AddHist( const std::string& , const std::string&, const std::string& , const std::string&, int, double, double );
   void FillHistograms( SampleMgr&, const double total_lumi ) ;

};

#endif /* end of include guard: __KINEMATIC_COMPARE_HIST_HH__ */
