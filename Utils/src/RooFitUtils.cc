/*******************************************************************************
 *
 *  Filename    : RooFitUtils.hh
 *  Description : Implementation of functions defined in RooFitUtils.hh
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
*******************************************************************************/

#include "TGraph.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"

TGraph* PlotOn( RooPlot* frame, RooAbsPdf* obj,
   const RooCmdArg& arg1,
   const RooCmdArg& arg2,
   const RooCmdArg& arg3,
   const RooCmdArg& arg4,
   const RooCmdArg& arg5 )
{
   obj->plotOn( frame, arg1, arg2, arg3, arg4, arg5 );
   return (TGraph*)frame->getObject( frame->numItems() -1 );
}

extern TGraph* PlotOn( RooPlot* frame , RooAbsData* obj,
   const RooCmdArg& arg1,
   const RooCmdArg& arg2,
   const RooCmdArg& arg3,
   const RooCmdArg& arg4,
   const RooCmdArg& arg5 )
{
   obj->plotOn( frame, arg1, arg2, arg3, arg4, arg5 );
   return (TGraph*)frame->getObject( frame->numItems() -1 );
}
