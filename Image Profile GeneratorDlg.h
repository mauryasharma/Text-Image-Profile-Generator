// Image Profile GeneratorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "AtlImage.h"


// CImageProfileGeneratorDlg dialog
class CImageProfileGeneratorDlg : public CDialog
{

	int ScaleTo;
	CImage m_Image;
	CDC *m_pDC;
	COLORREF WHITE, BLACK, RED, BLUE, GREEN, color;
	bool ImageLoaded;
// Construction
public:
	CImageProfileGeneratorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IMAGEPROFILEGENERATOR_DIALOG };

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
	afx_msg void OnBnClickedBrowse();
	CButton m_BtnGenerate;
	BOOL m_Vertical;
	BOOL m_Horizontal;
	afx_msg void OnBnClickedVertical();
	afx_msg void OnBnClickedHorizontal();
	afx_msg void OnBnClickedGenerateprofiles();

	void ResetDC();
	void PutImage();
	CStatic m_MyImage;
};
