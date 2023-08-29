/*
ROOT macro that gets hooked during stepping to apply geometry based cuts to the set-up.
*/
#include <cmath>


struct Cylinder_Data {
    int Zmin;
    int Zmax;
    float radius;
    std::string To_check;
    std::vector<int> PDGs;
};


std::vector<std::string> splitString(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

Cylinder_data parseCSVLine(const std::string &line) {
    Cylinder_data entry;
    std::vector<std::string> tokens = splitString(line, ',');
    
    if (tokens.size() < 4) {
        std::cerr << "Invalid CSV line: " << line << std::endl;
        return entry;
    }
    
    entry.Zmin = std::stoi(tokens[0]);
    entry.Zmax = std::stoi(tokens[1]);
    entry.radius = std::stof(tokens[2]);
    entry.To_check = tokens[3];
    
    for (size_t i = 4; i < tokens.size(); ++i) {
        entry.unnamedInts.push_back(std::stoi(tokens[i]));
    }
    
    return entry;
}

std::vector<Cylinder_Data> readCSVFile(const std::string &filename) {
    std::vector<Cylinder_data> dataEntries;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return dataEntries;
    }
    
    while (std::getline(file, line)) {
        dataEntries.push_back(parseCSVLine(line));
    }
    
    file.close();
    return dataEntries;
}



//Cylinder Volume
bool cylinder_check(float Zmin, float Zmax, float radius, float X, float Y, float Z){   
    //If position is inside the cylinder return false, if outside, return true
    //Assumes the cylindrical axis is on the Z axis

    if (Z > Zmin || Z < Zmax){
       float current_radius = sqrt( pow(X,2) + pow(Y,2));
        
        if (current_radius < radius){
            return false;
        }

        else{
        return true; 
         }
    }

    else{
        return true; 
    }
}

bool PDG_check(std::vector<int> PDGs, std::string To_check, int pdg){
    //Returns true if the PDG should be considered, otherwise skips to the next volume
    if (To_check == "All"){
        return true;
    }

    if (To_check == "All_but"){
        bool is_in = std::find(vec.begin(), vec.end(), target) != vec.end();
        
        if (is_in){
            return false;
        }

        else{
            return true;
        }
    }

    if (To_check == "Only"){
        bool is_in_only = std::find(vec.begin(), vec.end(), target) != vec.end();
        
        if (is_in_only){
            return true;
        }

        else{
            return false;
        }
    }

    //Just skips everything. The code shouldn't really hit here unless a file hasn't been given
    else{
        return false;
    }
}


bool GeometryCutsDev(int PDG, float X,float Y, float Z){
    //ROOT macro that gets hooked during stepping to apply geometry based cuts to the set-up.

    //Can add more shapes in the future.
    //I guess method is to read from CSV file the geometry of the cylinders and to loop through them all,
    // Also checking PDG before even calling the cylinder check
    // if any are set to true, stop the track! 

    //Gets all the cylinder data - we want to read this once, not every step:) 
    std::vector<Cylinder_Data> Data_on_cylinders = readCSVFile("");

    for (const auto& entry : Data_on_cylinders){
        bool checked_pdg = PDG_check(entry.PDGs,entry.To_check,PDG)

        if (checked_pdg){

            bool stop_track = cylinder_check(entry.Zmin,entry.Zmax,entry.radius, X,Y,Z);
            
            if stop_track{
                return stop_track;
            
            } 
        }
    }

    

}