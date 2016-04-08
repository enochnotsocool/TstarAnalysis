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
extern std::string  GetChannel();
extern std::string  GetChannelPlotLabel();
extern std::string  GetJsonFile();
extern std::string  GetTexSummaryFile();
extern std::string  GetKinematicPlotFile( const std::string& );
extern std::string  GetRooFitPlot_Template_File();
extern std::string  GetRooFitObj_Template_File();
extern std::string  GetTemplate_CardFile( const std::string& );
extern std::string  GetTemplate_PlotFile();
extern std::string  GetTemplate_HiggCombineStoreFile( const std::string& , int );
extern std::string  GetHiggCombineOutputFile( const std::string& , int );

#endif /* end of include guard: __FILENAME_HH__ */
