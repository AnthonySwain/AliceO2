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
void BinaryListToMapping(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, string filename/*, std::list<bool> listOfValues ,int NbinX, int NbinsY, int NbinsZ*/){
  //Reads a binary list and converts it to a voxelmap for the optimisation

  std::vector<bool> data;
  char digit;
  std::ifstream file;
  file.open(filename);
  
  // Annoyingly, there is no way to read boolean directly from .txt files, this shouldn't take much time compared to the length of the simulations though. 
  if (file.is_open()) {
    while (file >> digit) {
      if (digit == '0'){
        data.push_back(false);
      }

      if (digit == '1'){
        data.push_back(true);
      }
  }
  file.close();
  }
  for (size_t i = 0; i < data.size(); ++i) {
        VoxelMap->addPropertyForKey(i,data[i]);
        //std::cout << data[i];// << std::endl;   
    }
} 



//from a random txt files with 0s and 1s in. 
void CreateHashMapFromTxt(string HashInfoFile, string SaveMapLoc, int Nx, int Ny, int Nz){

    vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
    vecgeom::Vector3D<float> Lengths(2000,2000,6000);
    int NumbBins[3] = {Nx,Ny,Nz};
    std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]); 
    BinaryListToMapping(VoxelMap.get(),HashInfoFile);
    VoxelMap->dumpToTFile(SaveMapLoc.c_str());

}

