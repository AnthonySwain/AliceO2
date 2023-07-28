// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
 * O2MCApplicationBase.h
 *
 *  Created on: Feb 13, 2019
 *      Author: swenzel
 */

#ifndef STEER_INCLUDE_STEER_O2MCAPPLICATIONBASE_H_
#define STEER_INCLUDE_STEER_O2MCAPPLICATIONBASE_H_

#include <FairMCApplication.h>
#include "Rtypes.h" // for Int_t, Bool_t, Double_t, etc
#include <TVirtualMC.h>
#include "SimConfig/SimParams.h"
#include <VecGeom/base/FlatVoxelHashMap.h> 

namespace o2
{
namespace steer
{

// O2 specific changes/overrides to FairMCApplication
// Here in particular for custom adjustments to stepping logic
// and tracking limits
class O2MCApplicationBase : public FairMCApplication
{
  //////////////////////////////////////////
  std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>> VoxelMap;
  std::vector<std::array<float,4>> SteppingData;
  /////////////////////////////////
 public:
  O2MCApplicationBase() : FairMCApplication(), mCutParams(o2::conf::SimCutParams::Instance()) {initTrackRefHook();}

  O2MCApplicationBase(const char* name, const char* title, TObjArray* ModList, const char* MatName) : FairMCApplication(name, title, ModList, MatName), mCutParams(o2::conf::SimCutParams::Instance())
  {

  vecgeom::Vector3D<float> MinValues(-1000,-1000,-3000);
  vecgeom::Vector3D<float> Lengths(2000,2000,6000); //Is this the lengths of individual voxels or the entire voxelmap?
  int NumbBins[3] = {200,200,600}; 
  VoxelMap = std::make_unique<vecgeom::FlatVoxelHashMap<bool,true>>(MinValues, Lengths, NumbBins[0],NumbBins[1],NumbBins[2]);
  std::cout << "Number of Filled Voxels: " << VoxelMap->size() << std::endl; 


  vecgeom::Vector3D<float> lower;
  vecgeom::Vector3D<float> upper;

  VoxelMap->Extent((VoxelMap->getKey(0,0,0)),lower,upper);
  std::cout << "Lower: " << lower << ", Upper: " << upper << std::endl;

  std::cout << "Min Values Z = " <<MinValues[2] << std::endl;
  BuildWallZYplane(100,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(110,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(120,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(130,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(140,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(150,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(160,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(170,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(180,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(190,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(200,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(210,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(220,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(230,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(240,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(250,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(260,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(270,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(280,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(290,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);
  BuildWallZYplane(300,MinValues[2], MinValues[2]+Lengths[2], MinValues[1], MinValues[1]+Lengths[1], NumbBins[2], NumbBins[1]);

  AssignVoxelTrue(0,0,0);
  //std::cout << FindVoxelCenter(0,0,0) << std::endl;

  std::cout << "Number of Filled Voxels: " << VoxelMap->size() << std::endl; 

  VoxelMap->print();
  initTrackRefHook();   

  }

  ~O2MCApplicationBase() override = default;

  void Stepping() override;
  void PreTrack() override;
  void BeginEvent() override;
  void FinishEvent() override;
  void ConstructGeometry() override;
  void InitGeometry() override;
  bool MisalignGeometry() override;
  void AddParticles() override;

  /////////////////////////////////////////////
  bool VoxelCheck(float x, float y, float z);
  void AssignVoxelTrue(float x, float y, float z);
  void RandomAllocation(int n, float Min, float Max);
  vecgeom::Vector3D<float> FindVoxelCenter(float x,float y, float z);
  void BuildWallZYplane(float Xval, int Zmin, int Zmax, int Ymin, int Ymax, int ZBins, int YBins);
  void BuildWallXYplane(float Zval, int Xmin, int Xmax, int Ymin, int Ymax, int XBins, int YBins);
  void BuildWallXZplane(float Yval, int Xmin, int Xmax, int Zmin, int Zmax, int XBins, int ZBins);
  void ReadHashMap();
  /////////////////////////////////////////////

  // specific implementation of our hard geometry limits
  double TrackingRmax() const override { return mCutParams.maxRTracking; }
  double TrackingZmax() const override { return mCutParams.maxAbsZTracking; }

  typedef std::function<void(TVirtualMC const*)> TrackRefFcn;

 protected:
  o2::conf::SimCutParams const& mCutParams; // reference to parameter system
  unsigned long long mStepCounter{0};
  std::map<int, std::string> mModIdToName{};      // mapping of module id to name
  std::map<int, std::string> mSensitiveVolumes{}; // collection of all sensitive volumes with
                                                  // keeping track of volumeIds and volume names

  double mLongestTrackTime = 0;
  /// some common parts of finishEvent
  void finishEventCommon();
  TrackRefFcn mTrackRefFcn; // a function hook that gets (optionally) called during Stepping
  void initTrackRefHook();

  ClassDefOverride(O2MCApplicationBase, 1);


  //private:
  //std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>> VoxelMap;//(vecgeom::Vector3D<float> const& lower,vecgeom::Vector3D<float> const& length,int Nx,int Ny,int Nz); //VoxelHashMap definition

};

} // end namespace steer
} // end namespace o2

#endif /* STEER_INCLUDE_STEER_O2MCAPPLICATIONBASE_H_ */
