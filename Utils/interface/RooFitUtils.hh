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

extern TGraph* PlotOn( RooPlot*, RooAbsPdf*,
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none() );

extern TGraph* PlotOn( RooPlot*, RooAbsData*,
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none(),
   const RooCmdArg& = RooCmdArg::none() );

extern void SetFrame(RooPlot*,unsigned); // Setting common frame options

#endif /* end of include guard: __ROOFIT_UTILS_HH__ */
