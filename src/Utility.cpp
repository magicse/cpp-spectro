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
    
    uint32_t reverse_bits(uint32_t val, int power){
        int reversed = 0;
        for (int i = 0; i < power; i++){
            bool cur_bit = (1 << i) & val;
            reversed |= (cur_bit << (power - i - 1));
        }
        return reversed;
    }

    //windows function Hann
    double hann_function(int n, int samples){
        return 0.5 * (1.0 - cos((2.0 * M_PI * n)/(samples - 1.0)));

    }
    
    //windows function Blackman-harris
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
    
    //Function takes in the sampling rate sr, the FFT size n_fft, and the number of Mel frequency bins n_mels
    //as inputs, and returns a matrix containing the Mel filterbank.
    //2595 is a constant value used in the mel scale conversion formula. It is the approximate number of mel values
    //per octave and is derived from the formula 1000/log(2) which is the conversion factor between hertz and mel.
    std::vector<std::vector<double>> mel_filterbank_htk(int sr, int n_fft, int n_mels) {

        double min_freq = 0 ;
        double max_freq = (double)sr / 2.0;
        //Convert min max Hz to max Mel
        float mel_fmin = 2595.0 * log10(1.0 + min_freq / 700.0);
        float mel_fmax = 2595.0 * log10(1.0 + max_freq / 700.0);

        std::vector<float> mel_points(n_mels+2);

        for (int i = 0; i < mel_points.size(); ++i) {
            float mel = mel_fmin + i * (mel_fmax - mel_fmin) / (n_mels+1 );
            float freq = 700.0 * (pow(10, mel / 2595.0) - 1.0);
            mel_points[i] = freq / static_cast<float>(sr) * n_fft;
        }

        std::vector<std::vector<double>> filterbank(n_mels, std::vector<double>(n_fft / 2 + 1));
        for (int i = 0; i < n_mels; ++i) {
            for (int j = 0; j < n_fft / 2 + 1; ++j) {
                double h = 0.0;
                if (mel_points[i] <= j && j <= mel_points[i + 1]) {
                    h = (double)(j - mel_points[i]) / (mel_points[i + 1] - mel_points[i]);
                } else if (mel_points[i + 1] <= j && j <= mel_points[i + 2]) {
                    h = (double)(mel_points[i + 2] - j) / (mel_points[i + 2] - mel_points[i + 1]);
                }
                filterbank[i][j] = h;
            }
        }

        return filterbank;
    }
    
}
