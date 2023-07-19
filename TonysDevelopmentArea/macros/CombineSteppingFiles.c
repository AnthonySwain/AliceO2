//Combines all the stepping root files

TList* openhistlist(std::string filepath)
{
    TFile * file = new TFile(filepath.c_str(),"READ");
    TList* list;
    file->GetObject("histlist",list);
    return(list);
}

int numDigits(int number)
{
    int digits = 1;

    while (number>=10) {
        number /= 10;
        digits++;
    }
    return digits;
    }

void savehistlist(TList* list, std::string filepath)
{ 
  //Create new histlist if one already exists 
  filepath += ".root";
  TFile *f = new TFile(filepath.c_str(),"RECREATE");
  list->Write("histlist", TObject::kSingleKey);
  delete f;
}

bool file_exists(const std::string &filename) {
  return std::filesystem::exists(filename);
}

void CombineSteppingFiles(){
    //Combines all the stepping root files
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211}; //Definitely need a way of synchronising this across files
    //Loop through root files, merging the histograms of the individual particles 

    //run in the folder where the simulation was ran.
    
    //HistList0.root, HistList1.root ect... ect...

    std::string filename;
    std::string ending;
    filename = "HistList0";
    ending = ".root";

  
    string filepath;
    filepath = filename + ending;
    //Read the HistList0.root, we will then add to the histograms in these:) or sometihng along tohse lines
    TList* zerolist;
    zerolist = openhistlist(filepath);

    //Loop through the histograms in the list
    /* - okay but why can't we just keep it as a list... 
    TH3I* histograms[8];
    for (int i = 0 ; i<(sizeof(pdgs)/sizeof(int)); i++){
        histograms[i] = (TH3I*)list->FindObject((std::to_string(pdgs[i])).c_str())
        
    }
    */

    int i = 0;
    

    TList* list;
    while (file_exists(filename+ending)){
   
    //read the root files into a dymamic vector then loop through the vector, merging them i guess?
    filepath = filename + ending;

    list = openhistlist(filepath);

    for (int i = 0 ; i<(sizeof(pdgs)/sizeof(int)); i++){
        ((TH3I*)zerolist->FindObject((std::to_string(pdgs[i])).c_str()))->Add((TH3I*)list->FindObject((std::to_string(pdgs[i])).c_str()));
        std::cout << filepath<< std::endl;
    }



    int digits = numDigits(i);
    for (int j=0; j<digits; j++){
    filename.pop_back();
    }
    i += 1;

    filename += std::to_string(i);
    }



    savehistlist(zerolist,"CombinedHistList");
    delete zerolist;
    
    TH3I* combinedPDGs = new TH3I("CombinedHistList", "AllParticleSteps",100,-1000,1000,100,-1000,1000,100,-3000,3000);
    TList* combinedlist;
    combinedlist = openhistlist("CombinedHistList.root");
    

    /**/
    for (int i = 0 ; i<(sizeof(pdgs)/sizeof(int)); i++){
        combinedPDGs->Add((TH3I*)combinedlist->FindObject((std::to_string(pdgs[i]).c_str())));

    }
    //savehistlist(combinedPDGs,"AllSteps");
    TFile *f = new TFile("AllSteps.root","RECREATE");
    combinedPDGs->Write("Allsteps", TObject::kSingleKey);

}