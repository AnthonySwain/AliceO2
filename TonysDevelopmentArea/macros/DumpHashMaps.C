
#include <VecGeom/base/FlatVoxelHashMap.h> //What is the method to do this

void DumpHashMaps(){

    vecgeom::Vector3D<float> MinValues = (-1000,-1000,-3000);
    vecgeom::Vector3D<float> Lengths = (1,1,1);
    int NumbBins[3] = {2000,2000,6000}; 
    std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]);

    //Assign some voxels as true


    //Dump to file
    VoxelMap->dumpToFile("HashMap1"); 

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

void RandomAllocationCenter(int N, float Min, float Max){
    //Assign N random voxels near the center as true (i.e blachholes )
    for (int i =0; i < N; i++){
    float x = UniformRandom(Min,Max);
    float y = UniformRandom(Min,Max);
    float z = UniformRandom(Min,Max);

    AssignVoxelTrue(x,y,z);
  }
}


void AssignVoxelTrue(float x, float y, float z){
    vecgeom::Vector3D<float> pos = (x, y, z);
    auto key = (VoxelMap)->getVoxelKey(pos);

    //If its already been set to true, don't touch:) 
    if (VoxelCheck(x,y,z)){}

    else{VoxelMap->addPropertyForKey(key, true);}
}
