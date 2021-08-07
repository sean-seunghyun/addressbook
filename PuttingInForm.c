//PuttingInForm.c
#include"PuttingInForm.h"
#include"BusinesscardBook.h"
#include "resource.h"
#include"Index.h"
#include"IndexBook.h"
#include<stdio.h>
#include <CommCtrl.h>


#pragma warning(disable: 4996)

BOOL CALLBACK PuttingInFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = PuttingInForm_OnInitDialog(hWnd, wParam, lParam); break;

	case WM_COMMAND:
		ret = PuttingInForm_OnCommand(hWnd, wParam, lParam); break;

	case WM_CLOSE:
		ret = PuttingInForm_OnClose(hWnd, wParam, lParam); break;
	default: ret = FALSE; break;
	}
	return ret;
}

BOOL PuttingInForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_PUTTINGIN:
		ret = PuttingInForm_OnPuttingInButtonClicked(hWnd, wParam, lParam); break;

	case IDC_EDIT_COMPANY_NAME:
		ret = PuttingInForm_OnCompanyNameKillFocus(hWnd, wParam, lParam); break;

	default:ret = FALSE; break;
	}
	return ret;
}

BOOL PuttingInForm_OnPuttingInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	
	Businesscard businesscard;
	BusinesscardBook *businesscardBook;
	HWND hWndForBusinesscardBook;
	Businesscard* index;
	char emailAddressId[16];
	char emailAddressDomain[16];
	IndexBook *indexBook;
	Index* indexLink;
	Index(*keys);
	Long count;
	Long i;
	Long subscript;
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TVITEM tvi = { 0, };
	TVINSERTSTRUCT tvins = { 0, };
	Businesscard* businesscardLink;
	TCHAR companyName[8];

	//2. �����ֱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam)==BN_CLICKED) {
		//2.1 ������Ʈ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONAL_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)businesscard.personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX_PERSONAL_POSITION), WM_GETTEXT, (WPARAM)9, (LPARAM)businesscard.personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONAL_CELLPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businesscard.personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONAL_EMAILADDRESS_ID), WM_GETTEXT, (WPARAM)16, (LPARAM)emailAddressId);
		SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX_PERSONAL_EMAILADDRESS_DOMAIN), WM_GETTEXT, (WPARAM)16, (LPARAM)emailAddressDomain);
		sprintf(businesscard.personal.emailAddress, "%s@%s", emailAddressId, emailAddressDomain);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_NAME), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_ADDRESS), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businesscard.company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_FAXNUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businesscard.company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_URL), WM_GETTEXT, (WPARAM)64, (LPARAM)businesscard.company.url);
		//businesscard.next = NULL;

		//2.2. ����ö �����츦 ã�´�.
		hWndForBusinesscardBook = FindWindow("#32770", "����ö");
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWndForBusinesscardBook, GWL_USERDATA);
		//2.3 ����ö �������� ����ö���� �����ִ´�.
		index = PutIn(businesscardBook, businesscard);
		//2.4. ����ö �������� ����ö���� �����ִ´�.
		indexBook = (IndexBook*)GetProp(hWndForBusinesscardBook, "indexBook");
		indexLink = IndexBook_PutIn(indexBook, index);
		//2.5 ����ö �������� ���Կ� ������ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//2.6. ����ö �������� ����ö���� ����Ʈ ����⸦ �Ѵ�.
		MakeList(indexBook, &keys, &count);
		//2.7. Ʈ������ �׸��� ��� �����.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)NULL);
		//2.8 ����ö �������� Ʈ���信 �׸��� �߰��Ѵ�.
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = "ȸ���";
		tvins.hParent = NULL;
		tvins.item = tvi;
		treeRoot = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		i = 0;
		while (i < count) {
			tvi.pszText = keys[i].companyName;
			tvi.cchTextMax = 8;
			tvins.hParent = treeRoot;
			tvins.item = tvi;
			treeParent= SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		
			subscript = 0;
			while (subscript < keys[i].length) {
				businesscardLink = Index_GetAt(keys + i, subscript);
				tvi.pszText = businesscardLink->personal.name;
				tvi.cchTextMax = 11;
				tvins.hParent = treeParent;
				tvins.item = tvi;
				SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0,(LPARAM)(LPTVINSERTSTRUCT)&tvins);
				subscript++;
			}
			i++;
		}
		//2.9. ����ö �������� ����ö���� ã�´�.
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.10. ����ö �������� ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.11. ����ö �������� Ʈ���信�� ã�´�.
		treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild= SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.12. ����ö �������� Ʈ���信�� �����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);
		//2.13 �����ֱ� �����츦 �ݴ´�.
		if (keys != NULL) {
			free(keys);
		}
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL PuttingInForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char positions[][9] = { "����","���","����","�븮","����","����","����","�̻�","��","����","����","�����Է�"};
	char domains[][16] = {"naver.com","daum.net","google.com","�����Է�"};
	Long i;
	//1. �����ֱ� �����찡 �����Ǿ��� ��
	//1.1. ��å �޺��ڽ��� ����Ʈ�� �����.
	i = 0;
	while (i < sizeof(positions) / sizeof(positions[0])) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX_PERSONAL_POSITION), CB_ADDSTRING, (WPARAM)0, (LPARAM)positions[i]);
		i++;
	}
	//1.2. ������ �޺��ڽ��� ����Ʈ�� �����.
	i = 0;
	while (i < sizeof(domains) / sizeof(domains[0])) {
		SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX_PERSONAL_EMAILADDRESS_DOMAIN), CB_ADDSTRING, (WPARAM)0, (LPARAM)domains[i]);
		i++;
	}
	return TRUE;
}


BOOL PuttingInForm_OnCompanyNameKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char companyName[11];
	HWND hWndForBusinesscardBook;
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;

	//3. ��ȣ��Ŀ���� �Ҿ��� ��
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//3.1 ��ȣ��Ʈ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_NAME), WM_GETTEXT, (WPARAM)16, (LPARAM)companyName);
		//3.2 ����ö �����츦 ã�´�.
		hWndForBusinesscardBook = FindWindow("#32770", "����ö");
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWndForBusinesscardBook, GWL_USERDATA);
		//3.3 ����ö �������� ����ö���� ã�´�.
		indexBook = (IndexBook*)GetProp(hWndForBusinesscardBook, "indexBook");
		indexLink = IndexBook_Find(indexBook, companyName);
		//3.4 ã������
		if (indexLink != NULL) {
			//3.4.1 ����ö �������� ���ο��� �迭�� ù��° ���Ը�ũ�� �������� �Ѥ�.
			index = Index_GetAt(indexLink, 0);
			//3.4.1 �ּ�, ��ȭ��ȣ, �ѽ���ȣ, url�� ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		}
	}
	return TRUE;
}


BOOL PuttingInForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. �ݱ��ư�� Ŭ������ ��
	//4.1. �����ֱ� �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}