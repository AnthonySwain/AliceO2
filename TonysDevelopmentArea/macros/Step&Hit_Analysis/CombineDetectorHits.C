#include <iostream>
#include <filesystem>

void CombineDetectorHits(){
    //Macro to create a 1D histogram / bar chart showing the number of hits per histogram 


    TFile *file = new TFile("HitsInDetectorsHistograms.root","READ");
    TList* list;
    TH3I* hist;

    TH3I* combinedHits = new TH3I("CombinedHistList", "AllParticleHits",100,-1000,1000,100,-1000,1000,100,-3000,3000);
    const char* detectors[12] = {"ITSHit","MFTHit","TOFHit","EMCHit","PHSHit","CPVHit",/*"FT0Hit",*/
    "FV0Hit","FDDHit","HMPHit", "MCHHit", "MIDHit", "ZDCHit"};
    
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211};
    
    for (int i =0; i<sizeof(detectors)/sizeof(const char*); i++)
    {
        file->GetObject(detectors[i],list);
        std::cout<<i<<std::endl;
        for (int j=0; j< sizeof(pdgs)/sizeof(int);j++)
        {   
            //std::cout << pdgs[j] << std::endl;
            hist = (TH3I*)list->FindObject((std::to_string(pdgs[j])).c_str());
            combinedHits->Add(hist);
            
        }
    }

    //TPC Detector

    TH3I* TPCHist;
    file->GetObject("TPC", TPCHist);
    std::cout << "\n TPCHist entries" <<  TPCHist->GetEntries() << std::endl;
    std::cout << "\n CombinedHist entries before" <<  combinedHits->GetEntries() << std::endl;
    combinedHits->Add(TPCHist);
    std::cout << "\n CombinedHist entries after" <<  combinedHits->GetEntries() << std::endl;
    //file->Close();

    TFile *f = new TFile("AllHits.root","RECREATE");
    combinedHits->Write("AllHits", TObject::kSingleKey);
    //f->Close();
    
}

