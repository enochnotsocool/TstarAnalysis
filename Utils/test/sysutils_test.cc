#include "TstarAnalysis/Utils/interface/SystemUtils.hh"
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
   string result = GetCMDOutput( "ls" );
   cout << result << endl;
   WaitProcess("zsh");
   return 0;
}
