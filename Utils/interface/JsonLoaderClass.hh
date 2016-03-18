/*******************************************************************************
 *
 *  Filename    : JsonLoaderClass.hh
 *  Description : Class that is initialized from a json file
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __JSONLOADERCLASS_HH__
#define __JSONLOADERCLASS_HH__

#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <string>
#include <cstdint>
// Custom types
#include "TstarAnalysis/Utils/interface/Parameter.hh"

class JsonLoader{
public:
   JsonLoader( const std::string& jsonfile, const std::string& class_label );
   virtual ~JsonLoader();

   void Print_Json( const boost::property_tree::ptree& tree, unsigned level = 0 ) const ;

protected:
   inline boost::property_tree::ptree& JsonParser() { return _json_parser; }
   inline const boost::property_tree::ptree& JsonParser() const { return _json_parser; }
   std::vector<std::string> GetStringList( const std::string& ) const;
   std::vector<double>      GetDoubleList( const std::string& ) const;
   std::vector<uint64_t>    GetUIntList  ( const std::string& ) const;
   Parameter                GetParameter ( const std::string& ) const;

private:
   boost::property_tree::ptree _json_parser;
};

#endif /* end of include guard: __JSONLOADERCLASS_HH__ */
