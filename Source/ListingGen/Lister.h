#ifndef __LISTER_H__
#define __LISTER_H__

#include <windows.h>

#define SAFE_DELETE_ARRAY(p) {if(p){delete[]p;p=NULL;}}

class CLister;

typedef CLister* (*FPOBTAINLISTER)();

class CLister
{
private:

public:
	CLister(){}
	virtual ~CLister(){}

	virtual BOOL RunDialog(HWND hwndParent)=0;
	virtual BOOL CreateListing(char szFilename[MAX_PATH])=0;
	virtual BOOL SaveListing(char szFilename[MAX_PATH])=0;
	virtual BOOL LoadListing(char szFilename[MAX_PATH])=0;
};

#endif //__LISTER_H__