#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

union byte2 {
        char byte[2];
        short int numint;
};

union byte4 {
        char byte[4];
        int numint;
        float numfloat;
};

void endianSwap4(union byte4 *un) {
    // swap
    char c1 = (*un).byte[0];
    (*un).byte[0] = (*un).byte[3];
    (*un).byte[3] = c1;
    c1 = (*un).byte[1];
    (*un).byte[1] = (*un).byte[2];
    (*un).byte[2] = c1;
}

short int endianSwap2int(short int a) {
        union byte2 un;
        un.numint = a;

        // swap
        char c1 = un.byte[0];
        un.byte[0] = un.byte[1];
        un.byte[1] = c1;

        return un.numint;
}

int endianSwap4int(int a) {
        union byte4 un;
        un.numint = a;

        // swap
        char c1 = un.byte[0];
        un.byte[0] = un.byte[3];
        un.byte[3] = c1;
        c1 = un.byte[1];
        un.byte[1] = un.byte[2];
        un.byte[2] = c1;

        return un.numint;
}

float endianSwap4float(float a) {
        union byte4 un;
        un.numfloat = a;

        // swap
        char c1 = un.byte[0];
        un.byte[0] = un.byte[3];
        un.byte[3] = c1;
        c1 = un.byte[1];
        un.byte[1] = un.byte[2];
        un.byte[2] = c1;

        return un.numfloat;
}

int main()
{
        FILE *myfile;

        int nSample,sampPeriod;
        int sampSize, parmKind;

        myfile = fopen("440c020r.fbank","rt");
        if (!myfile)
        {
                printf("Unable to open the file");
                return 1;
        }

        fread(&nSample,4,1,myfile);
        fread(&sampPeriod,4,1,myfile);
        fread(&sampSize,2,1,myfile);
        fread(&parmKind,2,1,myfile);

        nSample = endianSwap4int(nSample);
        sampPeriod = endianSwap4int(sampPeriod);
        sampSize = endianSwap2int(sampSize);
        sampSize /= 4;
        parmKind = endianSwap2int(parmKind);
        
        printf("%d\n%d\n%d\n%d\n",nSample,sampPeriod,sampSize,parmKind);
        
        float **Feature;
        int i,j;
        Feature = (float**)malloc(sizeof(float*)*nSample);
        for (i=0;i<nSample;++i)
        {
                Feature[i] = (float*)malloc(sizeof(float)*sampSize);
                for (j=0;j<sampSize;++j)
                {
                        fread(&Feature[i][j],4,1,myfile);
                        Feature[i][j] = endianSwap4float(Feature[i][j]);
                }
        }
        fclose(myfile);
        return 0;
}
