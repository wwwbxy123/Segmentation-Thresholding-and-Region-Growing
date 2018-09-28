/*********************************************************************/
/* vits:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  DVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int dist;                   /* minimum distance between maxima   */

    VXparse(&argc, &argv, par);    /* parse the command line         */

    dist = 10;                    /* default dist */
    if (DVAL) dist = atoi(DVAL);  /* if d= was specified, get value */
    if (dist < 0 || dist > 255) {
        fprintf(stderr, "d= must be between 0 and 255\nUsing d=10\n");
        dist = 10;
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
 fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }
    


//Compute the average gray value of image 
//and use it as an initial threshold thresh
int sum = 0;
int pixnum = 0;
for(y = im.ylo; y <= im.yhi; y++){
    for(x = im.xlo; x <= im.xhi; x++){
        sum = sum + im.u[y][x];
        pixnum ++;
    }
}
int avg0 = sum / pixnum;

int thresh = avg0;
int sumabove = 0;
int sumbelow = 0;
int pixnumabove = 0;
int pixnumbelow = 0;
int avgabove = 0;
int avgbelow = 0;
int oldavgabove = 1;
int oldavgbelow = 0;

while( avgabove != oldavgabove && avgbelow != oldavgbelow){
    oldavgabove = avgabove;
    oldavgbelow = avgbelow;
        for(y = im.ylo; y <= im.yhi; y++){
            for(x = im.xlo; x <= im.xhi; x++){
                if(im.u[y][x] > thresh){
                sumabove = sumabove + im.u[y][x];
                pixnumabove ++;
                }
                if(im.u[y][x] < thresh){
                sumbelow = sumbelow + im.u[y][x];
                pixnumbelow ++;
                }
            }
        }

        if(pixnumabove == 0) avgabove = 0;
        if(pixnumabove == 0) avgbelow = 0;
        else {
        avgabove = sumabove / pixnumabove;
        avgbelow = sumbelow / pixnumbelow;
	thresh = (avgabove + avgbelow) / 2;
	}
    
    }

        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }

        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}

