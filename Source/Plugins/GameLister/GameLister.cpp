#include <stdio.h>
#include "GameLister.h"
#include "resource.h"

#define COPYSTRINGEX(out, in) {if(out){delete[]out;out=NULL;}if(in){out=new char[strlen(in)+1];if(out)memcpy(out, in, strlen(in)+1);}}
#define GETTEXTEX(out, ctlid){hwndTemp=GetDlgItem(hwnd, ctlid);dwLen=GetWindowTextLength(hwndTemp);if(dwLen<1){out=NULL;}else{out=new char[dwLen+1];GetWindowText(hwndTemp, out, dwLen+1);}}

static const char szModule[]="GameLister.dll";

static RECT g_rcWindow;

typedef enum tagDLGSCREENS{
	SCREEN_FINISHED=0, 
	SCREEN_TITLEDESC, 
	SCREEN_SYSREQ, 
	SCREEN_SHIPPING,
	SCREEN_TERMS
}DLGSCREENS;


typedef struct tagDGLRESULT
{
	int nResult;
	RECT rcWindow;
}DLGRESULT, *LPDLGRESULT;

// The function that acquires the lister. //
CLister * ObtainLister()
{
	return new CGameLister;
}

///////////////////////////////////////
// The callbacks for the dialog box. //
///////////////////////////////////////

// Callback for the terms. //
BOOL CALLBACK Terms(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CGameLister * lpLister=NULL;

	char * szShippingTerms=NULL;
	char * szFeedbackTerms=NULL;
	char * szWarrantyTerms=NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		
		MoveWindow(
			hwnd, 
			g_rcWindow.left, 
			g_rcWindow.top, 
			g_rcWindow.right-g_rcWindow.left, 
			g_rcWindow.bottom-g_rcWindow.top,
			TRUE);
		

		if(lpLister==NULL)
		{
			lpLister=(CGameLister*)lParam;
			SetDlgItemText(hwnd, IDC_TERMSHIP, "Shipping costs are actual PLUS handling, packaging, and materials fee.");
			SetDlgItemText(hwnd, IDC_TERMFEEDBACK, "I leave feedback upon feedback, that way I know if everything has gone smoothly and has been completed successfully as planned. Please keep in mind, if you do not let me know what has gone wrong I will not know and assume all is well, if you let me know before leaving a negative or a neutral feedback I will try to fix any and all problems that have occurred.");
			SetDlgItemText(hwnd, IDC_TERMWARRANTY, "This item is guaranteed not to be Dead-On-Arriaval (DOA).");
		}
		else
		{
			SetDlgItemText(hwnd, IDC_TERMSHIP, lpLister->m_szShippingTerms);
			SetDlgItemText(hwnd, IDC_TERMFEEDBACK, lpLister->m_szFeedbackTerms);
			SetDlgItemText(hwnd, IDC_TERMWARRANTY, lpLister->m_szWarrantyTerms);
		}
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_PREV:
			//fall through.
		case IDC_NEXT:
		{
			HWND hwndTemp=NULL;
			DWORD dwLen=0;

			GETTEXTEX(szShippingTerms, IDC_TERMSHIP);
			GETTEXTEX(szFeedbackTerms, IDC_TERMFEEDBACK);
			GETTEXTEX(szWarrantyTerms, IDC_TERMWARRANTY);

			lpLister->SetShippingTerms(szShippingTerms);
			lpLister->SetFeedbackTerms(szFeedbackTerms);
			lpLister->SetWarrantyTerms(szWarrantyTerms);

			SAFE_DELETE_ARRAY(szShippingTerms);
			SAFE_DELETE_ARRAY(szFeedbackTerms);
			SAFE_DELETE_ARRAY(szWarrantyTerms);
			GetWindowRect(hwnd, &g_rcWindow);
			EndDialog(hwnd, LOWORD(wParam));
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// Callback to acquire shipping and payment info. //
BOOL CALLBACK ShipPay(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CGameLister * lpLister=NULL;

	char * szShippingCost=NULL;
	char * szShippingInfo=NULL;
	char * szPaymentOptions=NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		MoveWindow(
			hwnd, 
			g_rcWindow.left, 
			g_rcWindow.top, 
			g_rcWindow.right-g_rcWindow.left, 
			g_rcWindow.bottom-g_rcWindow.top,
			TRUE);

		if(lpLister==NULL)
		{
			lpLister=(CGameLister*)lParam;
			SetDlgItemText(hwnd, IDC_COST, "ACTUAL plus handling, packaging, and materials fee");
			SetDlgItemText(hwnd, IDC_SHIPINFO, "E-Mail me or use ebay's calculator if a shipping cost estimate is desired. Any USPS shipping options may be added at the cost of the buyer. I ship worldwide. Item usually ships business day after payment is recieved.");
			SetDlgItemText(hwnd, IDC_PAYOPTION, "I accept Paypal and US Money Order/Cashiers Check only. Payment must be in United States Dollars (USD), and should be sent within 10 days of the auction end.");
		}
		else
		{
			SetDlgItemText(hwnd, IDC_COST, lpLister->m_szShippingCost);
			SetDlgItemText(hwnd, IDC_SHIPINFO, lpLister->m_szShippingInfo);
			SetDlgItemText(hwnd, IDC_PAYOPTION, lpLister->m_szPaymentOptions);
		}
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_PREV:
			//fall through.
		case IDC_NEXT:
		{
			HWND hwndTemp=NULL;
			DWORD dwLen=0;


			GETTEXTEX(szShippingCost, IDC_COST);
			GETTEXTEX(szShippingInfo, IDC_SHIPINFO);
			GETTEXTEX(szPaymentOptions, IDC_PAYOPTION);

			lpLister->SetShippingInfo(szShippingCost, szShippingInfo);
			lpLister->SetPaymentOptions(szPaymentOptions);

			SAFE_DELETE_ARRAY(szShippingCost);
			SAFE_DELETE_ARRAY(szShippingInfo);
			SAFE_DELETE_ARRAY(szPaymentOptions);
			GetWindowRect(hwnd, &g_rcWindow);
			EndDialog(hwnd, LOWORD(wParam));
			break;
		}
		default:
			break;
		}
		break;

	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


// Callback to acquire system requirements. //
BOOL CALLBACK SysRequire(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CGameLister * lpLister=NULL;

	char * szOS=NULL;
	char * szCPU=NULL;
	char * szRAM=NULL;
	char * szCD=NULL;
	char * szHDD=NULL;
	char * szVideo=NULL;
	char * szSound=NULL;
	char * szInput=NULL;
	char * szOther=NULL;
	char * szRecommended=NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		MoveWindow(
			hwnd, 
			g_rcWindow.left, 
			g_rcWindow.top, 
			g_rcWindow.right-g_rcWindow.left, 
			g_rcWindow.bottom-g_rcWindow.top,
			TRUE);

		if(lpLister==NULL)
		{
			lpLister=(CGameLister*)lParam;
		}
		else
		{
			//Attemp to replace the values.
			SetDlgItemText(hwnd, IDC_OS, lpLister->m_szOS);
			SetDlgItemText(hwnd, IDC_CPU, lpLister->m_szCPU);
			SetDlgItemText(hwnd, IDC_RAM, lpLister->m_szRAM);
			SetDlgItemText(hwnd, IDC_CD, lpLister->m_szCD);
			SetDlgItemText(hwnd, IDC_HDD, lpLister->m_szHDD);
			SetDlgItemText(hwnd, IDC_VIDEO, lpLister->m_szVideo);
			SetDlgItemText(hwnd, IDC_SOUND, lpLister->m_szSound);
			SetDlgItemText(hwnd, IDC_INPUT, lpLister->m_szInput);
			SetDlgItemText(hwnd, IDC_OTHER, lpLister->m_szOther);
			SetDlgItemText(hwnd, IDC_RECOMMENDED, lpLister->m_szRecommended);
		}
		break;
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_NEXT:
			//Fall through
		case IDC_PREV:
		{
			HWND hwndTemp=NULL;
			DWORD dwLen=0;

			GETTEXTEX(szOS, IDC_OS);
			GETTEXTEX(szCPU, IDC_CPU);
			GETTEXTEX(szRAM, IDC_RAM);
			GETTEXTEX(szCD, IDC_CD);
			GETTEXTEX(szHDD, IDC_HDD);
			GETTEXTEX(szVideo, IDC_VIDEO);
			GETTEXTEX(szSound, IDC_SOUND);
			GETTEXTEX(szInput, IDC_INPUT);
			GETTEXTEX(szOther, IDC_OTHER);
			GETTEXTEX(szRecommended, IDC_RECOMMENDED);

			lpLister->SetSystemRequirements(
				szOS,
				szCPU,
				szRAM,
				szCD,
				szHDD,
				szVideo,
				szSound,
				szInput,
				szOther,
				szRecommended);

			SAFE_DELETE_ARRAY(szOS);
			SAFE_DELETE_ARRAY(szCPU);
			SAFE_DELETE_ARRAY(szRAM);
			SAFE_DELETE_ARRAY(szCD);
			SAFE_DELETE_ARRAY(szHDD);
			SAFE_DELETE_ARRAY(szVideo);
			SAFE_DELETE_ARRAY(szSound);
			SAFE_DELETE_ARRAY(szInput);
			SAFE_DELETE_ARRAY(szOther);
			SAFE_DELETE_ARRAY(szRecommended);
			GetWindowRect(hwnd, &g_rcWindow);
			EndDialog(hwnd, LOWORD(wParam));
			break;
		}
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// Callback to acquire title, type, and descriptions. //
BOOL CALLBACK TitleDesc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CGameLister * lpLister=NULL;

	char * szCondition=NULL;

	switch(uMsg)
	{
	case WM_INITDIALOG:
	{
		MoveWindow(
			hwnd, 
			g_rcWindow.left, 
			g_rcWindow.top, 
			g_rcWindow.right-g_rcWindow.left, 
			g_rcWindow.bottom-g_rcWindow.top,
			TRUE);
		
		if(lpLister==NULL)
		{
			lpLister=(CGameLister*)lParam;

		}
		else
		{
			//Attempt to restore previous text.
			SetDlgItemText(hwnd, IDC_ITEMNAME, lpLister->m_szItemName);
			SetDlgItemText(hwnd, IDC_ITEMTYPE, lpLister->m_szItemType);
			SetDlgItemText(hwnd, IDC_DESC1, lpLister->m_szItemDesc1);
			SetDlgItemText(hwnd, IDC_DESC2, lpLister->m_szItemDesc2);
			SetDlgItemText(hwnd, IDC_CONDITION, lpLister->m_szItemCondition);
		}
		break;
	}
	case WM_COMMAND:
	{
		switch(LOWORD(wParam))
		{
		case IDC_NEXT:
		{
			char * szTemp=NULL, * szTemp2=NULL;
			DWORD dwLen=0;
			HWND hwndTemp=NULL;

			//Get and set the item name and type.
			GETTEXTEX(szTemp, IDC_ITEMNAME);
			GETTEXTEX(szTemp2, IDC_ITEMTYPE);
			lpLister->SetItemName(szTemp, szTemp2);
			SAFE_DELETE_ARRAY(szTemp);
			SAFE_DELETE_ARRAY(szTemp2);

			//Get and set the item descriptions.
			GETTEXTEX(szTemp, IDC_DESC1);
			GETTEXTEX(szTemp2, IDC_DESC2);
			lpLister->SetItemDesc(szTemp, szTemp2);
			SAFE_DELETE_ARRAY(szTemp);
			SAFE_DELETE_ARRAY(szTemp2);

			GETTEXTEX(szCondition, IDC_CONDITION);
			lpLister->SetItemCondition(szCondition);
			SAFE_DELETE_ARRAY(szCondition);

			//Finish up the dialog and specify that next was set.
			GetWindowRect(hwnd, &g_rcWindow);
			EndDialog(hwnd, LOWORD(wParam));
			break;
		}
		case IDC_PREV:
			EndDialog(hwnd, IDC_PREV);
			break;
		}
		return TRUE;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}



BOOL CGameLister::RunDialog(HWND hwndParent)
{
	DLGSCREENS nScreen=SCREEN_TITLEDESC;
	int nResult=0;

	SetRect(&g_rcWindow, 0, 0, 609, 409);

	do
	{
		nResult=0;

		switch(nScreen)
		{
		case SCREEN_TITLEDESC:
		{
			nResult=DialogBoxParam(
				GetModuleHandle(szModule),
				MAKEINTRESOURCE(IDD_TITLEDESC),
				hwndParent,
				TitleDesc,
				(LPARAM)this);
			if(nResult==IDC_NEXT)
			{
				nScreen=SCREEN_SYSREQ;
				break;
			}
			else
			{
				nScreen=SCREEN_FINISHED;
				return FALSE;
				break;
			}
			break;
		}
		case SCREEN_SYSREQ:
		{
			nResult=DialogBoxParam(
				GetModuleHandle(szModule),
				MAKEINTRESOURCE(IDD_SYSREQ),
				hwndParent,
				SysRequire,
				(LPARAM)this);

			switch(nResult)
			{
			case IDC_PREV:
				nScreen=SCREEN_TITLEDESC;
				break;
			case IDC_NEXT:
				nScreen=SCREEN_SHIPPING;
				break;
			default:
				nScreen=SCREEN_FINISHED;
				return FALSE;
				break;
			}
			break;
		}
		case SCREEN_SHIPPING:
		{
			nResult=DialogBoxParam(
				GetModuleHandle(szModule),
				MAKEINTRESOURCE(IDD_SHIPPAY),
				hwndParent,
				ShipPay,
				(LPARAM)this);
			switch(nResult)
			{
			case IDC_PREV:
				nScreen=SCREEN_SYSREQ;
				break;
			case IDC_NEXT:
				nScreen=SCREEN_TERMS;
				break;
			default:
				nScreen=SCREEN_FINISHED;
				return FALSE;
				break;
			}
			break;
		}
		case SCREEN_TERMS:
		{
			nResult=DialogBoxParam(
				GetModuleHandle(szModule),
				MAKEINTRESOURCE(IDD_TERMS),
				hwndParent,
				Terms,
				(LPARAM)this);
			switch(nResult)
			{
			case IDC_PREV:
				nScreen=SCREEN_SHIPPING;
				break;
			case IDC_NEXT:
				nScreen=SCREEN_FINISHED;
				break;
			default:
				nScreen=SCREEN_FINISHED;
				return FALSE;
				break;
			}
			break;
		}
		default:
			nScreen=SCREEN_FINISHED;
			break;
		}
	}while(nScreen != SCREEN_FINISHED);

	return TRUE;
}

BOOL CGameLister::SaveListing(char szFilename[MAX_PATH])
{
	MessageBox(NULL, "This Template does not support the save feature!", "GameLister", MB_OK|MB_ICONERROR);
	return TRUE;
}

BOOL CGameLister::LoadListing(char szFilename[MAX_PATH])
{
	return FALSE;
}

BOOL CGameLister::CreateListing(char szFilename[MAX_PATH])
{
	FILE * fout=NULL;

	fout=fopen(szFilename, "w");

	//Begin the table and print out the item name and type.
	fprintf(fout, "<table border=5 cellspacing=0>");
	fprintf(fout, "<tr><th bgcolor=0xFFB0B0FF><font color=0x00000000>");
	if(m_szItemName)
		fprintf(fout, "<h1>%s</h1>", m_szItemName);
	if(m_szItemType)
		fprintf(fout, "<h2>%s</h2>", m_szItemType);
	fprintf(fout, "</font></th></tr>");

	//Print the item description.
	fprintf(fout, "<tr><td bgcolor=white>");
	if(m_szItemDesc1)
		fprintf(fout, "<font color=black>%s</font>", m_szItemDesc1);
	if(m_szItemDesc2)
		fprintf(fout, "<p><font color=darkred>%s</font>", m_szItemDesc2);
	fprintf(fout, "</td></tr>");

	//Print the condition of the item.
	if(m_szItemCondition)
	{
		fprintf(fout, "<tr><th bgcolor=orange><font color=black><big>Item Condition</big></font></th></tr>");
		fprintf(fout, "<tr><td>%s</td></tr>", m_szItemCondition);
	}
	//Print the system requirements.
	fprintf(fout, "<tr><th bgcolor=orange>System Requirements</th></tr>");
	fprintf(fout, "<tr><td><ul>");
	if(m_szOS)
		fprintf(fout, "<li><b>OS:</b> %s.</li>", m_szOS);
	if(m_szCPU)
		fprintf(fout, "<li><b>CPU:</b> %s.</li>", m_szCPU);
	if(m_szRAM)
		fprintf(fout, "<li><b>RAM:</b> %s.</li>", m_szRAM);
	if(m_szCD)
		fprintf(fout, "<li><b>CD/DVD-ROM Speed:</b> %s.</li>", m_szCD);
	if(m_szHDD)
		fprintf(fout, "<li><b>Drive Space:</b> %s.</li>", m_szHDD);
	if(m_szVideo)
		fprintf(fout, "<li><b>Video:</b> %s.</li>", m_szVideo);
	if(m_szSound)
		fprintf(fout, "<li><b>Sound:</b> %s.</li>", m_szSound);
	if(m_szInput)
		fprintf(fout, "<li><b>Input:</b> %s.</li>", m_szInput);
	if(m_szOther)
		fprintf(fout, "<li><b>Other:</b> %s.</li>", m_szOther);
	fprintf(fout, "</ul></td></tr>");

	if(m_szRecommended)
	{
		fprintf(fout, "<tr><th bgcolor=orange>Recommended</th></tr>");
		fprintf(fout, "<tr><td>%s</td></tr>", m_szRecommended);
	}

	//Close the table
	fprintf(fout, "</table><p>");

	//Start the shipping and payment table.
	fprintf(fout, "<table border=5 cellspacing=0>");
	//Start the shipping information.
	fprintf(fout, "<tr><th bgcolor=black><font color=white>Shipping Information</font></th></tr><tr><td>");
	if(m_szShippingCost)
		fprintf(fout, "Shipping is<big><b>%s</b></big>.", m_szShippingCost);
	if(m_szShippingInfo)
		fprintf(fout, " %s", m_szShippingInfo);

	fprintf(fout, "</td></tr>");
	//Start the payment information.
	if(m_szPaymentOptions)
		fprintf(fout, "<tr><th bgcolor=black><font color=white>Payment Options</th></tr><tr><td>%s</td></tr>", m_szPaymentOptions);

	//Close the table.
	fprintf(fout, "</table><p>");

	//Print the last table terms and conditions.
	fprintf(fout, "<table border=5 cellspacing=0><tr><th bgcolor=darkblue><font color=white>Terms and Conditions</font></th></tr>");
	fprintf(fout, "<tr><td>");
	if(m_szShippingTerms)
		fprintf(fout, "<b>Shipping:</b> %s<p>", m_szShippingTerms);
	if(m_szFeedbackTerms)
		fprintf(fout, "<b>Feedback:</b> %s<p>", m_szFeedbackTerms);
	if(m_szWarrantyTerms)
		fprintf(fout, "<b>Warranty:</b> %s<p>", m_szWarrantyTerms);

	fprintf(fout, "</td></tr></table>");

	//We're done.  Close the file.

	fclose(fout);
	return TRUE;
}

BOOL CGameLister::SetItemName(LPSTR szItemName, LPSTR szItemType)
{
	COPYSTRINGEX(m_szItemName, szItemName);
	COPYSTRINGEX(m_szItemType, szItemType);

	return TRUE;
}

BOOL CGameLister::SetItemDesc(LPSTR szItemDesc1, LPSTR szItemDesc2)
{
	COPYSTRINGEX(m_szItemDesc1, szItemDesc1);
	COPYSTRINGEX(m_szItemDesc2, szItemDesc2);

	return TRUE;
}

BOOL CGameLister::SetItemCondition(LPSTR szItemCondition)
{
	COPYSTRINGEX(m_szItemCondition, szItemCondition);

	return TRUE;
}

BOOL CGameLister::SetSystemRequirements(
	LPSTR szOS, 
	LPSTR szCPU,
	LPSTR szRAM, 
	LPSTR szCD, 
	LPSTR szHDD,
	LPSTR szVideo,
	LPSTR szSound,
	LPSTR szInput,
	LPSTR szOther,
	LPSTR szRecommended)
{
	COPYSTRINGEX(m_szOS, szOS);
	COPYSTRINGEX(m_szCPU, szCPU);
	COPYSTRINGEX(m_szRAM, szRAM);
	COPYSTRINGEX(m_szCD, szCD);
	COPYSTRINGEX(m_szHDD, szHDD);
	COPYSTRINGEX(m_szVideo, szVideo);
	COPYSTRINGEX(m_szSound, szSound);
	COPYSTRINGEX(m_szInput, szInput);
	COPYSTRINGEX(m_szOther, szOther);
	COPYSTRINGEX(m_szRecommended, szRecommended);

	return TRUE;
}

BOOL CGameLister::SetShippingInfo(LPSTR szShippingCost, LPSTR szShippingInfo)
{
	COPYSTRINGEX(m_szShippingCost, szShippingCost);
	COPYSTRINGEX(m_szShippingInfo, szShippingInfo);

	return TRUE;
}

BOOL CGameLister::SetPaymentOptions(LPSTR szPaymentOptions)
{
	COPYSTRINGEX(m_szPaymentOptions, szPaymentOptions);

	return TRUE;
}

BOOL CGameLister::SetShippingTerms(LPSTR szShippingTerms)
{
	COPYSTRINGEX(m_szShippingTerms, szShippingTerms);

	return TRUE;
}

BOOL CGameLister::SetFeedbackTerms(LPSTR szFeedbackTerms)
{
	COPYSTRINGEX(m_szFeedbackTerms, szFeedbackTerms);

	return TRUE;
}

BOOL CGameLister::SetWarrantyTerms(LPSTR szWarrantyTerms)
{
	COPYSTRINGEX(m_szWarrantyTerms, szWarrantyTerms);

	return TRUE;
}