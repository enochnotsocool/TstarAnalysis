/*******************************************************************************
 *
 *  Filename    : SampleHistMgr.hh
 *  Description : Augmented SampleMgr with histograms stored
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __SAMPLEHISTMGR_HH__
#define __SAMPLEHISTMGR_HH__

#include "TstarAnalysis/Utils/interface/SampleGroup.hh"
#include <vector>
#include <string>
#include "TH1D.h"

class SampleHistMgr : public SampleGroup {
public:
   SampleHistMgr( const std::string& );
   ~SampleHistMgr();

   // Root object handling, removing explicit copying.
   SampleHistMgr( const SampleHistMgr&) = delete ;
   SampleHistMgr& operator=(const SampleHistMgr&)=delete;

   void SetColor( const Color_t& );
   void Scale( const double );
   TH1D* GetHist( const std::string& );
   std::vector<std::string> AvailableHistList() const;

private:
   std::vector<TH1D*> _histlist;

   void AddHist( const std::string&, const std::string& , const std::string&, int, double, double );
   void FillHistograms(SampleMgr&) ;

};

#endif /* end of include guard: __SAMPLEHISTMGR_HH__ */
