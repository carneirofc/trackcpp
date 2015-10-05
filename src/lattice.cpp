// The MIT License (MIT)
//
// Copyright (c) 2015 LNLS Accelerator Division
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <trackcpp/flat_file.h>
#include <trackcpp/lattice.h>
#include <trackcpp/elements.h>
#include <trackcpp/auxiliary.h>
#include <algorithm>
#include <numeric>
#include <fstream>

std::vector<Element> latt_join(const std::vector<std::vector<Element> >& v_) {
	std::vector<Element> v;
	for(unsigned int i=0; i<v_.size(); ++i) {
		v.insert(v.end(), v_[i].begin(), v_[i].end());
	}
	return v;
}

std::vector<Element> latt_reverse(const std::vector<Element>& v_) {
	std::vector<Element> v(v_);
	std::reverse(v.begin(), v.end());
	return v;
}


void latt_print(const std::vector<Element>& lattice) {

	for(unsigned int i=0; i<lattice.size(); ++i) {
		std::cout << "element#      : " << i+1 << std::endl;
		std::cout << lattice[i];
		std::cout << std::endl;
	}
}

std::vector<int> latt_range(const std::vector<Element>& lattice) {
	std::vector<int> r(lattice.size());
	std::iota(r.begin(), r.end(), 0);
	return r;
}

std::vector<double> latt_findspos(const std::vector<Element>& lattice, const std::vector<int>& idx) {
	std::vector<double> r;
	double              len = 0;
	unsigned int        pos = 0;
	for(unsigned int i=0; (i<(unsigned int)lattice.size()) and (pos<idx.size()); ++i) {
		if ((int) i == idx[pos]) {
			r.push_back(len);
			pos++;
		}
		len += lattice[i].length;
	}
	if (idx[idx.size()-1] == (int) (1+lattice.size()))
		r.push_back(len);
	return r;
}

double latt_findspos(const std::vector<Element>& lattice, const int idx) {
	std::vector<int> v; v.push_back(idx);
	std::vector<double> r = latt_findspos(lattice, v);
	return r[0];
}

void latt_setcavity(std::vector<Element>& lattice, const std::string& state) {
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if (lattice[i].frequency != 0) {
			if (state == "on") {
				lattice[i].pass_method = PassMethod::pm_cavity_pass;
			} else if (state == "off") {
				if (lattice[i].length == 0) {
					lattice[i].pass_method = PassMethod::pm_identity_pass;
				} else {
					lattice[i].pass_method = PassMethod::pm_drift_pass;
				}
			}
		}
	}
}

std::vector<int> latt_findcells_fam_name(const std::vector<Element>& lattice, const std::string& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (lattice[i].fam_name != value)) or (!reverse and (lattice[i].fam_name == value))) {
			r.push_back(i);
		}
	};
	return r;
};

std::vector<int> latt_findcells_frequency(const std::vector<Element>& lattice, const double& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (lattice[i].frequency != value)) or (!reverse and (lattice[i].frequency == value))) {
			r.push_back(i);
		}
	};
	return r;
};

std::vector<int> latt_findcells_angle(const std::vector<Element>& lattice, const double& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (lattice[i].angle != value)) or (!reverse and (lattice[i].angle == value))) {
			r.push_back(i);
		}
	};
	return r;
};

std::vector<int> latt_findcells_polynom_b(const std::vector<Element>& lattice, unsigned int n, const double& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (lattice[i].polynom_b[n] != value)) or (!reverse and (lattice[i].polynom_b[n] == value))) {
			r.push_back(i);
		}
	};
	return r;
}

std::vector<int> latt_findcells_polynom_a(const std::vector<Element>& lattice, unsigned int n, const double& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (lattice[i].polynom_a[n] != value)) or (!reverse and (lattice[i].polynom_a[n] == value))) {
			r.push_back(i);
		}
	};
	return r;
}

std::vector<int> latt_findcells_pass_method(const std::vector<Element>& lattice, const std::string& value, bool reverse) {
	std::vector<int> r;
	for(unsigned int i=0; i<lattice.size(); ++i) {
		if ((reverse and (pm_dict[lattice[i].pass_method] != value)) or (!reverse and (pm_dict[lattice[i].pass_method] == value))) {
			r.push_back(i);
		}
	};
	return r;
}


std::vector<Element> latt_set_num_integ_steps(const std::vector<Element>& orig_lattice) {

	std::vector<Element> lattice = orig_lattice;

	// dipoles
	std::vector<int> bends = latt_findcells_angle(lattice, 0, true);
	double dl = 0.035;
	std::vector<double> bends_len = latt_getcellstruct<double>(lattice, "length", bends);
	for(unsigned int i=0; i<bends.size(); ++i) {
		int nr_steps = std::max((int) std::ceil(bends_len[i] / dl), 10);
		lattice[bends[i]].nr_steps = nr_steps;
	}
	std::vector<int> tmp;
	// quads
	tmp   = latt_findcells_polynom_b(lattice, 1, 0, true);
	std::vector<int> quads;
	std::set_difference(tmp.begin(), tmp.end(), bends.begin(), bends.end(), std::inserter(quads, quads.end()));
	for(unsigned int i=0; i<quads.size(); ++i) lattice[quads[i]].nr_steps = 10;
	// sexts
	tmp   = latt_findcells_polynom_b(lattice, 2, 0, true);
	std::vector<int> sexts;
	std::set_difference(tmp.begin(), tmp.end(), bends.begin(), bends.end(), std::inserter(sexts, sexts.end()));
	//std::set_difference(tmp.begin(), tmp.end(), quads.begin(), quads.end(), std::inserter(sexts, sexts.end()));
	for(unsigned int i=0; i<sexts.size(); ++i) lattice[sexts[i]].nr_steps = 5;

	return lattice;
}

Status::type latt_read_flat_file(const std::string& filename, Accelerator& accelerator) {

	return read_flat_file(filename, accelerator);

}