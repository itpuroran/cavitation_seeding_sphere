// new metod for searching cluster
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

   bool space(char c)    // РґР»СЏ СЂР°Р·РґРµР»РµРЅРёСЏ СЃС‚СЂРѕРєРё
 {
   return isspace(c);
 }

 bool not_space(char c)  // РґР»СЏ СЂР°Р·РґРµР»РµРЅРёСЏ СЃС‚СЂРѕРєРё
 {
   return !isspace(c);
 }

  std::vector<std::string> split(const std::string& str)  // СЂР°Р·РґРµР»РµРЅРёРµ СЃС‚СЂРѕРєРё
{
   typedef std::string::const_iterator iter;
   std::vector<std::string> ret;

   iter i = str.begin();
   while (i != str.end()) {

      // ignore leading blanks
      i = find_if(i, str.end(), not_space);

      // find end of next word
      iter j = find_if(i, str.end(), space);

      // copy the characters in [i,j)
      if (i != str.end()) {
         ret.push_back(std::string(i,j));
      }
      i = j;
   }
   return ret;
}

  std::map<std::string, std::string> readparams(const std::string fname)  // С‡С‚РµРЅРёРµ РІС…РѕРґРЅС‹С… РїР°СЂР°РјРµС‚СЂРѕРІ
{
   std::map<std::string, std::string> params;
   std::ifstream infile;
   infile.open(fname.c_str());
   std::string sline;
   std::vector<std::string> spline;
     
   // warning: there is no real error checking here   
   while (infile) { 

      std::getline(infile,sline);

      // comments # must be first char on line
      if (sline[0] != '#') {
         spline = split(sline);
         if (spline.size() == 2) {
            params[spline[0]] = spline[1];
         }
      }
   }
   infile.close();
   return params;
}

void read_dump (int n_atoms, int num_reading, std::vector<std::vector<double>> &positions, std::vector<int> &neighbors, std::string fout_dump, \
				double &lboxx0, double &lboxxl, double &lboxy0, double &lboxyl, double &lboxz0, double &lboxzl)
{
	 int line_count = 0;
	 std::ifstream file;
	 std::string line;
	 file.open(fout_dump);
    
	 int line_start = num_reading * (n_atoms + 9);
	 int line_stop = line_start + (n_atoms + 9);
	 std::cout << line_start <<" " << line_stop << std::endl;
	 while (line_count < line_start)
	 {
		 if (!std::getline(file,line).eof())
		 {
			 line_count++;
		 }
		 else
		 {
			 std::cout << " the end of file " << std::endl;
			 return void();
		 }		 		
	 }
	 while(line_count < line_stop)
	 {
         std::getline(file,line);
		 std::string buf; // Have a buffer string
		 std::stringstream ss(line); // Insert the string into a stream
		 std::vector<std::string> tokens; // Create vector to hold our words
		 while (ss >> buf)
		     tokens.push_back(buf);

         if ((line_count == line_start + 1) && (tokens.size() == 1))	
		 {
			 int timestep = std::stoi(tokens[0]);
		 }          
		 else if ((line_count == line_start +  5) && (tokens.size() == 2))
		 {
			 lboxx0 = std::stod(tokens[0]); 
			 lboxxl = std::stod(tokens[1]);
		 }
		 else if ((line_count == line_start + 6) && (tokens.size() == 2))
		 {
			 lboxy0 = std::stod(tokens[0]);
			 lboxyl = std::stod(tokens[1]);
		 }
		 else if ((line_count == line_start + 7) && (tokens.size() == 2))
		 {
			 lboxz0 = std::stod(tokens[0]);
			 lboxzl = std::stod(tokens[1]);
		 }
		 else if ((line_count >= line_start + 9) && (line_count <= line_start + 9 + n_atoms))
		 {
			 int atom_id = std::stoi(tokens[0]); 
			 positions[atom_id][0] = std::stod(tokens[1]);
			 positions[atom_id][1] = std::stod(tokens[2]);
			 positions[atom_id][2] = std::stod(tokens[3]);
			 neighbors[atom_id] = std::stoi(tokens[4]); 
		 }
         line_count++;
     }
         file.close();

}

int number(int j, int l, int k, int num_cells)
{
    return (j%num_cells)*num_cells*num_cells+(l%num_cells)*num_cells+(k%num_cells);
}

void number_to_jlk (int numb, int &j, int &l, int &k, int num_cells)
{
     k = numb%num_cells;   
	 int div = numb/num_cells;  
	 l = div%num_cells;  
	 div = div/num_cells;      
	 j = div%num_cells;
}

     int main(int argc, char* argv[]) 
{ 
	 std::cout << argc << " " << argv[1] << argv[2] << std::endl;
     std::string fin_param = "params.txt";
     std::map<std::string, std::string> params = readparams(fin_param);  // С‡С‚РµРЅРёРµ РІС…РѕРґРЅС‹С… РїР°СЂР°РјРµС‚СЂРѕРІ
     double still_sep = atof(params["still_sep"].c_str());
     int num_neighbors = atoi(params["num_neighbors"].c_str());
     int num_cells = atoi(params["num_cells"].c_str());
     //int num_rad = atoi(params["num_rad"].c_str());
	 //int num_copy = atoi(params["num_copy"].c_str());

	 std::system("mkdir -p results");

	 //std::vector<std::vector<double>>  cluster_volume(num_rad+1, std::vector<double>(num_copy+1));
	 //std::vector<std::vector<double>>  cluster_volume0(num_rad+1, std::vector<double>(num_copy+1));

	 std::string file_result_gen="Result_gen.txt";
     std::ofstream f_result_gen(file_result_gen,std::ios_base::out | std::ios_base::app);

     int ri = std::stoi(argv[1]);
     int cj = std::stoi(argv[2]);
	 double cluster_volume0_av = 0;
	 double grow_prob = 0;
	 
		 //int time_start = time(NULL);	 
		 std::string file_result="results/result_"+std::to_string(ri)+ "_" + std::to_string(cj) +".txt";
         std::ofstream f_result(file_result,std::ios_base::out | std::ios_base::app);
		 std::cout << ri << " " << cj <<  std::endl;
		 std::string file_temp = "temp_" + std::to_string(ri) + "_" + std::to_string(cj) + ".txt";
		 //std::ofstream ftemp;
		 //ftemp.open(file_temp);  

		 int n_atoms; 	
		 double lboxx0;
		 double lboxxl;
		 double lboxy0;
		 double lboxyl;
		 double lboxz0;
		 double lboxzl;
	
		 std::string fout_dump = "dumps_end/dump_end_" + std::to_string(ri) + "_" + std::to_string(cj) + ".txt";
		 std::ifstream file;
		 std::string line;

		 file.open(fout_dump);
		 if(!file) 
		 {
			 std::cerr << "Error! Unable to read " << fout_dump << "\n"; 
		 }

		 int line_count = 0;
		 while (line_count <= 4)
		 {
			 std::getline(file,line);
			 std::string buf; 
			 std::stringstream ss(line); 
			 std::vector<std::string> tokens; 
			 while (ss >> buf)
				 tokens.push_back(buf);
			 if ((line_count == 3) && (tokens.size() == 1))
				 n_atoms = std::stoi(tokens[0]);
			 line_count++;
		 }	
	 	 file.close();

		 std::vector<std::vector<double>>  positions(n_atoms+1, std::vector<double>(3));
		 std::vector<int> neighbors(n_atoms+1);
         
		 int num_reading = 0;
		 std::cout << "check1" << std::endl;
		 while (num_reading <=1)
		 {
			 //ftemp << num_reading << std::endl;
			 read_dump(n_atoms, num_reading, positions, neighbors, fout_dump, lboxx0, lboxxl, lboxy0, lboxyl, lboxz0, lboxzl);
			 std::cout << "check2" << std::endl;
			 for(int n = 0; n< n_atoms; n++)
			 {
				 //ftemp << n << " " << positions[n][0] << " " << positions[n][1] << " " << positions[n][2] << " " << neighbors[n] << std::endl;
			 } 
			
			 enum PCLASS {LIQ, VAP};    // По умолчанию, значения присваиваются по порядку начиная с нуля, т.е. LIQ == 0, VAP == 1 
			 std::vector<PCLASS> parclass(n_atoms, LIQ);   // вектор parclass из Ntot элементов с начальным значением LIQ   
   
   			 int n_VAP=0;
   
   			 for (int i=0; i < n_atoms; ++i) 
			 {
	   			 if (neighbors[i] < num_neighbors) 
	  			 {
		   			 parclass[i] = VAP;
		   			 ++n_VAP;
	   			 }
   			 }
			
  			 int cellclass[num_cells+1][num_cells+1][num_cells+1];  // тип узла решетки jlk
  			 int cellclass_numb[num_cells*num_cells*num_cells];     // тип узла решетки n
  			 int cellclust[num_cells+1][num_cells+1][num_cells+1];  // принадлежность к кластеру узла jlk
  			 int cellclust_numb[num_cells*num_cells*num_cells];     // приндлежность к кластеру узла n
  
  			 int ncell = 0; // число узлов пара в наибольшем кластере
  
 			 double cellposx[num_cells+1][num_cells+1][num_cells+1]; // x координаты узлов
  			 double cellposy[num_cells+1][num_cells+1][num_cells+1]; // y координаты узлов
 			 double cellposz[num_cells+1][num_cells+1][num_cells+1]; // z координаты узлов

			 double lcellx = (lboxxl-lboxx0)/num_cells;  // длина ребра решетки
  			 double lcelly = (lboxyl-lboxy0)/num_cells;
  			 double lcellz = (lboxzl-lboxz0)/num_cells;

			 double lboxx = (lboxxl-lboxx0);
			 double lboxy = (lboxyl-lboxy0);
			 double lboxz = (lboxzl-lboxz0);
			
			 int numb=0;  // текущий номер узла
  
  			 for (int j=0; j<= num_cells; ++j)   // задаем координаты узлов решетки
  			 {
	  			 for (int l=0; l<= num_cells; ++l)
	  			 {
		  			 for (int k=0; k<= num_cells; ++k)
		  			 {
						 cellposx[j][l][k] = lboxx0+(j%num_cells)*lcellx;  
						 cellposy[j][l][k] = lboxy0+(l%num_cells)*lcelly; 
						 cellposz[j][l][k] = lboxz0+(k%num_cells)*lcellz; 
						 cellclass[j][l][k] = 1;
						 cellclust[j][l][k] = 0;
						 numb = (j%num_cells)*num_cells*num_cells+(l%num_cells)*num_cells+(k%num_cells);
						 cellclust_numb[numb]=0;
						 cellclass_numb[numb]=1;
		  			 }
	  			 }
  			 };

			 for (int i=0; i < n_atoms; ++i)  // определяем типы узлов решетки
    		 {
	   			 if (parclass[i] == LIQ) 
				 {
		   			 for (int j=0; j<=num_cells; ++j)
             		 {
				 		 for (int l=0; l< num_cells; ++l)
				 		 {
					 		 for (int k=0; k< num_cells; ++k)
		             		 {
								 double sepx,sepy,sepz;
								 sepx = positions[i][0] - cellposx[j][l][k];
								 sepy = positions[i][1] - cellposy[j][l][k];
								 sepz = positions[i][2] - cellposz[j][l][k];
     
                        		 if (sepx > 0.5 * lboxx) 
								 {
									 sepx = sepx - lboxx;
								 }
								 else if (sepx < -0.5 * lboxx) 
								 {
									 sepx = sepx + lboxx;
					    		 }
					    
					    		 if (sepy > 0.5 * lboxy) 
								 {
									 sepy = sepy - lboxy;
								 }
								 else if (sepy < -0.5 * lboxy) 
								 {
									 sepy = sepy + lboxy;
								 }
						
								 if (sepz > 0.5 * lboxz) 
								 {
									 sepz = sepz - lboxz;
								 }
								 else if (sepz < -0.5 * lboxz) 
								 {
									 sepz = sepz + lboxz;
								 }							

								 double rsq = sepx*sepx+sepy*sepy+sepz*sepz;
								 if (rsq < still_sep*still_sep) 
								 {
									 cellclass[j][l][k] = 0;  // 0=liq
									 numb = (j%num_cells)*num_cells*num_cells+(l%num_cells)*num_cells+(k%num_cells);
									 cellclass_numb[numb]=0;
								 }
							  }
						 }
					 }
				 }
			 }

			 for (int j=0; j<=num_cells; ++j)  
             {
				 for (int l=0; l< num_cells; ++l)
				 {
					 cellclass[j][l][num_cells] = cellclass[j][l][0];
					 cellclass[j][num_cells][l] = cellclass[j][0][l];
				 }
				 cellclass[j][num_cells][num_cells] = cellclass[j][0][0];
			 };
			 int clust_tmp = 0;
             std::queue<int> NeighborsIDQueue;
			 while (!NeighborsIDQueue.empty())
			 {
				 NeighborsIDQueue.pop();
			 }
			 int current_number =0;
			 std::vector<int> queue_checking((num_cells+1)*(num_cells+1)*(num_cells+1));

					 for (int cell_number = 0; cell_number < num_cells*num_cells*num_cells; ++cell_number)
		             {
						 int j;
						 int l;
						 int k;
						 number_to_jlk(cell_number,j,l,k,num_cells);
						 if ((cellclass[j][l][k] == 1) && (cellclust[j][l][k] == 0))
                         {
							 clust_tmp++;
							 //ftemp << "new cluster start " << cell_number << " " << j << " " << l << " " << k <<std::endl;
                             NeighborsIDQueue.push(number(j,l,k,num_cells));
							 queue_checking[number(j,l,k,num_cells)] = 1;
                             while (!NeighborsIDQueue.empty())
                             {
                                 int j2;
                                 int l2;
                                 int k2;
								 current_number = NeighborsIDQueue.front();
                                 number_to_jlk(current_number,j2,l2,k2,num_cells);
								// ftemp << "cluster contain " << current_number << " " << j2 << " " << l2 << " " << k2 << " " << clust_tmp << std::endl;
                                 cellclust[j2][l2][k2] = clust_tmp;
                                 for (int j3=j2-1; j3 <= j2+1; ++j3)
                                 {
                                     for (int l3=l2-1; l3 <= l2+1; ++l3)
                                     {
                                         for (int k3=k2-1; k3 <= k2+1; ++k3)
                                         {
                                             if ((cellclass[j3][l3][k3] == 1)&&(cellclust[j3][l3][k3] == 0)&&(j3 >= 0)&&(l3 >= 0)&&(k3 >= 0))
                                             {
												 if (queue_checking[number(j3,l3,k3,num_cells)] != 1)
												 {
                                                 	 NeighborsIDQueue.push(number(j3,l3,k3,num_cells));
												 	 queue_checking[number(j3,l3,k3,num_cells)] = 1;
												 	 //ftemp << "add to queue " << j3 << " " << l3 << " " << k3 << std::endl;
												 }
                                             }
                                         }
                                     }
                                 }
                                 NeighborsIDQueue.pop();
                             }
                         }
					 }

			 for (int j=0; j < num_cells; ++j)  
             {
				 for (int l=0; l< num_cells; ++l)
				 {
					 cellclass[j][l][num_cells] = cellclass[j][l][0];
					 cellclass[j][num_cells][l] = cellclass[j][0][l];
					 cellclass[num_cells][j][l] = cellclass[0][j][l];
					 cellclust[j][l][num_cells] = cellclust[j][l][0];
					 cellclust[j][num_cells][l] = cellclust[j][0][l];
					 cellclust[num_cells][j][l] = cellclass[0][j][l];
				 }
				 cellclass[j][num_cells][num_cells] = cellclass[j][0][0];
				 cellclass[num_cells][j][num_cells] = cellclass[0][j][0];
				 cellclass[num_cells][num_cells][j] = cellclass[0][0][j];
				 cellclust[j][num_cells][num_cells] = cellclust[j][0][0];
				 cellclust[num_cells][j][num_cells] = cellclust[0][j][0];
				 cellclust[num_cells][num_cells][j] = cellclust[0][0][j];
			 };
			 cellclass[num_cells][num_cells][num_cells] = cellclass[0][0][0];
			 cellclust[num_cells][num_cells][num_cells] = cellclust[0][0][0];

			  //ftemp << " cells " << std::endl;
			 int max_clust = 0;
  			 for (int j=0; j< num_cells; ++j) 
	  		 {
		  		 for (int l=0; l< num_cells; ++l)
	             {
					 for (int k=0; k< num_cells; ++k) 
		              {
						  //ftemp << cellposx[j][l][k] << " " << cellposy[j][l][k] << " " << cellposz[j][l][k] << " " << cellclass[j][l][k] << " " << cellclust[j][l][k] << std::endl;
						  if (cellclust[j][l][k] > max_clust) 
						  {
							 max_clust = cellclust[j][l][k];
						  }
					  }
				  }
			 };

			 int clust_size[max_clust+2]; 	
  
  			 for (int i=0; i< max_clust+1; ++i) 
			 {
	  			 clust_size[i]=0;
    		 };
    
    
    		 for (int j=0; j< num_cells; ++j) 
	  		 {
		  		 for (int l=0; l< num_cells; ++l)
	             {
					 for (int k=0; k< num_cells; ++k) 
		             {
						 if (cellclust[j][l][k]!= 0) 
						 {
							 ++clust_size[cellclust[j][l][k]];
						 }
					 }
				 }
	   		 };

						  
			 for (int i=1; i<= max_clust; ++i) 
			 {
				 if (ncell<clust_size[i]) 
				 {
					 ncell=clust_size[i];
				 }
			 };

			 f_result << num_reading << " " << ncell << " " << ncell*lcellx*lcelly*lcellz << std::endl;

			 num_reading++;
		 } // while num_reading
		 f_result.close();	
		 //int time_final = time(NULL);	 
		 //int time_gen = time_start - time_final;
		// std::cout << time_gen << std::endl;    
	 
	 //cluster_volume0_av = cluster_volume0_av/num_copy;
	 //grow_prob = grow_prob/num_copy;
	 //f_result_gen << std::to_string(ri) << " " << std::to_string(cluster_volume0_av) << " " << std::to_string(grow_prob) << std::endl;
 
 f_result_gen.close();
 //ftemp.close();
}

