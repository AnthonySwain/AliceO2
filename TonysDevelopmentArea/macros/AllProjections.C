//Macro to create various projections from a 3D histogram of data. 
//Code is quite dirty currently, needs a clean-up

#include <algorithm>
#include <iostream>
#include <filesystem>


void ProjectionHistogram(string projectionaxis, TH3I* hist, string savename){

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
    std::filesystem::create_directories("./AllParticlesListed");
    string name = "./AllParticlesListed/" + projectionaxis + savename +".pdf";
    c3->Print(name.c_str());
}

TList* openhistlist(std::string filepath)
{
    TFile* file = new TFile(filepath.c_str(),"READ");
    TList* list;
    file->GetObject("Allsteps",list);
    return(list);
}


bool file_exists(const std::string &filename) {
  return std::filesystem::exists(filename);
}

void AllProjections(){
    
    //Opening HitsHistogram
    string filepathHits = "/home/answain/alice/O2Sim/AllHits.root";
    TFile* fileHits = new TFile(filepathHits.c_str(),"READ");
    TH3I* histHits;
    fileHits->GetObject("AllHits",hist);
   
    //Opening StepsHistogram
    string filepathSteps = "/home/answain/alice/O2Sim/AllSteps.root";
    TFile* fileSteps = new TFile(filepathSteps.c_str(),"READ");
    TH3I* histSteps;
    fileSteps->GetObject("Allsteps",histSteps);


    //Hits Histogram Projections
    histHits->GetXaxis()->SetTitle("x");
    histHits->GetYaxis()->SetTitle("y");
    histHits->GetZaxis()->SetTitle("z");
    histHits->SetTitle("Hits in ALICE particle detector");
    histHits->SetContour(1000);
    
    TCanvas *c1 = new TCanvas("c1");
    histHits->Draw();
    std::filesystem::create_directories("./AllParticlesListed");
    string name = "./AllParticlesListed/3DHits.pdf";
    histHits->SetSats(0);
    c1 ->Print(name.c_str());
    delete c1;

    ProjectionHistogram("yx",histHits,"ProjectionHits");
    ProjectionHistogram("zx",histHits,"ProjectionHits");
    ProjectionHistogram("zy",histHits,"ProjectionHits");


    //Steps Histogram Projections
    histSteps->GetXaxis()->SetTitle("x");
    histSteps->GetYaxis()->SetTitle("y");
    histSteps->GetZaxis()->SetTitle("z");
    histSteps->SetTitle("Steps in ALICE particle detector");
    histSteps->SetContour(1000);

    name = "./AllParticlesListed/3DSteps.pdf";
    histSteps->SetSats(0);
    c1 ->Print(name.c_str());
    delete c1;

    ProjectionHistogram("yx",histSteps,"ProjectionSteps");
    ProjectionHistogram("zx",histSteps,"ProjectionSteps");
    ProjectionHistogram("zy",histSteps,"ProjectionSteps");



    // Hits Over Steps Projections
    TH3I HitsDividedSteps = histHits;

    HitsDividedSteps->Divide(histSteps);

    HitsDividedSteps->GetXaxis()->SetTitle("x");
    HitsDividedSteps->GetYaxis()->SetTitle("y");
    HitsDividedSteps->GetZaxis()->SetTitle("z");
    HitsDividedSteps->SetTitle("Hits/Steps in ALICE particle detector");
    HitsDividedSteps->SetContour(1000);

    TCanvas *c1 = new TCanvas("c1");
    HitsDividedSteps->SetSats(0);
    HitsDividedSteps->Draw();
    
    name = "./AllParticlesListed/HitsOverSteps.pdf";
    c1 ->Print(name.c_str());
    delete c1;

    ProjectionHistogram("yx",HitsDividedSteps,"ProjectionHitsOverSteps");
    ProjectionHistogram("zx",HitsDividedSteps,"ProjectionHitsOverSteps");
    ProjectionHistogram("zy",HitsDividedSteps,"ProjectionHitsOverSteps");
    
    }




void HitStepGraph(TH3I* HitHist, TH3I* StepHist, TH3I* HitOverStepHist){

    int TotalHits = HitHist->GetEntries();

    int BinsX = HitHist->GetNBinsX();
    int BinsY = HitHist->GetNBinsY();
    int BinsZ = HitHist->GetNBinsZ();

    int BinNumb;
    int NumbHits;
    int NumbSteps;


    //Setting up scatter graph




    //Iterate over all voxels
    for (int i = 0; i <BinsX;i++)
    {
        for (int j=0; j <BinsY; j++)
        {
            for (int k=0; k<BinsZ; k++)
            {   
                //I assume the bins will be numbered the same for all 3 histograms as the only difference is their entries? 
                int BinNumb = HitHist->GetBin(i,j,k);
                NumbHits = HitHist->GetBinContent(BinNumb);
                NumbSteps = StepHist->GetBinContent(BinNumb);

                //Graph->Fill:) 


            }
        }
    }

    /*
    
    GetNBinsX obvious
    GetNBinsY
    GetNBinsZ

    GetBin (x,y,z) //retrieve bin number
    GetBinContent(bin number) //get number of entries in a bin 
    */

}