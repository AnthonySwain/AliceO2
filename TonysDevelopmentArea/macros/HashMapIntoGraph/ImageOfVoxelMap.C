void ImageOfVoxelMap(std::string filepath, int Nx, int Ny, int Nz){
    /* Takes a voxel map and turns it into a root histogram*/

vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
vecgeom::Vector3D<float> Lengths(2000,2000,6000); //Is this the lengths of individual voxels or the entire voxelmap?
int NumbBins[3] = {Nx,Ny,Nz}; 

//Create histogram
TH3S HashMapVisual;

//assign values to the histogram (1 = true, 0 = false)
//probably best done just by iterating through the hashmap and filling the histogram:) 

//then try on some obvious shapes to see if it works properly. 


}