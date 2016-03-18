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
#include "TColor.h"
#include <vector>
#include <string>

class SampleGroup : public JsonLoader {
public:
   SampleGroup(const std::string& file , const std::string& label );
   virtual ~SampleGroup ();

   const std::string& Name() const { return _name; }
   const std::string& LatexName() const { return _latexname; }
   std::vector<SampleMgr>& SampleList(){ return _samplelist; }
private:
   const std::string _name;
   const std::string _latexname;
   std::vector<SampleMgr> _samplelist;
};

#endif /* end of include guard: __SAMPLEGROUP_H__ */
