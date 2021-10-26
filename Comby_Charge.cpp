#include <fstream>
#include <string>
#include "Comby_Charge.h"

int Comby_Charge() {

	string infile_name = "data/pythia_CuAu200";
	
	TFile *output;
	
	TH1D *PPbar, *PKbar, *PbarK *KKbar, *PIKbar, *PIbarK *PIPIbar, *PPIbar, *PbarPI;
	
	array <const int, 3> idPart = {211, 321, 2212};
	
	ifstream infile;
	
	infile.open(infile_name.c_str());
	
	vector <double> *nvec, *idvec, 
	
	if (!infile) {
	
		cerr << "File " << infile_name << " does not exist" << endl;
		exit(1);
	
	}
	
	int ev_low_edge = 0, ev_high_edge = 1000;
	
	while (infile) {
		
		int id, int nevent;
		double e, px, py, pz;
		
		infile << nevent << id << e << px << py << pz;
		
		
	
	}
	
	

return 0;

}

