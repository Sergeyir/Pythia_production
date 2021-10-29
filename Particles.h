#pragma once

#include <vector>
#include <iostream>

class Particles {
	
	protected:
	
	std::vector <int> p_id, pbar_id;
	std::vector <unsigned int> p_iEvent, pbar_iEvent;
	std::vector <double> p_e, p_px, p_py, p_pz, pbar_e, pbar_px, pbar_py, pbar_pz;
	
	public:
	
	Particles() {	}
	
	void AddParticle(const int id, const unsigned int iEvent, const double e, const double px, const double py, const double pz) {
		
		if (id > 0) {
		
			p_id.push_back(id);
			p_iEvent.push_back(iEvent);
			p_e.push_back(e);
			p_px.push_back(px);
			p_py.push_back(py);
			p_pz.push_back(pz);
		
		}
		
		else if (id < 0) { 
			
			pbar_id.push_back(id);
			pbar_iEvent.push_back(iEvent);
			pbar_e.push_back(e);
			pbar_px.push_back(px);
			pbar_py.push_back(py);
			pbar_pz.push_back(pz);
			
		}
		
		return;
		
	}

};
