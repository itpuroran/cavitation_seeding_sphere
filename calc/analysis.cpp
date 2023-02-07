// analysis of growing probability
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <queue>
#include <ctime>

using namespace std;

void read_results (int n_rad, int n_copy,  std::string fout_dump, std::vector<double> &start, std::vector<double> &finish)
{
	 int line_count = 0;
	 std::ifstream file;
	 std::string line;
	 file.open(fout_dump);
    
	 while (!std::getline(file,line).eof())
	 {
		 std::string buf; // Have a buffer string
		 std::stringstream ss(line); // Insert the string into a stream
		 std::vector<std::string> tokens; // Create vector to hold our words
		 while (ss >> buf)
		     tokens.push_back(buf);
         if ((tokens.size() == 3)&&(line_count == 0))	
		 {
			 start[0] = std::stoi(tokens[0]);
             start[1] = std::stoi(tokens[1]);
             start[2] = std::stod(tokens[2]);
		 } 
		 else if ((tokens.size() == 3)&&(line_count > 0))
		 {
			 finish[0] = std::stoi(tokens[0]);
             finish[1] = std::stoi(tokens[1]);
             finish[2] = std::stod(tokens[2]); 
		 }
         line_count++;
     }
         file.close();

}

     int main(int argc, char* argv[]) 
{ 
     int num_rad = std::stoi(argv[1]);
	 int num_copy = std::stoi(argv[2]);

	 std::vector<std::vector<double>>  cluster_volume(num_rad+1, std::vector<double>(num_copy+1));
	 std::vector<std::vector<double>>  cluster_volume0(num_rad+1, std::vector<double>(num_copy+1));

	 std::string file_result_av="Result_av.txt";
         std::ofstream f_result_av(file_result_av,std::ios_base::out | std::ios_base::app);
         std::string file_result_gen="Result_gen.txt";
         std::ofstream f_result_gen(file_result_gen,std::ios_base::out | std::ios_base::app);
		 int is_growth;

 for (int ri = 0; ri < num_rad; ++ri) 
 {
	 double cluster_volume0_av = 0;
	 double grow_prob = 0;
     std::vector<double> start(3);
     std::vector<double> finish(3);

	 for (int cj = 0; cj < num_copy; ++cj) 
	 {
		 int time_start = time(NULL);	 
		 std::string file_result="results/result_"+std::to_string(ri)+ "_" + std::to_string(cj) +".txt";
         read_results(ri,cj,file_result,start, finish);
		 std::cout << ri << " " << cj <<  std::endl;

		 cluster_volume0[ri][cj] = start[2];
		 cluster_volume0_av = cluster_volume0_av + cluster_volume0[ri][cj];
		 cluster_volume[ri][cj] = finish[2];
		 if (cluster_volume[ri][cj] > cluster_volume0[ri][cj])
		 	 {
			 ++grow_prob;
		 	 is_growth = 1;
			 }
		 else
		 {
			 is_growth = 0;
		 }		 
		 f_result_gen << " " << start[2] << " " << finish[2] << " " << is_growth << std::endl;
	 }
	 cluster_volume0_av = cluster_volume0_av/num_copy;
	 grow_prob = grow_prob/num_copy;
	 f_result_av << std::to_string(ri) << " " << std::to_string(cluster_volume0_av) << " " << std::to_string(grow_prob) << std::endl;
 }
 f_result_av.close();
 f_result_gen.close();
}

