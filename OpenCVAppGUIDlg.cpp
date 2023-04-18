
// OpenCVAppGUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OpenCVAppGUI.h"
#include "OpenCVAppGUIDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define NAME_WINDOW_SRC "srcView"
#define NAME_WINDOW_DEBUG "srcDebug"
#define NAME_WINDOW_RESULT "srcResult"
#define WM_ADD_STRING	(WM_USER + 500)

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
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


// COpenCVAppGUIDlg 대화 상자



COpenCVAppGUIDlg::COpenCVAppGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVAPPGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCVAppGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, _listBoxLog);
}

BEGIN_MESSAGE_MAP(COpenCVAppGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, &COpenCVAppGUIDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COpenCVAppGUIDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_INSPECTION, &COpenCVAppGUIDlg::OnBnClickedBtnInspection)
	ON_BN_CLICKED(IDC_BTN_INSPECTION_CV, &COpenCVAppGUIDlg::OnBnClickedBtnInspectionCv)
//	ON_BN_CLICKED(IDC_BTN_SAMPLE_CODE, &COpenCVAppGUIDlg::OnBnClickedBtnSampleCode)
	ON_MESSAGE(WM_ADD_STRING, &COpenCVAppGUIDlg::OnAddString)
END_MESSAGE_MAP()


// COpenCVAppGUIDlg 메시지 처리기

BOOL COpenCVAppGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_MINIMIZE);



	//opencv -> window handle
	namedWindow(NAME_WINDOW_SRC, WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_SRC);
	HWND hParent = (HWND)FindWindow(NULL, NAME_WINDOW_SRC);
	HWND hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);


	namedWindow(NAME_WINDOW_DEBUG, WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_DEBUG);
	hParent = (HWND)FindWindow(NULL, NAME_WINDOW_DEBUG);
	hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE_DEBUG)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);

	namedWindow(NAME_WINDOW_RESULT, WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_RESULT);
	hParent = (HWND)FindWindow(NULL, NAME_WINDOW_RESULT);
	hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE_RESULT)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);




	//GetDlgItem(IDC_PICTURE)->GetWindowRect(_rtImageView);
	//ScreenToClient(_rtImageView);
	_mWndImageView.clear();
	_mWndImageView.insert(pair<int, CRect>(IDC_PICTURE, CRect(0, 0, 0, 0)));
	_mWndImageView.insert(pair<int, CRect>(IDC_PICTURE_DEBUG, CRect(0, 0, 0, 0)));
	_mWndImageView.insert(pair<int, CRect>(IDC_PICTURE_RESULT, CRect(0, 0, 0, 0)));

	for (auto iter = _mWndImageView.begin(); iter != _mWndImageView.end(); iter++) {
		int resID = (iter->first);
		CRect rtImgView;
		GetDlgItem(resID)->GetWindowRect(rtImgView);
		ScreenToClient(rtImgView);
		iter->second = rtImgView;
	}


	_mMatBuff.clear();
	_mInsps.clear();

	AddString("System Log :: Start()");
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COpenCVAppGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COpenCVAppGUIDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.



	if (IsIconic())
	{
		//CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		//if (!_imageView.IsNull())
		//{
		//	dc.SetStretchBltMode(COLORONCOLOR);
		//	_imageView.Draw(dc, _rtImageView);
		//}
//		DisplayImage(IDC_PICTURE, _DrawImage);


		//CRect rect;
		//::GetWindowRect(GetDlgItem(IDC_PICTURE)->m_hWnd, rect);
		//ScreenToClient(rect);

		CBrush brush(HS_DIAGCROSS, RGB(200, 200, 200));
		//dc.FillRect(CRect(rect.left + 1, rect.top + 1, rect.right, rect.bottom), &brush);
		for (auto iter = _mWndImageView.begin(); iter != _mWndImageView.end(); iter++) {
			CRect rt = iter->second;
			dc.FillRect(rt, &brush);
		}


		if (!_mMatBuff[eImgSrcColor].empty())
		{
			Mat Img = _mMatBuff[eImgSrcColor];
			Mat DrawResize;
			Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
			resize(Img, DrawResize, size);

			resizeWindow(NAME_WINDOW_SRC, size.width, size.height);
			imshow(NAME_WINDOW_SRC, DrawResize);
		}

		if (_bShowResult)
		{
			if (!_mMatBuff[eImgDrawColor].empty())
			{
				Mat Img = _mMatBuff[eImgDrawColor];
				Mat DrawResize;
				Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
				resize(Img, DrawResize, size);

				resizeWindow(NAME_WINDOW_RESULT, size.width, size.height);
				imshow(NAME_WINDOW_RESULT, DrawResize);
			}
		}

		if (_bShowDebug)
		{
			if (!_mMatBuff[eImgDebugColor].empty())
			{
				Mat Img = _mMatBuff[eImgDebugColor];
				Mat DrawResize;
				Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
				resize(Img, DrawResize, size);

				resizeWindow(NAME_WINDOW_DEBUG, size.width, size.height);
				imshow(NAME_WINDOW_DEBUG, DrawResize);
			}
		}




		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COpenCVAppGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenCVAppGUIDlg::UpdateDispSrc()
{
	_bShowResult = false;
	CRect rectSrc;
	::GetWindowRect(GetDlgItem(IDC_PICTURE)->m_hWnd, rectSrc);
	double dScaleX = rectSrc.Width() / (double)_SourceImage.cols;
	double dScaleY = rectSrc.Height() / (double)_SourceImage.rows;
	_dSrcScale = min(dScaleX, dScaleY);
	_dNewScale = _dSrcScale;
	//_bShowResult = FALSE;
	//防止顯示不同比例圖片時DC殘留
	CWnd* pWnd = GetDlgItem(IDC_PICTURE);
	pWnd->GetClientRect(&rectSrc);//제어 클라이언트 영역의 좌표 얻기
	pWnd->ClientToScreen(rectSrc);//제어 클라이언트 영역에서 대화 영역으로 영역 좌표 변환
	this->ScreenToClient(rectSrc); //다이얼로그 영역의 영역 좌표를 다이얼로그의 클라이언트 영역 좌표로 변환
	InvalidateRect(rectSrc);
	//비율이 다른 사진을 표시할 때 DC 잔여물 방지

	//Scroll Bar


	//Test
	//double d1 = clock ();
	//Mat matResult;
	//matchTemplate (_DrawImage, _matPtrn, matResult, CV_TM_CCORR);
	//double d2 = clock ();

	////From ImageShop
	//matResult.create (_DrawImage.rows - _matPtrn.rows + 1,
	//	_DrawImage.cols - _matPtrn.cols + 1, CV_32FC1);
	//matResult.setTo (0);
	//cv::Mat& matTemplate = _matPtrn;

	//int  t_r_end = matTemplate.rows, t_r = 0;
	//for (int r = 0; r < matResult.rows; r++)
	//{
	//	float* r_matResult = matResult.ptr<float> (r);
	//	uchar* r_source = _DrawImage.ptr<uchar> (r);
	//	uchar* r_template, *r_sub_source;
	//	for (int c = 0; c < matResult.cols; ++c, ++r_matResult, ++r_source)
	//	{
	//		r_template = matTemplate.ptr<uchar> ();
	//		r_sub_source = r_source;
	//		for (t_r = 0; t_r < t_r_end; ++t_r, r_sub_source += _DrawImage.cols, r_template += matTemplate.cols)
	//		{
	//			*r_matResult = *r_matResult + IM_Conv_SIMD (r_template, r_sub_source, matTemplate.cols);
	//		}
	//	}
	//}
	//double d3 = clock ();

	//double d = (d3 - d2) / (d2 - d1);
	//CString str; str.Format ("%.3f", d); AfxMessageBox (str);
	//
}


void COpenCVAppGUIDlg::OnBnClickedBtnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		//string fileName = path;
		//_SourceImage = cv::imread(fileName, IMREAD_ANYCOLOR);

		//if (_SourceImage.type() == CV_8UC1)
		//	cvtColor(_SourceImage, _DrawImage, COLOR_GRAY2BGR);
		//else
		//	_SourceImage.copyTo(_DrawImage);

		string fileName = path;
		_SourceImage = cv::imread(fileName, IMREAD_ANYCOLOR);

		OnAllocateBuffer(_SourceImage.cols, _SourceImage.rows);
		UpdateDispSrc();

		//add update inspection list
		UpdateInspList();
		

		InvalidateRect(_rtImageView, FALSE);
		//AfxMessageBox("Image Loaded");

	}


}

void COpenCVAppGUIDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//string fileName = "jetson_clone.bmp";
	//_imageView.Save(fileName.c_str());

	CFileDialog dlgFileSave(FALSE, "*.*", NULL,//화일명 없음
		OFN_FILEMUSTEXIST,
		_T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"), NULL);

	//옵션 선택 부분.
	if (dlgFileSave.DoModal() == IDOK)
	{
		string str = dlgFileSave.GetPathName();
		imwrite(str, _SourceImage);
	}

	//AfxMessageBox("Image Saved");
}

void COpenCVAppGUIDlg::OnBnClickedBtnInspection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_bShowResult = false;
	_bShowDebug = false;
	//input
		//OnInspection(InputArray src, OutputArray dst);
	Mat src = _mMatBuff[eImgSrcGray];


	//fucntion
	Mat dst = _mMatBuff[eImgDebugGray];

	// using pointer to data
	OnInspection(src.data, src.cols, src.rows, dst.data);

	// get cross points - 4 {lt, rt, lb, rb}
	Point* pPoints = new Point[4];

	// lt
	for (size_t topIndex = 0; topIndex < _vLinePoints_Top.size(); topIndex++)
	{
		for (size_t leftIndex = 0; leftIndex < _vLinePoints_Left.size(); leftIndex++)
		{
			Point ptT = _vLinePoints_Top[topIndex];
			Point ptL = _vLinePoints_Left[leftIndex];
			if (ptT.x == ptL.x && ptT.y == ptL.y)
			{
				pPoints[0].x = ptL.x;
				pPoints[0].y = ptT.y;
			}
		}
	}
	

	// rt
	for (size_t topIndex = 0; topIndex < _vLinePoints_Top.size(); topIndex++)
	{
		for (size_t rightIndex = 0; rightIndex < _vLinePoints_Right.size(); rightIndex++)
		{
			Point ptT = _vLinePoints_Top[topIndex];
			Point ptR = _vLinePoints_Right[rightIndex];
			if (ptT.x == ptR.x && ptT.y == ptR.y)
			{
				pPoints[1].x = ptR.x;
				pPoints[1].y = ptT.y;
			}
		}
	}

	// lb
	for (size_t btmIndex = 0; btmIndex < _vLinePoints_Btm.size(); btmIndex++)
	{
		for (size_t leftIndex = 0; leftIndex < _vLinePoints_Left.size(); leftIndex++)
		{
			Point ptB = _vLinePoints_Btm[btmIndex];
			Point ptL = _vLinePoints_Left[leftIndex];
			if (ptB.x == ptL.x && ptB.y == ptL.y)
			{
				pPoints[2].x = ptL.x;
				pPoints[2].y = ptB.y;
			}
		}
	}
	// rb
	for (size_t btmIndex = 0; btmIndex < _vLinePoints_Btm.size(); btmIndex++)
	{
		for (size_t rightIndex = 0; rightIndex < _vLinePoints_Right.size(); rightIndex++)
		{
			Point ptB = _vLinePoints_Btm[btmIndex];
			Point ptR = _vLinePoints_Right[rightIndex];
			if (ptB.x == ptR.x && ptB.y == ptR.y)
			{
				pPoints[3].x = ptR.x;
				pPoints[3].y = ptB.y;
			}
		}
	}

	//result & display
	_bShowResult = true;
	_bShowDebug = true;
	dst.copyTo(_mMatBuff[eImgDebugGray]);
	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);

	Mat draw = _mMatBuff[eImgDrawColor];
	//cv::rectangle(draw, Rect(10, 10, 50, 50), Scalar(0, 0, 255), 3);
	int thickness = 5;
	int lineType = LINE_8;
	int w = 100;
	/*
	line(draw, Point(100, 100), Point(200,100), Scalar(255, 0, 0), thickness, lineType);
	line(draw, Point(200, 100), Point(200, 200), Scalar(0, 255, 0), thickness, lineType);
	line(draw, Point(200, 200), Point(100, 200), Scalar(0, 255, 0), thickness, lineType);
	line(draw, Point(100, 200), Point(100, 100), Scalar(0, 255, 255), thickness, lineType);

	line(draw, Point(_pt1.x - 10, _pt1.y), Point(_pt1.x + 10, _pt1.y), Scalar(255, 0, 0), thickness, lineType);
	line(draw, Point(_pt1.x, _pt1.y - 10), Point(_pt1.x, _pt1.y + 10), Scalar(255, 0, 0), thickness, lineType);

	line(draw, Point(_pt2.x - 10, _pt2.y), Point(_pt2.x + 10, _pt2.y), Scalar(255, 0, 0), thickness, lineType);
	line(draw, Point(_pt2.x, _pt2.y - 10), Point(_pt2.x, _pt2.y + 10), Scalar(255, 0, 0), thickness, lineType);
	line(draw, Point(_pt1.x, _pt1.y), Point(_pt2.x, _pt2.y), Scalar(255, 0, 0), thickness, lineType);
	*/

	for (size_t i = 0; i < _vLinePoints_Left.size(); i++)
		line(draw, Point(_vLinePoints_Left[i].x, _vLinePoints_Left[i].y), Point(_vLinePoints_Left[i].x, _vLinePoints_Left[i].y), Scalar(255, 0, 0), thickness, lineType);
	for (size_t i = 0; i < _vLinePoints_Right.size(); i++)
		line(draw, Point(_vLinePoints_Right[i].x, _vLinePoints_Right[i].y), Point(_vLinePoints_Right[i].x, _vLinePoints_Right[i].y), Scalar(0, 255, 0), thickness, lineType);
	for (size_t i = 0; i < _vLinePoints_Top.size(); i++)
		line(draw, Point(_vLinePoints_Top[i].x, _vLinePoints_Top[i].y), Point(_vLinePoints_Top[i].x, _vLinePoints_Top[i].y), Scalar(0, 0, 255), thickness, lineType);
	for (size_t i = 0; i < _vLinePoints_Btm.size(); i++)
		line(draw, Point(_vLinePoints_Btm[i].x, _vLinePoints_Btm[i].y), Point(_vLinePoints_Btm[i].x, _vLinePoints_Btm[i].y), Scalar(255, 0, 255), thickness, lineType);


	draw = _mMatBuff[eImgDebugColor];

	for (size_t i = 0; i < 4; i++)
	{
		Point pt = pPoints[i];
		draw = _mMatBuff[eImgDrawColor];
		line(draw, Point(pt.x - 10, pt.y-10), Point(pt.x + 10, pt.y+10), Scalar(255, 255, 0), thickness, lineType);
		line(draw, Point(pt.x+10, pt.y - 10), Point(pt.x-10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
		draw = _mMatBuff[eImgDebugColor];
		line(draw, Point(pt.x - 10, pt.y - 10), Point(pt.x + 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
		line(draw, Point(pt.x + 10, pt.y - 10), Point(pt.x - 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
	}

	line(draw, Point(_vLinePoints_Left[0].x, 0), Point(_vLinePoints_Left[0].x, draw.rows - 1), Scalar(255, 0, 255), 1, lineType);
	line(draw, Point(_vLinePoints_Right[0].x, 0), Point(_vLinePoints_Right[0].x, draw.rows - 1), Scalar(255, 255, 0), 1, lineType);
	line(draw, Point(0, _vLinePoints_Top[0].y), Point(draw.cols - 1, _vLinePoints_Top[0].y), Scalar(0, 255, 255), 1, lineType);
	line(draw, Point(0, _vLinePoints_Btm[0].y), Point(draw.cols - 1, _vLinePoints_Btm[0].y), Scalar(255, 255, 0), 1, lineType);


	if (pPoints != nullptr)
	{
		delete[] pPoints;
		pPoints = nullptr;
	}

	Invalidate(FALSE);
}

int COpenCVAppGUIDlg::OnAllocateBuffer(int cols, int rows)
{
	_mMatBuff.clear();

	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgSrcColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgSrcGray, Mat(rows, cols, CV_8UC1)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDebugGray, Mat(rows, cols, CV_8UC1)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDebugColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDrawColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgResultColor, Mat(rows, cols, CV_8UC3)));

	//source to source of map
	//_SourceImage.copyTo(_mMatBuff[eImgSrc]);

	//source to draw of map
	if (_SourceImage.type() == CV_8UC1)
	{
		cvtColor(_SourceImage, _mMatBuff[eImgSrcColor], COLOR_GRAY2BGR);
		_SourceImage.copyTo(_mMatBuff[eImgSrcGray]);
	}
	else
	{
		_SourceImage.copyTo(_mMatBuff[eImgSrcColor]);
		cvtColor(_SourceImage, _mMatBuff[eImgSrcGray], COLOR_BGR2GRAY);
	}

	Mat vi = _mMatBuff[eImgSrcColor];



	_mMatBuff[eImgSrcColor].copyTo(_mMatBuff[eImgDrawColor]);

	//debug of map ... just create buffer as initilized value
	_mMatBuff[eImgDebugGray] = 0;

	//result of map ... just create buffer as initilized value
	_mMatBuff[eImgResultColor] = 255;

	return 0;
}

int COpenCVAppGUIDlg::UpdateInspList()
{
	_mInsps.insert(make_pair("OnInspFindcontourSample", COpenCVAppGUIDlg::CallInspFindcontourSample));
	_mInsps.insert(make_pair("OnInspFindShape", COpenCVAppGUIDlg::CallInspFindShape));
	_mInsps.insert(make_pair("OnInspFindMultiShape", COpenCVAppGUIDlg::CallInspFindMultiShape));
	_mInsps.insert(make_pair("OnInspFindColor", COpenCVAppGUIDlg::CallInspFindColor));
	_mInsps.insert(make_pair("OnInspSegmentColor", COpenCVAppGUIDlg::CallInspSegmentColor));
	_mInsps.insert(make_pair("OnInspMorpology", COpenCVAppGUIDlg::CallInspMorpology));
	_mInsps.insert(make_pair("OnInspMorpologyMultiObjects", COpenCVAppGUIDlg::CallInspMorpologyMultiObjects));
	_mInsps.insert(make_pair("OnInspHistoEqulization", COpenCVAppGUIDlg::CallInspHistoEqulization));

	return 1;
}

int COpenCVAppGUIDlg::OnInspection(InputArray src, OutputArray dst)
{

	return 0;
}

int COpenCVAppGUIDlg::OnInspection(Mat src, Mat dst)
{
	return 0;
}

int COpenCVAppGUIDlg::OnInspection(uchar* pSrc, size_t cols, size_t rows, uchar* pDst)
{
	/*
	pSrc = pointer 2d
	cols = width
	rows = height
	pDst = result data, pointer 2d
	*/
	/* todo*/
	// 조건 pSrc = GrayScale
	// pSrc > 128 ? pDst = 255 : pDst = 0
	// 코드를 작성하세요

	for (size_t i = 0; i < cols * rows; i++)
	{
		pSrc[i] > 128 ? pDst[i] = 255 : pDst[i] = 0;
	}

	_pt1 = cv::Point(0, 0);
	_pt2 = cv::Point(0, 0);

	_vLinePoints_Left.clear();
	_vLinePoints_Right.clear();
	_vLinePoints_Top.clear();
	_vLinePoints_Btm.clear();

	//Left->Right
	for (size_t row = 0; row < rows; row++)
		for (size_t col = 0; col < cols; col++)
			if (pDst[row * cols + col] > 100){
				_vLinePoints_Left.push_back(cv::Point(col, row));
				break;
			}
	//Right->Left
	for (size_t row = 0; row < rows; row++)
		for (size_t col = cols-1; col > 0; col--)
			if (pDst[row * cols + col] > 100) {
				_vLinePoints_Right.push_back(cv::Point(col, row));
				break;
			}


	//Top->Bottom
	for (size_t col = 0; col < cols; col++)
		for (size_t row = 0; row < rows; row++)
			if (pDst[row * cols + col] > 100) {
				_vLinePoints_Top.push_back(cv::Point(col, row));
				break;
			}
	//Bottom->Top
	for (size_t col = 0; col < cols; col++)
		for (size_t row = rows-1; row > 0; row--)
			if (pDst[row * cols + col] > 100) {
				_vLinePoints_Btm.push_back(cv::Point(col, row));
				break;
			}

	return 0;
}

//static_cast
//double a = (double)(int -> double 1);
//a 1.000000
//const_cast
//dynamic_cast ...상속관계
//reinterpret_cast 

int COpenCVAppGUIDlg::CallInspFindcontourSample(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspFindcontourSample();
}

int COpenCVAppGUIDlg::CallInspFindShape(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspFindShapes();
}

int COpenCVAppGUIDlg::CallInspFindMultiShape(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspFindMultiShape();
}

int COpenCVAppGUIDlg::CallInspFindColor(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspFindColor();
}

int COpenCVAppGUIDlg::CallInspSegmentColor(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspSegmentColor();
}

int COpenCVAppGUIDlg::CallInspMorpology(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspMorpology();
}
int COpenCVAppGUIDlg::CallInspMorpologyChar(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspMorpologyChar();
}
int COpenCVAppGUIDlg::CallInspMorpologyMultiObjects(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspMorpologyMultiObjects();
}
int COpenCVAppGUIDlg::CallInspHistoEqulization(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspHistoEqulization();
}
int COpenCVAppGUIDlg::OnInspFindcontourSample()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	int thresh = 128;

	RNG rng(12345);

	//Mat canny_output;
	//Canny(src_gray, canny_output, thresh, thresh * 2);

	Mat thr_img;

	threshold(src_gray, thr_img, thresh, 255, THRESH_BINARY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(thr_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//canny_output.copyTo(_mMatBuff[eImgDebugGray]);
	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);
	Mat drawing = _mMatBuff[eImgDebugColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}
	drawing = _mMatBuff[eImgDrawColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}

	int thickness = 5;
	int lineType = LINE_8;
	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t k = 0; k < contours[i].size(); k++)
		{
			Point pt = contours[i][k];
			drawing = _mMatBuff[eImgDrawColor];
			line(drawing, Point(pt.x - 10, pt.y - 10), Point(pt.x + 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			line(drawing, Point(pt.x + 10, pt.y - 10), Point(pt.x - 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			drawing = _mMatBuff[eImgDebugColor];
			line(drawing, Point(pt.x - 10, pt.y - 10), Point(pt.x + 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			line(drawing, Point(pt.x + 10, pt.y - 10), Point(pt.x - 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
		}
	}
	//imshow("Contours", drawing);
	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);

	return 0;
}

int COpenCVAppGUIDlg::OnInspFindShapes()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	int thresh = 50;

	RNG rng(12345);

	Mat thr_img;
	threshold(src_gray, thr_img, thresh, 255, THRESH_BINARY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(thr_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	int circle_index = 0, triangle_index = 0, rectangle_index = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);

		if (contours[i].size() >= 100 && contours[i].size() <= 150)
		{
			circle_index = i; 
			continue;
		}
		else if (contours[i].size() == 4)
		{
			rectangle_index = i;
			continue;
		}
		else if (contours[i].size() >= 150)
		{
			triangle_index = i;
			continue;
		}

	}

	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);
	Mat drawing = _mMatBuff[eImgDebugColor];
	drawing = _mMatBuff[eImgDrawColor];

	drawContours(drawing, contours, (int)circle_index, Scalar(0,0, 255), 2, LINE_8, hierarchy, 0);
	drawContours(drawing, contours, (int)rectangle_index, Scalar(255, 0, 0), 2, LINE_8, hierarchy, 0);
	drawContours(drawing, contours, (int)triangle_index, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);

	Mat mask = _mMatBuff[eImgDrawColor].clone();
	mask = 0;
	drawContours(mask, contours, circle_index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	drawing = drawing & mask;

	//imshow("Contours", drawing);
	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);

	return 0;
}

int COpenCVAppGUIDlg::OnInspFindMultiShape()
{

	Mat src_gray = _mMatBuff[eImgSrcGray];
	int thresh = 50;

	RNG rng(12345);

	Mat thr_img;
	threshold(src_gray, thr_img, thresh, 255, THRESH_BINARY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(thr_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);
	_mMatBuff[eImgSrcColor].copyTo(_mMatBuff[eImgDebugColor]);
	Mat drawing = _mMatBuff[eImgDebugColor];
	//


	vector<int> vCircle_index, vTriangle_index, vRectangle_index;
	double spec_min_Triangle = 0.5;
	double spec_max_Triangle = 0.65;
	double spec_min_Rectangle = 0.7;
	double spec_max_Rectangle = 0.8;
	double spec_min_Circle = 0.88;
	double spec_max_Circle = 1.0;

	for (size_t i = 0; i < contours.size(); i++)
	{
		RotatedRect rt = minAreaRect(contours[i]);
		double area = contourArea(contours[i]);
		double perimeter = arcLength(contours[i], true);
		double circularity = 4 * CV_PI * area / (perimeter * perimeter);
		ostringstream ss;
		ss << std::fixed;
		ss << "[";
		ss << to_string(i+1);
		ss << "]";
		ss << " area = ";
		ss << std::setprecision(3) << area;
		ss << " perimeter = ";
		ss << std::setprecision(3) << perimeter;
		ss << " circularity = ";
		ss << std::setprecision(3) << circularity;
		string ssTxt = ss.str();
		AddString(ssTxt.c_str());


		ostringstream tag;
		tag << "[";
		tag << to_string(i + 1);
		tag << "]";
		putText(drawing, tag.str(), rt.boundingRect().tl(), CV_FONT_HERSHEY_TRIPLEX,0.6,Scalar(255,255,255));



		if (circularity >= spec_min_Triangle && circularity <= spec_max_Triangle)
		{
			vTriangle_index.push_back(i);
			continue;
		}
		else if (circularity >= spec_min_Rectangle && circularity <= spec_max_Rectangle)
		{
			vRectangle_index.push_back(i);
			continue;
		}
		else if (circularity >= spec_min_Circle && circularity <= spec_max_Circle)
		{
			vCircle_index.push_back(i);
			continue;
		}
	}


	Mat mask = _mMatBuff[eImgDrawColor].clone();
	mask = 0;
	//for (size_t index = 0; index < vCircle_index.size(); index++)
	//	drawContours(mask, contours, vCircle_index[index], Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	for (size_t index = 0; index < vRectangle_index.size(); index++)
		drawContours(mask, contours, vRectangle_index[index], Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	for (size_t index = 0; index < vTriangle_index.size(); index++)
		drawContours(mask, contours, vTriangle_index[index], Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	
	drawing = _mMatBuff[eImgDrawColor];
	drawing = drawing & mask;

	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);


	return 0;
}






void COpenCVAppGUIDlg::OnBnClickedBtnInspectionCv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//
	//auto f = _mInsps["OnInspFindCircle"];
	//auto f = _mInsps["OnInspFindFaceID"];
	//auto ret = f(this); // int COpenCVAppGUIDlg::OnInspFindShapes()

	//auto f = _mInsps["OnInspFindShape"];
	auto f = _mInsps["OnInspHistoEqulization"];
	if (f == nullptr) return;
	auto ret = f(this); // int COpenCVAppGUIDlg::OnInspFindShapes()


}


void COpenCVAppGUIDlg::OnBnClickedBtnSampleCode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	vector<Point> contour;//single object
	vector<vector<Point> > contours;//multi objects
	
	//contour #1...object1
	contour.clear();
	contour.push_back(Point(10, 10));
	contour.push_back(Point(20, 10));
	contour.push_back(Point(10, 20));
	contour.push_back(Point(20, 20));
	contours.push_back(contour);

	//contour #2...object2
	contour.clear();
	contour.push_back(Point(110, 110));
	contour.push_back(Point(120, 110));
	contour.push_back(Point(110, 120));
	contour.push_back(Point(120, 120));
	contours.push_back(contour);

	Point a = contours[0][0];
	Point b = contours[1][0];
}
void COpenCVAppGUIDlg::AddString(LPCTSTR lpszLog)
{
	SendMessage(WM_ADD_STRING, 0, (LPARAM)lpszLog);
}
LRESULT COpenCVAppGUIDlg::OnAddString(WPARAM wParam, LPARAM lParam)
{
	if (!GetSafeHwnd()) return 0;

	if (_listBoxLog.GetCount() > 500)
	{
		_listBoxLog.ResetContent();
	}

	LPCTSTR lpszLog = reinterpret_cast <LPCTSTR> (lParam);
	SYSTEMTIME t;
	GetLocalTime(&t);
	CString nStrMsg = _T("");
	nStrMsg += lpszLog;
	int nIdx = _listBoxLog.AddString(nStrMsg);
	_listBoxLog.SetTopIndex(_listBoxLog.GetCount() - 1);

	stringstream ssTime;
	time_t const now_c = time(NULL);
	//ssTime << put_time(localtime(&now_c), "%a %d %b %Y - %I_%M_%S%p");
	ssTime << put_time(localtime(&now_c), "%a %d %b %Y-%I_%M");
	string time_c = ssTime.str();

	ofstream file;
	string fileName;
	fileName += "log";
	fileName += ssTime.str();
	fileName += ".txt";

	file.open(fileName, ios::out | ios::app);
	file << nStrMsg << endl;
	file.close();

	return 0;
}

int COpenCVAppGUIDlg::OnInspFindColor()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	int thresh = 50;

	RNG rng(12345);

	Mat thr_img;
	threshold(src_gray, thr_img, thresh, 255, THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(thr_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//vector<int> circle_index , triangle_index, rectangle_index ;
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	//double area = contourArea(contours[i]);

	//	if (contours[i].size() >= 28 && contours[i].size() <= 100)
	//	{
	//		circle_index.push_back(i);
	//		continue;
	//	}
	//	else if (contours[i].size() == 4)
	//	{
	//		rectangle_index.push_back(i);
	//		continue;
	//	}
	//	else if (contours[i].size() >= 100)
	//	{
	//		triangle_index.push_back(i);
	//		continue;
	//	}
	//}

	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);
	Mat drawing = _mMatBuff[eImgDebugColor];
	drawing = _mMatBuff[eImgDrawColor];
	vector<vector<Point>> reds;
	vector<Point> red, blue;
	vector<int> green;
	for (size_t row = 0; row < drawing.rows; row++)
	{
		for (size_t col = 0; col < drawing.cols; col++)
		{
			int index = row * drawing.cols + col;
			index *= 3;
			int b = drawing.data[index + 0];//uchar * pData = img.data;
			int g = drawing.data[index + 1];
			int r = drawing.data[index + 2];
			if (b > 220)
				blue.push_back(Point(col, row));
			else if (g > 220)
				green.push_back(index);
			else if (r > 220)
				red.push_back(Point(col, row));
		}
	}
	reds.push_back(red);

	if (1)
	{
		Mat vImg = _mMatBuff[eImgDebugColor].clone();
		vImg = Scalar(255, 255, 255);

		{
			for (size_t i = 0; i < red.size(); i++)
			{
				Point pt = red[i];

				int index = pt.y * drawing.cols + pt.x;
				index *= 3;
				//if (pt.x == col && pt.y == row)
				{
					vImg.data[index + 0] = 0;//uchar * pData = img.data;
					vImg.data[index + 1] = 0;
					vImg.data[index + 2] = 255;

				}


			}
		}

		int a = 0;
	}



	Mat mask = _mMatBuff[eImgDebugColor].clone();

	//drawContours(mask, reds, 0, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	drawContours(mask, reds, 0, Scalar(0, 0, 255));

	drawing = drawing & mask;
	//for (size_t i = 0; i < triangle_index.size(); i++)
	//{
	//	drawContours(drawing, contours, triangle_index[i], Scalar(255, 0, 0), 2, LINE_8, hierarchy, 0);
	//}
	//for (size_t i = 0; i < rectangle_index.size(); i++)
	//{
	//	drawContours(drawing, contours, rectangle_index[i], Scalar(0, 0, 255), 2, LINE_8, hierarchy, 0);
	//}


	//imshow("Contours", drawing);
	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);

	return 0;
}

int COpenCVAppGUIDlg::OnInspSegmentColor()
{
	Mat src_color = _mMatBuff[eImgSrcColor];

	Mat src_hsv;
	// Convert from BGR to HSV colorspace
	cvtColor(src_color, src_hsv, COLOR_BGR2HSV);

	const int max_value_H = 360 / 2;
	const int max_value = 255;
	int low_H = 0, low_S = 0, low_V = 0;
	int high_H = max_value_H, high_S = max_value, high_V = max_value;

	// Detect the object based on HSV Range Values
	Mat src_hsv_threshold_red, src_hsv_threshold_green, src_hsv_threshold_blue;

	low_H = 160; high_H = 180;
	inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_red);
	low_H = 20; high_H = 60;
	inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_green);
	low_H = 80; high_H = 110;
	inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_blue);

	cvtColor(src_hsv_threshold_blue, _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);


	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(src_hsv_threshold_blue, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	Mat drawing = _mMatBuff[eImgDebugColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		RotatedRect rt = minAreaRect(contours[i]);
		double area = contourArea(contours[i]);
		double perimeter = arcLength(contours[i], true);
		double circularity = 4 * CV_PI * area / (perimeter * perimeter);
		ostringstream ss;
		ss << std::fixed;
		ss << "[";
		ss << to_string(i + 1);
		ss << "]";
		ss << " area = ";
		ss << std::setprecision(3) << area;
		ss << " perimeter = ";
		ss << std::setprecision(3) << perimeter;
		ss << " circularity = ";
		ss << std::setprecision(3) << circularity;
		string ssTxt = ss.str();
		AddString(ssTxt.c_str());


		ostringstream tag;
		tag << "[";
		tag << to_string(i + 1);
		tag << "]";
		putText(drawing, tag.str(), rt.boundingRect().tl(), CV_FONT_HERSHEY_TRIPLEX, 0.6, Scalar(255, 255, 255));
	}

	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);

	return 0;
}

int COpenCVAppGUIDlg::OnInspMorpology()
{
	Mat src_color = _mMatBuff[eImgSrcColor];
	Mat src_hsv;
	cvtColor(src_color, src_hsv, COLOR_BGR2HSV);
	const int max_value_H = 360 / 2;
	const int max_value = 255;
	int low_H = 0, low_S = 0, low_V = 0;
	int high_H = max_value_H, high_S = max_value, high_V = max_value;
	Mat src_hsv_threshold_red, src_hsv_threshold_green, src_hsv_threshold_blue;
	//low_H = 160; high_H = 180;
	//inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_red);
	//low_H = 20; high_H = 60;
	//inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_green);
	low_H = 80; high_H = 110;
	inRange(src_hsv, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), src_hsv_threshold_blue);



	//Erosion...침식
	int kernel_type = MORPH_ELLIPSE;// MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
	//   0 0 0     0            0 0
	//   0 0 0   0 0 0        0 0 0
	//   0 0 0     0          0 0 

	int kernel_size = 1;
	Mat element = getStructuringElement(kernel_type,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));
	Mat src_bin = src_hsv_threshold_blue, src_erode;
	erode(src_bin, src_erode, element);

	Mat diff = src_bin - src_erode;
	//Dialation...팽창


	src_bin = src_hsv_threshold_blue;
	Mat src_dilate;
	dilate(src_bin, src_dilate, element);
	diff = src_dilate - src_bin;



	return 0;
}

int COpenCVAppGUIDlg::OnInspMorpologyChar()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	resize(src_gray, src_gray, Size(src_gray.cols * 2, src_gray.rows * 2));
	Mat src_invert = ~src_gray;
	//threshold(src_gray, src_bin, 128, 255, ThresholdTypes::THRESH_BINARY_INV);


	//Erosion...침식
	int kernel_type = MORPH_CROSS;// MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
	//   0 0 0     0            0 0
	//   0 0 0   0 0 0        0 0 0
	//   0 0 0     0          0 0 

	int kernel_size = 1;
	Mat element = getStructuringElement(kernel_type,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));
	Mat src_erode;
	erode(src_invert, src_erode, element);

	Mat diff = src_invert - src_erode;
	Mat res;
	threshold(src_erode, res, 100, 255, ThresholdTypes::THRESH_BINARY);
	return 0;
}

int COpenCVAppGUIDlg::OnInspMorpologyMultiObjects()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	//resize(src_gray, src_gray, Size(src_gray.cols * 2, src_gray.rows * 2));
	Mat src_bin;
	threshold(src_gray, src_bin, 128, 255, ThresholdTypes::THRESH_BINARY_INV);


	//Erosion...침식
	int kernel_type = MORPH_RECT;// MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
	//   0 0 0     0            0 0
	//   0 0 0   0 0 0        0 0 0
	//   0 0 0     0          0 0 

	int kernel_size = 1;
	Mat element = getStructuringElement(kernel_type,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));
	Mat src_erode;
	erode(src_bin, src_erode, element);

	Mat diff = src_bin - src_erode;


	//openning
	Mat openning = src_bin.clone();
	int openning_cnt = 1;
	for (size_t i = 0; i < openning_cnt; i++)
	{
		erode(openning, openning, element, Point(-1,-1),1);
		dilate(openning, openning, element, Point(-1, -1), 1);
	}


	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(openning, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	cvtColor(openning, _mMatBuff[eImgDebugColor], CV_GRAY2BGR);
	Mat drawing = _mMatBuff[eImgDebugColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		RotatedRect rt = minAreaRect(contours[i]);
		double area = contourArea(contours[i]);
		double perimeter = arcLength(contours[i], true);
		double circularity = 4 * CV_PI * area / (perimeter * perimeter);
		ostringstream ss;
		ss << std::fixed;
		ss << "[";
		ss << to_string(i + 1);
		ss << "]";
		ss << " area = ";
		ss << std::setprecision(3) << area;
		ss << " perimeter = ";
		ss << std::setprecision(3) << perimeter;
		ss << " circularity = ";
		ss << std::setprecision(3) << circularity;
		string ssTxt = ss.str();
		AddString(ssTxt.c_str());

		
		ostringstream tag;
		tag << "[";
		tag << to_string(i + 1);
		tag << "]";
		putText(drawing, tag.str(), rt.boundingRect().tl(), CV_FONT_HERSHEY_TRIPLEX, 1.0, Scalar(0, 255, 255));
		rectangle(drawing, boundingRect(contours[i]), Scalar(255, 255, 0));
	}


	drawing = _mMatBuff[eImgDrawColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area < 100) continue;
		RotatedRect rt = minAreaRect(contours[i]);
		
		double perimeter = arcLength(contours[i], true);
		double circularity = 4 * CV_PI * area / (perimeter * perimeter);
		ostringstream ss;
		ss << "Final>>";
		ss << std::fixed;
		ss << "[";
		ss << to_string(i + 1);
		ss << "]";
		ss << " area = ";
		ss << std::setprecision(3) << area;
		ss << " perimeter = ";
		ss << std::setprecision(3) << perimeter;
		ss << " circularity = ";
		ss << std::setprecision(3) << circularity;
		string ssTxt = ss.str();
		AddString(ssTxt.c_str());


		ostringstream tag;
		tag << "[";
		tag << to_string(i + 1);
		tag << "]";
		putText(drawing, tag.str(), rt.boundingRect().tl(), CV_FONT_HERSHEY_TRIPLEX, 0.7, Scalar(0, 0, 255));
		rectangle(drawing, boundingRect(contours[i]), Scalar(0, 0, 255));
	}

	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);


	return 0;
}

int COpenCVAppGUIDlg::OnInspHistoEqulization()
{
	_bShowDebug = _bShowResult = false;

	Mat src_gray = _mMatBuff[eImgSrcGray];

	const int histo_max = 256;
	int histo[histo_max] = { 0, };
	int cumulative_freq[histo_max] = { 0, };
	int new_gray_value[histo_max] = { 0, };
	//get histogram :: 히스토그램
	for (size_t i = 0; i < src_gray.cols*src_gray.rows; i++) histo[src_gray.data[i]]++;

	//get cumulative frequence :: 누적분포
	cumulative_freq[0] = histo[0];
	//for (size_t i = 1; i < histo_max; i++) cumulative_freq[i]=histo[i] + cumulative_freq[i-1];

	//get new value
	int acc = 0;
	for (size_t i = 0; i < histo_max; i++)
	{
		acc += histo[i];
		cumulative_freq[i] = acc;
		//new_gray_value[i] = round((((float)cumulative_freq[i]) * 255) / src_gray.cols * src_gray.rows);
		new_gray_value[i] = round((((float)acc) * 255) / (src_gray.cols * src_gray.rows));
	}

	//drawing...histo debug color area
	int thickness = 1;
	int lineType = LINE_8;

	cvtColor(_mMatBuff[eImgSrcGray], _mMatBuff[eImgDebugColor], ColorConversionCodes::COLOR_GRAY2BGR);
	
	Mat debug_draw = _mMatBuff[eImgDebugColor];
	//draw   |------>
	bool view_horizontal = false;
	bool view_vertical = true;
	if (view_horizontal)
	{
		for (size_t row = 0; row < histo_max; row++)
		{
			line(debug_draw, Point(0, row * 2), Point(histo[row] / 10, row * 2), Scalar(255, 0, 255), thickness, lineType);
		}
	}
	
	if (view_vertical)
	{
		int height = src_gray.rows;
		for (size_t col = 0; col < histo_max; col++)
		{
			//draw histo
			Point pt1 = Point(col * 2, height - 1);//start
			Point pt2 = Point(col * 2, height - 1 - histo[col] / 10);//end
			line(debug_draw, pt1, pt2, Scalar(0, 255, 128), thickness, lineType);
		}

		int max_cumulate = cumulative_freq[histo_max - 1];
		for (size_t col = 0; col < histo_max; col++)
		{
			if (col == 0)
			{
				int scaled_v = (cumulative_freq[col] / max_cumulate) * height;
				Point pt1 = Point(col * 2, scaled_v);//start
				Point pt2 = Point(col * 2, scaled_v);//end
				line(debug_draw, pt1, pt2, Scalar(0, 0, 255), thickness, lineType);
				continue;
			}
			//draw cumulate
			int scaled_v1 = (cumulative_freq[col - 1] * 1.0 / max_cumulate) * height;
			int scaled_v2 = (cumulative_freq[col] * 1.0 / max_cumulate) * height;
			Point pt1 = Point(col * 2, height - scaled_v1);//start
			Point pt2 = Point(col * 2, height - scaled_v2);//end
			line(debug_draw, pt1, pt2, Scalar(0, 0, 255), thickness, lineType);

		}
	}
	


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mat src_gray_histoEqual = _mMatBuff[eImgSrcGray].clone();
	for (size_t i = 0; i < src_gray.cols * src_gray.rows; i++)
	{
		src_gray_histoEqual.data[i] = new_gray_value[src_gray.data[i]];
	}
	cvtColor(src_gray_histoEqual, _mMatBuff[eImgDrawColor], ColorConversionCodes::COLOR_GRAY2BGR);

	

	//get histogram :: 히스토그램
	for (size_t i = 0; i < src_gray.cols * src_gray.rows; i++) histo[src_gray_histoEqual.data[i]]++;

	//get cumulative frequence :: 누적분포
	cumulative_freq[0] = histo[0];
	//for (size_t i = 1; i < histo_max; i++) cumulative_freq[i]=histo[i] + cumulative_freq[i-1];

	//get new value
	acc = 0;
	for (size_t i = 0; i < histo_max; i++)
	{
		acc += histo[i];
		cumulative_freq[i] = acc;
		//new_gray_value[i] = round((((float)cumulative_freq[i]) * 255) / src_gray.cols * src_gray.rows);
		new_gray_value[i] = round((((float)acc) * 255) / (src_gray.cols * src_gray.rows));
	}

	//drawing...histo debug color area
	thickness = 1;
	lineType = LINE_8;


	Mat color_draw = _mMatBuff[eImgDrawColor];
	//draw   |------>
	view_horizontal = false;
	view_vertical = true;
	if (view_horizontal)
	{
		for (size_t row = 0; row < histo_max; row++)
		{
			line(color_draw, Point(0, row * 2), Point(histo[row] / 10, row * 2), Scalar(255, 0, 255), thickness, lineType);
		}
	}

	if (view_vertical)
	{
		int height = src_gray.rows;
		for (size_t col = 0; col < histo_max; col++)
		{
			//draw histo
			Point pt1 = Point(col * 2, height - 1);//start
			Point pt2 = Point(col * 2, height - 1 - histo[col] / 13);//end
			line(color_draw, pt1, pt2, Scalar(0, 255, 128), thickness, lineType);
		}

		int max_cumulate = cumulative_freq[histo_max - 1];
		for (size_t col = 0; col < histo_max; col++)
		{
			if (col == 0)
			{
				int scaled_v = (cumulative_freq[col] / max_cumulate) * height;
				Point pt1 = Point(col * 2, scaled_v);//start
				Point pt2 = Point(col * 2, scaled_v);//end
				line(color_draw, pt1, pt2, Scalar(0, 0, 255), thickness, lineType);
				continue;
			}
			//draw cumulate
			int scaled_v1 = (cumulative_freq[col - 1] * 1.0 / max_cumulate) * height;
			int scaled_v2 = (cumulative_freq[col] * 1.0 / max_cumulate) * height;
			Point pt1 = Point(col * 2, height - scaled_v1);//start
			Point pt2 = Point(col * 2, height - scaled_v2);//end
			line(color_draw, pt1, pt2, Scalar(0, 0, 255), thickness, lineType);

		}
	}


















	








	_bShowDebug = _bShowResult = true;

	Invalidate(FALSE);
	return 0;
}
