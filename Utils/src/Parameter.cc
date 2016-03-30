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
#include <stdlib.h>

using namespace std;

//------------------------------------------------------------------------------
//   Constructor and desctructor
//------------------------------------------------------------------------------
Parameter::Parameter()
{
   _centralValue = 0;
   _error_up = 0;
   _error_down = 0;
}

Parameter::Parameter(
   const double c ,
   const double error_up ,
   const double error_down ):
   _centralValue(c),
   _error_up( error_up ),
   _error_down( error_down)
{
   if( _error_up < 0 ){
      cerr << "Warning! Upper error is small than zero! Assuming flipped sign" << endl;
      _error_up = - _error_up;
   }
   if( _error_down < 0 ){
      cerr << "Warning! Lower error is small than zero! Assuming flipped sign" << endl;
      _error_down = - _error_down;
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
   _error_up      = x._error_up;
   _error_down    = x._error_down ;
   return *this;
}

Parameter& Parameter::operator+=( const Parameter& x )
{
   _centralValue += x._centralValue;
   _error_up      = sqrt(_error_up   *_error_up    + x._error_up  *x._error_up  );
   _error_down    = sqrt(_error_down *_error_down  + x._error_down*x._error_down);
   return *this;
}

Parameter& Parameter::operator*=( const Parameter& x )
{
   _error_up      = sqrt( x._centralValue *x._centralValue*_error_up   *_error_up     + _centralValue* _centralValue * x._error_up   *x._error_up  );
   _error_down    = sqrt( x._centralValue *x._centralValue*_error_down *_error_down  + _centralValue* _centralValue * x._error_down *x._error_down);
   _centralValue *= x._centralValue;
   return *this;
}

Parameter& Parameter::operator*=( const double x )
{
   _centralValue *= x;
   _error_up *= fabs(x);
   _error_down *= fabs(x);
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

//------------------------------------------------------------------------------
//   Output functions
//------------------------------------------------------------------------------

string Parameter::LatexFormat() const
{
   char buffer_1[1024];
   char buffer_2[1024];
   double cen = _centralValue;
   double up  = _error_up;
   double down= _error_down;
   int    exp = 0;

   if( fabs(cen) < 0.0001 ){
      while( fabs(cen) < 1. ){
         cen*=10;
         up*=10;
         down*=10;
         --exp;
      }
   } else if( fabs(cen) > 10000 ){
      while( fabs(cen) > 10.  ){
         cen  /=10;
         up   /=10;
         down /=10;
         ++exp;
      }
   }

   if( up == down ){
      if( up == 0. ){
         sprintf( buffer_1 , "%lg" , cen );
      } else {
         sprintf( buffer_1, "%lg\\pm%lg" , cen , up );
      }
   } else {
      sprintf( buffer_1, "%lg^{%lg}_{%lg}" , cen, up , down );
   }

   if( exp != 0 ){
      if( up == 0. ){
         sprintf( buffer_2 , "$%s\\times 10^{%d}$" , buffer_1 , exp );
      } else {
         sprintf( buffer_2 , "$(%s)\\times 10^{%d}$" , buffer_1, exp );
      }
   } else {
      sprintf( buffer_2 , "$%s$" , buffer_1 );
   }

   return buffer_2;
}

string Parameter::DataCardFormat() const
{
   char buffer[1024];
   if( _error_up == _error_down ){
      sprintf( buffer, "%lg" , 1.+RelAvgError() );
   } else {
      sprintf( buffer, "%lg/%lg" , 1.+RelUpperError() , 1.-RelLowerError() );
   }
   return buffer;
}
