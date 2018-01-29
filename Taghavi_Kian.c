
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main(){
    int yes = 0;
    char readname[100],writename[100],*header;
    FILE *gread,*gwrite;

    header = (char*)malloc(44);

    printf("\nPlease enter a file to open ending with .wav.\n");
    scanf("%s",readname);
    gread = fopen(readname,"rb");

    while (yes == 0){


        if( gread == NULL){
        printf("\nFile does not exist please enter a new file name.\n");
            scanf("%s",readname);
            gread = fopen(readname,"rb");
            continue;}

        if ( (strstr(readname,".wav")) == NULL ){
        printf("\nNot a .wav file please enter a new file name.\n");
            scanf("%s",readname);
            gread = fopen(readname,"rb");
            continue;}

    fread(header,sizeof(char),44,gread);

        unsigned short int *numchannels;
        unsigned short int *bitspersample;

    numchannels= (unsigned short int *)(header+22);
    bitspersample= (unsigned short int *)(header+34);

        if (*bitspersample != 16 || *numchannels != 1){
        printf("\nFile has incorrect .wav characteristics please enter another file name.\n");
            scanf("%s",readname);
            gread = fopen(readname,"rb");
            continue;}

        yes = 1;

    }

    printf("\nPlease enter a file name to write to: \n");
    scanf("%s",writename);
    strcat(writename,".wav");
    gwrite = fopen(writename,"wb");

    char *chunkid=(char*)header;
    char *format=(char*)(header+8);
    char *subchunk1id=(char*)header+12;
    char *subchunk2id=(char*)(header+36);
    unsigned int *chunksize=(unsigned int*)(header+4);
    unsigned int *subchunk1size=(unsigned int*)(header+16);
    unsigned int *samplerate=(unsigned int*)(header+24);
    unsigned int *byterate=(unsigned int*)(header+28);
    unsigned short int*audioformat=(unsigned short int*)(header+20);
    unsigned short int *numchannels=(unsigned short int*)(header+22);
    unsigned short int *blockalign=(unsigned short int*)(header+32);
    unsigned short int *bitspersample;
    bitspersample=(unsigned short int*)(header+34);
    unsigned int*subchunk2size=((unsigned int*)(header+40));

    *subchunk2size=*subchunk2size<<1;
    *chunksize=(unsigned int)(36+*subchunk2size);
    *numchannels=(unsigned short int)2;
    *byterate=(*samplerate)*(unsigned int)(*numchannels)*(unsigned int)(*bitspersample)/8;
    *blockalign=(unsigned short int)4;

    fwrite(header,sizeof(char),44,gwrite);




    printf("\n%c%c%c%c\n",*chunkid,*(chunkid+1),*(chunkid+2),*(chunkid+3));
    printf("\n%d\n",*chunksize);
    printf("\n%c%c%c%c\n",*format,*(format+1),*(format+2),*(format+3));
    printf("\n%c%c%c%c\n",*subchunk1id,*(subchunk1id+1),*(subchunk1id+2),*(subchunk1id+3));
    printf("\n%d\n",*subchunk1size);
    printf("\n%d\n",*audioformat);
    printf("\n%d\n",*numchannels);
    printf("\n%d\n",*samplerate);
    printf("\n%d\n",*byterate);
    printf("\n%d\n",*blockalign);
    printf("\n%d\n",*bitspersample);
    printf("\n%c%c%c%c\n",*subchunk2id,*(subchunk2id+1),*(subchunk2id+2),*(subchunk2id+3));
    printf("\n%d\n",*subchunk2size);


    int fsize=(int)(*subchunk2size)/2;
    unsigned short int * data=(unsigned short int*)malloc(fsize);
    fseek(gread,44,SEEK_SET);
    fread(data,2,fsize/2,gread);
    int i,up;
    unsigned short int side;
    unsigned short int * left=(unsigned short int*)malloc(fsize);
    unsigned short int * right=(unsigned short int*)malloc(fsize);


        for(i=0;i<fsize/2;i++)
    {
        side=data[i];
        up=(int)(44*(1+.9*cos(2*M_PI*.2*i/44100)));
        if (up>i)
            left[i]=data[i];
        else
        {
            up=i-up;
            left[i]=(unsigned short int)(((double)side+(.7*(double)data[up]))/(1.7));
        }
        up=(int)(44*(1+.9*sin(2*M_PI*.2*i/44100)));
        if (up>i) right[i]=data[i];
        else
        {
            up=i-up;
            right[i]=(unsigned short int)(((double)side+(.7*(double)data[up]))/(1.7));
        }
    }
    for (i=0;i<fsize/2;i++)
    {
        fwrite(&left[i],2,1,gwrite);
        fwrite(&right[i],2,1,gwrite);
    }

    fclose(gread);


    return 0;
}
