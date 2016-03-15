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
   SampleMgr( const std::string& );
   virtual ~SampleMgr ();

   // Access members
   const std::string& Name() const {return _name; }
   const std::string& LatexName() const { return _latexname; }
   const Parameter&   CrossSection() const { return _crossSection; }
   const Parameter&   SelectionEfficiency() const { return _selectionEff; }
   const Parameter&   Weight() const { return _weight; }
   fwlite::ChainEvent& Event() { return _event; }

   // Extended functions
   size_t    EventsInFile() { return _event.size(); }
   Parameter ExpectedYield( const double totalLumi ) const;


private:
   const std::string _name;
   const std::string _latexname;
   const Parameter   _crossSection;
   const Parameter   _selectionEff;
   const Parameter   _weight;
   fwlite::ChainEvent  _event;

};


//------------------------------------------------------------------------------
//   Utility Macros
//------------------------------------------------------------------------------
#define LOOP_EVENT( EVENT )\
   for( (EVENT.toBegin()) ; !(EVENT.atEnd()) ; ++(EVENT) )


#endif /* end of include guard: __SAMPLE_MGR_HH__ */
