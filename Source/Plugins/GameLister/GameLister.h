#ifndef __GAMELISTER_H__
#define __GAMELISTER_H__

#include "Lister.h"

class CGameLister;

CLister * ObtainLister();

class CGameLister: public CLister
{
private:
	char * m_szItemName;
	char * m_szItemType;

	char * m_szItemDesc1;
	char * m_szItemDesc2;

	char * m_szItemCondition;

	char * m_szOS;
	char * m_szCPU;
	char * m_szRAM; 
	char * m_szCD;
	char * m_szHDD;
	char * m_szVideo;
	char * m_szSound;
	char * m_szInput;
	char * m_szOther;
	char * m_szRecommended;

	char * m_szShippingCost;
	char * m_szShippingInfo;

	char * m_szPaymentOptions;

	char * m_szShippingTerms;
	char * m_szFeedbackTerms;
	char * m_szWarrantyTerms;

public:
	CGameLister()
		{
			m_szItemName=NULL;
			m_szItemType=NULL;

			m_szItemDesc1=NULL;
			m_szItemDesc2=NULL;

			m_szItemCondition=NULL;

			m_szOS=NULL;
			m_szCPU=NULL;
			m_szRAM=NULL; 
			m_szCD=NULL;
			m_szHDD=NULL;
			m_szVideo=NULL;
			m_szSound=NULL;
			m_szInput=NULL;
			m_szOther=NULL;
			m_szRecommended=NULL;

			m_szShippingCost=NULL;
			m_szShippingInfo=NULL;
			
			m_szPaymentOptions=NULL;

			m_szShippingTerms=NULL;
			m_szFeedbackTerms=NULL;
			m_szWarrantyTerms=NULL;
		}
	virtual ~CGameLister()
		{
			SAFE_DELETE_ARRAY(m_szItemName);
			SAFE_DELETE_ARRAY(m_szItemType);

			SAFE_DELETE_ARRAY(m_szItemDesc1);
			SAFE_DELETE_ARRAY(m_szItemDesc2);

			SAFE_DELETE_ARRAY(m_szItemCondition);

			SAFE_DELETE_ARRAY(m_szOS);
			SAFE_DELETE_ARRAY(m_szCPU);
			SAFE_DELETE_ARRAY(m_szRAM); 
			SAFE_DELETE_ARRAY(m_szCD);
			SAFE_DELETE_ARRAY(m_szHDD);
			SAFE_DELETE_ARRAY(m_szVideo);
			SAFE_DELETE_ARRAY(m_szSound);
			SAFE_DELETE_ARRAY(m_szInput);
			SAFE_DELETE_ARRAY(m_szOther);
			SAFE_DELETE_ARRAY(m_szRecommended);

			SAFE_DELETE_ARRAY(m_szShippingCost);
			SAFE_DELETE_ARRAY(m_szShippingInfo);
			SAFE_DELETE_ARRAY(m_szPaymentOptions);

			SAFE_DELETE_ARRAY(m_szShippingTerms);
			SAFE_DELETE_ARRAY(m_szFeedbackTerms);
			SAFE_DELETE_ARRAY(m_szWarrantyTerms);
		}

	virtual BOOL CreateListing(char szFilename[MAX_PATH]);
	virtual BOOL RunDialog(HWND hwndParent);
	virtual BOOL SaveListing(char szFilename[MAX_PATH]);
	virtual BOOL LoadListing(char szFilename[MAX_PATH]);

	BOOL SetItemName(LPSTR szItemName, LPSTR szItemType);
	BOOL SetItemDesc(LPSTR szItemDesc1, LPSTR szItemDesc2);
	BOOL SetItemCondition(LPSTR szItemCondition);
	BOOL SetSystemRequirements(
		LPSTR szOS, 
		LPSTR szCPU,
		LPSTR szRAM, 
		LPSTR szCD, 
		LPSTR szHDD,
		LPSTR szVideo,
		LPSTR szSound,
		LPSTR szInput,
		LPSTR szOther,
		LPSTR szRecommended);
	BOOL SetShippingInfo(LPSTR szShippingCost, LPSTR szShippingInfo);
	BOOL SetPaymentOptions(LPSTR szPaymentOptions);

	BOOL SetShippingTerms(LPSTR szShippingTerms);
	BOOL SetFeedbackTerms(LPSTR szFeedbackTerms);
	BOOL SetWarrantyTerms(LPSTR szWarrantyTerms);

	//Friend Functions.
	friend BOOL CALLBACK TitleDesc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend BOOL CALLBACK SysRequire(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend BOOL CALLBACK ShipPay(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend BOOL CALLBACK Terms(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif //__GAMELISTER_H__