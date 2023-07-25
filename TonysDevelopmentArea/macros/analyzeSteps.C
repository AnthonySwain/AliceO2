//Combines all the stepping root files
#include <filesystem>

TList* openhistlist2(std::string filepath)
{
    TFile * file = new TFile(filepath.c_str(),"READ");
    TList* list;
    file->GetObject("histlist",list);
    return(list);
}

int numDigits2(int number)
{
    int digits = 1;

    while (number>=10) {
        number /= 10;
        digits++;
    }
    return digits;
    }

TList* createhistlist2(int (&pdgs)[8])
//Creates and returns a list of histograms for each particle that is wanted to be investigated
// Input is a int array of PDGid of particles.
{   
    TList *list = new TList();
    
    for (int i : pdgs){
        std::string name1 = std::to_string(i);
        std::string name2 = "Histogram"+std::to_string(i);
        TH3I *h1 = new TH3I(name1.c_str(), name2.c_str(),100,-1000,1000,100,-1000,1000,100,-3000,3000);
        list -> Add(h1);
    }
   return (list);
}


void savehistlist2(TList* list, std::string filepath)
{ 
  //Create new histlist if one already exists 
  filepath += ".root";
  TFile *f = new TFile(filepath.c_str(),"RECREATE");
  list->Write("histlist", TObject::kSingleKey);
  delete f;
}

bool file_exists3(const std::string &filename) {
  return std::filesystem::exists(filename);
}


void analyzeSteps(){
    //Combines all the stepping root files
    int pdgs[8] = {11,13,-11,-13,22,111,211,-211}; //Definitely need a way of synchronising this across files

    TList* empty = createhistlist2(pdgs);


    int i =0;
    int j=0;

    //THIS IS SO BAD ITS HILARIOUS 6 WORKER NODES!! 
    while(j<6){
        if (file_exists3("voxel_"+std::to_string(i)+".root")){
            j +=1;
            //std::cout<<j<<std::endl;
            TFile *f = new TFile(("voxel_"+std::to_string(i)+".root").c_str(),"READ");
            for (int pdg : pdgs)
            {      
                std::cout<<("voxel_"+std::to_string(i)+".root").c_str()<<std::endl;
                
                //Need to specify the cycle?
                int no_cycles =  f->GetKey((std::to_string(pdg)).c_str())->GetCycle();

                for (int k = 1; k<=no_cycles;k++){

                ((TH3I*)empty->FindObject((std::to_string(pdg)).c_str()))->Add(((TH3I*)f->Get((std::to_string(pdg)).c_str())));
                }
                
            }
            f->Close();

        }
        i+=1;
        //std::cout<<i<<std::endl;
    }

TFile *f = new TFile("AllSteps.root","RECREATE");
empty->Write("Allsteps",TObject::kSingleKey);
f->Close();
}