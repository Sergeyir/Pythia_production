#pragma once

#include <array>
#include <vector>
#include <iostream>

class Particles {
	
	protected:
	
	std::vector <int> p_id, pbar_id;
	std::vector <unsigned int> p_iEvent, pbar_iEvent;
	std::vector <double> p_e, p_px, p_py, p_pz, pbar_e, pbar_px, pbar_py, pbar_pz;
	std::vector <unsigned int> p_centr, pbar_centr;
	
	std::array <double, 5> pool_nevents = {0, 0, 0, 0, 0};
	
	std::array <const unsigned int, 6> centr_det{200, 85, 41, 17, 5, 2}; //centality ranges for CuAu200
	
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
	
	int SetCentrality(unsigned const long long int iEvent, unsigned const long long int ncharged) {
		
		unsigned int centr = 4;
		
		for (int count = 0; count < centr_det.size() - 1; count++) {
		
			if (ncharged > centr_det[count+1] && ncharged < centr_det[count]) centr = count;
			
		}
		
		pool_nevents[centr]++;
		
		for (unsigned int count = 0; count < p_iEvent.size(); count++) {
		
			if (iEvent == p_iEvent[count]) p_centr.push_back(centr);
		
		}
		
		for (unsigned int count = 0; count < pbar_iEvent.size(); count++) {
		
			if (iEvent == pbar_iEvent[count]) pbar_centr.push_back(centr);
		
		}
		
		return centr;
		
	}

};

