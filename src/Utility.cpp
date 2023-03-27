/*
 * Copyright (C) magicse, 2023
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "Utility.h"
#include <cmath>
#include <functional>

using namespace std;

namespace Utility {

    double hann_function(int n, int samples){
        return 0.5 * (1.0 - cos((2.0 * M_PI * n)/(samples - 1.0)));

    }

    double blackman_harris(int n, int samples){

        const float a0 = 0.35875;
        const float a1 = 0.48829;
        const float a2 = 0.14128;
        const float a3 = 0.01168;

        const float arg = 2 * M_PI * n / (samples - 1);

        return a0 - a1 * cos(arg) + a2 * cos(2 * arg) - a3 * cos( 3 * arg);
    }
    
    double mag(complex_d& c){
        return sqrt(c.real() * c.real() + c.imag() * c.imag());
    }
    
}
