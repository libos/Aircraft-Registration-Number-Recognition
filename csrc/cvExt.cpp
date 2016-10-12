#include "cvExt.h"
static double cvGetThreshVal_Otsu( const CvHistogram* hist )
{
    double max_val = 0;

    int i, count;
    const float* h;
    double sum = 0, mu = 0;
    bool uniform = false;
    double low = 0, high = 0, delta = 0;
    float* nu_thresh = 0;
    double mu1 = 0, q1 = 0;
    double max_sigma = 0;

    count = hist->mat.dim[0].size;
    h = (float*)cvPtr1D( hist->bins, 0 );

    if( !CV_HIST_HAS_RANGES(hist) || CV_IS_UNIFORM_HIST(hist) )
    {
        if( CV_HIST_HAS_RANGES(hist) )
        {
            low = hist->thresh[0][0];
            high = hist->thresh[0][1];
        }
        else
        {
            low = 0;
            high = count;
        }

        delta = (high-low)/count;
        low += delta*0.5;
        uniform = true;
    }
    else
        nu_thresh = hist->thresh2[0];

    for( i = 0; i < count; i++ )
    {
        sum += h[i];
        if( uniform )
            mu += (i*delta + low)*h[i];
        else
            mu += (nu_thresh[i*2] + nu_thresh[i*2+1])*0.5*h[i];
    }
    
    sum = fabs(sum) > FLT_EPSILON ? 1./sum : 0;
    mu *= sum;

    mu1 = 0;
    q1 = 0;

    for( i = 0; i < count; i++ )
    {
        double p_i, q2, mu2, val_i, sigma;

        p_i = h[i]*sum;
        mu1 *= q1;
        q1 += p_i;
        q2 = 1. - q1;

        if( MIN(q1,q2) < FLT_EPSILON || MAX(q1,q2) > 1. - FLT_EPSILON )
            continue;

        if( uniform )
            val_i = i*delta + low;
        else
            val_i = (nu_thresh[i*2] + nu_thresh[i*2+1])*0.5;

        mu1 = (mu1 + val_i*p_i)/q1;
        mu2 = (mu - q1*mu1)/q2;
        sigma = q1*q2*(mu1 - mu2)*(mu1 - mu2);
        if( sigma > max_sigma )
        {
            max_sigma = sigma;
            max_val = val_i;
        }
    }

    return max_val;
}
int graythresh(IplImage *src)
{
		CvHistogram* hist = 0;
        float _ranges[] = { 0, 256 };
        float* ranges = _ranges;
        int hist_size = 256;
        void* srcarr0 = src;

        hist = cvCreateHist( 1, &hist_size, CV_HIST_ARRAY, &ranges );
        cvCalcArrHist( &srcarr0, hist );
        int thresh = cvFloor(cvGetThreshVal_Otsu( hist ));
		return thresh;
}