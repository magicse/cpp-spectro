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
}
