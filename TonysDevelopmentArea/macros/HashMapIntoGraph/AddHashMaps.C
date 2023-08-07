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

//The resultant map. 
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

long key1;
long key2;

for (int i=0; i<Nx ; i++){

    for (int j=0; i<Ny ; j++){
        
        for (int k=0; i<Nz ; k++){
            key1 = VoxelMap1->getKeyFromCells(i,j,k);
            key2 = VoxelMap2->getKeyFromCells(i,j,k);

            std::cout << key1 << " <- key1 | key2 -> " << key2 << std::endl;//these should be the same

            if (VoxelMap1->isOccupied(key1) or VoxelMap2->isOccupied(key2)){
                VoxelMap->addPropertyForKey(key1,true);
            }

            else{
                // you don't actually need to do anything here, it will be false by default. 
            }

        }
    }
}

VoxelMap->dumpToTFile(SaveMapLoc.c_str());





}