/*******************************************************************************
 *
 *  Filename    : JsonLoaderClass.hh
 *  Description : Class that is initialized from a json file
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __JSONLOADER_HH__
#define __JSONLOADER_HH__

#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <string>
#include <cstdint>
// Custom types
#include "TstarAnalysis/Utils/interface/Parameter.hh"

class JsonLoader{
public:
   JsonLoader( const std::string& name );
   virtual ~JsonLoader();

   // Setting up which json file to read
   static void LoadJsonFile( const std::string& );

   const std::string& Name() const { return _name; }
   void Print_Json( const boost::property_tree::ptree& tree, unsigned level = 0 ) const ;

   // Methods for accessing additional settings
   static std::string GetStaticString( const std::string& );
   static std::vector<std::string> GetStaticStringList( const std::string& );
   static double      GetStaticDouble( const std::string& );
   std::string GetString( const std::string& ) const ;
   double      GetDouble( const std::string& ) const ;
   std::vector<std::string> GetStringList( const std::string& ) const;
   std::vector<double>      GetDoubleList( const std::string& ) const;
   std::vector<uint64_t>    GetUIntList  ( const std::string& ) const;
   Parameter                GetParameter ( const std::string& ) const;

protected:
   // Accesing to original tree
   static boost::property_tree::ptree& JsonParser();
   boost::property_tree::ptree&        ClassInstance();
   const boost::property_tree::ptree&  ClassInstance() const;

   // Prebuild helper functions

private:
   const std::string _name;
   static boost::property_tree::ptree _json_parser;
};

#endif /* end of include guard: __JSONLOADER_HH__ */
