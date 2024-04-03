
// ReplotSpaceManDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ReplotSpaceMan.h"
#include "ReplotSpaceManDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PLOTCHECKTIMER 1

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonAddToList();
	afx_msg void OnBnClickedQuit();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


CReplotSpaceManDlg::CReplotSpaceManDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REPLOTSPACEMAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CReplotSpaceManDlg::~CReplotSpaceManDlg()
{

}


void CReplotSpaceManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCVSLISTBOX_OLDFOLDERLIST, cOldPlotLocationsList);
}

BEGIN_MESSAGE_MAP(CReplotSpaceManDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_GO, &CReplotSpaceManDlg::OnBnClickedMfcbuttonGo)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_QUIT, &CReplotSpaceManDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_MFCBUTTON_WAIT, &CReplotSpaceManDlg::OnBnClickedMfcbuttonWait)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CReplotSpaceManDlg message handlers

BOOL CReplotSpaceManDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(_T("Re-plot Space Manager"));

	//Resize window to last Position/State
	UINT nBytes = 0;
	BYTE* pData = 0;
	theApp.GetProfileBinary(_T("Settings"), _T("WindowPos"), &pData, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		if (((WINDOWPLACEMENT*)pData)->showCmd == SW_SHOWMINIMIZED)
			((WINDOWPLACEMENT*)pData)->showCmd = SW_SHOWNORMAL;
		AfxGetMainWnd()->SetWindowPlacement((WINDOWPLACEMENT*)pData);
	}

	if (pData && nBytes)
	{
		delete[] pData;
		pData = NULL;
	}


	//Load saved list
	CString csFolderLocation, csIniSettingEntryName;
	int iEntry=0;

	do {
		csIniSettingEntryName.Format(_T("Old Plot %d"), iEntry++);
		csFolderLocation = theApp.GetProfileStringW(_T("OldPlotFoldersList"), csIniSettingEntryName, _T(""));
		if ( !csFolderLocation.IsEmpty() )
			cOldPlotLocationsList.AddItem(csFolderLocation);
	} while ( !csFolderLocation.IsEmpty() );


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReplotSpaceManDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReplotSpaceManDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReplotSpaceManDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReplotSpaceManDlg::OnBnClickedMfcbuttonGo()
{
	GetDlgItem(IDC_MFCBUTTON_GO)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBUTTON_WAIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MFCVSLISTBOX_OLDFOLDERLIST)->EnableWindow(FALSE);

	int iTickPeriod = 0;

	iTickPeriod = theApp.GetProfileIntW(_T("Config"), _T("Seconds Between Checks"), 5);
	m_iNewPlotSize = theApp.GetProfileIntW(_T("Config"), _T("New Plot Size (GB)"), 75);

	if ( m_PlotCheckTimer > 0 )
		KillTimer(PLOTCHECKTIMER);

	m_PlotCheckTimer = (UINT) SetTimer(PLOTCHECKTIMER, iTickPeriod * 1000, NULL); // Set Processing timer

	SetupFolders();
	UpdateFolders();
}

void CReplotSpaceManDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == PLOTCHECKTIMER)
	{
		UpdateFolders();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CReplotSpaceManDlg::OnBnClickedQuit()
{
	if (m_PlotCheckTimer > 0)
		KillTimer(PLOTCHECKTIMER);

	SetWindowText(_T("Exiting..."));
	SleepEx(1000, TRUE);

	EndDialog(0);
}


void CReplotSpaceManDlg::OnBnClickedMfcbuttonWait()
{
	GetDlgItem(IDC_MFCBUTTON_GO)->EnableWindow(TRUE);
	GetDlgItem(IDC_MFCBUTTON_WAIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCVSLISTBOX_OLDFOLDERLIST)->EnableWindow(TRUE);

	if (m_PlotCheckTimer > 0)
		KillTimer(PLOTCHECKTIMER);

	SetWindowText(_T("Waiting...  Press GO! to reload settings and resume"));
}

void CReplotSpaceManDlg::SetupFolders()
{
	//reset or hide as necessary
	int iCount = cOldPlotLocationsList.GetCount();

	if (iCount > 15)
	{
		CString csMessage;
		csMessage.Format(_T("Replot SpaceMan only support up to 15 plot locations\r\nEntry '%s' and any following it will be ignored."), cOldPlotLocationsList.GetItemText(15));
		AfxMessageBox(csMessage);
	}

	if (iCount > 0)
	{
		GetDlgItem(IDC_PROGRESS0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH0)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH0)->ShowWindow(SW_HIDE);
	}

	if (iCount > 1)
	{
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH1)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH1)->ShowWindow(SW_HIDE);
	}

	if (iCount > 2)
	{
		GetDlgItem(IDC_PROGRESS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH2)->ShowWindow(SW_HIDE);
	}

	if (iCount > 3)
	{
		GetDlgItem(IDC_PROGRESS3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH3)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH3)->ShowWindow(SW_HIDE);
	}

	if (iCount > 4)
	{
		GetDlgItem(IDC_PROGRESS4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH4)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH4)->ShowWindow(SW_HIDE);
	}

	if (iCount > 5)
	{
		GetDlgItem(IDC_PROGRESS5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH5)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH5)->ShowWindow(SW_HIDE);
	}

	if (iCount > 6)
	{
		GetDlgItem(IDC_PROGRESS6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH6)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH6)->ShowWindow(SW_HIDE);
	}

	if (iCount > 7)
	{
		GetDlgItem(IDC_PROGRESS7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH7)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH7)->ShowWindow(SW_HIDE);
	}

	if (iCount > 8)
	{
		GetDlgItem(IDC_PROGRESS8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH8)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH8)->ShowWindow(SW_HIDE);
	}

	if (iCount > 9)
	{
		GetDlgItem(IDC_PROGRESS9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH9)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH9)->ShowWindow(SW_HIDE);
	}

	if (iCount > 10)
	{
		GetDlgItem(IDC_PROGRESS10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH10)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH10)->ShowWindow(SW_HIDE);
	}

	if (iCount > 11)
	{
		GetDlgItem(IDC_PROGRESS11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH11)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH11)->ShowWindow(SW_HIDE);
	}

	if (iCount > 12)
	{
		GetDlgItem(IDC_PROGRESS12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH12)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH12)->ShowWindow(SW_HIDE);
	}

	if (iCount > 13)
	{
		GetDlgItem(IDC_PROGRESS13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH13)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH13)->ShowWindow(SW_HIDE);
	}

	if (iCount > 14)
	{
		GetDlgItem(IDC_PROGRESS14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPATH14)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_PROGRESS14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPATH14)->ShowWindow(SW_HIDE);
	}
}

void CReplotSpaceManDlg::UpdateFolders()
{
	CString csFolderPath, csStatus;
	int iPos=0, iOldPlotCount=0;
	double dFreeSpace = 0.00, dTotalSpace = 0.00, dTotalOldPlotsSize = 0.00;
	BOOL bWorking=FALSE;
	BOOL bSpaceCanBeMade = FALSE;
	CFileFind finder;
	CFile cFile;
	CFileStatus cFileStatus;

	for (int iCount = 0; iCount < cOldPlotLocationsList.GetCount(); iCount++)
	{
		//Get Space and update progress bar to indicate how full each drive is
		csFolderPath = cOldPlotLocationsList.GetItemText(iCount);

		csFolderPath += _T('\\');

		ULARGE_INTEGER lTotalDiskSpace, lFreeDiskSpace, lFreeDiskSpaceAvailable;
		GetDiskFreeSpaceExW(csFolderPath, &lFreeDiskSpaceAvailable, &lTotalDiskSpace, &lFreeDiskSpace);

		dFreeSpace = (double) lFreeDiskSpace.QuadPart/1073741824;
		dTotalSpace = (double) lTotalDiskSpace.QuadPart/1073741824;

		if ( dFreeSpace < m_iNewPlotSize )
			csStatus.Format(_T("%0.2lf GB free. No old plots left to cull, you can remove this drive (%s) from the list"), dFreeSpace, cOldPlotLocationsList.GetItemText(iCount));
		else
			csStatus.Format(_T("%0.2lf GB free. No old plots left to cull. Next time, just leave it to me, OK???"), dFreeSpace);

		iPos = (int) (1000 - ((dFreeSpace / dTotalSpace) * 1000));

		//Check to see if we have any plots to delete
		if (!finder.FindFile(csFolderPath + _T("*")))
		{
			csStatus.Format(_T("Entry %d: '%s' is not a valid path"), iCount + 1, cOldPlotLocationsList.GetItemText(iCount));
			iPos = 0;
		}

		bSpaceCanBeMade = FALSE;
		dTotalOldPlotsSize = 0.00;
		iOldPlotCount = 0;
		bWorking = finder.FindFile(csFolderPath+_T("*.plot"));

		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files
			if (!finder.IsDots())
			{
				if (dFreeSpace < m_iNewPlotSize)
				{
					_tunlink(finder.GetFilePath());
					
					GetDiskFreeSpaceExW(csFolderPath, NULL, &lTotalDiskSpace, &lFreeDiskSpace);
					dFreeSpace = (double)lFreeDiskSpace.QuadPart / 1073741824;
					dTotalSpace = (double)lTotalDiskSpace.QuadPart / 1073741824;

					iPos = (int) (dFreeSpace / dTotalSpace) * 1000;

					csStatus.Format(_T("%0.2lf GB free. I made space :)"), dFreeSpace);
				}
				else
				{
					iOldPlotCount++;
					cFile.GetStatus(finder.GetFilePath(), cFileStatus);
					dTotalOldPlotsSize += cFileStatus.m_size;
					bSpaceCanBeMade = TRUE;
				}
			}
		}

		if (bSpaceCanBeMade)
			csStatus.Format(_T("%0.1lf GB free. I can delete up to %d old plots and %0.1lf GB space, when needed"), dFreeSpace, iOldPlotCount, dTotalOldPlotsSize / 1073741824);

		if (iCount == 0)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS0))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS0))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC0, csStatus);
			SetDlgItemText(IDC_SPATH0, csFolderPath);
		}
		else if (iCount == 1)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC1, csStatus);
			SetDlgItemText(IDC_SPATH1, csFolderPath);
		}
		else if (iCount == 2)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS2))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS2))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC2, csStatus);
			SetDlgItemText(IDC_SPATH2, csFolderPath);
		}
		else if (iCount == 3)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS3))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS3))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC3, csStatus);
			SetDlgItemText(IDC_SPATH3, csFolderPath);
		}
		else if (iCount == 4)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS4))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS4))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC4, csStatus);
			SetDlgItemText(IDC_SPATH4, csFolderPath);
		}
		else if (iCount == 5)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS5))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS5))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC5, csStatus);
			SetDlgItemText(IDC_SPATH5, csFolderPath);
		}
		else if (iCount == 6)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS6))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS6))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC6, csStatus);
			SetDlgItemText(IDC_SPATH6, csFolderPath);
		}
		else if (iCount == 7)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS7))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS7))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC7, csStatus);
			SetDlgItemText(IDC_SPATH7, csFolderPath);
		}
		else if (iCount == 8)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS8))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS8))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC8, csStatus);
			SetDlgItemText(IDC_SPATH8, csFolderPath);
		}
		else if (iCount == 9)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS9))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS9))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC9, csStatus);
			SetDlgItemText(IDC_SPATH9, csFolderPath);
		}
		else if (iCount == 10)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS10))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS10))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC10, csStatus);
			SetDlgItemText(IDC_SPATH10, csFolderPath);
		}
		else if (iCount == 11)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS11))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS11))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC11, csStatus);
			SetDlgItemText(IDC_SPATH11, csFolderPath);
		}
		else if (iCount == 12)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS12))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS12))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC12, csStatus);
			SetDlgItemText(IDC_SPATH12, csFolderPath);
		}
		else if (iCount == 13)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS13))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS13))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC13, csStatus);
			SetDlgItemText(IDC_SPATH13, csFolderPath);
		}
		else if (iCount == 14)
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS14))->SetRange(0, 1000);
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS14))->SetPos(iPos);
			SetDlgItemText(IDC_STATIC14, csStatus);
			SetDlgItemText(IDC_SPATH14, csFolderPath);
		}
	}
}

void CReplotSpaceManDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	CString csIniSettingEntryName, csFolderLocation;
	int iEntry = 0;

	//Empty all existing ini entries
	do {
		csIniSettingEntryName.Format(_T("Old Plot %d"), iEntry++);
		csFolderLocation = theApp.GetProfileStringW(_T("OldPlotFoldersList"), csIniSettingEntryName, _T(""));
		if (!csFolderLocation.IsEmpty())
			theApp.WriteProfileStringW(_T("OldPlotFoldersList"), csIniSettingEntryName, _T(""));
	} while (!csFolderLocation.IsEmpty());

	for (int iCount = 0; iCount < cOldPlotLocationsList.GetCount(); iCount++)
	{
		csFolderLocation = cOldPlotLocationsList.GetItemText(iCount);
		csIniSettingEntryName.Format(_T("Old Plot %d"), iCount);
		theApp.WriteProfileStringW(_T("OldPlotFoldersList"), csIniSettingEntryName, csFolderLocation);
	}

	WINDOWPLACEMENT wp;
	AfxGetMainWnd()->GetWindowPlacement(&wp);
	theApp.WriteProfileBinary(_T("Settings"), _T("WindowPos"), (BYTE*)&wp, sizeof(wp));
}
