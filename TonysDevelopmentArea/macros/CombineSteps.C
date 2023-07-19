
#include <iostream>
#include <filesystem>




void CombineSteps(){
    //Macro to create a 1D histogram / bar chart showing the number of hits per histogram 


    TFile *file = new TFile("AllSteps.root","READ");
    TList* list;
    file->GetObject("Allsteps",list);
    TH3I* hist;

    TH3I* combinedSteps = new TH3I("CombinedHistList", "AllParticleSteps",100,-1000,1000,100,-1000,1000,100,-3000,3000);
    
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211};
    
    for (int j=0; j< sizeof(pdgs)/sizeof(int);j++)
    {   

        //std::cout << pdgs[j] << std::endl;
        //hist = ((TH3I*)list->FindObject((std::to_string(pdgs[j])).c_str()));
        combinedSteps->Add(((TH3I*)list->FindObject((std::to_string(pdgs[j])).c_str())));
        
    }


    TFile *f = new TFile("AllStepsCombined.root","RECREATE");
    combinedSteps->Write("AllStepsCombined", TObject::kSingleKey);

    
}

