#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "primer_bed.h"

int get_primers_with_mismatches(std::string bed, std::string vpath, std::string out, std::string primer_pair_file){
  std::vector<primer> primers = populate_from_file(bed);
  populate_pair_indices(primers, primer_pair_file);
  std::vector<unsigned int> indices;
  std::string line, cell;
  std::ifstream fin(vpath.c_str());
  out += ".txt";
  std::ofstream fout(out.c_str());
  unsigned int ctr, pos;
  int ind;
  std::stringstream line_stream;
  while (std::getline(fin, line)){
    line_stream << line;
    ctr = 0;
    pos = 0;
    while(std::getline(line_stream,cell,'\t')){
      switch(ctr){
      case 1:
	if(cell != "POS")
	  pos = stoi(cell);
	break;
      default:
	break;
      }
      ctr++;
    }
    if(pos == 0){
      line_stream.clear();
      continue;
    }
    pos--;			// 1 based to 0 based
    ind = get_primer_indice(primers, pos);
    if(std::find(indices.begin(), indices.end(), ind) == indices.end() && ind != -1){
      indices.push_back(ind);
      if(primers.at(ind).get_pair_indice() != -1)
	indices.push_back(primers.at(ind).get_pair_indice());
    }
    line_stream.clear();
  }
  for(std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it) {
    fout << primers.at(*it).get_name();
    std::cout << primers.at(*it).get_name();
    if(it != indices.end() - 1){
      fout << "\t";
      std::cout << "\t";
    }
  }
  std::cout << std::endl;
  return 0;
}
