/*
the following code does resample to given source signal (with known source rate and length) to output destination (with wanted rate)
assume the signals, rates and length are defined elsewhere.
The algorithm is to take the average between two source samples, according to the needed position.
*/
double pos = 0.;
double delta = src_srate / dest_srate;
for (int i = 0; i < dest_len; ++i){
    int idx = int(pos);
    if (idx < src_len)    {
        double frac = pos - floor(pos);
        double interp = (1. - frac) * src[idx];
        if (++idx < src_len) interp += frac * src[idx];
        pos += delta;
        *dest++ = interp;
    }
    else{
        *dest++ = 0;
    }
}