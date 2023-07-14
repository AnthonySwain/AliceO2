void test()
{
    TList* list = new TList();

    TH3I *h1 = new TH3I("beep","boop", 100,-1000,1000,100,-1000,1000,100,-3000,3000);
    TFile *f = new TFile("beepboop.root","UPDATE");
    list->Add(h1);
    list->Write("listylist", TObject::kSingleKey);

    TList* list2 = new TList();
    delete f;


    TFile *g = new TFile("beepboop.root","UPDATE");
    TH3I *h2 = new TH3I("beep","boop", 100,-1000,1000,100,-1000,1000,100,-3000,3000);
    list2->Add(h2);
    list2->Write("listylist", TObject::kSingleKey);
    delete g;
}
