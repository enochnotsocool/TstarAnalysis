/*******************************************************************************
 *
 *  Filename    : Parameter.cc
 *  Description : Implmentation of arithmetics for Parameter class
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#include "TstarAnalysis/Utils/interface/Parameter.hh"
#include <math.h>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
Parameter::Parameter(){
   _centralValue = 0;
   _stat_up = 0;
   _stat_down = 0;
   _sys_up = 0 ;
   _sys_down = 0 ;
}

Parameter::Parameter(
   const double c ,
   const double stat_up ,
   const double stat_down ,
   const double sys_up,
   const double sys_down):
   _centralValue(c),
   _stat_up( stat_up ),
   _stat_down( stat_down),
   _sys_up( sys_up ),
   _sys_down( sys_down )
{
   if( _stat_up < 0 ){
      cerr << "Warning! statistical upper error is small than zero! Assuming flipped sign" << endl;
      _stat_up = - _stat_up;
   }
   if( _stat_down < 0 ){
      cerr << "Warning! statistical lower error is small than zero! Assuming flipped sign" << endl;
      _stat_down = - _stat_down;
   }
   if( _sys_up < 0 ){
      cerr << "Warning! systematic upper error is small than zero! Assuming flipped sign" << endl;
      _sys_up = - _sys_up ;
   }
   if( _sys_down < 0 ){
      cerr << "Warning! systematic lower error is small than zero! Assuming flipped sign" << endl;
      _sys_down = - _sys_down;
   }
}

Parameter::Parameter( const Parameter& x )
{
   *this = x;
}

Parameter::~Parameter(){}

//------------------------------------------------------------------------------
//  Arithmatics:
//    * All assuming 0 correlation
//    * Using simple but wrong algorithm for asymmetric errors
//  TODO: Update to http://www.phas.ubc.ca/~oser/p509/Lec_10.pdf
//------------------------------------------------------------------------------
Parameter& Parameter::operator=( const Parameter& x )
{
   _centralValue = x._centralValue;
   _stat_up      = x._stat_up;
   _stat_down    = x._stat_down ;
   _sys_up       = x._sys_up;
   _sys_down     = x._sys_down;
   return *this;
}

Parameter& Parameter::operator+=( const Parameter& x )
{
   _centralValue += x._centralValue;
   _stat_up      = sqrt(_stat_up   *_stat_up    + x._stat_up  *x._stat_up  );
   _stat_down    = sqrt(_stat_down *_stat_down  + x._stat_down*x._stat_down);
   _sys_up       = sqrt(_sys_up    *_sys_up     + x._sys_up   *x._sys_up   );
   _sys_down     = sqrt(_sys_down  *_sys_down   + x._sys_down *x._sys_down );
   return *this;
}

Parameter& Parameter::operator*=( const Parameter& x )
{
   _stat_up      = sqrt( x._centralValue * _stat_up *_stat_up     + _centralValue * x._stat_up   *x._stat_up  );
   _stat_down    = sqrt( x._centralValue *_stat_down *_stat_down  + _centralValue * x._stat_down *x._stat_down);
   _sys_up       = sqrt( x._centralValue *_sys_up    *_sys_up     + _centralValue * x._sys_up    *x._sys_up   );
   _sys_down     = sqrt( x._centralValue *_sys_down  *_sys_down   + _centralValue * x._sys_down  *x._sys_down );
   _centralValue *= x._centralValue;
   return *this;
}

Parameter& Parameter::operator*=( const double x )
{
   _centralValue *= x;
   _stat_up *= fabs(x);
   _stat_down *= fabs(x);
   _sys_up *= fabs(x);
   _sys_down *= fabs(x);
   return *this;
}

// Extended Arithmatic functions
Parameter Parameter::operator+( const Parameter& x ) const
{
   Parameter ans( *this );
   ans+=x;
   return ans;
}

Parameter Parameter::operator*( const Parameter& x ) const
{
   Parameter ans( *this );
   ans *= x;
   return ans;
}

Parameter Parameter::operator*( const double x ) const
{
   Parameter ans( *this );
   ans *= x;
   return ans;
}

Parameter operator*( const double y , const Parameter& x )
{
   return x * y ;
}
