
void Spectrograph::save_image( std::string fname, bool log_mode){
    float top_db= 80;
    const double epsilon = 1e-10;
    const int data_size = spectrogram_.front().size();
    
    // Create opencv mat for spectrogram
    cv::Mat spectrum(height_, spectrogram_.size(), CV_32F);
    
    // Only the data below 1/2 of the sampling rate (nyquist frequency) is useful
    float multiplier = 0.5;
    for (int i = 1; i < file_handle_.channels(); i++){
        multiplier *= 0.5;
    }
    
    const int data_size_used = data_size * multiplier;
    log_max_db = 20*log10(mag_max);
    
    for (int x = 0; x < spectrogram_.size(); x++){
        int freq = 0;
        for (int y = 1; y <= height_; y++){
            double mag_val = abs(spectrogram_[x][freq]);
            
            log_spec = 20*log10(std::max(mag_val, epsilon));
            //subtracts the threshold from the maximum value to get the minimum amplitude below which all frequencies are considered as noise.
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
    cv::Mat colormap;
    // ApplyColorMap(spectrum, colormap, cv::COLORMAP_JET);
    applyColorMap(spectrum, spectrum, cv::COLORMAP_MAGMA);
    // Show the color spectrogram
    cv::namedWindow("Spectrogram", cv::WINDOW_NORMAL);
    imshow("Spectrogram", spectrum);
    cv::imwrite("spect.png",spectrum);
    cv::waitKey();
}
