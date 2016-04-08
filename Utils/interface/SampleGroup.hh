/*******************************************************************************
 *
 *  Filename    : SampleGroup.hh
 *  Description : Listing a group of samples for easier plotting commands
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __SAMPLEGROUP_H__
#define __SAMPLEGROUP_H__

#include "TstarAnalysis/Utils/interface/SampleMgr.hh"
#include "TstarAnalysis/Utils/interface/Parameter.hh"
#include "TColor.h"
#include <vector>
#include <string>

class SampleGroup : public JsonLoader {
public:
   SampleGroup( const std::string& );
   virtual ~SampleGroup ();

   const std::string& LatexName() const { return _latexname; }

   // Access single object
   SampleMgr&       Sample()       { return _samplelist.front(); }
   const SampleMgr& Sample() const { return _samplelist.front(); }

   // Access list
   std::vector<SampleMgr>&       SampleList()      { return _samplelist; }
   const std::vector<SampleMgr>& SampleList() const {return _samplelist; }

   // Summing all expected yield
   static double TotalLuminosity();
   unsigned  EventsInFile() const;
   Parameter ExpectedYield() const;

private:
   const std::string _latexname;
   std::vector<SampleMgr> _samplelist;
};

#endif /* end of include guard: __SAMPLEGROUP_H__ */
