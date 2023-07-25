
void AllMacros(){

    gROOT->ProcessLine(".x analyzeHits.C");
    gROOT->ProcessLine(".x analyzeSteps.C");
    gROOT->ProcessLine(".x CombineDetectorHits.C");
    gROOT->ProcessLine(".x CombineSteps.C");
    gROOT->ProcessLine(".x AllProjections.C");
}