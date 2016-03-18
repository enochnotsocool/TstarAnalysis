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

class Parameter {
public:
   Parameter();
   Parameter(
      const double central ,
      const double stat_up ,
      const double stat_down,
      const double sys_up = 0 ,  // Assuming no systematic error by default
      const double sys_down = 0);
   Parameter( const Parameter& );

   virtual ~Parameter ();

   // Basic access functions
   inline double CentralValue() const { return _centralValue; }
   inline double AbsUpperError() const { return _stat_up+_sys_up; }
   inline double AbsLowerError() const { return _sys_down+_sys_down; }
   inline double RelUpperError() const { return (_stat_up+_sys_up)/_centralValue; }
   inline double RelLowerError() const { return (_stat_down+_sys_down)/_centralValue; }
   inline double RelAvgError()   const { return (RelUpperError()+RelLowerError()) /2.; }
   inline double AbsStatUpperError() const { return _stat_up; }
   inline double AbsStatLowerError() const { return _stat_down; }
   inline double AbsSysUpperError()  const { return _sys_up ; }
   inline double AbsSysLowerError()  const { return _sys_down; }

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

private:
   double _centralValue;
   double _stat_up;
   double _stat_down;
   double _sys_up;
   double _sys_down;

};

#endif /* end of include guard: __PARAMETER_HH__ */
