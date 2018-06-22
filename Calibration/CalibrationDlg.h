
// CalibrationDlg.h : header file
//

#pragma once


// CCalibrationDlg dialog
class CCalibrationDlg : public CDialogEx
{
// Construction
public:
	CCalibrationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CALIBRATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRightImage();
	afx_msg void OnBnClickedLeftImage();
	afx_msg void OnBnClickedGo();

private:
	CString FilePathNameLeft;
	CString FilePathNameRight;
	char* strFileLeft;
	char* strFileRight;

private:
	void CString2char(CString cstr, char* str, int len/*length of str*/);

};
