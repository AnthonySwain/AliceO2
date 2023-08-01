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

#include <VecGeom/base/FlatVoxelHashMap.h>

template <typename P, bool ScalarProperties>
vecgeom::Vector3D<float> FindVoxelCenter(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float x,float y, float z)
{
  vecgeom::Vector3D<float> pos(x, y, z);
  auto key = VoxelMap->getVoxelKey(pos);
  return VoxelMap->keyToPos(key);
}

template <typename P, bool ScalarProperties>
bool VoxelCheck(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float x,float y, float z)
{
  vecgeom::Vector3D<float> pos(x, y, z);
  //auto key = VoxelMap->getVoxelKey(pos);
  if (VoxelMap->isOccupied(pos)){
    std::cout << "IS BLACKHOLE Particle Deleted, POSITION: " << x << ", " << y << ", " << z << "\n";
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
  if (VoxelCheck(VoxelMap, x,y,z)) {
    return;
  }
  VoxelMap->addProperty(pos, true);
  std::cout << "BLACKHOLE Set, POSITION: " << x << ", " << y << ", " << z << "\n";
}

float UniformRandom(float upper_bound, float lower_bound){
  std::random_device rd;

  // Use the Mersenne Twister algorithm for random number generation
  std::mt19937 mt(rd());

  // Create a uniform distribution for integers within the range
  std::uniform_real_distribution<float> dist(lower_bound, upper_bound);

  float random_number = dist(mt);

  return(random_number);
}

template <typename P, bool ScalarProperties>
void RandomAllocation(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, int N, float Min, float Max){
  //Assign N random voxels near the center as true (i.e blachholes )
  for (int i =0; i < N; i++){
  float x = UniformRandom(Min,Max);
  float y = UniformRandom(Min,Max);
  float z = UniformRandom(Min,Max);

  AssignVoxelTrue(VoxelMap, x, y, z);
  }
}

template <typename P, bool ScalarProperties>
void BuildWallZYplane(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float Xval, int thickness, int Zmin, int Zmax, int Ymin, int Ymax, int ZBins, int YBins){
  float deltaY = (Ymax - Ymin)/YBins;
  float deltaZ = (Zmax - Zmin)/ZBins;

  for (float k = 0; k < thickness ;k++){
  for (float j = (float(Ymin) + deltaY/2); j < float(Ymax); j += deltaY ){
    for (float i = (float(Zmin) + deltaZ/2); i < float(Zmax); i += deltaZ ){
      AssignVoxelTrue(VoxelMap, Xval+k, j, i);
  }}
  }
}

template <typename P, bool ScalarProperties>
void BuildWallXYplane(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float Zval, int thickness, int Xmin, int Xmax, int Ymin, int Ymax, int XBins, int YBins){
  float deltaY = (Ymax - Ymin)/YBins;
  float deltaX = (Xmax - Xmin)/XBins;

  for (float k = 0; k<thickness; k++){
  for (float j = (float(Ymin) + deltaY/2); j < float(Ymax); j += deltaY ){
    for (float i = (float(Xmin) + deltaX/2); i < float(Xmax); i += deltaX ){
      AssignVoxelTrue(VoxelMap, i, j, Zval+k);
  }}
  }
}

template <typename P, bool ScalarProperties>
void BuildWallXZplane(vecgeom::FlatVoxelHashMap<P, ScalarProperties>* VoxelMap, float Yval, int thickness, int Xmin, int Xmax, int Zmin, int Zmax, int XBins, int ZBins){
  float deltaZ = (Zmax - Zmin)/ZBins;
  float deltaX = (Xmax - Xmin)/XBins;

  for (float k = 0; k<thickness; k++){
    for (float j = (float(Zmin) + deltaZ/2); j < float(Zmax); j += deltaZ ){
      for (float i = (float(Xmin) + deltaX/2); i < float(Xmax); i += deltaX ){
        AssignVoxelTrue(VoxelMap, i, Yval +k, j);
      }
    }
  }
}


void DumpHashMaps(){
    //LoadLibraries();

    vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
    vecgeom::Vector3D<float> Lengths(2000,2000,6000);
    int NumbBins[3] = {2000,2000,6000};
    std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]);

    //Assign some voxels as true
    AssignVoxelTrue(VoxelMap.get(), 0,0,0);

    //Dump to file
    VoxelMap->dumpToTFile("HashMap1.root");

}
