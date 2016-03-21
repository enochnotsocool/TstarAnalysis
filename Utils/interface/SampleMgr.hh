/*******************************************************************************
 *
 *  Filename    : SampleMgr
 *  Description : Manager class for a Single Prcoess objects
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Note:
 *       * CrossSection units in pb
 *       * Lumimosity units in pb-1
 *
*******************************************************************************/
#ifndef __SAMPLE_MGR_HH__
#define __SAMPLE_MGR_HH__

//------------------------------------------------------------------------------
//   Libraries
//------------------------------------------------------------------------------
//----- Custom storage classes -----
#include "TstarAnalysis/Utils/interface/Parameter.hh"
#include "TstarAnalysis/Utils/interface/JsonLoaderClass.hh"

//----- CMSSW Classes for looping events -----
#include "DataFormats/FWLite/interface/ChainEvent.h"

//----- System Libraries for loading config files -----
#include <boost/property_tree/ptree.hpp> // Helper object for initializing via json file
#include <string>
#include <vector>


//------------------------------------------------------------------------------
//   Class definition
//------------------------------------------------------------------------------
class SampleMgr : public JsonLoader {
public:
   SampleMgr( const std::string& , const std::string& );
   virtual ~SampleMgr ();

   // Access members
   const std::string&  LatexName() const { return _latexname; }
   const Parameter&    CrossSection()        const { return _cross_section; }
   const Parameter&    KFactor()             const { return _k_factor; }
   const Parameter&    SelectionEfficiency() const { return _selection_eff;}
   fwlite::ChainEvent& Event() { return _event; }

   // Extended functions
   size_t    EventsInFile() { return _event.size(); }
   size_t    WeightedEventsInFile() const { return CountSelectedEvent(); }
   Parameter ExpectedYield( const double totalLumi ) const;
   Parameter GetSampleWeight( const double ) ;

private:
   fwlite::ChainEvent  _event;
   const std::string   _latexname;
   const Parameter     _cross_section;
   const Parameter     _k_factor;
   const Parameter     _selection_eff; // Caching selection results

   Parameter MakeSelectionEfficiency() const ;
   uint64_t  CountOriginalEvent() const;
   uint64_t  CountSelectedEvent() const;
   uint64_t  CountEvent( const std::string& ) const ;
};


//------------------------------------------------------------------------------
//   Utility Macros
//------------------------------------------------------------------------------
#define LOOP_EVENT( EVENT )\
   for( (EVENT.toBegin()) ; !(EVENT.atEnd()) ; ++(EVENT) )


#endif /* end of include guard: __SAMPLE_MGR_HH__ */
