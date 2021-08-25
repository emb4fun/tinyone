/****************************************************************************
*  Target Script for the AM335x CPU.
****************************************************************************/

function Reset()
{
   TargetInterface.resetAndStop();

   // b2init, see https://www.emb4fun.de/arm/jldbg/index.html
   TargetInterface.pokeBinary(0x402f0400, "$(ProjectDir)/b2init.bin");
   TargetInterface.runFromAddress(0x402f0400, 2000);    
   
   TargetInterface.setMaximumJTAGFrequency(25000000);
}

function EnableTrace(traceInterfaceType)
{
  // TODO: Enable trace
}

/*** EOF ***/
