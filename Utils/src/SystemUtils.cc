/*******************************************************************************
 *
 *  Filename    : SystemUtils.cc
 *  Description : System interaction utility functions
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/SystemUtils.hh"
#include <string>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <time.h>

using namespace std;

#define MAX_BUFFER 65536

string GetCMDOutput( const string& cmd )
{
   FILE* pipe = popen(cmd.c_str(), "r");
   if (!pipe) {return "";}
   char buffer[MAX_BUFFER];
   std::string result = "";
   while(!feof(pipe)) {
      if(fgets(buffer, MAX_BUFFER, pipe)){
         result += buffer;
      }
   }
   pclose(pipe);
   return result;
}

int HasProcess( const string& x , const string& exclude )
{
   string cmd = "ps aux | grep ^${USER}";
   cmd += " | grep --invert-match grep ";
   if( exclude != "" ) {
      cmd += "| grep --invert-match  " + exclude ;
   }
   cmd += " | grep " + x ;
   cmd += " | wc --lines";

   string ans = GetCMDOutput(cmd);
   return stoi(ans);
}

void WaitProcess( const string& x , const string& exclude)
{
   while(1){
      int proc_count = HasProcess(x , exclude );
      string time_string = CurrentDateTime();
      printf("\r[%s] Still %d instance(s) running..." ,
         time_string.c_str() ,
         proc_count );
      fflush(stdout);
      if( proc_count == 0 ){ break; }
      system("sleep 1");
   }
   printf("All Done!\n");
   fflush(stdout);
}



string CurrentDateTime()
{
   // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
   // for more information about date/time format
   time_t     now = time(0);
   struct tm  tstruct;
   char       buf[80];
   tstruct = *localtime(&now);
   strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
   return buf;
}
