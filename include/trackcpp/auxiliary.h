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

#ifndef _AUXILIARY_H
#define _AUXILIARY_H

#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <cmath>

struct PassMethod {
    enum type {
        pm_identity_pass                  = 0,
        pm_drift_pass                     = 1,
        pm_str_mpole_symplectic4_pass     = 2,
        pm_bnd_mpole_symplectic4_pass     = 3,
        pm_corrector_pass                 = 4,
        pm_cavity_pass                    = 5,
        pm_thinquad_pass                  = 6,
        pm_thinsext_pass                  = 7,
        pm_kicktable_pass                 = 8,
        pm_nr_pms                         = 9
    };
};

const std::vector<std::string> pm_dict = {
        "identity_pass",
        "drift_pass",
        "str_mpole_symplectic4_pass",
        "bnd_mpole_symplectic4_pass",
        "corrector_pass",
        "cavity_pass",
        "thinquad_pass",
        "thinsext_pass",
        "kicktable_pass"
};

struct Status {
    enum type {
        success = 0,
        passmethod_not_defined = 1,
        passmethod_not_implemented = 2,
        particle_lost = 3,
        inconsistent_dimensions = 4,
        uninitialized_memory = 5,
        findorbit_not_converged = 6,
        findorbit_one_turn_matrix_problem = 7,
        file_not_found = 8,
        file_not_opened = 9,
        kicktable_not_defined = 10,
        kicktable_out_of_range = 11,
        flat_file_error = 12,
        newton_not_converged = 13,
    };
};

const std::vector<std::string> string_error_messages = {
        "success",
        "passmethod_not_defined",
        "passmethod_not_implemented",
        "particle_lost",
        "inconsistent_dimensions",
        "uninitialized_memory",
        "findorbit_not_converged",
        "findorbit_one_turn_matrix_problem",
        "file_not_found",
        "file_not_opened",
        "kicktable_not_defined",
        "kicktable_out_of_range",
        "flat_file_error",
        "newton_not_converged",
};

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define VER_STR STR(VERSION)

const std::string string_version = "TRACKCPP version " + std::string(VER_STR) + " (" + std::string(__DATE__) + " " + std::string(__TIME__) + ")";

struct Plane {
	enum type {
		no_plane = 0,
		x = 1,
		y = 2,
		z = 3
	};
};

// template <typename T> class Pos;
// class Element;

const double light_speed              = 299792458;         // [m/s]   - definition
const double vacuum_permeability      = 4*M_PI*1e-7;       // [T.m/A] - definition
const double electron_charge          = 1.60217656535e-19; // [C]     - 2014-06-11
const double electron_mass            = 9.1093829140e-31;  // [Kg]    - 2014-06-11
const double electron_rest_energy     = electron_mass * pow(light_speed,2);             // [Kg.m^2/s^2] - derived
const double vaccum_permitticity      = 1/(vacuum_permeability * pow(light_speed,2));   // [V.s/(A.m)]  - derived
const double electron_rest_energy_MeV = (electron_rest_energy / electron_charge) / 1e6; // [MeV] - derived
const double electron_radius          = pow(electron_charge,2)/(4*M_PI*vaccum_permitticity*electron_rest_energy); // [m] - derived

template <typename T>
int sgn(T val) {
	if (val >= 0) return 1; else return -1;
}

bool isfinite(const double& v);
std::string get_timestamp();

typedef double Vector4[4];
typedef double Vector6[6];
typedef double Matrix4[4][4];

typedef std::vector<double> Vector;
typedef std::vector<std::vector<double> > Matrix;

#undef sqr

#endif