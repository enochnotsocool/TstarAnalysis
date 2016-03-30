/*******************************************************************************
 *
 *  Filename    : SampleHistMgr.hh
 *  Description : Augmented SampleMgr with histograms stored
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __SAMPLEHISTMGR_HH__
#define __SAMPLEHISTMGR_HH__

#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include <vector>
#include <string>
#include "TH1D.h"

class SampleHistMgr : public SampleMgr {
public:
   SampleHistMgr( const std::string&, const double );
   ~SampleHistMgr();

   void SetColor( const Color_t& );
   TH1D* GetHist( const std::string& );
   std::vector<std::string> AvailableHistList() const;

private:
   std::vector<TH1D*> _histlist;

   void AddHist( const std::string&, const std::string& , const std::string&, int, double, double );
   void FillHistograms( const double ) ;

};

#endif /* end of include guard: __SAMPLEHISTMGR_HH__ */
