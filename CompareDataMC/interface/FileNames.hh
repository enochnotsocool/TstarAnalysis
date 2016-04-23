/*******************************************************************************
 *
 *  Filename    : FileNames.hh
 *  Description : Setting Save file names for options (see src/FileNames.cc)
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __FILENAME_HH__
#define __FILENAME_HH__

#include <string>

extern void SetChannelType( const std::string& );
extern std::string  ResultsDir();
extern std::string  GetChannel();
extern std::string  GetChannelPlotLabel();
extern std::string  GetJsonFile();
extern std::string  GetTexSummaryFile();
extern std::string  GetKinematicPlotFile( const std::string& );

// Higgs combine related files
extern void SetMethod( const std::string& );
extern std::string GetMethod();
extern std::string GetMethodLabel();
extern std::string GetCardFile( const std::string& );
extern std::string GetRooObjFile();
extern std::string GetRooObjPlot( const std::string& );

extern void SetHCMethod( const std::string& );
extern std::string GetHCMethod();
extern std::string HCRawOutputFile( int );
extern std::string HCStoreFile( int );
extern std::string LimitPlotFile();

// Misc. Functions
extern int GetSignalMass( const std::string& );


#endif /* end of include guard: __FILENAME_HH__ */
