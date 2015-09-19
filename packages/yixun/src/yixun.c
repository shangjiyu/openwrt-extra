//Yixun Keygen

// By Lyer @ 
//    Shiningacg/XDU 
// lyerpal@126.com
// hi.baidu.com/lyer

//Apr 14,2012
//Apr 16,2012
//EDT:Apr 17,2012

#include <time.h>
#include "md5lib.h"
//typedef long int __int64 ;
int bswap(int v1)
{
	int eax=0;
	char *pedx = (char *)&v1;
	char *peax = (char *)&eax;
	peax[0]=pedx[3];
	peax[1]=pedx[2];
	peax[2]=pedx[1];
	peax[3]=pedx[0];	
return eax;
}
void lendian2byte(unsigned int a,unsigned char * b)
{
	b[0]=a;
	b[1]=a>>8;
	b[2]=a>>16;
	b[3]=a>>24;
}
unsigned int byte2lendian(unsigned char * b)
{
return 	b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24);	
}

int enc1(unsigned int v1)
{
//	__asm("int3");
	int edx;//=(__int64)v1*0x66666667/0x200000000;
  int i[2];
  int o[2];
  i[0]=v1 & 0xFFFF;
  i[1]=v1 >>16;
 	o[1]=o[0]=i[0] * 0x3333; 
	o[1]+=edx=i[1] * 0x3333;
	o[1]+=o[0]>>16;
	edx+=o[1]>>16;
	//edx+=edx/0x80000000;
	return bswap(edx);
}
int enc2(int * a1)
{
int i,eax=3,ecx=0,edx=0;
int v0=(*a1);
//unsigned char * v4=(unsigned char *)&v0;
//unsigned char * arg0=(unsigned char *)&v1;
unsigned char v4[4];
unsigned char arg0[4]={0,0,0,0};
unsigned char al,cl,dl;
lendian2byte(v0,v4);
for(i=1;i<=32;i++)
	{
		al=v4[eax];
		cl=arg0[ecx];
		arg0[ecx]=(cl<<1)|(al&1);
		v4[eax]=al>>1;
		eax=3-i/8;
		ecx=i%4;
	}
*a1=byte2lendian(arg0);
return *a1;
}
char* enc3(int * v1, char * v2)
{
  int i;
  unsigned char v5;
//	unsigned char *a1=(unsigned char *)v1;
  unsigned char *a2=(unsigned char *)v2;
  unsigned char a1[4];
  lendian2byte(*v1,a1);
  
    a2[0] = a1[0]>>2;
    a2[1] = (a1[0] & 3)*16 + (a1[1] >>4);
    a2[2] = (a1[1] & 0xF)*4 + (a1[2]>>6);
    a2[3] = a1[2] & 0x3F;
    a2[4] = a1[3] >> 2;
    a2[5] = (a1[3] & 3)*16;
    for(i=0;i<6;i++) 
    {
      a2[i] += 32;
      if ( a2[i] >= 0x40 ) a2[i]+=1;
    }
  return v2; 
}

void main(int argc,char *argv[])
{
	char buf[7]={0};
	char hashmodel[31];
	char username[15];
	unsigned char hash;
	int tim=time(&tim);
	int a,b,c;
	a=enc1(tim);
  c=a;
  b=enc2(&a);
	//printf("%d 0\n",c);
	//scanf("%s",username);
	//username[0]="";
//	memcpy(hashmodel,&c,4);
  lendian2byte(c,hashmodel);
 
	memcpy(hashmodel+4,argv[1],11);
	memcpy(hashmodel+15,&"sh_xi@xiaoyuanyi",16);
//	__asm int 3;
	hash=MD5DAT(hashmodel,31);
	enc3(&b,buf);


	printf("%s%02x%s",buf,hash,argv[1]);
//	return "%s%02x%s",buf,hash,username;		
/////////////////////////////////////////////////////////////
//  char usernameout[25];
//	sprintf(usernameout,"\r\n%s%02x%s",buf,hash,username);	
/////////////////////////////////////////////////////////////  
}
