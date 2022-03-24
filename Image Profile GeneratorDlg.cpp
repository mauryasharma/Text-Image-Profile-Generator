// Image Profile GeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Image Profile Generator.h"
#include "Image Profile GeneratorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CImageProfileGeneratorDlg dialog




CImageProfileGeneratorDlg::CImageProfileGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageProfileGeneratorDlg::IDD, pParent)
	, m_Vertical(FALSE)
	, m_Horizontal(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ScaleTo = 1;
}

void CImageProfileGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENERATEPROFILES, m_BtnGenerate);
	DDX_Check(pDX, IDC_VERTICAL, m_Vertical);
	DDX_Check(pDX, IDC_HORIZONTAL, m_Horizontal);
	DDX_Control(pDX, IDC_PICTURE, m_MyImage);
}

BEGIN_MESSAGE_MAP(CImageProfileGeneratorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BROWSE, &CImageProfileGeneratorDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_VERTICAL, &CImageProfileGeneratorDlg::OnBnClickedVertical)
	ON_BN_CLICKED(IDC_HORIZONTAL, &CImageProfileGeneratorDlg::OnBnClickedHorizontal)
	ON_BN_CLICKED(IDC_GENERATEPROFILES, &CImageProfileGeneratorDlg::OnBnClickedGenerateprofiles)
END_MESSAGE_MAP()


// CImageProfileGeneratorDlg message handlers

BOOL CImageProfileGeneratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	m_pDC = m_MyImage.GetDC();
	WHITE = RGB(255,255,255);
	BLACK = RGB(0,0,0);
	RED = RGB(255,0,0);
	GREEN = RGB(0,255,0);
	BLUE = RGB(0,0,255);
	ImageLoaded = false;
	GetDlgItem(IDC_VERTICAL)->EnableWindow(false);
	GetDlgItem(IDC_HORIZONTAL)->EnableWindow(false);
	m_BtnGenerate.EnableWindow(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageProfileGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageProfileGeneratorDlg::OnPaint()
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
		CDialog::OnPaint();
	}
	//if (ImageLoaded)
	//	PutImage();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageProfileGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageProfileGeneratorDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here

	m_BtnGenerate.EnableWindow(false);
	m_Vertical = m_Horizontal = false;
	ImageLoaded = false;
	UpdateData(false);
	ResetDC();
	CString Infilename;
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	register int index = 0;
	
	hResult = m_Image.GetExporterFilterString(strFilter,aguidFileTypes);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format(_T("GetExporterFilter failed"));//:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		AfxMessageBox(fmt);
		return;
	}

	strFilter = _T("Supported Images|*.BMP;*.DIB;*.RLE;*.JPG;*.JPEG;*.JPE;*.JFIF;*.GIF;*.TIF;*.TIFF;*.PNG||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_FILEMUSTEXIST, strFilter);
	hResult = (int)dlg.DoModal();
	
	if(hResult != IDOK)
		return;
	else
	{
		HRESULT hResult;
		m_Image.Destroy();
		Infilename = dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
		hResult = m_Image.Load(Infilename);
		if (FAILED(hResult)) {
			CString fmt;
			fmt.Format(_T("Load image failed"));
			AfxMessageBox(fmt);
			return;
		}
		ImageLoaded = true;
		if (m_Image.GetWidth() > 600)
			ScaleTo = 2;
		else
			ScaleTo = 1;
		GetDlgItem(IDC_VERTICAL)->EnableWindow();
		GetDlgItem(IDC_HORIZONTAL)->EnableWindow();		
		m_Vertical = false;
		m_Horizontal = false;
		UpdateData(false);
		PutImage();
	}	
}

void CImageProfileGeneratorDlg::OnBnClickedVertical()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_Vertical || m_Horizontal)
		m_BtnGenerate.EnableWindow();
	else		
		m_BtnGenerate.EnableWindow(false);
}

void CImageProfileGeneratorDlg::OnBnClickedHorizontal()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_Horizontal || m_Vertical)
		m_BtnGenerate.EnableWindow();
	else		
		m_BtnGenerate.EnableWindow(false);
}

void CImageProfileGeneratorDlg::OnBnClickedGenerateprofiles()
{
	// TODO: Add your control notification handler code here
	int *HoriProfiles, *VertProfiles, x, y, w, h;
	int sx, ex, sy, ey, xx, yy;
	CRect ri;
	ri.left = ri.top = 0;
	w =  m_Image.GetWidth();
	h = m_Image.GetHeight();
	ri.right = w*2;
	ri.bottom = h*2;
	HoriProfiles = NULL;
	VertProfiles = NULL;
	if (m_Horizontal)
	{
		ex = ri.right;// - ri.left + 1;
		HoriProfiles = new int[h];		
		for (y = 0;y  < h;y++)
		{
			HoriProfiles[y] = 0;
			for (x = 0; x < w;x++)
			{
				color = m_Image.GetPixel(x,y);
				if (color != WHITE)
					HoriProfiles[y]++;
			}
			sx = ex - HoriProfiles[y];
			for (xx = sx;xx < ex;xx++)
				m_pDC->SetPixel(xx/ScaleTo,y/ScaleTo,RED);
		}
	}

	if (m_Vertical)
	{
		ey = ri.bottom;// - ri.top + 1;
		VertProfiles = new int[w];
		for (x = 0;x < w;x++)
		{
			VertProfiles[x] = 0;
			for (y = 0;y < h;y++)
			{
				color = m_Image.GetPixel(x,y);
				if (color != WHITE) //== BLACK)
					VertProfiles[x]++;
			}
			sy = ey - VertProfiles[x];
			for (yy = sy; yy < ey; yy++)
				m_pDC->SetPixel(x/ScaleTo,yy/ScaleTo,GREEN);
		}
	}
	if (HoriProfiles != NULL)
		delete [] HoriProfiles;
	if (VertProfiles != NULL)
		delete [] VertProfiles;
}

void CImageProfileGeneratorDlg::ResetDC()
{
	CRect ri;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(&ri);
	ri.right -= (ri.left+1);
	ri.bottom -= (ri.top+1);
	ri.left -= ri.left;
	ri.top -= ri.top;
	CBrush pBrush(WHITE);
	m_pDC->FillRect(ri, &pBrush);
}

void CImageProfileGeneratorDlg::PutImage()
{
	int x,y;
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = m_Image.GetWidth();
	rect.bottom = m_Image.GetHeight();
	CRect r, ri, r2;
	CPoint p2;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(&ri);
	ri.right -= (ri.left+1);
	ri.bottom -= (ri.top+1);
	ri.left -= ri.left;
	ri.top -= ri.top;

	ResetDC();
	
	p2.x = p2.y = 0;
	r.left = r.top = 0;
	r.right = rect.Width();
	r.bottom = rect.Height();

	r2.left = 0;
	r2.top = 0;
	
	for (y = rect.top;y < rect.bottom;y+=ScaleTo,p2.y++)
	{
		p2.x = 0;
		for (x = rect.left;x < rect.right;x+=ScaleTo,p2.x++)
		{
			if (p2.x < ri.right && p2.y < ri.bottom)
			{
				color = m_Image.GetPixel(x,y);
				m_pDC->SetPixel(p2.x,p2.y,color);
			}
		}
	}
	r2.right = p2.x;
	r2.bottom = p2.y;
}