#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define n 4
#define N 288 /* frame dimension for QCIF format */
#define M 352 /* frame dimension for QCIF format */
#define filename "bus_cif_60_yuv444.yuv"
#define file_y "ergasia1.yuv"


/* code for armulator*/
#pragma arm section zidata="ram"
int current_y[N][M];

#pragma arm section
int i,j,t,f,x,r[n+1],ki[n+2],iter;
double e[n+1],p[255],m[255],paranom[n+1],arithm[n+1],max;


void read()
{
  FILE *frame_c;
  if((frame_c=fopen(filename,"rb"))==NULL)
  {
    printf("current frame doesn't exist\n");
    exit(-1);
  }

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      current_y[i][j]=fgetc(frame_c);
    }
  }
  
  fclose(frame_c);
}

void write()
{
  FILE *frame_y;
  frame_y=fopen(file_y,"wb");

  for(i=0;i<N;i++)
  {
    for(j=0;j<M;j++)
    {
      fputc(current_y[i][j],frame_y);
    }
  }
  fclose(frame_y); 
}


int main()
{
read();

printf("Arxika Katoflia\n"); //arxika katoflia
for(i=1;i<n+1;i++)
{
ki[i]=256*i/(n+1);
printf("%d\n",ki[i]);
}

 //ypologismos sixnotita fwteinothtas
for(f=0;f<256;f++)
{
 for(j=0;j<M;j++)
  {
	for(i=0;i<N;i++)
	{
	if(current_y[i][j]==f)
	p[f]=p[f]+1;	
    }
  }
  p[f]=p[f]/(N*M);
}


ki[0]=0; //prin to proto katofli
ki[n+1]=255; //meta to teleutaio katofli
for(i=0;i<n+1;i++) //exoume n+1 plithos m
{
 for(x=ki[i];x<=ki[i+1];x++)
 {
	paranom[i]=paranom[i]+p[x];
	arithm[i]=arithm[i]+x*p[x];
 } 
 m[i]=arithm[i]/paranom[i];
}


iter=0;
do
{
iter=iter+1;
for(i=1;i<n+1;i++)
{
e[i]=(m[i-1]+m[i])/2-ki[i];
ki[i]=ki[i]+rint(e[i]);
}
for(j=1;j<n+1;j++)
{
max=-50;
if(fabs(e[j])>max)
max=fabs(e[j]);
}
}while(max>=0.5);


printf("Finally after %d iterations:\n",iter);
for(i=1;i<n+1;i++)
{
printf("%d" , ki[i]);
printf("\n");
}

printf("Classify the pixels regarding the new thresholds:\n");
ki[n+2]=300;
for(i=0;i<N;i++)
{
  for(j=0;j<M;j++)
  { 
	for(t=0;t<=n+1;t++)
	{
      if(current_y[i][j]>=ki[t] && current_y[i][j]<ki[t+1])
       current_y[i][j]=ki[t];	
	
	}
  }
}


write();


for(i=0;i<N;i++)
{
   for(j=0;j<M;j++)
    {
    //printf("%d\n" , current_y[i][j]-new_y[i][j]);  ิม PIXEL มหหมฮมอ สมิม
	printf("%d\n" , current_y[i][j]);
	}
}
return 0;
}



