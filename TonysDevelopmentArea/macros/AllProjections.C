//Macro to create various projections from a 3D histogram of data. 
//Code is quite dirty currently, needs a clean-up


/*
analyzeHits.C, anlyzeSteps.C
CombineSteps, CombineDetectorHits
AllProjections

*/




#include <algorithm>
#include <iostream>
#include <filesystem>
#include <math.h>  

void HitStepGraphRadial(TH3I* HitHist, TH3I* StepHist, TH3F* HitOverStepHist){

    int TotalHits = HitHist->GetEntries();

    int BinsX = HitHist->GetNbinsX();
    int BinsY = HitHist->GetNbinsY();
    int BinsZ = HitHist->GetNbinsZ();

    int BinNumb;
    int NumbHits;
    int NumbSteps;

    int Radius;
    int x;
    int y;

    //Setting up scatter graph
    auto graph = new TGraph2D();

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
                if (NumbSteps ==0)
                {continue;}
                else{
                x = StepHist->GetXaxis()->GetBinCenter(i);
                y = StepHist->GetYaxis()->GetBinCenter(j);

                Radius = sqrt((x*x) + (y*y));
                graph->AddPoint(/*BinNumb,*/(float(NumbHits)/float(NumbSteps)),(float(NumbHits)/float(TotalHits)),Radius);
                //std::cout<< (float(NumbHits)/float(NumbSteps))<<std::endl;}
                }
            }
        }
    }

    graph->SetTitle("Ratio for Hits/Steps to Hits/Total Hits for each voxel against radius");
    graph->GetXaxis()->SetTitle("Hits/Steps");
    graph->GetYaxis()->SetTitle("Hits/Total Hits");
    graph->GetZaxis()->SetTitle("Radius");
    

    TCanvas* c1 = new TCanvas();

    graph->SetMarkerStyle(20);
    graph->Draw("pcol");
    string name = "./AllParticlesListed/HitStepRatioGraphRadial.png";
    c1->Print(name.c_str());
    //c1->Close();
}

void HitStepGraph(TH3I* HitHist, TH3I* StepHist, TH3F* HitOverStepHist){

    int TotalHits = HitHist->GetEntries();

    int BinsX = HitHist->GetNbinsX();
    int BinsY = HitHist->GetNbinsY();
    int BinsZ = HitHist->GetNbinsZ();

    int BinNumb;
    int NumbHits;
    int NumbSteps;



    //Setting up scatter graph
    auto graph = new TGraph();

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
                if (NumbSteps ==0)
                {continue;}
                else{
                graph->AddPoint((float(NumbHits)/float(NumbSteps)),(float(NumbHits)/float(TotalHits)));
                //std::cout<< (float(NumbHits)/float(NumbSteps))<<std::endl;}
                }
            }
        }
    }

    graph->SetTitle("Ratio for Hits/Steps to Hits/Total Hits for each voxel");
    graph->GetXaxis()->SetTitle("Hits/Steps");
    graph->GetYaxis()->SetTitle("Hits/Total Hits");

    TCanvas* c1 = new TCanvas();

    graph->Draw("A * P");
    string name = "./AllParticlesListed/HitStepRatioGraph.png";
    c1->Print(name.c_str());
    c1->Close();
}

template <typename T>
void ProjectionHistogram(string projectionaxis, T hist, string savename){

  /*projection axis is the axis you want to take the projection of, eg xy
    hist is the 3D histogram*/

    TCanvas *c3 = new TCanvas("c3");
    TH1* projection = hist->Project3D(projectionaxis.c_str());
    projection -> SetStats(0);


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
    projection->GetXaxis()->SetTitleOffset(0.9);
    projection->GetYaxis()->SetTitleOffset(1.2);
    projection->GetZaxis()->SetTitleOffset(1.4);
    projection ->GetZaxis()->SetTitle("count");
    projection ->Draw("colz");

    c3->SetRightMargin(0.20);
    c3->SetLeftMargin(0.10);
    c3->SetBottomMargin(0.10);
    

    //Create directory to save and saving the plots
    std::filesystem::create_directories("./AllParticlesListed");
    string name = "./AllParticlesListed/" + projectionaxis + savename +".pdf";
    c3->Print(name.c_str());
    c3->Close();
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
    string filepathHits = "AllHits.root";
    TFile* fileHits = new TFile(filepathHits.c_str(),"READ");
    TH3I* histHits;
    fileHits->GetObject("AllHits",histHits);
    
    //Opening StepsHistogram
    string filepathSteps = "AllStepsCombined.root";
    TFile* fileSteps = new TFile(filepathSteps.c_str(),"READ");
    TH3I* histSteps;
    histSteps = ((TH3I*)fileSteps->Get("AllStepsCombined"));

    //Hits Histogram Projections
    histHits->GetXaxis()->SetTitle("x");
    histHits->GetYaxis()->SetTitle("y");
    histHits->GetZaxis()->SetTitle("z");
    histHits->SetTitle("Hits in ALICE particle detector");
    histHits->SetContour(100);
    
    TCanvas *c1 = new TCanvas("c1");
    histHits->Draw("BOX2");
    std::filesystem::create_directories("./AllParticlesListed");
    string name = "./AllParticlesListed/3DHits.pdf";
    histHits->SetStats(0);
    histHits->GetXaxis()->SetTitleOffset(1.8);
    histHits->GetYaxis()->SetTitleOffset(1.8);
    histHits->GetZaxis()->SetTitleOffset(1.5);
    c1->SetRightMargin(0.10);
    c1->SetLeftMargin(0.10);
    c1->SetBottomMargin(0.10);
    c1 ->Print(name.c_str());
    c1->Close();    

    

    ProjectionHistogram("yx",histHits,"ProjectionHits");
    ProjectionHistogram("zx",histHits,"ProjectionHits");
    ProjectionHistogram("zy",histHits,"ProjectionHits");
    

    //Steps Histogram Projections
    histSteps->GetXaxis()->SetTitle("x");
    histSteps->GetYaxis()->SetTitle("y");
    histSteps->GetZaxis()->SetTitle("z");
    histSteps->SetTitle("Steps in ALICE particle detector");
    histSteps->SetContour(100);
    
    TCanvas *c2 = new TCanvas("c2");
    string name2 = "./AllParticlesListed/3DSteps.pdf";
    histSteps->Draw("BOX2");
    histSteps->SetStats(0);
    histSteps->GetXaxis()->SetTitleOffset(1.8);
    histSteps->GetYaxis()->SetTitleOffset(1.8);
    histSteps->GetZaxis()->SetTitleOffset(1.5);
    c2->SetRightMargin(0.10);
    c2->SetLeftMargin(0.10);
    c2->SetBottomMargin(0.10);
    c2->Print(name2.c_str());
    c2->Close();

    ProjectionHistogram("yx",histSteps,"ProjectionSteps");
    ProjectionHistogram("zx",histSteps,"ProjectionSteps");
    ProjectionHistogram("zy",histSteps,"ProjectionSteps");



    // Hits Over Steps Projections
    TH3F* HitsDividedSteps = new TH3F("Hits/Steps", "Hits/Steps",1000,-1000,1000,1000,-1000,1000,3000,-3000,3000);

    int HitBinsX = histHits->GetNbinsX();
    int HitBinsY = histHits->GetNbinsY();
    int HitBinsZ = histHits->GetNbinsZ();

    int NumbHits;
    int NumbSteps;

     for (int i = 0; i <HitBinsX;i++)
    {
        for (int j=0; j <HitBinsY; j++)
        {
            for (int k=0; k<HitBinsZ; k++)
            {   
                //I assume the bins will be numbered the same for all 3 histograms as the only difference is their entries? 
                int BinNumb = histHits->GetBin(i,j,k);
                NumbHits = histHits->GetBinContent(BinNumb);
                NumbSteps = histSteps->GetBinContent(BinNumb);
                if (NumbSteps ==0)
                {continue;}
                else{
                HitsDividedSteps->AddBinContent(BinNumb,(float(NumbHits)/float(NumbSteps)));
                
                }
            }
        }
    }


    HitsDividedSteps->GetXaxis()->SetTitle("x");
    HitsDividedSteps->GetYaxis()->SetTitle("y");
    HitsDividedSteps->GetZaxis()->SetTitle("z");
    HitsDividedSteps->SetTitle("Hits/Steps in ALICE particle detector");
    HitsDividedSteps->SetContour(100);

    TCanvas* c3 = new TCanvas("c3");
    HitsDividedSteps->SetStats(0);
    HitsDividedSteps->Draw("BOX2");
    HitsDividedSteps->GetXaxis()->SetTitleOffset(1.8);
    HitsDividedSteps->GetYaxis()->SetTitleOffset(1.8);
    HitsDividedSteps->GetZaxis()->SetTitleOffset(1.5);
    string name3 = "./AllParticlesListed/HitsOverSteps.pdf";
    c3->SetRightMargin(0.10);
    c3->SetLeftMargin(0.10);
    c3->SetBottomMargin(0.10);
    c3 ->Print(name3.c_str());
    c3->Close();

    ProjectionHistogram("yx",HitsDividedSteps,"ProjectionHitsOverSteps");
    ProjectionHistogram("zx",HitsDividedSteps,"ProjectionHitsOverSteps");
    ProjectionHistogram("zy",HitsDividedSteps,"ProjectionHitsOverSteps");
    
    HitStepGraph(histHits,histSteps,HitsDividedSteps);
    HitStepGraphRadial(histHits,histSteps,HitsDividedSteps);
    }


