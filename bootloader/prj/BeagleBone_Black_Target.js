/******************************************************************************
  Target Script for BeagleBone Black

  Copyright (c) 2013 Rowley Associates Limited.

  This file may be distributed under the terms of the License Agreement
  provided with this software.

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ******************************************************************************/

load("targets/Sitara/AM335x_Target.js")

function SDRAMReset()
{
  Reset();
  if (TargetInterface.implementation() == "crossworks_simulator")
    return;
  TargetInterface.pokeBinary(0x40300000, "$(TargetsDir)/BeagleBone_Black/BeagleBone_Black_init.bin");
  TargetInterface.runFromAddress(0x40300000, 2000);  
}
