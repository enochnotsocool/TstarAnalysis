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
#include "TAxis.h"

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

TGraph* PlotOn( RooPlot* frame , RooAbsData* obj,
   const RooCmdArg& arg1,
   const RooCmdArg& arg2,
   const RooCmdArg& arg3,
   const RooCmdArg& arg4,
   const RooCmdArg& arg5 )
{
   obj->plotOn( frame, arg1, arg2, arg3, arg4, arg5 );
   return (TGraph*)frame->getObject( frame->numItems() -1 );
}

void SetFrame( RooPlot* frame, unsigned font_size )
{
   frame->GetXaxis()->SetLabelFont(43);
   frame->GetXaxis()->SetLabelSize( font_size );
   frame->GetXaxis()->SetTitleFont(43);
   frame->GetXaxis()->SetTitleSize( font_size );
   frame->GetXaxis()->SetTitleOffset( 1.2 );
   frame->GetYaxis()->SetLabelFont(43);
   frame->GetYaxis()->SetLabelSize( font_size);
   frame->GetYaxis()->SetTitleFont(43);
   frame->GetYaxis()->SetTitleSize( font_size );
   frame->GetYaxis()->SetTitleOffset( 1.2 );
   frame->SetTitle("");
}
