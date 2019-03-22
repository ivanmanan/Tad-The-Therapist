#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <ti/dsplib/dsplib.h>
#include <string.h>

#define PI 3.14159265358979323
#define numMFCCs 13
#define numData 80 // 62 before
#define frameSize 512//1024 //number of samples per frame (real+imag)
#define freq_inc 31.25//15.625 // increment between adjacent FFT frequencies
#define mel_inc 92.4444 //increment between adjacent mel frequencies

float corr = 0;
float slope;
int16_t left_sample;

float PSD_arr [frameSize];
float hamming_window [frameSize];
float Xm[26]; //input to DCT
float Ym[26];
float MFCC_arr[numData][numMFCCs];
float cos_table[26][13]; //DCT lookup table; mrow kcol
int filter_peaks[28]; //contains PSD array sample indices of the filter peaks
int samp_ctr = 0; //iterates through clock cycles
int call_compute = 0; //flag that tells main to run computation
int pc_busy = 0; //=1 when computer is busy running HMMs
int MFCC_idx = 0;

int frame_tracker = 0; //track array that is having first half filled


//file pointers
FILE * mfcc_data; //contains MFCCs that are sent to the PC

/* Align the tables that we have to use */

// The DATA_ALIGN pragma aligns the symbol in C, or the next symbol declared in C++, to an alignment boundary.
// The alignment boundary is the maximum of the symbol's default alignment value or the value of the constant in bytes.
// The constant must be a power of 2. The maximum alignment is 32768.
// The DATA_ALIGN pragma cannot be used to reduce an object's natural alignment.

//The following code will locate mybyte at an even address.
//#pragma DATA_ALIGN(mybyte, 2)
//char mybyte;

//The following code will locate mybuffer at an address that is evenly divisible by 1024.
//#pragma DATA_ALIGN(mybuffer, 1024)
//char mybuffer[256];
#pragma DATA_ALIGN(x_in,8);
int16_t x_in[2*frameSize];

#pragma DATA_ALIGN(x_sp,8);
float   x_sp0 [2*frameSize]; //input samples array
float   x_sp1 [2*frameSize]; //input samples array
float   x_sp2 [2*frameSize]; //input samples array
#pragma DATA_ALIGN(y_sp,8);
float   y_sp [2*frameSize]; //fft output array
#pragma DATA_ALIGN(w_sp,8);
float   w_sp [2*frameSize]; //twiddle factor

// brev routine called by FFT routine
unsigned char brev[64] = {
    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};


void export_mfcc();

void all_off() { // Turn off all LED lights
    LCDK_LED_off(4);
    LCDK_LED_off(5);
    LCDK_LED_off(6);
    LCDK_LED_off(7);
    return;
}

void all_on() {
    LCDK_LED_on(4);
    LCDK_LED_on(5);
    LCDK_LED_on(6);
    LCDK_LED_on(7);
    return;
}


// Function for generating sequence of twiddle factors
void gen_twiddle_fft_sp (float *w, int n)
{
    int i, j, k;
    double x_t, y_t, theta1, theta2, theta3;

    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j)
        {
            theta1 = 2 * PI * i / n;
            x_t = cos (theta1);
            y_t = sin (theta1);
            w[k] = (float) x_t;
            w[k + 1] = (float) y_t;

            theta2 = 4 * PI * i / n;
            x_t = cos (theta2);
            y_t = sin (theta2);
            w[k + 2] = (float) x_t;
            w[k + 3] = (float) y_t;

            theta3 = 6 * PI * i / n;
            x_t = cos (theta3);
            y_t = sin (theta3);
            w[k + 4] = (float) x_t;
            w[k + 5] = (float) y_t;
            k += 6;
        }
    }
}


void get_filter_peaks(){ // get indices of filter peaks and end points
    int i;
    for(i = 0; i < 28; ++i){
        //convert each mel frequency into real frequency and divide by freq_inc to get sample index
        //filter_peaks[0,27] are the indices of the zeros at the far ends
        filter_peaks[i] = (int)round(700 * (powf(10,((344 + i * mel_inc)/2595)) - 1) / freq_inc);
    }
}

void magnitude_square(){ //take magnitude squared of FFT to get PSD
    int i = 0;
    for(i=0; i < frameSize; ++i){
        PSD_arr[i] = sqrtf(y_sp[2*i]*y_sp[2*i] + y_sp[2*i+1]*y_sp[2*i+1]);
    }
    return;
}

void fill_hamming(){ //generate 1024 samples hamming window
    int i;
    for(i = 0; i < frameSize; ++i){
        hamming_window[i] = 0.54 - 0.46 * cosf((2 * PI * i) / (frameSize - 1));
    }
}

void gen_cos_table(){
    int m;
    int k;

    for(m = 1; m < 27; ++m){
        for (k = 1; k < 14; ++k){
            cos_table[m-1][k-1] = cosf((m-0.5) * ((float)k*PI/26.0));
        }
    }
}

// Function that returns a '0' or '1'
char readCharFile(const char* FILE_NAME) {
    FILE* file = fopen(FILE_NAME, "r");
    return fgetc(file);
}

// Function that writes into the file a single char value
void writeCharFile(const char* FILE_NAME, char new_value) {
    FILE* file = fopen(FILE_NAME, "w");
    fprintf(file, "%c", new_value);
    fclose(file);
}


void compute(){
    // store fft of input samples in y_sp
    switch (frame_tracker){
        case 0 :
            DSPF_sp_fftSPxSP(frameSize,x_sp1,w_sp,y_sp,brev,4,0,frameSize);
            magnitude_square(); // store PSD of input in PSD_arr
            break;
        case 1 :
            DSPF_sp_fftSPxSP(frameSize,x_sp2,w_sp,y_sp,brev,4,0,frameSize);
            magnitude_square(); // store PSD of input in PSD_arr
            break;
        case 2 :
            DSPF_sp_fftSPxSP(frameSize,x_sp0,w_sp,y_sp,brev,4,0,frameSize);
            magnitude_square(); // store PSD of input in PSD_arr
            break;
    }

    int i;
    int j;

    for(i = 1; i < 27; ++i){ // Iterate through filter peaks
        corr = 0;
        for(j = filter_peaks[i-1]; j < filter_peaks[i]; ++j){ //upslope
            slope = 1 / (float)(filter_peaks[i] - filter_peaks[i-1]);
            corr += PSD_arr[j] * (j - filter_peaks[i-1]) * slope;
        }
        for(j = filter_peaks[i]; j <= filter_peaks[i+1]; ++j){ //downslope
            slope = 1 / (float)(filter_peaks[i+1] - filter_peaks[i]);
            corr += PSD_arr[j] * (filter_peaks[i+1] - j) * slope;
        }
        Ym[i-1] = corr; //this Xm = Ym
    }

    float logVal;
    // Take log of Ym to get Xm
    for(i = 0; i < 26; ++i){
        logVal = log10f(Ym[i]);
        Xm[i] = logVal;
    }

    // DCT
    if(MFCC_idx < numData) {
        for(i = 0; i < numMFCCs; ++i){ // i=k
            MFCC_arr[MFCC_idx][i] = 0;

            for(j = 0; j < 26; ++j){ // j=m
                MFCC_arr[MFCC_idx][i] += Xm[j] * cos_table[j][i];
            }
        }
        MFCC_idx++;
    }

}

void export_mfcc() {

    // w+ is update; while w is overwrite
    mfcc_data = fopen("input.txt", "w");
    int row, col;
    for (row = 5; row < numData; row++) {
        for (col = 0; col < numMFCCs; col++) {
            fprintf(mfcc_data, "%f ", MFCC_arr[row][col]);
        }
        fprintf(mfcc_data, "\n");
    }
    fclose(mfcc_data);

    // Perform bit flip on lcdk/done.txt
    const char* done_file = "done.txt";
    char init = readCharFile(done_file);
    if (init == '0'){
        writeCharFile(done_file, '1');
    }
    else {
        writeCharFile(done_file, '0');
    }
}

int change = 0;
int prevChange = 0;
int ctt = 0;
// Reinitialize the L138 function again after printf
interrupt void interrupt4(void) // interrupt service routine
{
    ctt++;
    if (ctt > 50000) {
        ctt = 0;
    }

    // Switch is turned on
    if(LCDK_SWITCH_state(5) == 1 && change == 0) {
        all_on();
        change = 1;
    }
    // Switch is turned off
    else if (LCDK_SWITCH_state(5) == 0 && change > 0) {
        all_off();
        change = 0;
    }

    if (LCDK_SWITCH_state(5) == 1 && change == 1){ //start recording
        //store audio input in x_sp
        switch (frame_tracker){
            case 0:
                left_sample = input_left_sample();
                x_sp0[2*samp_ctr] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp0[2*samp_ctr + 1] = 0; //odd index = imag
                x_sp2[2*samp_ctr + frameSize] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp2[2*samp_ctr + frameSize + 1] = 0; //odd index = imag
                ++samp_ctr;
                if (samp_ctr == frameSize/2) { //array is full, move to processing
                    samp_ctr = 0;
                    frame_tracker = 1;
                    call_compute = 1;
                }
                break;
            case 1:
                left_sample = input_left_sample();
                x_sp1[2*samp_ctr] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp1[2*samp_ctr + 1] = 0; //odd index = imag
                x_sp0[2*samp_ctr + frameSize] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp0[2*samp_ctr + frameSize + 1] = 0; //odd index = imag
                ++samp_ctr;
                if (samp_ctr == frameSize/2) {
                    samp_ctr = 0;
                    frame_tracker = 2;
                    call_compute = 1;
                }
                break;
            case 2:
                left_sample = input_left_sample();
                x_sp2[2*samp_ctr] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp2[2*samp_ctr + 1] = 0; //odd index = imag
                x_sp1[2*samp_ctr + frameSize] = (float)left_sample * hamming_window[samp_ctr]; // even index = real
                x_sp1[2*samp_ctr + frameSize + 1] = 0; //odd index = imag
                ++samp_ctr;
                if (samp_ctr == frameSize/2) {
                    samp_ctr = 0;
                    frame_tracker = 0;
                    call_compute = 1;
                }
                break;
        }

        // Record for 2 seconds
        if (MFCC_idx >= numData) {
            MFCC_idx = 0;
            change = 2;
            all_off();
            // Export the current MFCC's into a text file
            export_mfcc();
        }
    }
    else { // Switch is turned off
        frame_tracker = 0;
        samp_ctr = 0;
        call_compute = 0;
    }

    output_left_sample(0);
    return;
}

int main(void)
{
    // Change to LCDK_MIC_input if using usb microphone
    LCDK_GPIO_init();
    LCDK_SWITCH_init();
    LCDK_LED_init();

    all_off();
    get_filter_peaks();
    fill_hamming();
    gen_cos_table();
    gen_twiddle_fft_sp(w_sp, frameSize);

    //////////////////////////////////////////////////////////////
    //Initialize Files

    // NOTE: User runs this program every time they want to initialize a therapy session
    // Performs bit flip on the lcdk/start.txt file
    const char* start_file = "start.txt";
    char init = readCharFile(start_file);
    if (init == '0'){
        writeCharFile(start_file, '1');
    }
    else {
        writeCharFile(start_file, '0');
    }
    // NOTE: Interrupt function does not work with fprintf
    L138_initialise_intr(FS_16000_HZ,ADC_GAIN_21DB,DAC_ATTEN_0DB,LCDK_MIC_INPUT);

    //////////////////////////////////////////////////////////////
    while (1) {

        if (prevChange != change) {
            prevChange = change;
            // Reinitialize interrupt function again due to exporting of mfcc's into a text file
            L138_initialise_intr(FS_16000_HZ,ADC_GAIN_21DB,DAC_ATTEN_0DB,LCDK_MIC_INPUT);

        }

        if(call_compute == 1){ //buffer is filled
            call_compute = 0;
            compute();
        }
    }
}
