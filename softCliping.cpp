/*
The following code does soft clipping to a signal.
Soft clipping is clipping signal to range of +-1 without discontinutiy which results in unpleasent sound.
The algorithm is to run the samples through a sigmoid function with output limited to +-1

Assume the buffer, number of samples and number of channels are defined elsewhere
*/

// parameter fo sigmoid function
int L = 2;
int k = -2;

	// sigmoid function
	for (int i = 0; i < numsamples * nch; i++) {
		buffer_tmp[i] = (L / (1 + exp(k * buffer_tmp[i]))) - L / 2;
	}
}