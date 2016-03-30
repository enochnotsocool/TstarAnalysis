/*******************************************************************************
 *
 *  Filename    : SampleRooFitMgr.hh
 *  Description : RooFit object manager class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __SAMPLE_ROOFIT_MGR_HH__
#define __SAMPLE_ROOFIT_MGR_HH__

#include <string>
#include <vector>
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"

class SampleRooFitMgr
{
public:
   SampleRooFitMgr( const std::string& ); // Single Sample ( for signal )
   SampleRooFitMgr( const std::string&, const std::vector<std::string>& ); // Multiple Sample (for background and data)
   ~SampleRooFitMgr();

// Static member functions
   static void LoadJsonFile( const std::string& );
   static RooRealVar& x() { return _x; }
   static RooRealVar& w() { return _w; }

   // Additional access functions
   const std::string& Name() const { return _name; }
   RooDataSet* OriginalDataSet() { return _dataset; }

   // Making/Acessing extended dataset
   RooDataSet* MakeReduceDataSet(
      const std::string&,
      const RooCmdArg&  ,
      const RooCmdArg& = RooCmdArg() );
   RooDataSet* GetReduceDataSet( const std::string& );

   // Adding/Accessing pdfs
   std::string MakePdfAlias( const std::string& x ) const;
   void AddPdf( RooAbsPdf* );
   RooAbsPdf* GetPdfFromAlias( const std::string& x );
   RooAbsPdf* GetPdfFromName( const std::string& x );

private:
   static RooRealVar _x;
   static RooRealVar _w;
   const std::string _name;
   RooDataSet* _dataset;
   std::vector<RooDataSet*> _ext_dataset;
   std::vector<RooAbsPdf*>  _pdf_list;

   void FillDataSet( const std::string& );
};

#endif /* end of include guard: __SAMPLE_ROOFIT_MGR_HH__ */
