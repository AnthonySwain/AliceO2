//OUTDATED (Tbh it probably isn't but I haven't used it in so long - made when I was learning everything so isn't the best coding techniques...)
//Macro to create a 3D histogram of different particles step locations in the ALICE detector. 
 
//I'm just going to assume the data is nicely laid out (i.e no steps outside the ALICE detector region), if this doesn't happen to be the case, a fix will have to be implemented but for now - fingers crossed!

#include "/home/answain/alice/VMCStepLogger/MCStepLogger/include/MCStepLogger/StepInfo.h"

#include <MCStepLogger/MCAnalysisFileWrapper.h>
#include <MCStepLogger/MetaInfo.h>
#include <algorithm>
#include <iostream>
#include <filesystem>


void ProjectionHistogram(string projectionaxis, TH3I* hist, int pdgnumber){

    /*projection axis is the axis you want to take the projection of, eg xy
    hist is the 3D histogram*/

    TCanvas *c3 = new TCanvas("c3");
    TH1* projection = hist->Project3D(projectionaxis.c_str());
    //projection -> SetStats(0);


   //cout << ytitle << endl;
    char xtitle;
    char ytitle;

    //Not the best work-around but ah well - it works;) 
    if (projectionaxis == "yx"){
        projection ->GetXaxis()->SetTitle("x");
        projection ->GetYaxis()->SetTitle("y");
    }

    if (projectionaxis == "zx"){
        projection ->GetXaxis()->SetTitle("x");//OUTDATED
        projection ->GetYaxis()->SetTitle("z");
    }

    if (projectionaxis == "zy"){
        projection ->GetXaxis()->SetTitle("y");
        projection ->GetYaxis()->SetTitle("z");
    }
    projection->SetContour(1000);
    projection ->GetZaxis()->SetTitle("count");
    projection ->Draw("colz");

    c3->SetRightMargin(0.15);
    c3->SetLeftMargin(0.15);
    c3->SetBottomMargin(0.10);

    //Create directory to save and saving the plots
    std::filesystem::create_directories("./" + std::to_string(pdgnumber));
    string name = "./" + std::to_string(pdgnumber) +"/" + projectionaxis + "Projection.pdf";
    c3 ->Print(name.c_str());
}

TList* createhistlist(int (&pdgs)[8])
{   
    TList *list = new TList();
    
    for (int i : pdgs){
        string name1 = std::to_string(i);
        string name2 = "Histogram"+std::to_string(i);
        TH3I *h1 = new TH3I(name1.c_str(), name2.c_str(),100,-1000,1000,100,-1000,1000,100,-3000,3000);
        list -> Add(h1);
    }
   return (list);
}

void savehistlist(TList* list, string filepath)
{
   TFile *f = new TFile(filepath.c_str(),"RECREATE");
   list->Write("histlist", TObject::kSingleKey);
}


void AnalyseMCStepLogger(){
    TFile *file = new TFile("MCStepLoggerOutput.root","READ");

    TTree *tree = nullptr;
    file->GetObject("StepLoggerTree",tree);

    std::vector<o2::StepInfo>* steps = nullptr;
    tree->SetBranchAddress("Steps",&steps);

    o2::StepLookups* lookups = nullptr; 
    tree->SetBranchAddress("Lookups", &lookups);

    int entries = tree->GetEntries();
    std::cout << "There are " << entries << " events." << std::endl;


    //Intersting particles to map the steps off
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211};
    int no_particles = sizeof(pdgs)/sizeof(int);
    TList *list = createhistlist(pdgs);

    TH3I *AllParticlesHistogram = new TH3I("AllParticles", "AllParticles",100,-1000,1000,100,-1000,1000,100,-3000,3000);

    for (Int_t i = 0; i<entries; i++){
        cout << "Event " << i << " added to histograms." << endl;
        tree->GetEntry(i);
        for (const auto& step: *steps){
            float x = step.x;
            float y = step.y;
            float z = step.z; 
            int pdg = lookups->tracktopdg[step.trackID];

            //Check if the PDG number is found. 
            auto pos = std::find(pdgs, pdgs + no_particles, pdg);
            if (pos != end(pdgs)){
                //int position_in_pdgs = std::distance(pdgs,pos);

                TH3I* hist = (TH3I*)list->FindObject((to_string(pdg)).c_str());
                hist->Fill(x,y,z,1.0);
                AllParticlesHistogram ->Fill(x,y,z,1.0);

                //cout << "we got here" << endl;
                //histograms[position_in_pdgs]->Fill(x,y,z,1.0);
            }

            else {
                continue;
            }
        }
    }
    
    for (int i  : pdgs){
        
        TH3I* hist = (TH3I*)list->FindObject((to_string(i)).c_str());
        //Titles n stuff 3DHist
        hist->GetXaxis()->SetTitle("x [unknown lengthscale]");
        hist->GetYaxis()->SetTitle("y [unknown lengthscale]");
        hist->GetZaxis()->SetTitle("z [unknown lengthscale]");
        hist->SetTitle("Steps in ALICE particle detector");
        hist->SetContour(1000);
    
        TCanvas *c1 = new TCanvas("c1");
        hist->Draw();

        std::filesystem::create_directories("./" + std::to_string(i));
        string name = "./" + std::to_string(i) +"/" + "3D.pdf";
        c1 ->Print(name.c_str());



        ProjectionHistogram("yx",hist,i);
        ProjectionHistogram("zx",hist,i);
        ProjectionHistogram("zy",hist,i);
    }
   TCanvas *c2 = new TCanvas("c2");
   AllParticlesHistogram->GetXaxis()->SetTitle("x");
    AllParticlesHistogram->GetYaxis()->SetTitle("y");
    AllParticlesHistogram->GetZaxis()->SetTitle("z");
    AllParticlesHistogram->SetTitle("Steps in ALICE particle detector");
    AllParticlesHistogram->SetContour(1000);
    AllParticlesHistogram->Draw();

    c2->Print("AllParticles.pdf");


    ProjectionHistogram("yx",AllParticlesHistogram,112);
    ProjectionHistogram("zx",AllParticlesHistogram,112);
    ProjectionHistogram("zy",AllParticlesHistogram,112);
}


//test