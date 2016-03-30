/*******************************************************************************
 *
 *  Filename    : Parameter.hh
 *  Description : Storage class for handling parameters
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *  Includes support for systematic and statistical uncertainties
 *
*******************************************************************************/
#ifndef __PARAMETER_HH__
#define __PARAMETER_HH__

#include <string>

class Parameter {
public:
   Parameter();
   Parameter(
      const double central ,
      const double error_up ,
      const double error_down );
   Parameter( const Parameter& );

   virtual ~Parameter ();

   // Basic access functions
   inline double CentralValue() const { return _centralValue; }
   inline double AbsUpperError() const { return _error_up; }
   inline double AbsLowerError() const { return _error_down; }
   inline double RelUpperError() const { return _error_up/_centralValue; }
   inline double RelLowerError() const { return _error_down/_centralValue; }
   inline double RelAvgError()   const { return (RelUpperError()+RelLowerError()) /2.; }

   // Arithmatics
   Parameter& operator=( const Parameter& );
   Parameter& operator+=( const Parameter& );
   Parameter& operator*=( const Parameter& );
   Parameter& operator*=( const double );
   Parameter operator+( const Parameter& ) const ;
   Parameter operator*( const Parameter& ) const ;
   Parameter operator*( const double )     const ;
   friend Parameter operator*( const double, const Parameter& );

   // Converstion operatos
   inline operator double() const{ return _centralValue; }

   // Output
   std::string LatexFormat() const;
   std::string DataCardFormat() const;

private:
   double _centralValue;
   double _error_up;
   double _error_down;

};

#endif /* end of include guard: __PARAMETER_HH__ */
