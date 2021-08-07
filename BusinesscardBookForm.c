//BusinesscardBookForm.c
#include"IndexBook.h"
#include"Index.h"
#include"BusinesscardBookForm.h"
#include"BusinesscardBook.h"
#include "PuttingInForm.h"
#include "FindingForm.h"
#include "PullingOutForm.h"
#include "resource.h"
#include <CommCtrl.h>



int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd) {
	int response;

	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_BUSINESSCARDBOOKFORM), NULL, BusinesscardBookFormProc);
	return response;
}


BOOL CALLBACK BusinesscardBookFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = BusinesscardBookForm_OnInitDialog(hWnd, wParam, lParam); break;

	case WM_COMMAND:
			ret = BusinesscardBookForm_OnCommand(hWnd,wParam,lParam); break;

	case WM_NOTIFY:
		ret = BusinesscardBookForm_OnNotify(hWnd, wParam, lParam); break;

	case WM_CLOSE:
		ret = BusinesscardBookForm_OnClose(hWnd, wParam, lParam); break;

	default:ret = FALSE; break;
	}
	return ret;
}

BOOL BusinesscardBookForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Long count;
	TVITEM tvi = { 0, };
	TVINSERTSTRUCT tvins = { 0, };
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	Businesscard* businesscardLink;
	Long i;
	Index(*keys);
	Long subscript;
	Businesscard* previous = NULL;
	TVITEM tvItem = { 0, };
	TCHAR companyName[8];
	//1. �����찡 ������ ��
	//1.1 ����ö�� �����.


	businesscardBook = (BusinesscardBook*)malloc(sizeof(BusinesscardBook));
	BusinesscardBook_Create(businesscardBook);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)businesscardBook);


	//1.2. ����ö�� �����.
	indexBook = (IndexBook*)malloc(sizeof(IndexBook));
	IndexBook_Create(indexBook);
	SetProp(hWnd, (LPCTSTR)"indexBook", (HANDLE)indexBook);

	//1.3 ����ö�� �����Ѵ�.
	count = Load(businesscardBook);

	//1.4 ����ö�� count��ŭ �����ִ´�.
	if (count > 0) {

		businesscardLink = BusinesscardBook_First(businesscardBook);
		while (businesscardLink != previous) {
			IndexBook_PutIn(indexBook, businesscardLink);
			previous = businesscardLink;
			businesscardLink = BusinesscardBook_Next(businesscardBook);
		}
	}

	//1.5 ����ö�� ����Ʈ����⸦ �Ѵ�.
	MakeList(indexBook, &keys, &count);
	//1.6 Ʈ���信 �׸��� �����Ѵ�.
	tvi.mask = TVIF_TEXT|TVIF_HANDLE;
	tvi.pszText = (LPTSTR)"ȸ���";
	tvins.hParent = NULL;
	tvins.item = tvi;
	treeRoot = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

	i = 0;
	while (i < count) {
		tvi.pszText = (LPTSTR)keys[i].companyName;
		tvi.cchTextMax = 8;
		tvins.item = tvi;
		tvins.hParent = treeRoot;
		treeParent = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		
		subscript = 0;
		while (subscript < keys[i].length) {
			businesscardLink = Index_GetAt(keys + i, subscript);
			tvi.pszText = (LPTSTR)businesscardLink->personal.name;
			tvi.cchTextMax = 11;
			tvins.item = tvi;
			tvins.hParent = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
			subscript++;
		}
		i++;
	}
	if(count>0){
		//1.7 ����ö���� ù��°�� �̵��Ѵ�.
		index = BusinesscardBook_First(businesscardBook);
		//1.8 ����ö �����쿡 ������ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		
		//1.9 ����ö���� ã�´�.
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//1.10 ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//1.11 Ʈ���信�� ã�´�.
		treeParent = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.hItem = treeParent;
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//1.12 Ʈ���信 �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);
	}
	if (keys != NULL) {
		free(keys);
	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {

	case IDC_BUTTON_FIRST:
		ret = BusinesscardBookForm_OnFirstButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_PREVIOUS:
		ret = BusinesscardBookForm_OnPreviousButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_NEXT:
		ret = BusinesscardBookForm_OnNextButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_LAST:
		ret = BusinesscardBookForm_OnLastButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_PUTTINGIN:
		ret = BusinesscardBookForm_OnPuttingInButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_FINDING:
		ret = BusinesscardBookForm_OnFindingButtonClicked(hWnd, wParam, lParam); break;

	case IDC_BUTTON_PULLINGOUT:
		ret = BusinesscardBookForm_OnPullingOutButtonClicked(hWnd, wParam, lParam); break;


	case IDC_BUTTON_ARRANGING:
		ret = BusinesscardBookForm_OnArrangingButtonClicked(hWnd, wParam, lParam); break;

	default: ret = FALSE;
	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Long subscript;
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	Long i;
	//2. �̵��ϱ��� ù��° ������ Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ����ö���� ù��°�� �̵��Ѵ�.
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinesscardBook_First(businesscardBook);
		//2.2. ������Ʈ�ѿ� �ش������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//2.3. ����ö���� ã�´�.
		indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.4. ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.5. Ʈ���信�� ã�´�. 
		treeRoot = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		treeParent= SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.6 Ʈ���信 �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);

	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Long subscript;
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	Long i;
	//2. �̵��ϱ��� ���� ������ Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ����ö���� ù��°�� �̵��Ѵ�.
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinesscardBook_Previous(businesscardBook);
		//2.2. ������Ʈ�ѿ� �ش������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//2.3. ����ö���� ã�´�.
		indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.4. ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.5. Ʈ���信�� ã�´�. 
		treeRoot = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.6 Ʈ���信 �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);

	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Long subscript;
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	Long i;
	//2. �̵��ϱ��� ���� ������ Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ����ö���� ù��°�� �̵��Ѵ�.
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinesscardBook_Next(businesscardBook);
		//2.2. ������Ʈ�ѿ� �ش������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//2.3. ����ö���� ã�´�.
		indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.4. ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.5. Ʈ���信�� ã�´�. 
		treeRoot = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.6 Ʈ���信 �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);

	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index* indexLink;
	Long subscript;
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	Long i;
	//2. �̵��ϱ��� ���� ������ Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 ����ö���� ù��°�� �̵��Ѵ�.
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinesscardBook_Last(businesscardBook);
		//2.2. ������Ʈ�ѿ� �ش������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)index->company.url);
		//2.3. ����ö���� ã�´�.
		indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
		indexLink = IndexBook_Find(indexBook, index->company.name);
		//2.4. ���ο��� ã�´�.
		subscript = Index_Find(indexLink, index);
		//2.5. Ʈ���信�� ã�´�. 
		treeRoot = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, indexLink->companyName) != 0) {
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
		treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		i = 0;
		while (i < subscript) {
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
			i++;
		}
		//2.6 Ʈ���信 �׸��� �����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);

	}
	return TRUE;
}


BOOL BusinesscardBookForm_OnPuttingInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//6. �����ֱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 �����ֱ� ������ �����츦 ����.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGINFORM), NULL, PuttingInFormProc);
	}
	return TRUE;
}

BOOL BusinesscardBookForm_OnFindingButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//7. ã���ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 ã�� ������ �����츦 ����.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL, FindingFormProc);
	}
	return TRUE;
}


BOOL BusinesscardBookForm_OnPullingOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//8. ������ ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 ������ ������ �����츦 ����.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PULLINGOUTFORM), NULL, PullingOutFormProc);
	}
	return TRUE;
}


BOOL BusinesscardBookForm_OnArrangingButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;
	Businesscard* index;
	IndexBook *indexBook;
	Index (*keys);
	Index* indexLink;
	Businesscard* businesscardLink;
	Long count;
	Long i;
	Long subscript;
	TVINSERTSTRUCT tvins = { 0, };
	TVITEM tvi = { 0, };
	HTREEITEM treeRoot;
	HTREEITEM treeParent;
	HTREEITEM treeChild;
	TCHAR companyName[8];

	//9. �����ϱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1 ����ö���� �����Ѵ�.
		indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
		IndexBook_Arrange(indexBook);
		//9.2 ����ö���� ����Ʈ ����⸦ �Ѵ�.
		MakeList(indexBook, &keys, &count);
		//9.3 Ʈ������ �׸���� ��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)NULL);
		//9.4 Ʈ���信 �׸��� �����.
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.pszText = "ȸ���";
		tvins.hParent = NULL;
		tvins.item = tvi;
		treeRoot = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
		i = 0;
		while (i < count) {
			tvi.pszText = keys[i].companyName;
			tvi.cchTextMax = 8;
			tvins.hParent = treeRoot;
			tvins.item = tvi;
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
			subscript = 0;
			while (subscript < keys[i].length) {
				businesscardLink = Index_GetAt(keys + i, subscript);
				tvi.pszText = businesscardLink->personal.name;
				tvi.cchTextMax = 11;
				tvins.hParent = treeParent;
				tvins.item = tvi;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, (WPARAM)0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
				subscript++;
			}
			i++;
		}
		//3.5. ����ö���� ù��°�� �̵��Ѵ�.
		businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
		businesscardLink = BusinesscardBook_First(businesscardBook);
		//3.6 ù��° ������ ������Ʈ�ѿ� ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.position);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.cellphoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.url);
		
		
		//3.7. ����ö���� ã�´�.
		indexLink = IndexBook_Find(indexBook, businesscardLink->company.name);
		//3.8. ���ο��� ã�´�.
		subscript = Index_Find(indexLink, businesscardLink);
		//3.9 Ʈ���信�� ã�´�.
		treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeRoot);
		tvi.pszText = companyName;
		tvi.cchTextMax = 8;
		tvi.hItem = treeParent;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		while (strcmp(companyName, businesscardLink->company.name) != 0) {
			treeParent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeParent);
			tvi.hItem = treeParent;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
		}
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)treeParent);
		
			i = 0;
			while (i < subscript) {
				treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
				i++;
			}
			//3.10. Ʈ���信�� �����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeRoot);
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)treeParent);
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)treeChild);
		

		if (keys != NULL) {
			free(keys);
		}
			
	}
	return TRUE;
}



BOOL BusinesscardBookForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_TREE_INDEXES:
		ret = BusinesscardBookForm_OnTreeView(hWnd, wParam, lParam); break;
	default:
		ret = FALSE;  break;
	}
	return ret;
}


BOOL BusinesscardBookForm_OnTreeView(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TVITEM tvi = { 0, };
	TCHAR companyName[8];
	TCHAR personalName[11];
	TCHAR otherPersonalName[11];
	HTREEITEM itemForCompanyName;
	HTREEITEM itemForPersonalName;
	IndexBook *indexBook;
	Index* indexLink;
	HTREEITEM treeChild;
	Long subscript = 0;
	Businesscard* businesscardLink;
	BusinesscardBook *businesscardBook;
	HWND hWndForBusinesscardBook;
	HTREEITEM treeItem;

	//2. Ʈ���� ��Ʈ���� ������ ����Ŭ������ ��
		//2.1 �ش��׸��� ��ġ�� �д´�.
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		itemForPersonalName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)NULL);
		//2.2. �׸��� �д´�.
		tvi.mask = TVIF_TEXT | TVIF_HANDLE;
		tvi.hItem = itemForPersonalName;
		tvi.pszText = personalName;
		tvi.cchTextMax = 11;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);

		//����Ŭ�� ���� ����
		treeItem = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)itemForPersonalName);

		if (treeItem == NULL) {
			//2.3 �׸��� �θ� �д´�.
			itemForCompanyName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)itemForPersonalName);
			tvi.hItem = itemForCompanyName;
			tvi.pszText = companyName;
			tvi.cchTextMax = 8;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
			//2.4 ����ö���� ã�´�.
			indexBook = (IndexBook*)GetProp(hWnd, "indexBook");
			indexLink = IndexBook_Find(indexBook, companyName);
			//2.5 Ʈ���信�� ���� �׸��� ������ ����.
			treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)itemForCompanyName);
			tvi.hItem = treeChild;
			tvi.pszText = otherPersonalName;
			tvi.cchTextMax = 11;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
			while (strcmp(otherPersonalName, personalName) != 0) {
				subscript++;
				treeChild = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)treeChild);
				tvi.hItem = treeChild;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, (WPARAM)0, (LPARAM)&tvi);
			}
			//2.6 ���ο��� �������⸦ �Ѵ�.
			businesscardLink = Index_GetAt(indexLink, subscript);
			//2.7. ������Ʈ�ѿ� ������ ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.name);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_POSITION), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.position);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_CELLPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.cellphoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_PERSONAL_EMAILADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->personal.emailAddress);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_NAME), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.name);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_ADDRESS), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_TELEPHONENUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_FAXNUMBER), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_STATIC_COMPANY_URL), WM_SETTEXT, (WPARAM)0, (LPARAM)businesscardLink->company.url);
			//2.8. ����ö�� ������ġ�� �����Ѵ�.		
			businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
			businesscardBook->current = businesscardLink;
		}
	}
	return TRUE;
}


BOOL BusinesscardBookForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinesscardBook *businesscardBook;

	//10. �ݱ��ư�� Ŭ������ ��
	//10.1 �ּҷ��� ���ش�.
	businesscardBook = (BusinesscardBook*)GetWindowLong(hWnd, GWL_USERDATA);
	if (businesscardBook != NULL) {
		Save(businesscardBook);
		BusinesscardBook_Destroy(businesscardBook);
		free(businesscardBook);
	}
	//10.2 ����ö ������ �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}