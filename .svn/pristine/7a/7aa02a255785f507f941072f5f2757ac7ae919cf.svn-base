
// ReplotSpaceManDlg.h : header file
//

#pragma once


// CReplotSpaceManDlg dialog
class CReplotSpaceManDlg : public CDialogEx
{
// Construction
public:
	CReplotSpaceManDlg(CWnd* pParent = nullptr);	// standard constructor
	~CReplotSpaceManDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPLOTSPACEMAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	UINT m_PlotCheckTimer = 0;
	int m_iNewPlotSize = 0;
	CVSListBox cOldPlotLocationsList;

// Implementation
protected:
	HICON m_hIcon;
	void SetupFolders();
	void UpdateFolders();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonGo();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedMfcbuttonWait();
	afx_msg void OnDestroy();
};
