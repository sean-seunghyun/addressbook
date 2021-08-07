//PullingOutForm.c
#include "PullingOutForm.h"
#include "BusinesscardBook.h"
#include "resource.h"
#include "Index.h"
#include"IndexBook.h"
#include <CommCtrl.h>

BOOL CALLBACK PullingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = PullingOutForm_OnInitDialog(hWnd, wParam, lParam); break;
	case WM_CLOSE:
		ret = PullingOutForm_OnClose(hWnd, wParam, lParam); break;
	default:ret = FALSE; break;
	}
	return ret;
}

BOOL PullingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	HWND hWndForBusinesscardBook;
	Businesscard businesscard;
	Businesscard* index;
	Businesscard* businesscardLink;
	Index* indexLink;
	Index(*keys);
	Long count;
	Long subscript;
	Long i;
	TVINSERTSTRUCT tvins = { 0, };
	TVITEM tvi = { 0,};
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TCHAR companyName[8];
	IndexBook *indexBook;

	//2. ������ �����찡 ������ ��
	//2.1 ����ö �����츦 ã�´�.
	hWndForBusinesscardBook = FindWindow("#32770", "����ö");
	businesscardBook = (BusinesscardBook*)GetWindowLong(hWndForBusinesscardBook, GWL_USERDATA);

	//2.2 ����ö �������� ����ö���� ������.
	index = businesscardBook->current;
	businesscard = PullOut(businesscardBook, index);

	//2.3. ����ö �������� ����ö���� ������.
	indexBook = (IndexBook*)GetProp(hWndForBusinesscardBook, "indexBook");
	IndexBook_PullOut(indexBook, index, businesscard.company.name);

	//2.4. ����ö �������� ����ö���� ����Ʈ ����⸦ �Ѵ�.
	MakeList(indexBook, &keys, &count);
	//2.5. Ʈ������ �׸���� ��� �����.
	SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)NULL);
	//2.6. ����ö �������� Ʈ���信 �׸��� �߰��Ѵ�.
	
	tvi.mask = TVIF_TEXT | TVIF_HANDLE;
	tvi.pszText = "ȸ���";
	tvins.hParent = NULL;
	tvins.item = tvi;
	
		treeRoot = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		i = 0;
		while (i < count) {
			tvi.pszText = keys[i].companyName;
			tvi.cchTextMax = 8;
			tvins.item = tvi;
			tvins.hParent = treeRoot;
			treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);


			subscript = 0;
			while (subscript < keys[i].length) {
				businesscardLink = Index_GetAt(keys + i, subscript);
				tvi.pszText = businesscardLink->personal.name;
				tvi.cchTextMax = 11;
				tvins.hParent = treeParent;
				tvins.item = tvi;
				SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
				subscript++;
			}
			i++;
		}
	
	//keys �Ҵ�����
	if (keys != NULL) {
		free(keys);
	}
	//2.6 ������ �������� ������Ʈ�ѿ� �߰��Ѵ�.
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.personal.name);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.personal.position);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.personal.cellphoneNumber);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.personal.emailAddress);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.company.name);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.company.address);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.company.telephoneNumber);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.company.faxNumber);
	SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscard.company.url);
	//2.7 ������ ����������
	if (businesscardBook->current != NULL) {
		//2.7.1 ���� ��ġ�� ������ ����ö �������� ���Կ� ����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->personal.name);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->personal.position);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->personal.emailAddress);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->company.name);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->company.address);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->company.telephoneNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->company.faxNumber);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardBook->current->company.url);
		//2.7.2 ����ö �������� ����ö���� ã�´�. 
		businesscardLink = businesscardBook->current;
		indexLink = IndexBook_Find(indexBook, businesscardLink->company.name);
		//2.7.3. ����ö �������� ���ο��� ã�´�. 
		subscript = Index_Find(indexLink, businesscardLink);
		//2.7.4. Ʈ���信�� ã�´�.
		treeRoot = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);

		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, businesscardLink->company.name) != 0) {
			treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.7.5 ����ö �������� Ʈ���信�� �����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);

	}
	else {
		//2.5. ������ ������ //2.5.1 ������ ����ö �������� ���Կ� ����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		//2.6. Ʈ���� ��� �����.

		//2.7 ��Ʈ�� �߰��Ѵ�.
	}
	
	return TRUE;
}

BOOL PullingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	HWND hWndForBusinesscardBook;
	Businesscard businesscard;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Businesscard* businesscardLink;
	Long subscript;
	Index(*keys);
	Long count;
	TVINSERTSTRUCT tvins = { 0, };
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	Long i;
	//2. �ݱ��ư�� Ŭ������ ��
	//2.1 �޼����ڽ��� ����.

	int response = MessageBox(
		hWnd,
		(LPCTSTR)"������ �ٽ� ��������ðڽ��ϱ�?",
		(LPCTSTR)"�˸�",
		MB_YESNOCANCEL);
	//2.2 ����ư�� Ŭ������ ��
	if (response == IDYES) {
		//2.2.1 ������ �������� ������Ʈ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME),	WM_GETTEXT, (WPARAM)11, (LPARAM)businesscard.personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_GETTEXT, (WPARAM)12, (LPARAM)businesscard.personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_GETTEXT, (WPARAM)32, (LPARAM)businesscard.personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_GETTEXT, (WPARAM)16, (LPARAM)businesscard.company.url);
		//businesscard.next = NULL;
		//2.2.2. ����ö �����츦 ã�´�
		hWndForBusinesscardBook = FindWindow("#32770", "����ö");
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWndForBusinesscardBook, GWL_USERDATA);
		//2.2.3. ����ö �������� ���Կ� �����ִ´�.
		index = PutIn(businesscardBook, businesscard);
		//2.2.4. ����ö �������� ����ö�� �����ִ´�.
		indexBook = (IndexBook*)GetProp(hWndForBusinesscardBook, "indexBook");
		indexLink = IndexBook_PutIn(indexBook, index);
		//2.2.5. ����Ʈ ����⸦ �Ѵ�.
		MakeList(indexBook, &keys, &count);
		//2.2.6. ����ö �������� Ʈ���� �׸��� ��� �����.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)NULL);
		//2.2.7. ����ö �������� Ʈ���信 �׸��� �߰��Ѵ�.
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
			treeParent = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

			subscript = 0;
			while (subscript < keys[i].length) {
				businesscardLink = Index_GetAt(keys + i, subscript);
				tvi.pszText = businesscardLink->personal.name;
				tvi.cchTextMax = 11;
				tvins.hParent = treeParent;
				tvins.item = tvi;
				SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
				subscript++;
			}
			i++;
		}

		if (keys != NULL) {
			free(keys);
		}
		//2.2.8. ����ö �������� ������Ʈ�ѿ� ������ġ�� ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);

		//2.2.9. ����ö �������� ����ö���� ã�´�.
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.2.10. ����ö �������� ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.2.11. Ʈ���信�� ã�´�.
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
		treeChild = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.2.12. ����ö �������� Ʈ���信�� �����Ѵ�.
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWndForBusinesscardBook, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);
		//2.13 �����ֱ� �����츦 �ݴ´�.
		
		//2.2.6. ������ �����츦 �ݴ´�.
		//2.2.6. ������ �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
		
	}
	//2.3. �ƴϿ� ��ư�� Ŭ������ ��
	else if (response == IDNO) {
		//2.3.1 ������ �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
		
	}
	
	return TRUE;
}
