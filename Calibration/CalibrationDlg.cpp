
// CalibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Calibration.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"

#include "arcsoft_calibration.h"
#include "string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalibrationDlg dialog



CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK2, &CCalibrationDlg::OnBnClickedRightImage)
	ON_BN_CLICKED(IDOK, &CCalibrationDlg::OnBnClickedLeftImage)
	ON_BN_CLICKED(IDOK3, &CCalibrationDlg::OnBnClickedGo)
	ON_BN_CLICKED(IDOK4, &CCalibrationDlg::OnBnClickedOpenCalibration)
END_MESSAGE_MAP()


void get_file_sufix(const char* path, char* sufix)
{
	int pointIndex = -1;
	for (int i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '.')
		{
			pointIndex = i;
			break;
		}
	}
	if (pointIndex >= 0)
	{
		strcpy(sufix, path + pointIndex + 1);
	}
}

unsigned long get_file_size(const char *path)
{
	unsigned long filesize = -1;
	FILE *fp;
	fp = fopen(path, "r");
	if (fp == NULL)
		return filesize;
	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);
	fclose(fp);
	return filesize;
}

int substr(char* dst, const char* src, int start, int len)
{
	//start为开始复制时的位置
	int src_index, dst_index;
	dst_index = 0;
	while (len>0 && start >= 0)
	{
		for (src_index = 0; src_index<start&&src[src_index] != '\0'; src_index++);
		while (len>0 && src[src_index] != '\0')/*为什么src[src_index]结束时还继续复制呢？*/
		{
			dst[dst_index] = src[src_index];
			src_index++;
			dst_index++;
			len--;
		}

	}
	dst[dst_index] = '\0';
	return dst_index;
}

void guessFileWH(const char* strFile, int* wh) {
	// 4_arcsoft6144x3456_output_317_ISO602.yuyv = 6144x3456
	// ax8_IMG_20150101_192924_0_2976x3968.yuyv = 2976x3968
	wh[0] = -1;	//width
	wh[1] = -1;	//height
	if (strFile == 0) {
		return;
	}

	const char* index_X = strrchr(strFile, 'x');
	if (index_X == NULL) {
		index_X = strrchr(strFile, 'X');
	}
	if (index_X != NULL) {
		int iStart = 0;
		for (iStart = index_X - strFile - 1; iStart >= 0; iStart--) {
			if (strFile[iStart]>'9' || strFile[iStart]<'0') {
				break;
			}
		}
		if (iStart >= 0) {
			char sWidth[32] = { 0 };
			substr(sWidth, strFile, iStart + 1, index_X - strFile - iStart);
			wh[0] = atoi(sWidth);
		}

		int iEnd = 0;
		for (iEnd = index_X - strFile + 1; iEnd<strlen(strFile); iEnd++) {
			if (strFile[iEnd]>'9' || strFile[iEnd]<'0')
			{
				break;
			}
		}
		if (iEnd<strlen(strFile)) {
			char sHeight[32] = { 0 };
			substr(sHeight, strFile, index_X - strFile + 1, iEnd - (index_X - strFile));
			wh[1] = atoi(sHeight);
		}
	}
}

// CCalibrationDlg message handlers

BOOL CCalibrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalibrationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCalibrationDlg::OnPaint()
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
HCURSOR CCalibrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCalibrationDlg::OnBnClickedLeftImage()
{
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if (dlg.DoModal() == IDOK)
	{
		FilePathNameLeft = dlg.GetPathName();
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_LEFT);
		pStatic->SetWindowText(FilePathNameLeft);
		strFileLeft = (char*)malloc(FilePathNameLeft.GetLength() + 1);
		CString2char(FilePathNameLeft, strFileLeft, FilePathNameLeft.GetLength() + 1);
	}
}

void CCalibrationDlg::OnBnClickedRightImage()
{
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if (dlg.DoModal() == IDOK)
	{
		FilePathNameRight = dlg.GetPathName();
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_RIGHT);
		pStatic->SetWindowText(FilePathNameRight);
		strFileRight = (char*)malloc(FilePathNameRight.GetLength() + 1);
		CString2char(FilePathNameRight, strFileRight, FilePathNameRight.GetLength() + 1);
	}
}

void CCalibrationDlg::OnBnClickedGo()
{
	char * device = "deviceID";
	MR_ENGINE m_hDPADEngine = 0;
	MUInt8* leftnv21 = 0;
	MUInt8* rightnv21 = 0;

	if (strFileLeft == 0 || strFileRight == 0)
	{
		MessageBox(L"no file selected");
		goto ERRRETURN;
	}
	char strFileSufix[8];
	get_file_sufix(strFileLeft, strFileSufix);
	if (strcmp(strFileSufix, "nv21") != 0 && strcmp(strFileSufix, "NV21") != 0)
	{
		MessageBox(L"not support left image, must nv21");
		goto ERRRETURN;
	}
	get_file_sufix(strFileRight, strFileSufix);
	if (strcmp(strFileSufix, "nv21") != 0 && strcmp(strFileSufix, "NV21") != 0)
	{
		MessageBox(L"not support right image, must nv21");
		goto ERRRETURN;
	}

	int wh[2] = { 0 };
	guessFileWH(strFileLeft, wh);
	int mainWidth = wh[0];
	int mainHeight = wh[1];
	int mainLength = get_file_size(strFileLeft);

	guessFileWH(strFileRight, wh);
	int auxWidth = wh[0];
	int auxHeight = wh[1];
	int auxLength = get_file_size(strFileRight);

	leftnv21 = (MUInt8*)malloc(mainLength);
	rightnv21 = (MUInt8*)malloc(auxLength);

	FILE* leftfp = fopen(strFileLeft, "rb");
	FILE* rightfp = fopen(strFileRight, "rb");
	fread(leftnv21, 1, mainLength, leftfp);
	fread(rightnv21, 1, auxLength, rightfp);
	fclose(leftfp);
	fclose(rightfp);

	ASVLOFFSCREEN leftoffscreen;
	ASVLOFFSCREEN rightoffscreen;

	memset(&leftoffscreen, 0, sizeof(ASVLOFFSCREEN));
	memset(&rightoffscreen, 0, sizeof(ASVLOFFSCREEN));

	leftoffscreen.i32Width = mainWidth;
	leftoffscreen.i32Height = mainHeight;
	leftoffscreen.u32PixelArrayFormat = ASVL_PAF_NV21;
	leftoffscreen.pi32Pitch[0] = mainWidth;
	leftoffscreen.pi32Pitch[1] = mainWidth;
	leftoffscreen.ppu8Plane[0] = leftnv21;
	leftoffscreen.ppu8Plane[1] = leftoffscreen.ppu8Plane[0] + leftoffscreen.pi32Pitch[0] * leftoffscreen.i32Height;

	rightoffscreen.i32Width = auxWidth;
	rightoffscreen.i32Height = auxHeight;
	rightoffscreen.u32PixelArrayFormat = ASVL_PAF_NV21;
	rightoffscreen.pi32Pitch[0] = auxWidth;
	rightoffscreen.pi32Pitch[1] = auxWidth;
	rightoffscreen.ppu8Plane[0] = rightnv21;
	rightoffscreen.ppu8Plane[1] = rightoffscreen.ppu8Plane[0] + rightoffscreen.pi32Pitch[0] * rightoffscreen.i32Height;

	m_hDPADEngine = MR_CreateEngine();
	MByte byteResult[2048] = { 0 };
	MUInt32 nLength = 2048;

	ArcCalibrationParameters configureP;
	memset(&configureP, 0, sizeof(ArcCalibrationParameters));
	configureP.blockSize = 15;	//15x15cm
	configureP.chessboardWidth = 19;//请按照实际chart规格填写，棋盘格横向方块个数
	configureP.chessboardHeight = 14;
	configureP.numberOfChessboards = 4;
	configureP.leftImg = &leftoffscreen;
	configureP.rightImg = &rightoffscreen;
	long ret = MR_ModuleCalibration(m_hDPADEngine, &configureP, byteResult, &nLength, device);
	if (ret == MOK)
	{
#if 0
		long ret1 = -1;
		MDouble rotationx = 0;
		ret1 = MR_GetCalibrationResultParam(m_hDPADEngine, RET_PARAMS_ROTATION_X, &rotationx);
		if (MOK == ret1)
		{
			printf("rotationx ----- %0.5f\n", rotationx);
		}
#endif
		static int ii = 0;
		int index = strrchr(strFileLeft, '\\') - strFileLeft;	//index of '\\', based on 0
		if (index <= 0)
		{
			//not found
			MessageBox(L"source file path error");
			goto ERRRETURN;
		}
		else
		{
			char folder[64];
			substr(folder, strFileLeft, 0, index+1/*length*/);
			char binPath[64];
			sprintf(binPath, "%scalibration_%d.bin", folder, ii);
			FILE* fp = fopen(binPath, "wb");
			fwrite(byteResult, 1, sizeof(byteResult), fp);
			fclose(fp);
			MessageBox(L"ok");
		}
	}
	else
	{
		char error[32];
		sprintf(error, "error = %d", ret);
		MessageBox((LPCTSTR)error);
	}

ERRRETURN:
	if (m_hDPADEngine)
	{
		MR_DestroyEngine(m_hDPADEngine);
		m_hDPADEngine = 0;
	}

	if (leftnv21 != 0)
	{
		free(leftnv21);
		leftnv21 = 0;
	}
	if (rightnv21 != 0)
	{
		free(rightnv21);
		rightnv21 = 0;
	}
	if (strFileLeft != 0)
	{
		free(strFileLeft);
		strFileLeft = 0;
	}
	if (strFileRight != 0)
	{
		free(strFileRight);
		strFileRight = 0;
	}
}
void CCalibrationDlg::CString2char(CString cstr, char* str, int len/*length of str*/)
{
	if (len < cstr.GetLength())
	{
		return;
	}
	char *pBuff = (char*)cstr.GetBuffer(cstr.GetLength());
	int i = 0;
	for (; i < cstr.GetLength(); i++)
	{
		str[i] = pBuff[i * 2];
	}
	str[i] = '\0';
}

void CCalibrationDlg::OnBnClickedOpenCalibration()
{
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();
		char* strFileCalibration = (char*)malloc(str.GetLength() + 1);
		CString2char(str, strFileCalibration, str.GetLength() + 1);
		int len = get_file_size(strFileCalibration);
		if (len != 2048)
		{
			MessageBox(L"not calibration file");
			goto ERRRETURN;
		}

		char* byteCalibration = (char*)malloc(len);
		FILE* fp = fopen(strFileCalibration, "rb");
		fread(byteCalibration, 1, len, fp);
		fclose(fp);

		MRESULT res = MOK;
		MHandle hReader = MNull;
		res = ArcCalibDataReaderInit((MByte*)byteCalibration, len, &hReader);

		if (hReader != MNull)
		{
			double rx = 9999.9, ry = 9999.9, rz = 9999.9, shiftX = 9999.9, shiftY = 9999.9;

			res = ArcCalibDataReaderGetParam(hReader, RET_PARAMS_ROTATION_X, &rx);
			res = ArcCalibDataReaderGetParam(hReader, RET_PARAMS_ROTATION_Y, &ry);
			res = ArcCalibDataReaderGetParam(hReader, RET_PARAMS_ROTATION_Z, &rz);
			res = ArcCalibDataReaderGetParam(hReader, RET_PARAMS_SHIFT_X, &shiftX);
			res = ArcCalibDataReaderGetParam(hReader, RET_PARAMS_SHIFT_Y, &shiftY);
			CStatic *pStatic = (CStatic*)GetDlgItem(IDC_INFO);
			CString strInfo;
			strInfo.Format(_T("ROTATION_X:%lf ROTATION_Y:%lf ROTATION_Z:%lf SHIFT_X:%lf SHIFT_Y:%lf"), rx, ry, rz, shiftX, shiftY);
			pStatic->SetWindowText(strInfo);
			ArcCalibDataReaderUninit(hReader);
			hReader = MNull;
		}

	ERRRETURN:
		if (strFileCalibration != 0)
		{
			free(strFileCalibration);
			strFileCalibration = 0;
		}
	}
}
