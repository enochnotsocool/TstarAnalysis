/*******************************************************************************
 *
 *  Filename    : SampleGroup.cc
 *  Description : Implementation for sample grouping class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SampleGroup.hh"

#include <iostream>
#include <exception>
using namespace std;

//------------------------------------------------------------------------------
//   Constructors
//------------------------------------------------------------------------------
SampleGroup::SampleGroup( const string& name ):
   JsonLoader( name ),
   _latexname( GetString("Latex Name") )
{
   try{
      _samplelist.push_back( SampleMgr(name) );
   }catch(exception& e){
      for( const auto& sample_name : GetStringList("Sample List") ){
         _samplelist.push_back( SampleMgr(sample_name) );
      }
   }
}

SampleGroup::~SampleGroup(){}

//------------------------------------------------------------------------------
//   Helper functions
//------------------------------------------------------------------------------
double SampleGroup::TotalLuminosity()
{
   return GetStaticDouble("Total Luminosity");
}

unsigned SampleGroup::EventsInFile() const
{
   unsigned ans = 0;
   for( const auto& sample : _samplelist ){
      ans += sample.EventsInFile();
   }
   return ans;
}

Parameter SampleGroup::ExpectedYield() const
{
   Parameter ans(0,0,0);
   for( const auto& sample : _samplelist ){
      ans += sample.ExpectedYield();
   }
   return ans;
}
