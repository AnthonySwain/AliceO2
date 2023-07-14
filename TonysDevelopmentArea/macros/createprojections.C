//Macro to create various projections from a 3D histogram of data. 
//Code is quite dirty currently, needs a clean-up

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
    projection -> SetStats(0);


   //cout << ytitle << endl;
    char xtitle;
    char ytitle;

    //Not the best work-around but ah well - it works;) 
    if (projectionaxis == "yx"){
        projection ->GetXaxis()->SetTitle("x");
        projection ->GetYaxis()->SetTitle("y");
    }

    if (projectionaxis == "zx"){
        projection ->GetXaxis()->SetTitle("x");
        projection ->GetYaxis()->SetTitle("z");
    }

    if (projectionaxis == "zy"){
        projection ->GetXaxis()->SetTitle("y");
        projection ->GetYaxis()->SetTitle("z");
    }

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

TList* openhistlist(std::string filepath)
{
    TFile * file = new TFile(filepath.c_str(),"READ");
    TList* list;
    file->GetObject("histlist",list);
    return(list);
}

void ThreeDHistogram(){
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211};
    
    string filepath = ;
    TList* list = openhistlist();
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
}
