/*******************************************************************************
 *
 *  Filename    : RooFitUtils.hh
 *  Description : Utility functions for better RooFit interaction
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/
#ifndef __ROOFIT_UTILS_HH__
#define __ROOFIT_UTILS_HH__

#include "TGraph.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"

template <class T>
TGraph* PlotOn( RooPlot* frame , T* rooobj,
   const RooCmdArg& arg1= RooCmdArg::none(),
   const RooCmdArg& arg2= RooCmdArg::none(),
   const RooCmdArg& arg3= RooCmdArg::none(),
   const RooCmdArg& arg4= RooCmdArg::none(),
   const RooCmdArg& arg5= RooCmdArg::none() )
{
   rooobj->plotOn( frame, arg1, arg2, arg3, arg4, arg5 );
   return (TGraph*)frame->getObject( frame->numItems() -1 );
}


extern void SetFrame(RooPlot*,unsigned); // Setting common frame options

#endif /* end of include guard: __ROOFIT_UTILS_HH__ */
