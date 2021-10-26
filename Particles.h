#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"

class Particles {

	private:
	
	vector <int> p_id, p_iEvent;
	double p_e, p_px, p_py, p_pz;
	
	public:
	
	Particles(int id, int iEvent, double e, double px, double py, double pz) {
	
		p_id.push_back(id);
		p_iEvents.push_back(iEvent);
		p_e.push_back(e);
		p_px.push_back(p_px);
		p_py.push_back(p_py);
		p_pz.push_back(p_pz);
	
	}
	
	~Particles() {
	
		p_id.clear();
		p_iEvent.clear();
		p_e.clear();
		p_px.clear();
		p_py.clear();
		p_pz.clear();
		
	}

}
