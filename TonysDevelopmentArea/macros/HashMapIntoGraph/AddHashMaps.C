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
bool VoxelCheck(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float x,float y, float z)
{
  vecgeom::Vector3D<float> pos(x, y, z);
  //auto key = VoxelMap->getVoxelKey(pos);
  if (VoxelMap->isOccupied(pos)){
    //std::cout << "IS BLACKHOLE Particle Deleted, POSITION: " << x << ", " << y << ", " << z << "\n";
    return true;
  }
  return false;
}

template <typename P, bool ScalarProperties>
void AssignVoxelTrue(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float x, float y, float z)
{
  vecgeom::Vector3D<float> pos(x, y, z);
  //auto key = (VoxelMap)->getVoxelKey(pos);

  //If its already been set to true, don't touch:)
  if (VoxelCheck(VoxelMap, x,y,z)){ 
    return;
  }
  VoxelMap->addProperty(pos, true);
  //std::cout << "BLACKHOLE Set, POSITION: " << x << ", " << y << ", " << z << "\n";
}



std::array<float, 3> CellToPoint(vecgeom::Vector3D<float> MinValues, vecgeom::Vector3D<float> Lengths, int NumbBins[3],
int Kx, int Ky, int Kz){
    /* Takes a cell position and reutrns the position of the center of the bin */

    float delta_x = Lengths[0] / NumbBins [0];
    float delta_y = Lengths[1] / NumbBins [1];
    float delta_z = Lengths[2] / NumbBins [2];

    float x = MinValues[0] + delta_x*float(Kx) + float(Kx)*0.5;
    float y = MinValues[1] + delta_y*float(Ky) + float(Ky)*0.5;
    float z = MinValues[2] + delta_z*float(Kz) + float(Kz)*0.5;
    std::array<float,3> point = {x,y,z};
    //std::cout << point[0] << ", " <<point[1] << ", " <<point[2] << std::endl;
    return point;

}


void AddHashMaps(string HashMapFileName1, string HashMapFileName2, int Nx, int Ny, int Nz, string SaveMapLoc){
    /*
    Function that adds 2 boolean hashmaps of the same(!!) dimension.
    True + True = True
    True + False = True
    False + False = False 
    */

std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>> VoxelMap;
std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>> VoxelMap1;
std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>> VoxelMap2;

vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
vecgeom::Vector3D<float> Lengths(2000,2000,6000); //Is this the lengths of individual voxels or the entire voxelmap?
int NumbBins[3] = {Nx,Ny,Nz}; 

//The resultant map (well, it will be). 
VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]);

if (HashMapFileName1 != ""){
  if (std::filesystem::exists(HashMapFileName1)){
    std::cout<< "File Exists" << std::endl;
    VoxelMap1.reset(vecgeom::FlatVoxelHashMap<bool,true>::readFromTFile(HashMapFileName1.c_str()));
  }

  else { std::cerr << "Hashmap file 1 does not exist." << std::endl;
  std::exit(EXIT_FAILURE);
    }
  }


if (HashMapFileName2 != ""){
  if (std::filesystem::exists(HashMapFileName2)){
    std::cout<< "File Exists" << std::endl;
    VoxelMap2.reset(vecgeom::FlatVoxelHashMap<bool,true>::readFromTFile(HashMapFileName2.c_str()));
  }

  else { std::cerr << "Hashmap file 2 does not exist." << std::endl;
  std::exit(EXIT_FAILURE);
    }
  }

// Some check for the dimensions of the hashmap, for now we will assume they are the same dimensions.

//Base method should just be to iterate over all voxels 

for (int i=0; i<Nx ; i++){

    for (int j=0; j<Ny ; j++){
        
        for (int k=0; k<Nz ; k++){
            //key1 = VoxelMap1->getKeyFromCells(i,j,k);
            //key2 = VoxelMap2->getKeyFromCells(i,j,k);

            //std::cout << key1 << " <- key1 | key2 -> " << key2 << std::endl;//these should be the same

            std::array<float,3> pos  = CellToPoint(MinValues,Lengths,NumbBins,i,j,k);
            vecgeom::Vector3D<float> point(pos[0], pos[1], pos[2]);

            if (VoxelMap1->isOccupied(point) or VoxelMap2->isOccupied(point)){
                AssignVoxelTrue(VoxelMap.get(),pos[0],pos[1],pos[2]);
                
            }
            //std::cout << k << std::endl;
        }
        //std::cout << j << std::endl;
    }
    //std::cout << i << std::endl;
}

VoxelMap->dumpToTFile(SaveMapLoc.c_str());





}