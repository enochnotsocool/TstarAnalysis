/*******************************************************************************
 *
 *  Filename    : CompareHistMgr.hh
 *  Description : Comparison Histograms Mangaer
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __COMPAREHISTMGR_HH__
#define __COMPAREHISTMGR_HH__

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "TstarAnalysis/TstarMassReco/interface/RecoResult.hh"

#include <vector>
#include <string>

#include "TH1D.h"

class CompareHistMgr {
public:
   CompareHistMgr(const std::string&,const std::string&,const std::string&,const std::string&);
   virtual ~CompareHistMgr();

   // Explicitly removing copy constructors
   CompareHistMgr( const CompareHistMgr& ) = delete;
   CompareHistMgr& operator=( const CompareHistMgr& ) = delete ;

   inline const std::string& Name() const { return _name; }

   void AddEvent( const fwlite::Event& );

   // Access functions
   TH1D* Hist( const std::string& );
   std::vector<std::string> AvailableHistList() const ;
   void SetColor( const Color_t );

private:
   const std::string _name;
   const std::string _module_label;
   const std::string _product_label;
   const std::string _process_label;
   fwlite::Handle<RecoResult> _result_handle;

   std::vector<TH1D*>  _hist_list;

   // Helper functions
   void AddHist(const std::string&,const std::string&, const std::string&, const int, const double, const double);
};

#endif /* end of include guard: __COMPAREHISTMGR_HH__ */
