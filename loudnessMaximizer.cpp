/*
The following code implement maximizer.
The maximizer purpose is to increase audio level to maximum, without going over the +-1 threshold, and without knowing the whole signal in advance
Algorithm - for a given buffer with size numsamples calculate the maximum valued sample, and divide by it.
            for smooth output we smooth with pervious buffer 

assume appropriate loop is written where the buffer stereoIn is initilized with audio every run
*/

int nch = 2; //input is assumed to be stereo, with left and right channels.

	// get the maximum value of the left and right chunk channels
	double val;
	double max_chunk_left = 0;
	double max_chunk_right = 0;

	for (int i = 0; i < numsamples * nch; i = i + 2) {
		val = std::abs(stereoIn[i]);
		if (val > max_chunk_left)
			max_chunk_left = val;
		val = std::abs(stereoIn[i + 1]);
		if (val > max_chunk_right)
			max_chunk_right = val;
	}

    double LM_current_val_L = 1/max_chunk_left;
    double LM_current_val_R = 1/max_chunk_right;
	

	// multiplate left and right chunk channels by the loudmax factor and applying smoothing at the beginning
	// according to the previus chunk from the last iterate
	int smooth_len = 32;
	double max_left_smoth;
	double max_right_smoth;
	int j = 0;

	for (int i = -smooth_len * nch; i < numsamples * nch; i = i + 2) {
		if (i < 0) {
			max_left_smoth = ((smooth_len - j)*LM_prev_val_L + (j)*LM_current_val_L) / smooth_len;     // smoothing previus chunk left
			max_right_smoth = ((smooth_len - j)*LM_prev_val_R + (j)*LM_current_val_R) / smooth_len;    // smoothing previus chunk right
			stereoIn[i] = (stereoIn[i] * max_prev_val_L / LM_prev_val_L) * max_left_smoth;
			stereoIn[i + 1] = (stereoIn[i + 1] * max_prev_val_R / LM_prev_val_R) * max_right_smoth;
			j++;
		}
		else if (i >= 0 && i < smooth_len * nch) {
			max_left_smoth = ((smooth_len - j)*LM_prev_val_L + (j)*LM_current_val_L) / smooth_len;     // smoothing current chunk left
			max_right_smoth = ((smooth_len - j)*LM_prev_val_R + (j)*LM_current_val_R) / smooth_len;    // smoothing current chunk left
			stereoIn[i] *= max_left_smoth;
			stereoIn[i + 1] *= max_right_smoth;
			j++;
		}
		else { // (i >= smooth_len * nch)
			stereoIn[i] *= LM_current_val_L;
			stereoIn[i + 1] *= LM_current_val_R;
		}
	}

	// save the LM factor for the next iteration
	LM_prev_val_L = LM_current_val_L;
	LM_prev_val_R = LM_current_val_R;
}

