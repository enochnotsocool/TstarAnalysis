/*******************************************************************************
 *
 *  Filename    : SystemUtils.hh
 *  Description : System interaction utiliy functions
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __SYSTEM_UTILS_HH__
#define __SYSTEM_UTILS_HH__

#include <string>
extern std::string GetCMDOutput( const std::string& x );
extern int  HasProcess( const std::string& x );
extern void WaitProcess( const std::string& x );

// Getting current time functions
extern std::string CurrentDateTime();

#endif /* end of include guard: __SYSTEM_UTILS_HH__ */
