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

	char * m_szItemDesc;

	char * m_szItemCondition;

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

			m_szItemDesc=NULL;

			m_szItemCondition=NULL;

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

			SAFE_DELETE_ARRAY(m_szItemDesc);

			SAFE_DELETE_ARRAY(m_szItemCondition);

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
	BOOL SetItemDesc(LPSTR szItemDesc);
	BOOL SetItemCondition(LPSTR szItemCondition);

	BOOL SetShippingInfo(LPSTR szShippingCost, LPSTR szShippingInfo);
	BOOL SetPaymentOptions(LPSTR szPaymentOptions);

	BOOL SetShippingTerms(LPSTR szShippingTerms);
	BOOL SetFeedbackTerms(LPSTR szFeedbackTerms);
	BOOL SetWarrantyTerms(LPSTR szWarrantyTerms);

	//Friend Functions.
	friend INT_PTR CALLBACK TitleDesc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend INT_PTR CALLBACK ShipPay(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend INT_PTR CALLBACK Terms(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif //__GAMELISTER_H__