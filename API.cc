// Create the API for the conversion of SUMO and VISSIM data to NS-3 traffic data  

//Include libraies.

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <algorithm>

using namespace std;

 /* 
  * Brief method to convert the VISSIM input file to a tcl file.
  * The file follows mobility of the vehicles and update the tcl file
  * based on the entry and the exit of the vehicles at each time instant.
  */ 

  void VISSIM_convert(std::string infile1, std::string outfile1)
  {
     std:: ifstream infile(infile1);
     std:: ofstream outfile (outfile1);
     
     std::string line;
     
     if (!infile.is_open() || !outfile.is_open()) 
     {
        std::cerr << "Error opening file!" << std::endl;
        
       // return 1;
     }
     
    double SimTime, SimTime1, speed, Xcoord, Ycoord,xy1,xy2, Zcoord = 1.6;
    
    int nodeid1, i = 0, x = 0;
    

    // Skip the first line
    std::getline(infile, line);

    while (std::getline(infile, line)) {
    
        
        std::replace(line.begin(), line.end(), ',', ' ');  // Replace all ',' with ' ' for the CSV file 
        
       // std::cout << line << std::endl;
        
        std::istringstream iss(line);
        
        // ws : white space to ignore the column corresponding to the ws position
        
        if (!(iss >> SimTime >> nodeid1 >> std::ws >> xy1 >> std::ws >> speed >> std::ws >> xy1 >> std::ws >> Xcoord >> std::ws >>Ycoord )) 
        
        {
        
            continue;  // Skip malformed lines
        }

       i++;  

 
       int node_index = nodeid1 -1;
        
        if (nodeid1 > x )
        {    
        
              outfile << "$node_(" << node_index << ") set X_ " << Xcoord << "\n";
              outfile << "$node_(" << node_index << ") set Y_ " << Ycoord << "\n";
              outfile << "$node_(" << node_index << ") set Z_ " << Zcoord << "\n";
              
              outfile << "$ns_ at " << SimTime  << " \"$node_(" << node_index << ") setdest " 
                << Xcoord << " " << Ycoord << " " << speed << "\"\n";
       }
        
        
        else if (nodeid1 <= x )
       {
        
        outfile << "$ns_ at " << SimTime << " \"$node_(" << node_index << ") setdest " 
                << Xcoord << " " << Ycoord << " " << speed << "\"\n";
        }


        if (nodeid1 >= x) 
        {
            x = nodeid1;
            
        }
            
        }
    infile.close();
   
    outfile.close();
  
  }
  
  
  // Define  a global directory of the tools folder/ SUMO folder.
  
  std::string sumo_directory = "./sumo/tools"; // Update it based on your directory

  
int main(int argc, char *argv[]) {

    std ::string Simulator_name = "VISSIM"; // VISSIM or SUMO
    
    // Add the file directory where the csv file of the VISSIM output, you can use txt file also.
    std:: string VISSIM_file = "./Mobility.csv"; // CSV file, VISSIM simulator 
 
    //Please replace D by the directory to the osm.sumocfg for the scenario you generate using OSM
    std:: string Dir1 = " D/osm.sumocfg --fcd-output trace.xml"; std:: string 
 
    //Please replace D1 by the directory to the traceExporter.py file. 
 
    std:: string Dir2 =  "D1/traceExporter.py"; 
  //Please replace D3 by the directory to the trace.xml file.
    std:: string Dir3 =  "D2/trace.xml"
    
    

    if (Simulator_name == "VISSIM")
    {
        std::string outputDir1 = "./"; // VISSIM output file directory  
    
        std::string outputDir2 = "./sumo/tools/";  // SUMO input file directory 
    
        std::string infile = VISSIM_file;
    
        std::string filename  = "Test_VISSIM";
    
        std::string outfile = outputDir1+ filename+ ".tcl";
        
        VISSIM_convert (infile,outfile);
    
    }
    
    else if (Simulator_name == "SUMO")
    
    {
    /*
     *
     * Make sure that the pip command is written as follows:
     * CMD = "sumo -c D/osm.sumocfg --fcd-output trace.xml"
     * Make sure to pass CMD the string into the popen without segmnting it.  
    */
 
    FILE* pipe = popen(CMD, "r");
    
    if (!pipe) 
    {
        std::cerr << "Failed to run SUMO!" << std::endl;
        //return 1;
    }
    
   pclose(pipe);

             
    /*
     *
     * Make sure that the pip command is written as follows:
     * CMD1 = "python Dir2 -i  Dir3 --ns2mobility-output=Test_SUMO.tcl"
     * Please make sure to pass CMD1 without segmnting it into the popen.  
    */
   
   int  pipe2 = system (CMD1);
     
    }
 
   /* CommandLine cmd (__FILE__);
    
    cmd.AddValue ("infile",
                 "CSV file for the coversion to tcl file ",
                 infile);
    cmd.AddValue ("outfile",
                 "tcl file name ",
                 infile);
    */

   // std::cout << "Output written to"+outfile<< std::endl;
    return 0;
}
