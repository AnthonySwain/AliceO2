
#include <VecGeom/base/FlatVoxelHashMap.h> //What is the method to do this

void LoadLibraries(){
    gSystem->AddIncludePath("VecGeom");
    gSystem->Load("VecGeom/base/FlatVoxelHashMap.h");
}



vecgeom::Vector3D<float> FindVoxelCenter(float x,float y, float z){
  vecgeom::Vector3D<float> pos(x, y, z);
  auto key = VoxelMap->getVoxelKey(pos);
  return (VoxelMap->keyToPos(key));   
}

bool VoxelCheck(float x,float y, float z){
  vecgeom::Vector3D<float> pos(x, y, z);
  //auto key = VoxelMap->getVoxelKey(pos);
  if ((VoxelMap)->isOccupied(pos)){
    std::cout << "IS BLACKHOLE Particle Deleted, POSITION: " << x << ", " << y << ", " << z << "\n"; 
    return(true);
  }

  return(false);
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

void RandomAllocation(int N, float Min, float Max){
  //Assign N random voxels near the center as true (i.e blachholes )
  for (int i =0; i < N; i++){
  float x = UniformRandom(Min,Max);
  float y = UniformRandom(Min,Max);
  float z = UniformRandom(Min,Max);

  AssignVoxelTrue(x,y,z);
  }
}

void AssignVoxelTrue(float x, float y, float z){
  vecgeom::Vector3D<float> pos(x, y, z);
  //auto key = (VoxelMap)->getVoxelKey(pos);

  //If its already been set to true, don't touch:) 
  if (VoxelCheck(x,y,z)){}
  
  else{
  VoxelMap->addProperty(pos, true);
  std::cout << "BLACKHOLE Set, POSITION: " << x << ", " << y << ", " << z << "\n";
  }}

void BuildWallZYplane(float Xval, int thickness, int Zmin, int Zmax, int Ymin, int Ymax, int ZBins, int YBins){
  float deltaY = (Ymax - Ymin)/YBins;
  float deltaZ = (Zmax - Zmin)/ZBins;

  for (float k = 0; k < thickness ;k++){
  for (float j = (float(Ymin) + deltaY/2); j < float(Ymax); j += deltaY ){
    for (float i = (float(Zmin) + deltaZ/2); i < float(Zmax); i += deltaZ ){
      AssignVoxelTrue(Xval+k, j , i);
  }}
  }
}

void BuildWallXYplane(float Zval, int thickness, int Xmin, int Xmax, int Ymin, int Ymax, int XBins, int YBins){
  float deltaY = (Ymax - Ymin)/YBins;
  float deltaX = (Xmax - Xmin)/XBins;

  for (float k = 0; k<thickness; k++){
  for (float j = (float(Ymin) + deltaY/2); j < float(Ymax); j += deltaY ){
    for (float i = (float(Xmin) + deltaX/2); i < float(Xmax); i += deltaX ){
      AssignVoxelTrue(i, j , Zval+k);
  }}
  }
}

void BuildWallXZplane(float Yval, int thickness, int Xmin, int Xmax, int Zmin, int Zmax, int XBins, int ZBins){
  float deltaZ = (Zmax - Zmin)/ZBins;
  float deltaX = (Xmax - Xmin)/XBins;

  for (float k = 0; k<thickness; k++){
  for (float j = (float(Zmin) + deltaZ/2); j < float(Zmax); j += deltaZ ){
    for (float i = (float(Xmin) + deltaX/2); i < float(Xmax); i += deltaX ){
      AssignVoxelTrue(i, Yval +k, j);
  }}
  }
}




void DumpHashMaps(){
    LoadLibraries();

    vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
    vecgeom::Vector3D<float> Lengths(2000,2000,6000);
    int NumbBins[3] = {2000,2000,6000}; 
    std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]);

    //Assign some voxels as true
    AssignVoxelTrue(0,0,0);

    //Dump to file
    VoxelMap->dumpToTFile("HashMap1.root"); 

}
