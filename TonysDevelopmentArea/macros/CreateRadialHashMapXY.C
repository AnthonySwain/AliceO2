#if defined(__linux__)
R__ADD_INCLUDE_PATH($VECGEOM_ROOT/../../Vc/latest/include)
#endif

R__ADD_INCLUDE_PATH($VECGEOM_ROOT/include)

#if defined(__linux__)
#include <VecCore/Types.h>
#include <VecCore/Common.h>
#include <VecCore/Backend/Implementation.h>
#include <VecCore/Backend/Interface.h>
#include <VecCore/Backend/VcVector.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <VecGeom/base/FlatVoxelHashMap.h>
#include <cmath>


template <typename P, bool ScalarProperties>
void CreateCircularLayersXYplane(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, int Nx, int Ny, int Nz,float minRadius, 
float Xmin, float Ymin, float Zmin, float Xmax, float Ymax, float Zmax){
/*
Creates blackholes beyond a certain radius in the xy plane. E.g. if r = 5, everything beyond x^2 + y^2 = 25, would be blackholes for the particles. 
*/

float delta_X = (Xmax -Xmin)/float(Nx);
float delta_Z = (Zmax -Zmin)/float(Nz);

float yplus;
float yminus; 

//this is for one radius, need to do it for more
for (float radius = minRadius; radius < Xmax ; radius + delta_X/2)
  for (float X = Xmin; X < Xmax; X + delta_X/2){

    //find y (remember 2 solns)
    yplus = sqrt( (radius * radius) - (X*X));
    yminus = -yplus; 

    //Assume Ymax = -Ymin
    if (yplus > Ymax){
      continue; //Outside the range of the voxel map, we don't care
    }

    for (float Z = Zmin; Z < Zmax; Z + delta_Z){ //over all Z 
      VoxelMap->addProperty(vecgeom::Vector3D<float>(X,yplus,Z));

      VoxelMap->addProperty(vecgeom::Vector3D<float>(X,yminus,Z));
    }
}
}



//create the circular layers
void CreateRadialHashMapXY(string SaveMapLoc, int Nx, int Ny, int Nz, float minRadius){

    //Hopefully pass the values here from config:) (or infer from the lenngth of the list of binary digits?)
    vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
    vecgeom::Vector3D<float> Lengths(2000,2000,6000);
    int NumbBins[3] = {Nx,Ny,Nz};
    std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]); 
    CreateCircularLayersXYplane(VoxelMap.get(), Nx, Ny, Nz, minRadius,
    MinValues[0],MinValues[1],MinValues[2], MinValues[0]+Lengths[0],MinValues[1]+Lengths[1],MinValues[2]+Lengths[2]);
    VoxelMap->dumpToTFile(SaveMapLoc.c_str());

}