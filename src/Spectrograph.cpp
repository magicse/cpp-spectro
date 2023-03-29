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

#include "Spectrograph.h"
#include "Utility.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <numeric>

void Spectrograph::save_image( std::string fname, bool log_mode){
    float top_db= 80;
    const double epsilon = 1e-10;
    const int data_size = spectrogram_.front().size();
    double log_spec;
    
    // Create opencv mat for spectrogram
    cv::Mat spectrum(height_, spectrogram_.size(), CV_32F);
    
    // Only the data below 1/2 of the sampling rate and number of channels (nyquist frequency) is useful
    float multiplier = pow(0.5, file_handle_.channels());
    const int data_size_used = data_size * multiplier;
    
    // Get max/min abs values for spectrogram magnitude
    double mag_min = std::numeric_limits<double>::max();
    double mag_max = std::numeric_limits<double>::lowest();
    for (const auto& row : spectrogram_) {
        for (const auto& val : row) {
            mag_min = std::min(mag_min, std::abs(val));
            mag_max = std::max(mag_max, std::abs(val));
        }
    }
    
    const int data_size_used = data_size * multiplier;
    log_max_db = 20*log10(mag_max);
    
    for (int x = 0; x < spectrogram_.size(); x++){
        int freq = 0;
        for (int y = 1; y <= height_; y++){
            double mag_val = abs(spectrogram_[x][freq]);
            
            log_spec = 20*log10(std::max(mag_val, epsilon));
            //Subtracts the threshold from the maximum value to get the minimum amplitude
            //below which all frequencies are considered as noise.
            spectrum.at<float>(y-1, x) = std::max(log_spec, log_max_db - top_db);
            
            float ratio = static_cast<float>(y)/height_;
            freq = static_cast<int>(ratio * data_size_used);
        }
    }
    cv::flip(spectrum, spectrum, 0);
    double minVal, maxVal;
    minMaxLoc(spectrum, &minVal, &maxVal);
    spectrum = (spectrum-minVal)/(maxVal-minVal)*1;
    
    spectrum.convertTo(spectrum, CV_8U, 255.0);
    // Apply a color map to the grayscale image
    applyColorMap(spectrum, spectrum, cv::COLORMAP_MAGMA);
    // Show the color spectrogram
    cv::namedWindow("Spectrogram", cv::WINDOW_NORMAL);
    imshow("Spectrogram", spectrum);
    cv::imwrite("spect.png",spectrum);
    cv::waitKey();
}
