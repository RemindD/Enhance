#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

union byte2 {
	char byte[2];
	short int numint;
};

union byte4 {
	char byte[4];
	int numint;
	float numfloat;
};

static void endianSwap4(union byte4 *un) {
    // swap
    char c1 = (*un).byte[0];
    (*un).byte[0] = (*un).byte[3];
    (*un).byte[3] = c1;
    c1 = (*un).byte[1];
    (*un).byte[1] = (*un).byte[2];
    (*un).byte[2] = c1;
}

static short int endianSwap2int(short int a) {
	union byte2 un;
	un.numint = a;

	// swap
	char c1 = un.byte[0];
	un.byte[0] = un.byte[1];
	un.byte[1] = c1;

	return un.numint;
}

static int endianSwap4int(int a) {
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

static float endianSwap4float(float a) {
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

static int readFeature(lua_State *L)
{
	const char  *name = luaL_checkstring(L, 1);

	FILE *myfile;
	
	int nSample,sampPeriod;
	int sampSize, parmKind;
	
	myfile = fopen(name,"rt");
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
	
	//printf("%d\n%d\n%d\n%d\n",nSample,sampPeriod,sampSize,parmKind);
	
	float *Feature;
	int i,j;
	Feature = (float*)malloc(sizeof(float*)*nSample*sampSize);
	for (i=0;i<nSample * sampSize;++i) 
	{
			fread(&Feature[i],4,1,myfile);
			Feature[i] = endianSwap4float(Feature[i]);						
	}	
	//THStorage THFeat = THStorage_(newWithData)(Feature, nSample * sampSize);
	//THFeature = THTensor_(newWithStorage)(THFeat);

	fclose(myfile);
	lua_pushnumber(L, nSample);
	lua_pushnumber(L, sampSize);
	lua_pushnumber(L, Feature)
	
	return 2;
}

static const struct luaL_Reg lib[] = {
	{"readFeature", readFeature},
	{NULL, NULL}
};

int luaopen_readHTK(lua_State* L)
{
    luaL_register(L, "readHTK", lib);
    return 1;
}
