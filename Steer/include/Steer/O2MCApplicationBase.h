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
#include "SimConfig/GlobalProcessCutSimParam.h"
#include <filesystem>
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

    



  //Read CSV from file - get the filename entered with the config from the code e.g.
  //o2-sim --configKeyValues="GlobalSimProcs.blackholeVoxelFile=/path/to/rootfile -g pythia8pp -n 20"



  auto& params2 = o2::GlobalProcessCutSimParam::Instance();
  std::string HashMaps_CSV_filepath = params2.blackholeVoxelFile;

  //Reads CSV File
  std::vector<std::pair<std::vector<std::string>, std::string>> MapData = ReadCSVFile(HashMaps_CSV_filepath);
  
  //Seperates into a vector of arrays of pdgs, and a vector of filepaths 
  std::vector<std::vector<std::string>> PDGs = MapData.first;
  std::vector<std::string> HashMapFilePaths = MapData.second;

  std::vector<std::unique_ptr<vecgeom::FlatVoxelHashMap<bool,true>>> VoxelMaps; 

  //Read all the voxelmaps, putting them in a vector of them 
  for (const string& HashMapFileName : HashMapFilePaths){
    if (HashMapFileName != ""){
      if (std::filesystem::exists(HashMapFileName)){
        std::cout<< "File Exists" << std::endl;
        VoxelMap.reset(vecgeom::FlatVoxelHashMap<bool,true>::readFromTFile(HashMapFileName.c_str()));
        VoxelMaps.push_back(VoxelMap);
    }

  else { std::cout << "Hashmap does not exist, using no hashmap." << std::endl;}
  }
  }
  
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
  std::vector<std::pair<std::vector<std::string>, std::string>> ReadCSVFile(); 


  template <typename P, bool ScalarProperties>
  bool VoxelCheck(vecgeom::FlatVoxelHashMap<P, ScalarProperties>*  CurrentMap, float x, float y, float z);
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
};

} // end namespace steer
} // end namespace o2

#endif /* STEER_INCLUDE_STEER_O2MCAPPLICATIONBASE_H_ */
