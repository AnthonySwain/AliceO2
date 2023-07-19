//Stopped doing this, not really the most needed. //To be finished...

#include <iostream>
#include <filesystem>

template <typename T, std::size_t W, std::size_t H>
void print_2d_array(T (&array)[W][H])
{
    for (auto &row : array)
    {
        for (auto &column : row)
        {
            std::cout << column << " ";
        }
        std::cout << std::endl;
    }
}

template <std::size_t W>
void create_bar_graph(const char* (&xnames)[W], int (&yvalues)[W], const char* name, const char* title){
    TCanvas *c1 = new TCanvas("c1");
    TH1F *h = new TH1F(name,title,3,0,3);
    h->SetStats(0);
    h->SetCanExtend(TH1::kAllAxes);
    c1->SetGrid();
    h->SetFillColor(38);
    for (int j=0 ; j < (sizeof(xnames)/sizeof(const char*)) ;j++){
        h->Fill(xnames[j],yvalues[j]);
    }
    h->LabelsDeflate();
    h->Draw("BAR");

    std::filesystem::create_directories("./DetectorHits");
   
    c1->Print(("./DetectorHits/" + std::string(title)+".pdf").c_str());


}


void CombineDetectorHits(){
    //Macro to create a 1D histogram / bar chart showing the number of hits per histogram 


    TFile *file = new TFile("/home/answain/alice/O2Sim/HitsInDetectorsHistograms.root","READ");
    TList* list;


    TH3I* combinedHits = new TH3I("CombinedHistList", "AllParticleHits",100,-1000,1000,100,-1000,1000,100,-3000,3000);
    const char* detectors[13] = {"ITSHit","MFTHit","TOFHit","EMCHit","PHSHit","CPVHit",
    "FT0Hit","FV0Hit","FDDHit","HMPHit", "MCHHit", "MIDHit", "ZDCHit"};
    
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211};
    
    for (int i =0; i<sizeof(detectors)/sizeof(char*); i++)
    {
        file->GetObject(detectors[i],list);

        for (int j=0; j< sizeof(pdgs)/sizeof(int);j++)
        {
            combinedHits->Add((TH3I*)list->FindObject((std::to_string(pdgs[i])).c_str()));
            std::cout << j << std::endl;
        }
    }

    TFile *f = new TFile("AllHits.root","RECREATE");
    combinedHits->Write("AllHits", TObject::kSingleKey);

    
}

