<!-- doxy
\page refSteer Module 'Steer'
/doxy -->

# ALICEO2 Steer

This directory contains ALICE O2 specific customizations
of steering classes from FairRoot

The purpose is either
a) quick prototyping of ideas without having to require immediate changes in FairRoot
   but with the idea to give back the changes some day
b) adaptations to classes because some things are done differently than in FairRoot
   (MC truth handling, parameter handling etc)

<!-- doxy
This module contains the following submodules:

* \subpage refSteerDigitizerWorkflow
/doxy -->

# Additions

# Particle Specific Maps

## Voxel Maps
Voxel maps used are created using VecGeom - https://gitlab.cern.ch/VecGeom/VecGeom/-/blob/89a05d148cc708d4efc2e7b0eb6e2118d2610057/VecGeom/base/FlatVoxelHashMap.h#L207

To pass voxel-maps to the simulation, create a .txt file that looks like: 
[All]|example1.root
[All_but, 13]|example2.root
[10,-10]|example3.root
[2,-2]|example4.root
[All_but, 22]|example5.root
[All]|example6.root

where exampleX.root is the filepath to the map (in this case, the same directory where the simlulations are run)
All - stop transport of all particles in areas of the map set to True
All_but - stop transport of all but specified particles in areas of the map set to true
individual pdgs - stop the transport of these specific particles in the areas of the map set to true

Include the addition to the simulation command to use the maps (if the maps are in a .txt file called maps.txt)
"--configKeyValues="GlobalSimProcs.blackholeVoxelFile=maps.txt"

## Cylindrical (Non-voxel) Maps
Very similar to voxel maps but is only for cylidners and the geometry is calculated in the stepping
function of (i.e. is the particle within the radii of the cylidner which currently defines its Z value, if not, stop the transport of the particle)

Include the addition to the simulation command to use the maps (if the .csv file is called cylinders.csv)
"--configKeyValues="GlobalSimProcs.GeoCutsCSVFile=cylinders.csv"


# Save-Steps
This is relativitly light-weight compared to VMCStepLogger but has significantly reduced features. Only saves X,Y,Z,pdg for each step of a particles transport. Can be analysed using the macros found in: https://github.com/AnthonySwain/AliceO2MacroDev/tree/main/StepandHit_Analysis 

To enable this, include "--configKeyValues="GlobalSimProcs.SaveSteps=true" in the command.
