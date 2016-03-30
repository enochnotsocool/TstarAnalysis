/*******************************************************************************
 *
 *  Filename    : SampleMgr.cc
 *  Description : Implementation for sample Manager
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "DataFormats/FWLite/interface/Run.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "TFile.h"
#include <iostream>
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
SampleMgr::SampleMgr( const std::string& name ):
   JsonLoader( name ),
   _event         ( GetStringList("EDM Files" )),
   _latexname     ( GetString("Latex Name") ),
   _cross_section ( GetParameter( "Cross Section" )),
   _k_factor      ( GetParameter( "K Factor")),
   _selection_eff ( MakeSelectionEfficiency() )
{
}

SampleMgr::~SampleMgr()
{}

//------------------------------------------------------------------------------
//   Get Static settings
//------------------------------------------------------------------------------
double SampleMgr::TotalLuminosity()
{
   return GetStaticDouble("Total Luminosity");
}

//------------------------------------------------------------------------------
//   Computation results
//------------------------------------------------------------------------------
Parameter SampleMgr::ExpectedYield( const double totalLumi )const
{
   return totalLumi * CrossSection()* SelectionEfficiency() * KFactor();
}

Parameter SampleMgr::GetSampleWeight( const double totalLumi )
{
   return ExpectedYield(totalLumi) * (1/(double)CountSelectedEvent()) ;
}

Parameter SampleMgr::MakeSelectionEfficiency() const
{
   double before = CountOriginalEvent();
   double after  = CountSelectedEvent();
   double eff = after/before;
   double err = sqrt(eff*(1-eff)/before);
   return Parameter( eff, err, err );
}

uint64_t SampleMgr::CountOriginalEvent() const
{
   return CountEvent("beforeAny");
}

uint64_t SampleMgr::CountSelectedEvent() const
{
   return CountEvent( GetStaticString("Selection Cut Label") );
}

uint64_t SampleMgr::CountEvent( const string& name ) const
{
   fwlite::Handle<edm::MergeableCounter> positive_count;
   fwlite::Handle<edm::MergeableCounter> negative_count;
   uint64_t count = 0 ;

   for( const auto& file: GetStringList("EDM Files") ){
      fwlite::Run run( TFile::Open(file.c_str()) );
      positive_count.getByLabel( run , name.c_str() , "positiveEvents" );
      negative_count.getByLabel( run , name.c_str() , "negativeEvents" );
      count += positive_count->value;
      count -= negative_count->value;
   }
   return count;
}
