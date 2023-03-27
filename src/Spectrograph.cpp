
void Spectrograph::save_image( std::string fname, bool log_mode){
    float top_db= 80;
    const double epsilon = 1e-10;
    // Only the data below 1/2 of the sampling rate (nyquist frequency) is useful
    float multiplier = 0.5;
    for (int i = 1; i < file_handle_.channels(); i++){
        multiplier *= 0.5;
    }
    for (int x = 0; x < spectrogram_.size(); x++){
        for (int y = 1; y <= height_; y++){
            double mag_val = abs(spectrogram_[x][freq]);
            
            log_spec = 20*log10(std::max(mag_val, epsilon));
            //subtracts the threshold from the maximum value to get the minimum amplitude below which all frequencies are considered as noise.
            spectrum.at<float>(y-1, x) = std::max(log_spec, 20*log10(mag_max) - top_db);
            
            float ratio = static_cast<float>(y)/height_;
            freq = static_cast<int>(ratio * data_size_used);
        }
    }    
}
