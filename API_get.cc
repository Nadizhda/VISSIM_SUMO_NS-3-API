// Create the API for the conversion of the VISSIM and SUMO traffic data to NS-3 traffic data.

// Include the required libraries

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>

// Use the standard libraries

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
    
        std::istringstream iss(line);
        
        // ws : white space to ignore the column corresponding to the ws position
        
        if (!(iss >> SimTime >> nodeid1 >> std::ws >> xy1 >> std::ws >> speed >> std::ws >> xy1 >> std::ws >> Xcoord >> std::ws >>Ycoord )) 
        
        {
        
            continue;  // Skip malformed lines
        }

       i++;  // Increment counter

 
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
  
  
  // Define  a global directory of the tools folder in the SUMO folder.
  
  std::string sumo_directory = "./sumo/tools";
  /* 
  * Brief method to convert the SUMO input file to a tcl file.
  * The method first determines the most recent folder in the global directory.
  * Then it converts the osm configure file to a trace.xml file.
  */ 
  
int main(int argc, char *argv[]) {

    std ::string Simulator_name = "SUMO";
    // Add the file directory where the csv file of the VISSIM output , you can use txt file also.
    std:: string VISSIM_file = "VISSIM_data.xlsx";
    std:: string first_dir = "Directory to the osm.sumocfg file";
    std:: string second_dir = "Directory to the trace.xml file";
    std:: string third_dir = "Directory to the traceExporter.py file";
    
    

    if (Simulator_name == "VISSIM")
    {
        std::string outputDir1 = "./"; // VISSIM output file directory  
    
        std::string outputDir2 = "./sumo/tools/";  // SUMO input file directory 
    
        std::string infile = "mobility.txt";
    
        std::string filename  = "Test";
    
        std::string outfile = outputDir1+ filename+ ".tcl";
        
        VISSIM_convert (infile,outfile);
    
    }
    
    else if (Simulator_name == "SUMO")
    
    {
 
    FILE* pipe = popen(" sumo -c" + first_dir +" osm.sumocfg --fcd-output trace.xml", "r");
    
    if (!pipe) 
    {
        std::cerr << "Failed to run SUMO!" << std::endl;
        //return 1;
    }
    
   pclose(pipe);
   
   int  pipe2 = system (" python" + third_dir + "/traceExporter.py -i" +second_dir+ "trace.xml --ns2mobility-output=mobility9.tcl");
     
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

