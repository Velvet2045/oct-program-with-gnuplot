
// OCT Program With GNUPlot 2Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "OCT Program With GNUPlot 2.h"
#include "OCT Program With GNUPlot 2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// global variables ///////////////////////////////////////////////////////////////////////////////

const int c_cMaxFiles = 130;
const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
static float **imgVertices;
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 255.0, 0.0);
static double pixelWidth = 0, pixelHeight = 0, pixelDepth = 0;

// function prototypes //////////////////////////////////////////////////////////////////////////////////////

int* diff(cv::Mat input, int size);
int* diff2(cv::Mat input, int size);
int* maximum(cv::Mat input);
cv::Mat imgDivision(cv::Mat input, int* key);
cv::Mat findILMLine(cv::Mat input, double thresh = 0, int type = CV_THRESH_OTSU);
cv::Mat correctILMLine(cv::Mat input1, cv::Mat input2, int size);
cv::Mat findRPELine(cv::Mat ILMLine, cv::Mat imgGrayscale, int maskHeight1, int maskHeight2);
cv::Mat correctRPELine(cv::Mat input, cv::Mat imgGrayscale, int lineUp, int size);
cv::Mat leaveInlineLayer(cv::Mat ILMLine, cv::Mat RPELine, cv::Mat imgGrayscale);
cv::Mat deleteILMLayer(cv::Mat input, cv::Mat ILMLine);
cv::Mat deleteRPELayer(cv::Mat input, cv::Mat RPELine);
cv::Mat deleteNFLayer(cv::Mat input, cv::Mat ILMLine, cv::Mat RPELine, double thresh);
cv::Mat deleteNFLayer2(cv::Mat input, cv::Mat ILMLine, cv::Mat RPELine, double thresh);
cv::Mat RANSAC(cv::Mat input, cv::Mat imgGrayscale, int size);
void  distanceCalculator(cv::Mat input, cv::Mat RPELine, double* sumOfDot, double* distanceOfY);

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// COCTProgramWithGNUPlot2Dlg ��ȭ ����



COCTProgramWithGNUPlot2Dlg::COCTProgramWithGNUPlot2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OCTPROGRAMWITHGNUPLOT2_DIALOG, pParent)
	, m_lenX(_T(""))
	, m_lenY(_T(""))
	, m_lenZ(_T(""))
	, m_volume(_T(""))
	, m_distance(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COCTProgramWithGNUPlot2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_lenX);
	DDV_MaxChars(pDX, m_lenX, 20);
	DDX_Text(pDX, IDC_EDIT_Y, m_lenY);
	DDV_MaxChars(pDX, m_lenY, 20);
	DDX_Text(pDX, IDC_EDIT_Z, m_lenZ);
	DDV_MaxChars(pDX, m_lenZ, 20);
	DDX_Text(pDX, IDC_VOLUME, m_volume);
	DDV_MaxChars(pDX, m_volume, 20);
	DDX_Text(pDX, IDC_DISTANCE, m_distance);
	DDV_MaxChars(pDX, m_distance, 20);
}

BEGIN_MESSAGE_MAP(COCTProgramWithGNUPlot2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COCTProgramWithGNUPlot2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COCTProgramWithGNUPlot2Dlg::OnBnClickedCancel)
	ON_COMMAND(ID_OPEN_VIDEOFILE, &COCTProgramWithGNUPlot2Dlg::OnOpenVideofile)
	ON_COMMAND(ID_IMAGEFILE_CIRRUSOCT, &COCTProgramWithGNUPlot2Dlg::OnImagefileCirrusoct)
	ON_COMMAND(ID_IMAGEFILE_SPECTRALIS, &COCTProgramWithGNUPlot2Dlg::OnImagefileSpectralis)
	ON_COMMAND(ID_FILE_EXIT, &COCTProgramWithGNUPlot2Dlg::OnFileExit)
	ON_COMMAND(ID_HELP_INTRODUCTION, &COCTProgramWithGNUPlot2Dlg::OnHelpIntroduction)
	ON_COMMAND(ID_HELP_ABOUT, &COCTProgramWithGNUPlot2Dlg::OnHelpAbout)
END_MESSAGE_MAP()


// COCTProgramWithGNUPlot2Dlg �޽��� ó����

BOOL COCTProgramWithGNUPlot2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	isAddress = false;
	pixelWidth = 6.188525;
	pixelHeight = 3.228723;
	pixelDepth = 46.875;
	meanDistance = 0;
	imgVerticesCols = 0;
	imgVerticesRows = 0;
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void COCTProgramWithGNUPlot2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void COCTProgramWithGNUPlot2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR COCTProgramWithGNUPlot2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COCTProgramWithGNUPlot2Dlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	imageCount = 0;
	std::vector<cv::Mat> matOfImg;
	CString File, strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("��� �̹��� ���� (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));

	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	// ����ȭ �̹��� ���� result�� ������ ��� �ش� ��η� ���� ��θ� �ű�
	if (isAddress) {
		CString FileAddress;
		CString address("\\result");
		FileAddress = folderPath + address;
		dlg.GetOFN().lpstrInitialDir = FileAddress;
	}
	if (dlg.DoModal() == IDOK) {
		int dotCount = 0;
		POSITION pos = dlg.GetStartPosition();
		cv::Mat imgOriginal;
		while (pos != NULL) {
			File = dlg.GetNextPathName(pos);
			CT2CA fileConvertedAnsiString(File);
			std::string inputFile(fileConvertedAnsiString);
			imgOriginal = cv::imread(inputFile);						// open sample image
			if (imgOriginal.empty()) {                                  // if unable to open image
				break;
			}
			matOfImg.push_back(imgOriginal);
			imageCount++;
		}
		imgVerticesCols = matOfImg[0].cols;
		imgVerticesRows = matOfImg[0].rows;
		imgVertices = new float*[imageCount * matOfImg[0].cols * matOfImg[0].rows];
		for (int j = 0; j < imageCount * matOfImg[0].cols * matOfImg[0].rows; j++) {
			imgVertices[j] = new float[3];
		}
		char dotIndex = 0;
		float dx = 0.0f, dy = 0.0f, dz = 0.0f;
		cv::Mat img;
		for (int z = 0; z < imageCount; z++) {
			img = matOfImg[imageCount - z - 1];
			for (int x = 0; x < img.cols; x++) {
				for (int y = 0; y < img.rows; y++) {
					dotIndex = img.at<cv::Vec3b>(y, x)[0];
					if (dotIndex) {
						imgVertices[dotCount][0] = (double)x * pixelWidth;
						imgVertices[dotCount][1] = (double)y * pixelHeight;
						imgVertices[dotCount][2] = (double)z * pixelDepth;
						dotCount++;
					}
				}
			}
		}
		FILE* fp;
		fopen_s(&fp, ".//data.txt", "w");

		if (fp == NULL) {
			AfxMessageBox(CString("file open failed"));
			CDialogEx::OnCancel();
		}
		for (int i = 0; i < dotCount; i++) {
			fprintf(fp, "%6.3f %6.3f %6.3f\n", imgVertices[i][0], imgVertices[i][1], imgVertices[i][2]);
		}
		fclose(fp);

		int lx = (int)((double)imgVerticesCols * pixelWidth);
		int ly = (int)((double)imgVerticesRows * pixelHeight);
		int lz = (int)((double)imageCount * pixelDepth);

		m_plot.init(".\\gnuplot\\bin\\wgnuplot.exe");
		m_plot.cmd("set style data dots");
		m_plot.cmd("set xlabel \"X(um)\"");
		m_plot.cmd("set ylabel \"Y(um)\"");
		m_plot.cmd("set zlabel \"Z(um)\"");
		m_plot.cmd("splot [0:%d][0:%d][0:%d] '.\\data.txt'", lx , ly, lz);

		OCTDlgOrthogonal dlg2;
		dlg2.m_plot = &m_plot;
		dlg2.DoModal();

	}
	else {
		CDialogEx::OnCancel();
	}
}


void COCTProgramWithGNUPlot2Dlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (imageCount) {
		for (int i = 0; i < imageCount * imgVerticesCols * imgVerticesRows; i++) {
			delete[] imgVertices[i];
		}
		delete[] imgVertices;
	}
	else { // if unable to open image

	}
	CDialogEx::OnCancel();
}


void COCTProgramWithGNUPlot2Dlg::OnOpenVideofile()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("��� ���� ���� (*.avi;*.mp4;)|*.avi;*.mp4||"));

	if (dlg.DoModal() == IDOK) {
		CString txtX, txtY, txtZ;
		isAddress = true;
		filePath.Format(_T("%s\\%s"), dlg.GetFolderPath(), dlg.GetFileName());
		folderPath.Format(_T("%s"), dlg.GetFolderPath());
		txtX.Format(_T("%10.5f"), pixelWidth);
		txtY.Format(_T("%10.5f"), pixelHeight);
		txtZ.Format(_T("%10.5f"), pixelDepth);
		m_lenX = txtX;
		m_lenY = txtY;
		m_lenZ = txtZ;
		UpdateData(false);	// ��ȭ���� ��ü���� ��ȭ���ڷ� �� ���� false
		int cnt = 0;
		CT2CA folderConvertedAnsiString(folderPath);
		std::string outputFolderPath(folderConvertedAnsiString);
		CT2CA txtConvertedAnsiString(filePath);
		std::string address(txtConvertedAnsiString);
		std::string outputFileName;
		CreateDirectory(folderPath + "\\image", NULL);
		// ���� ��ü ����
		cv::VideoCapture capVideo;
		cv::Mat imgFrame;

		// ���� ���� ����
		capVideo.open(address);

		if (!capVideo.isOpened()) {                                                 // if unable to open video file
			std::cout << "\nerror reading video file" << std::endl << std::endl;      // show error message
			_getch();                    // it may be necessary to change or remove this line if not using Windows                 
			return;						// and exit program
		}

		if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
			std::cout << "\nerror: video file must have at least two frames";
			_getch();
			return;
		}

		// ���� ���� ������ ����
		capVideo.read(imgFrame);

		bool blnFirstFrame = true;
		const int c_cMaxFiles = 130;

		// �ݺ��� ����
#pragma omp parallel for
		while (capVideo.isOpened()) {
			cv::Mat imgGrayscale, imgResize;
			cv::Rect partialRect(300, 5, 300, 195);
			cv::Mat imgOriginal = imgFrame(partialRect);						// open sample image
			cv::Rect blackRect(0, (int)(imgOriginal.rows * 0.7), (int)(imgOriginal.cols), 195);
			cv::rectangle(imgOriginal, blackRect, SCALAR_BLACK, -1);
			cv::cvtColor(imgOriginal,				// input image
				imgGrayscale,                       // output image
				CV_BGR2GRAY);						// change image from BGR to GRAY scale
			cv::resize(imgGrayscale, imgResize, cv::Size(924, 616), 0, 0, CV_INTER_LINEAR);

			outputFileName = outputFolderPath + "\\image\\" + std::to_string(cnt) + ".bmp";
			cv::imwrite(outputFileName, imgResize);

			cnt++;
			// now we prepare for the next iteration
			if ((capVideo.get(CV_CAP_PROP_POS_FRAMES)) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {       // if there is at least one more frame
				capVideo.read(imgFrame);                            // read it
			}
			else {                                                  // else
				break;
			}

		}
	}
	else {
		isAddress = false;
	}
}


void COCTProgramWithGNUPlot2Dlg::OnImagefileCirrusoct()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("��� �̹��� ���� (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));

	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if (dlg.DoModal() == IDOK) {
		CString File, txtX, txtY, txtZ;
		isAddress = true;
		int* distanceData = new int[c_cMaxFiles];
		int* dotData = new int[c_cMaxFiles];
		double distance = 0, dot = 0, totalDot = 0;
		int cnt = 0;
		POSITION pos = dlg.GetStartPosition();
		filePath.Format(_T("%s\\"), dlg.GetFolderPath());
		folderPath.Format(_T("%s"), dlg.GetFolderPath());
		txtX.Format(_T("%10.5f"), pixelWidth);
		txtY.Format(_T("%10.5f"), pixelHeight);
		txtZ.Format(_T("%10.5f"), pixelDepth);
		m_lenX = txtX;
		m_lenY = txtY;
		m_lenZ = txtZ;
		UpdateData(false);	// ��ȭ���� ��ü���� ��ȭ���ڷ� �� ���� false
		CT2CA folderConvertedAnsiString(folderPath);
		std::string outputFolderPath(folderConvertedAnsiString);
		CT2CA txtConvertedAnsiString(filePath);
		std::string address(txtConvertedAnsiString);
		CreateDirectory(folderPath + "\\result", NULL);
		UpdateData(true);
#pragma omp parallel for
		while (pos != NULL) {
			File = dlg.GetNextPathName(pos);
			CT2CA fileConvertedAnsiString(File);
			std::string inputFile(fileConvertedAnsiString);
			std::string outputFileName;

			cv::Mat imgGrayscale, imgBlurred, imgBinarized;
			cv::Mat imgOriginal = cv::imread(inputFile);						// open sample image

			if (imgOriginal.empty()) {                                  // if unable to open image
				std::cout << "error: image not read from file\n\n";     // show error message on command line
				_getch();                                               // may have to modify this line if not using Windows
				break;                                              // and exit program
			}

			cv::Rect blackRect(0, (int)(imgOriginal.rows * 0.7), imgOriginal.cols, (int)(imgOriginal.rows * 0.3));
			cv::rectangle(imgOriginal, blackRect, SCALAR_BLACK, -1);
			
			cv::cvtColor(imgOriginal,				// input image
				imgGrayscale,                       // output image
				CV_BGR2GRAY);						// change image from BGR to GRAY scale

			cv::Mat ILMLine = findILMLine(imgGrayscale);	// OTSU Thresholding ������� ILM Layer�� ã�� �Լ�
//			cv::Mat notOtsuILMLine = findILMLine(imgGrayscale, 120, CV_THRESH_BINARY); // OTSU�� �ƴ� �Ӱ谪 ����ȭ ������� ILM Layer�� ã�� �Լ�
//			cv::Mat smoothILMLine = correctILMLine(ILMLine, notOtsuILMLine, imgGrayscale.cols);	// ILM Layer ���� smooth �ϰ� �������ִ� �Լ�
			cv::Mat RPELine = findRPELine(ILMLine, imgGrayscale, 60, 25);		// RPE Layer�� ã�� �Լ�
			cv::Mat smoothRPELine = correctRPELine(RPELine, imgGrayscale, 15, imgGrayscale.cols); // RPE Layer�� �����ϴ� �Լ�
			cv::Mat imgInlineLayer = leaveInlineLayer(ILMLine, smoothRPELine, imgGrayscale);
			cv::Mat imgDeleteNFL = deleteNFLayer(imgInlineLayer, ILMLine, smoothRPELine, 130);	// ���� �̹��� �Ӱ谪 130, �� �̹��� 150~160
			imgDeleteNFL = deleteNFLayer2(imgDeleteNFL, ILMLine, smoothRPELine, 130);	// ��� �������� NF Layer�� 20~40 �ȼ���ŭ �����ϴ� �Լ� 

			cv::threshold(imgDeleteNFL, imgBinarized, 166, 255, CV_THRESH_BINARY);
			distanceCalculator(imgBinarized, smoothRPELine, &dot, &distance);
			distanceData[cnt] = (int)distance;
			dotData[cnt] = (int)dot;
			totalDot += dot;
			outputFileName = outputFolderPath + "\\result\\" + std::to_string(cnt) + ".bmp";
			cv::imwrite(outputFileName, imgBinarized);
			cnt++;

//			cv::imshow("ILMLine", ILMLine);
//			cv::imshow("RPELine", smoothRPELine);
//			cv::waitKey(0);
		}
		double d = 0;
		for (int i = 0; i < cnt; i++) {
			d += ((double)dotData[i] / totalDot) * distanceData[i];
		}
		meanDistance = d * pixelHeight;
		double volume = (totalDot * pixelWidth * pixelHeight* pixelDepth * 1e-9);
		UpdateData(true);
		m_distance.Format(_T("%10.5f"), meanDistance);
		m_volume.Format(_T("%10.8f"), volume);
		UpdateData(false);
	}
	else {
		isAddress = false;
	}
}


void COCTProgramWithGNUPlot2Dlg::OnImagefileSpectralis()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString strFileList;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("��� �̹��� ���� (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));

	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;

	if (dlg.DoModal() == IDOK) {
		CString File, txtX, txtY, txtZ;
		isAddress = true;
		int cnt = 0;
		int key = 60; // �̹��� ������ ���� RPE Layer�� ����ũ �β��� �����ϴ� ����
		int* distanceData = new int[c_cMaxFiles];
		int* dotData = new int[c_cMaxFiles];
		double distance = 0, dot = 0, totalDot = 0;
		POSITION pos = dlg.GetStartPosition();
		filePath.Format(_T("%s\\"), dlg.GetFolderPath());
		folderPath.Format(_T("%s"), dlg.GetFolderPath());
		UpdateData(false);	// ��ȭ���� ��ü���� ��ȭ���ڷ� �� ���� false
		CT2CA folderConvertedAnsiString(folderPath);
		std::string outputFolderPath(folderConvertedAnsiString);
		CT2CA txtConvertedAnsiString(filePath);
		std::string address(txtConvertedAnsiString);
		CreateDirectory(folderPath + "\\result", NULL);
		std::vector<cv::Mat> matOfImg;
		UpdateData(true);

#pragma omp parallel for
		while (pos != NULL) {
			File = dlg.GetNextPathName(pos);
			CT2CA fileConvertedAnsiString(File);
			std::string inputFile(fileConvertedAnsiString);
			std::string outputFileName;

			cv::Mat imgResized, imgGrayscale, imgBlurred, imgBinarized;
			cv::Mat imgOriginal = cv::imread(inputFile);						// open sample image

			if (imgOriginal.empty()) {                                  // if unable to open image
				std::cout << "error: image not read from file\n\n";     // show error message on command line
				_getch();                                               // may have to modify this line if not using Windows
				break;                                               // and exit program
			}

			imgResized = imgDivision(imgOriginal, &key);

			if (!cnt) {
				txtX.Format(_T("%10.5f"), pixelWidth);
				txtY.Format(_T("%10.5f"), pixelHeight);
				txtZ.Format(_T("%10.5f"), pixelDepth);
				m_lenX = txtX;
				m_lenY = txtY;
				m_lenZ = txtZ;
				UpdateData(false);
			}
			cv::Rect blackRect(0, (int)(imgResized.rows * 0.7), imgResized.cols, (int)(imgResized.rows * 0.3));
			cv::rectangle(imgResized, blackRect, SCALAR_BLACK, -1);
			
			cv::cvtColor(imgResized,				// input image
				imgGrayscale,                       // output image
				CV_BGR2GRAY);						// change image from BGR to GRAY scale
			
			cv::Mat ILMLine = findILMLine(imgGrayscale);	// OTSU Thresholding ������� ILM Layer�� ã�� �Լ�
			cv::Mat notOtsuILMLine = findILMLine(imgGrayscale, 120, CV_THRESH_BINARY); // OTSU�� �ƴ� �Ӱ谪 ����ȭ ������� ILM Layer�� ã�� �Լ�
			cv::Mat smoothILMLine = correctILMLine(ILMLine, notOtsuILMLine, imgGrayscale.cols);	// ILM Layer ���� smooth �ϰ� �������ִ� �Լ�
			cv::Mat RPELine = findRPELine(smoothILMLine, imgGrayscale, key, 25);		// RPE Layer�� ã�� �Լ�
			cv::Mat smoothRPELine = correctRPELine(RPELine, imgGrayscale, 15, imgGrayscale.cols); // RPE Layer�� �����ϴ� �Լ�
			cv::Mat imgInlineLayer = leaveInlineLayer(smoothILMLine, smoothRPELine, imgGrayscale);
			cv::Mat imgDeleteNFL = deleteNFLayer(imgInlineLayer, smoothILMLine, smoothRPELine, 150);	// ���� �̹��� �Ӱ谪 130, �� �̹��� 150~160
			
			cv::threshold(imgDeleteNFL, imgBinarized, 166, 255, CV_THRESH_BINARY);
			distanceCalculator(imgBinarized, smoothRPELine, &dot, &distance);
			distanceData[cnt] = (int)distance;
			dotData[cnt] = (int)dot;
			totalDot += dot;
			outputFileName = outputFolderPath + "\\result\\" + std::to_string(cnt) + ".bmp";
			cv::imwrite(outputFileName, imgBinarized);
			cnt++;
		}
		double d = 0;
		for (int i = 0; i < cnt; i++) {
			d += ((double)dotData[i] / totalDot) * distanceData[i];
		}
		std::cout << "cnt :" << cnt << std::endl;
		meanDistance = d * pixelHeight;
		double volume = (totalDot * pixelWidth * pixelHeight* pixelDepth* 1e-9);
		UpdateData(true);
		m_distance.Format(_T("%10.5f"), meanDistance);
		m_volume.Format(_T("%10.8f"), volume);

		UpdateData(false);
	}
	else {
		isAddress = false;
	}
}


void COCTProgramWithGNUPlot2Dlg::OnFileExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (imageCount) {
		for (int i = 0; i < imageCount * imgVerticesCols * imgVerticesRows; i++) {
			delete[] imgVertices[i];
		}
		delete[] imgVertices;
	}
	else { // if unable to open image

	}
	CDialogEx::OnCancel();
}


void COCTProgramWithGNUPlot2Dlg::OnHelpIntroduction()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OCTIntroBox dlg;
	dlg.DoModal();
}


void COCTProgramWithGNUPlot2Dlg::OnHelpAbout()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	OCTProgramAbou dlg;
	dlg.DoModal();
}

/*
void COCTProgramDlg::OnBnClickedBtnPlot2()
{
// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
// ����ȭ �̹����� ����.
// �̹����� x, y, z ������ ���� ���ϰ� �ִ밪���� ������ 255�� ���Ѵ�.
// imshow�� ����.
CString File, strFileList;
CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("��� �̹��� ���� (*.jpg;*.bmp;*.png;*.tif)|*.jpg;*.bmp;*.png;*.tif||"));

dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
dlg.GetOFN().nMaxFile = c_cbBuffSize;


if (dlg.DoModal() == IDOK) {
int imageCount = 0;
std::vector<cv::Mat> matOfImg;
int cnt = 1;

POSITION pos = dlg.GetStartPosition();

while (pos != NULL) {
File = dlg.GetNextPathName(pos);
CT2CA fileConvertedAnsiString(File);
std::string inputFile(fileConvertedAnsiString);

cv::Mat imgOriginal = cv::imread(inputFile);						// open sample image

if (imgOriginal.empty()) {                                  // if unable to open image
break;
}

matOfImg.push_back(imgOriginal);
imageCount = cnt;
cnt++;
}
const int xAxis = matOfImg[0].cols, yAxis = matOfImg[0].rows, zAxis = imageCount;
cv::Mat img;
cv::Mat xOrthographic(yAxis, zAxis, CV_8UC1, cv::Scalar(0));
cv::Mat yOrthographic(xAxis, zAxis, CV_8UC1, cv::Scalar(0));
cv::Mat zOrthographic(yAxis, xAxis, CV_8UC1, cv::Scalar(0));

cv::Mat xArr(yAxis, zAxis, CV_64FC1, cv::Scalar(0));
cv::Mat yArr(xAxis, zAxis, CV_64FC1, cv::Scalar(0));
cv::Mat zArr(yAxis, xAxis, CV_64FC1, cv::Scalar(0));
double val = 0;

// x�� �翵 �����
for (int z = 0; z < imageCount; z++) {
img = matOfImg[z];
for (int y = 0; y < yAxis; y++) {
for (int x = 0; x < xAxis; x++) {
xArr.at<double>(y, z) += img.at<uchar>(y, x);
}
}
}

// y�� �翵 �����
for (int z = 0; z < imageCount; z++) {
img = matOfImg[z];
for (int x = 0; x < xAxis; x++) {
for (int y = 0; y < yAxis; y++) {
yArr.at<double>(x, z) += img.at<uchar>(y, x);
}
}
}

// z�� �翵 �����
for (int y = 0; y < yAxis; y++) {
for (int x = 0; x < xAxis; x++) {
for (int z = 0; z < imageCount; z++) {
img = matOfImg[z];
zArr.at<double>(y, x) += img.at<uchar>(y, x);
}
}
}


// x�� �翵 Matrix �̹���
for (int z = 0; z < imageCount; z++) {
for (int y = 0; y < yAxis; y++) {
val = xArr.at<double>(y, z);
val = round(val / (double)xAxis);
xOrthographic.at<uchar>(y, z) = (uchar)val;
}
}

// y�� �翵 Matrix �̹���
for (int z = 0; z < imageCount; z++) {
for (int x = 0; x < xAxis; x++) {
val = yArr.at<double>(x, z);
val = round(val / (double)yAxis);
yOrthographic.at<uchar>(x, z) = (uchar)val;
}
}

// z�� �翵 Matrix �̹���
for (int y = 0; y < yAxis; y++) {
for (int x = 0; x < xAxis; x++) {
val = zArr.at<double>(y, x);
val = round(val / (double)zAxis);
zOrthographic.at<uchar>(y, x) = (uchar)val;
}
}

cv::equalizeHist(xOrthographic, xOrthographic);
cv::equalizeHist(yOrthographic, yOrthographic);
cv::equalizeHist(zOrthographic, zOrthographic);

cv::imshow("x�� ���翵", xOrthographic);
cv::imshow("y�� ���翵", yOrthographic);
cv::imshow("z�� ���翵", zOrthographic);

cv::waitKey(0);
}
}
*/

cv::Mat imgDivision(cv::Mat input, int* key) {
	cv::Mat imgGrayscale, imgBinarized, output;
	cv::Mat imgOriginal = input.clone();						// open sample image
	bool find = false;
	cv::cvtColor(imgOriginal,				// input image
		imgGrayscale,                       // output image
		CV_BGR2GRAY);						// change image from BGR to GRAY scale

	cv::threshold(imgGrayscale,				// input image
		imgBinarized,						// output image
		180,									// thresholding value
		255,								// maximum value
		CV_THRESH_BINARY);					// thresholding ���
											//	cv::imshow("OTSU", imgBinarized);


	std::vector<std::vector<cv::Point> > morphContours, morphLargeContours, morphSmallContours;							// make contour variables
	cv::Mat imgMorphContours(imgBinarized.size(), CV_8UC1, cv::Scalar(0));							// makes contours image
																									//	cv::imshow("x", imgMorph);
	cv::findContours(imgBinarized, morphContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);		// find contours

	for (unsigned int i = 0; i < morphContours.size(); i++) {
		if (cv::contourArea(morphContours[i]) > 62 && cv::contourArea(morphContours[i]) < 67 &&
			((float)cv::boundingRect(morphContours[i]).width / (float)cv::boundingRect(morphContours[i]).height) < 0.5 &&
			(float)cv::boundingRect(morphContours[i]).width / (float)cv::boundingRect(morphContours[i]).height > 0.3) {
			morphLargeContours.push_back(morphContours[i]);
			find = true;
		}
	}

	if (find) {
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (cv::boundingRect(morphContours[i]).y + cv::boundingRect(morphContours[i]).height == boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height) {
				morphSmallContours.push_back(morphContours[i]);
			}
		}
	}
	else if (!find) {
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (cv::contourArea(morphContours[i]) > 25 && cv::contourArea(morphContours[i]) < 30 &&
				((float)cv::boundingRect(morphContours[i]).width / (float)cv::boundingRect(morphContours[i]).height) < 1.1 &&
				(float)cv::boundingRect(morphContours[i]).width / (float)cv::boundingRect(morphContours[i]).height > 0.9) {
				morphLargeContours.push_back(morphContours[i]);
			}
		}
		for (unsigned int i = 0; i < morphContours.size(); i++) {
			if (cv::boundingRect(morphContours[i]).y + cv::boundingRect(morphContours[i]).height == boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height) {
				morphSmallContours.push_back(morphContours[i]);
			}
		}
	}
	//	cv::drawContours(imgMorphContours, morphLargeContours, -1, SCALAR_WHITE, -1);						// draw contours
	//	cv::drawContours(imgMorphContours, morphSmallContours, -1, SCALAR_WHITE, -1);						// draw contours

	cv::Rect rectContour(boundingRect(morphLargeContours[0]).x, 0,
		imgOriginal.cols - (boundingRect(morphLargeContours[0]).x), boundingRect(morphLargeContours[0]).y + boundingRect(morphLargeContours[0]).height + 10);

	pixelWidth = 200 / (double)boundingRect(morphLargeContours[0]).width; // �ʺ�� 200um 
	pixelHeight = 200 / (double)boundingRect(morphLargeContours[0]).height; // ���̴� 200um
	pixelDepth = 200 / (double)boundingRect(morphSmallContours[0]).height; // �β��� 200um
	output = imgOriginal(rectContour);
	//	cv::rectangle(imgOriginal, rectContour, SCALAR_GREEN, 1);

	if (!find) { *key = 40; }
	return output;
}

// function details ////////////////////////////////////////////////////////////////////////////
/* *@brief ��輱 �̺� �Լ�1
@param input �Է� �迭
@param size �Է� �迭�� �� ��
*/
int* diff(cv::Mat input, int size) {
	int* xData = maximum(input);
	int diffSize = size - 1;
	int *outputData = new int[size];
	int x = 0;
	for (int m = 0; m < size; m++) {
		x = xData[m + 1] - xData[m];
		outputData[m] = x;
	}
	return outputData;
}

////////////////////////////////////////////////////////////////////////////////////
/* *brief ��輱 �̺��Լ�2
@param input �Է� �迭
@param size �Է� �迭�� �� ��
*/
int* diff2(cv::Mat input, int size) {
	int* xData = maximum(input);
	int diffSize = size - 1;
	int *outputData = new int[diffSize];
	int x = 0;
	for (int m = 0; m < diffSize; m++) {
		x = xData[m + 1] - xData[m];
		if (x > 2 || x < -2) {
			outputData[m] = x;
		}
		else { outputData[m] = 0; }
	}
	return outputData;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief ��� �ִ밪 ��� �Լ�
@param input �Է� �迭
*/
int* maximum(cv::Mat input) {
	cv::Mat inputCopy = input.clone();
	uchar* xData = (uchar*)inputCopy.data;
	int *outputData = new int[input.cols];
	int max, val, maxIdx = 0;
	for (int x = 0; x < input.cols; x++) {
		max = xData[x];
		maxIdx = 0;
		for (int y = 1; y < input.rows; y++) {
			val = xData[y * input.cols + x];
			if (val > max) {
				max = val;
				maxIdx = y;
			}
		}
		outputData[x] = maxIdx;
	}
	return outputData;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief ILM Layer�� ã�� �Լ�
@param input �Է� �̹���
@param thresh �̹��� ����ȭ ��谪
@param type �̹��� ����ȭ ���
*/
cv::Mat findILMLine(cv::Mat input, double thresh, int type) {
	cv::Mat imgGrayscale = input.clone();
	cv::Mat imgBlurred, imgBinarized, imgMorphological, imgEdged, imgFilled;

	cv::GaussianBlur(imgGrayscale,          // input image
		imgBlurred,                         // output image
		cv::Size(5, 5),                     // smoothing window width and height in pixels
		15);                               // sigma value, determines how much the image will be blurred

	cv::threshold(imgBlurred,				// input image
		imgBinarized,						// output image
		thresh,									// thresholding value
		255,								// maximum value
		type);					// ��Ʈ������ ����ϴ� Otsu's thresholding ���


	std::vector<std::vector<cv::Point> > contours, possibleContours, reContours;							// make contour variables
	cv::Mat imgContours(imgBinarized.size(), CV_8UC1, cv::Scalar(0));							// makes contours image

	cv::findContours(imgBinarized, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);		// find contours

	for (unsigned int i = 0; i < contours.size(); i++) {
		if (cv::contourArea(contours[i]) > 40) {												// checks contours's area
			possibleContours.push_back(contours[i]);
		}
	}

	cv::drawContours(imgContours, possibleContours, -1, SCALAR_WHITE, -1);						// draw contours


	cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));	// ��Ʈ�� strel�� ���� ���� ��� ��ü ����
	cv::morphologyEx(imgContours, imgMorphological, cv::MORPH_CLOSE, structuringElement3x3);		// �ݱ� ������ �������� �̹����� strel ũ�⸸�� ��κ� ����, ���� ����

	imgEdged = imgMorphological.clone();
	cv::floodFill(imgEdged,					// input image
		cv::Point(300, 0),					// seed Point
		CV_RGB(255, 255, 255));				// ä������ �� ����
											// ������ �迭���� ������ ��ġ�� ����� �κ��� �����ϴ� �˰���, seedPoint ũ�⸸�� �� ȭ�鿡 ���� ĥ�ϴ� ���
	bitwise_not(imgEdged, imgEdged);

	// 2017-11-15 ����: 4.avi ���� ILM ��輱�� y=0�� ������� �߻��ϴ� ���� ����
	cv::findContours(imgEdged, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);		// find contours
	for (unsigned int i = 0; i < contours.size(); i++) {
		if (cv::boundingRect(contours[i]).x + cv::boundingRect(contours[i]).width < imgGrayscale.cols ||
			cv::boundingRect(contours[i]).y > 0 || cv::boundingRect(contours[i]).width == imgGrayscale.cols) {												// checks contours's area
			reContours.push_back(contours[i]);
		}
	}
	cv::drawContours(imgContours, reContours, -1, SCALAR_WHITE, -1);						// draw contours

	imgFilled = (imgContours | imgMorphological);	// ���� OR ����

//	cv::imshow("imgFilled", imgContours);
//	cv::waitKey(0);

	cv::Mat output(imgFilled.size(), CV_8UC1, cv::Scalar(0));				// Internal Limiting Membrane Layer
	uchar* data = (uchar*)imgFilled.data;					// imgFilled �̹����� data ���� ����
	uchar* outputData = (uchar*)output.data;				// imgILMLayer �̹����� data ���� ����
	int* outputCopyData = new int[output.cols];
	int con_y1, con_y2, con_y3;								// y�� ���ǰ�
	int count = 0, sumILM = 0;											// ILM ���� �ִ� y�� ��

																		// �̹��� ������ �������� ������ �˻��Ͽ� ������ �����ϴ� �ݺ���
	for (int x = 0; x < imgFilled.cols; x++) {
		for (int y = 0; y < (imgFilled.rows - 9); y++) {
			con_y1 = data[(y + 1)*imgFilled.cols + x];
			con_y2 = data[(y + 3)*imgFilled.cols + x];
			con_y3 = data[(y + 9)*imgFilled.cols + x];
			if (con_y1 && con_y2 && con_y3) {
				outputCopyData[x] = y;
				sumILM += y;
				count++;
				break;
			}
		}
	}

	int averageILM = 0;
	if (count) {
		averageILM = sumILM / count;	// ILM Layer�� y�� ��� ��ǥ��
	}
	// ������ ã�� ���� �ε����� ��հ����� �޲�
	for (int x = 0; x < output.cols; x++) {
		if (outputCopyData[x] < 0) {
			outputCopyData[x] = averageILM;
		}
		else if (outputCopyData[x] > output.rows) {
			outputCopyData[x] = 0;
		}
	}
	int y = 0;
	for (int k = 0; k < output.cols; k++) {
		y = outputCopyData[k];
		outputData[y * imgFilled.cols + k] = 255;
	}
	delete[] outputCopyData;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief ILM Layer ��輱�� �����ϴ� �Լ�
@param input1 OTSU ����� ����ȭ �̹���, ILM Layer ��輱�� ����
@param input2 thresh�� �̿��� ����� ����ȭ �̹���, input1�� ���Ͽ� ��輱 ����
@param size �̹��� �� ��
*/
cv::Mat correctILMLine(cv::Mat input1, cv::Mat input2, int size) {
	int con1 = 0, con2 = 0;		// condition 1,2
	int leftIdx = 0, rightIdx = 0;
	int count = 0;
	int Y = 0, A = 0, B = 0, C = 0;
	double num = 0;
	int* diffIdx = diff2(input1, size);
	int* outputCopyData = new int[size];
	int* xData1 = maximum(input1);
	int* xData2 = maximum(input2);
	cv::Mat output(input1.size(), CV_8UC1, cv::Scalar(0));
	uchar* outputData = (uchar*)output.data;

	for (int x = 0; x < size; x++) {
		outputCopyData[x] = xData1[x];
		if (xData2[x] - xData1[x] < 15 && xData2[x] - xData1[x] > 0) {
			xData1[x] = xData2[x];
		}
	}

	for (int x = 0; x < size - 3; x++) {
		if (!diffIdx[x] && diffIdx[x + 1] && !con1 && !con2) {
			leftIdx = x;
			con1 = 1;
		}
		else if (!diffIdx[x] && diffIdx[x + 1] && con1 && !con2) {
			con2 = 1;
		}
		else if (diffIdx[x] && !diffIdx[x + 1] && !diffIdx[x + 2] && con2) {
			rightIdx = x + 1;
			count = rightIdx - leftIdx;
			A = xData1[leftIdx];
			B = xData1[rightIdx + 1];
			C = xData1[leftIdx + 1];
			num = (double)(B - C) / (double)count;
			//			std::cout << "count:" << count << " => A:" << A << " B:" << B << " C:" << C  << std::endl;
			for (int m = 0; m < count; m++) {
				Y = A + num * (m + 1);
				if (Y > 0) outputCopyData[leftIdx + m] = Y;
				else outputCopyData[leftIdx + m] = 0;
			}
			con1 = 0;
			con2 = 0;
		}
	}
	int y = 0;
	for (int x = 0; x < output.cols; x++) {
		y = outputCopyData[x];
		outputData[y * output.cols + x] = 255;
	}

	delete diffIdx, xData1, xData2, outputCopyData;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief RPE Layer�� ã�� �Լ�, uchar�� 255 �̻��� ���� ������ �� ���� ������ int * ���
@param ILMLine ILM Layer�� ��輱 �̹���, ����ŷ�� ���� ���
@param imgGrayscale ����ŷ�� ������ ��� �̹���
@param maskHeight1 ù ��° ����ŷ�� ���� ����ũ ��
@param maskHeight2 �� ��° ����ŷ�� ���� ����ũ ��
*/
cv::Mat findRPELine(cv::Mat ILMLine, cv::Mat imgGrayscale, int maskHeight1, int maskHeight2) {
	cv::Mat imgGrayCopy = imgGrayscale.clone();
	cv::Mat imgMasked1 = imgGrayscale.clone();
	int* ILMLineData = maximum(ILMLine);
	uchar* imgGrayCopyData = (uchar*)imgGrayCopy.data;
	uchar* imgMaskedData1 = (uchar*)imgMasked1.data;
	cv::Mat imgMaskBlurred1, imgMaskBlurred2;

	int y = 0;
	for (int x = 0; x < imgGrayscale.cols; x++) {
		y = ILMLineData[x];
		if (y > 10) {
			for (int h = 0; h < maskHeight1; h++) {
				imgMaskedData1[(y + h) * imgGrayscale.cols + x] = 0;
			}
		}
		else {
			for (int h = 0; h < 10; h++) {
				imgMaskedData1[(y + h) * imgGrayscale.cols + x] = 0;
			}
		}
	}

	cv::GaussianBlur(imgMasked1,            // input image
		imgMaskBlurred1,                    // output image
		cv::Size(5, 5),                     // smoothing window width and height in pixels
		1.5);                               // sigma value, determines how much the image will be blurred
											//	cv::imshow("imgMaskLayer", imgMaskBlurred1);

	int* maskMaxIdx = maximum(imgMaskBlurred1);
	cv::Mat imgMasked2(imgGrayCopy.size(), CV_8UC1, cv::Scalar(0));
	uchar* imgMaskedData2 = (uchar*)imgMasked2.data;

	for (int x = 0; x < imgGrayscale.cols; x++) {
		y = maskMaxIdx[x];
		if (y > maskHeight2) {
			for (int h = 0; h < maskHeight2; h++) {
				imgMaskedData2[(y - h) * imgGrayCopy.cols + x] = imgGrayCopyData[(y - h) * imgGrayCopy.cols + x];
			}
		}
	}

	cv::GaussianBlur(imgMasked2,          // input image
		imgMaskBlurred2,                         // output image
		cv::Size(7, 7),                     // smoothing window width and height in pixels
		1.5);                               // sigma value, determines how much the image will be blurred
	maskMaxIdx = maximum(imgMaskBlurred2);

	cv::Mat output(imgGrayscale.size(), CV_8UC1, cv::Scalar(0));
	uchar* outputData = (uchar*)output.data;
	for (int x = 0; x < imgGrayscale.cols; x++) {
		y = maskMaxIdx[x];
		outputData[y*imgGrayscale.cols + x] = 255;
	}
	delete ILMLineData, maskMaxIdx;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief RPE Layer ��輱�� �����ϴ� �Լ�
@param input RPE Layer ��輱 �̹���
@param imgGrayscale ����� ���� ��� �̹���
@param lineUp RPE Layer�� y�� -�������� �󸶳� �ø� ������ �����ϴ� ��
@param size �迭 ũ��
*/
cv::Mat correctRPELine(cv::Mat input, cv::Mat imgGrayscale, int lineUp, int size) {
	int* inputData = maximum(input);
	int* diffIdx = diff2(input, size);
	int* outputCopyData = maximum(input);
	cv::Mat inputRANSAC(imgGrayscale.size(), CV_8UC1, cv::Scalar(0));
	uchar* inputRANSACData = (uchar*)inputRANSAC.data;
	int con1 = 0, con2 = 0, con3 = 0;
	int count = 0, leftIdx = 0, rightIdx = 0, diffNum = 0;
	double num = 0;
	int Y = 0, A = 0, B = 0, C = 0;

	for (int x = 0; x < size - 4; x++) {
		if (diffIdx[x] >= 0 && diffIdx[x + 1] < 0 && !con1 && !con2) {
			leftIdx = x;
			con1 = 1;
			con3 = 0;
		}
		else if (diffIdx[x] <= 0 && diffIdx[x + 1] > 0 && con1 && !con2) {
			con2 = 1;
		}
		else if (diffIdx[x] > 0 && diffIdx[x + 1] <= 0 && diffIdx[x + 2] <= 0 && con2) {
			rightIdx = x + 1;
			count = rightIdx - leftIdx;
			A = inputData[leftIdx];
			B = inputData[rightIdx + 1];
			C = inputData[leftIdx + 1];
			num = (double)(B - C) / (double)count;
			for (int m = 0; m < count; m++) {
				Y = A + num * (double)(m + 1);
				outputCopyData[leftIdx + m] = Y;
			}
			con1 = 0, con2 = 0;
			if (diffNum) {
				con3 = 1;
			}
		}
		else if (diffIdx[x] <= 0 && diffIdx[x + 1] > 0 && con3) {
			con2 = 1;
		}
	}

	int y = 0;
	for (int x = 0; x < inputRANSAC.cols; x++) {
		y = outputCopyData[x];
		inputRANSACData[y * inputRANSAC.cols + x] = 255;
	}

	inputRANSAC = RANSAC(inputRANSAC, imgGrayscale, size);	// RANSAC �Լ� ���

	outputCopyData = maximum(inputRANSAC);
	cv::Mat output(imgGrayscale.size(), CV_8UC1, cv::Scalar(0));
	uchar* outputData = (uchar*)output.data;

	for (int x = 0; x < output.cols; x++) {
		y = outputCopyData[x];
		if (y < 10) { outputData[x] = 255; }
		else { outputData[(y - 10) * output.cols + x] = 255; }
	}
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief ILM Layer �� �κа� RPE Layer �Ʒ� �κ��� �����ϰ� �� ������ ���̾ ����� �Լ�
@param ILMLine ILM Layer ��輱 �̹���
@param RPELine RPE Layer ��輱 �̹���
@param imgGrayscale ����� ���� ��� �̹���
*/
cv::Mat leaveInlineLayer(cv::Mat ILMLine, cv::Mat RPELine, cv::Mat imgGrayscale) {
	int* smoothILMLineData = maximum(ILMLine);
	int* smoothRPELineData = maximum(RPELine);

	cv::Mat imgGrayCopy = imgGrayscale.clone();
	uchar* imgGrayCopyData = (uchar*)imgGrayCopy.data;

	cv::Mat output(imgGrayscale.size(), CV_8UC1, cv::Scalar(0));
	uchar* outputData = (uchar*)output.data;

	// ILM Layer�� RPE Layer ������ �׷��� �̹����� ���� 
	int y1 = 0, y2 = 0;
	for (int x = 0; x <output.cols; x++) {
		y1 = smoothILMLineData[x];
		y2 = smoothRPELineData[x];
		if (y1 < y2) {
			for (int i = y1; i < y2; i++) {
				outputData[i*imgGrayscale.cols + x] = imgGrayCopyData[i*imgGrayscale.cols + x];
			}
		}
	}

//	cv::imshow("imginline", output);
//	cv::waitKey(0);
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief NF Layer�� �����ϴ� ���� �Լ�, ILM Layer ��輱���κ��� ���������� �Ʒ� 20 �ȼ� ����
@param input �Է� �̹���
@param ILMLine ILM Layer ��輱 �̹���
*/
cv::Mat deleteILMLayer(cv::Mat input, cv::Mat ILMLine) {
	int* ILMCopyData = maximum(ILMLine);
	cv::Mat output = input.clone();
	uchar* outputData = (uchar*)output.data;
	int y = 0;
	for (int x = 0; x < input.cols; x++) {
		y = ILMCopyData[x];
		for (int k = 0; k < 30; k++) {
			outputData[(y + k)*output.cols + x] = 0;
		}
	}
	delete ILMCopyData;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief NF Layer�� �����ϴ� ���� �Լ�, RPE Layer ��輱���κ��� ���������� �� 10 �ȼ� ����
@param input �Է� �̹���
@param ILMLine ILM Layer ��輱 �̹���
*/
cv::Mat deleteRPELayer(cv::Mat input, cv::Mat RPELine) {
	int* RPECopyData = maximum(RPELine);
	cv::Mat output = input.clone();
	uchar* outputData = (uchar*)output.data;
	int y = 0;
	for (int x = 0; x < input.cols; x++) {
		y = RPECopyData[x];
		if (y > 10) {
			for (int k = 0; k < 10; k++) {
				outputData[(y - k)*output.cols + x] = 0;
			}
		}
		else {

		}
	}
	delete RPECopyData;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief NF Layer�� �����ϴ� �Լ�
@param input �Է� �̹���
@param ILMLine ILM Layer ��輱 �̹���
@param RPELine RPE Layer ��輱 �̹���
@param thresh �Է� �̹��� ����ȭ�� ���� ��谪, ���� ���� 130, ���ο� ���� 150~160 ���
*/
cv::Mat deleteNFLayer(cv::Mat input, cv::Mat ILMLine, cv::Mat RPELine, double thresh) {
	int* ILMCopyData = maximum(ILMLine);
	int* RPECopyData = maximum(RPELine);
	cv::Mat output;
	cv::Mat imgGaussianBlurred, imgBinarized, imgMorph, imgEdged;

	cv::GaussianBlur(input,            // input image
		imgGaussianBlurred,                    // output image
		cv::Size(5, 5),                     // smoothing window width and height in pixels
		1.5);                               // sigma value, determines how much the image will be blurred

	cv::threshold(imgGaussianBlurred,				// input image
		imgBinarized,						// output image
		thresh,									// thresholding value ������ 130 ���� 150 �̻�
		255,								// maximum value
		CV_THRESH_BINARY);					// ��Ʈ������ ����ϴ� Otsu's threshold ���


	std::vector<std::vector<cv::Point> > morphContours, morphPossibleContours, largeConvexHull;							// make contour variables
	cv::Mat imgConvexHull(input.size(), CV_8UC1, cv::Scalar(0));							// makes contours image
	cv::Mat imgMorphContours(input.size(), CV_8UC1, cv::Scalar(0));
	cv::findContours(imgBinarized, morphContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);		// find contours
	float dblAspectRatio = 0;
	bool isConvex = false;
	cv::Point idx;
	for (unsigned int i = 0; i < morphContours.size(); i++) {
		dblAspectRatio = (float)cv::boundingRect(morphContours[i]).width / (float)cv::boundingRect(morphContours[i]).height;
		idx.x = cv::boundingRect(morphContours[i]).x;
		idx.y = cv::boundingRect(morphContours[i]).y;

		if (dblAspectRatio < 1.5 && cv::boundingRect(morphContours[i]).area() < 1000 &&
			abs(ILMCopyData[idx.x] - idx.y) > 15) {
			morphPossibleContours.push_back(morphContours[i]);
			isConvex = true;

		}
	}

	std::vector<cv::Point> convexHulls;
	std::vector<cv::Point> pts;

	for (size_t i = 0; i< morphPossibleContours.size(); i++)
		for (size_t j = 0; j< morphPossibleContours[i].size(); j++)
			pts.push_back(morphPossibleContours[i][j]);
	if (isConvex) {
		convexHull(pts, convexHulls);

		cv::polylines(imgConvexHull, convexHulls, true, SCALAR_WHITE, 2);
		cv::findContours(imgConvexHull, largeConvexHull, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		cv::drawContours(imgConvexHull, largeConvexHull, -1, SCALAR_WHITE, -1);
	}

//	cv::imshow("imgConvexHull", imgConvexHull);
//	cv::waitKey(0);

	imgEdged = imgConvexHull.clone();
	output = (input & imgEdged);
	output = deleteILMLayer(output, ILMLine);
	output = deleteRPELayer(output, RPELine);

	delete ILMCopyData, RPECopyData;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief NF Layer�� �����ϴ� �Լ�
@param input �Է� �̹���
@param ILMLine ILM Layer ��輱 �̹���
@param RPELine RPE Layer ��輱 �̹���
@param thresh �Է� �̹��� ����ȭ�� ���� ��谪, ���� ���� 130, ���ο� ���� 150~160 ���
*/
cv::Mat deleteNFLayer2(cv::Mat input, cv::Mat ILMLine, cv::Mat RPELine, double thresh) {
	int* ILMCopyData = maximum(ILMLine);
	int* RPECopyData = maximum(RPELine);
	cv::Mat output;
	cv::Mat imgGaussianBlurred, imgBinarized;


	output = input.clone();
	uchar* outputData = (uchar*)output.data;
	int y = 0;
	int x;

	for (x = 0; x < input.cols * 0.5; x++) {
		y = ILMCopyData[x];
		for (int k = 0; k < 20; k++) {
				outputData[(y + k)*output.cols + x] = 0;
		}
	}

	for (x = round(input.cols * 0.5); x < input.cols * 0.75; x++) {
		y = ILMCopyData[x];
		for (int k = 0; k < 30; k++) {
				outputData[(y + k)*output.cols + x] = 0;
		}

	}

	for (x = round(input.cols * 0.75); x < input.cols; x++) {
		y = ILMCopyData[x];
		for (int k = 0; k < 40; k++) {
				outputData[(y + k)*output.cols + x] = 0;
		}
	}

	delete ILMCopyData, RPECopyData;
	return output;
}

/* *@brief ROI�� �����Ͽ� ����ϴ� �Լ�
@param input NF Layer ���� ���ŵ� �̹���
@param imgOriginal ����� ���� ���� �̹���
*/
/*
cv::Mat selectROI(cv::Mat input, cv::Mat imgOriginal) {
cv::Mat imgThresh, imgMorph;
cv::Mat output = imgOriginal.clone();
cv::GaussianBlur(input,            // input image
input,                    // output image
cv::Size(5, 5),                     // smoothing window width and height in pixels
1.5);                               // sigma value, determines how much the image will be blurred
cv::threshold(input,				// input image
imgThresh,						// output image
166,									// thresholding value
255,								// maximum value
CV_THRESH_BINARY);					// ��Ʈ������ ����ϴ� Otsu's threshold ���
//	cv::imshow("con0", imgThresh);
//	cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));	// ��Ʈ�� strel�� ���� ���� ��� ��ü ����
cv::Mat structuringElement9x9 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));	// ��Ʈ�� strel�� ���� ���� ��� ��ü ����
//	cv::morphologyEx(imgThresh, imgMorph, cv::MORPH_OPEN, structuringElement3x3);
//	cv::imshow("con1", imgMorph);
cv::morphologyEx(imgThresh, imgMorph, cv::MORPH_DILATE, structuringElement9x9);
//	cv::imshow("con2", imgMorph);

std::vector<std::vector<cv::Point> > contours;
cv::findContours(imgMorph, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

for (int i = 0; i < contours.size(); i++) {
cv::rectangle(output, cv::boundingRect(contours[i]), SCALAR_GREEN, 1);
}

return output;
}
*/

////////////////////////////////////////////////////////////////////////////////////
/* *@brief RANSAC �Լ�, ���ø��� ���� �̾Ƴ� �������� ���� ��� ���Ͽ� �ٻ�� �������� ���ϴ� �Լ�
@param input �������� �׸��� ���� �Է� �̹���
@param imgGrayscale ����� ���� ��� �̹���
@param size �̹��� ũ��
*/
cv::Mat RANSAC(cv::Mat input, cv::Mat imgGrayscale, int size) {
	int* inputData = maximum(input);
	cv::Mat output(imgGrayscale.size(), CV_8UC1, cv::Scalar(0));
	uchar* outputData = (uchar*)output.data;

	int* x = new int[size];
	int* y = new int[size];
	for (int i = 0; i < size; i++)
	{
		x[i] = i;
		y[i] = inputData[i];
	}

	//-------------------------------------------------------------- build matrix
	cv::Mat A1(size, 3, CV_64FC1);
	cv::Mat B1(size, 1, CV_64FC1);
	for (int i = 0; i<size; i++) {
		A1.at<double>(i, 0) = x[i] * x[i];
	}
	for (int i = 0; i<size; i++) {
		A1.at<double>(i, 1) = x[i];
	}
	for (int i = 0; i<size; i++) {
		A1.at<double>(i, 2) = 1.0;
	}

	for (int i = 0; i<size; i++) {
		B1.at<double>(i, 0) = y[i];
	}

	//-------------------------------------------------------------- RANSAC fitting 
	int n_data = 100;
	int N = 100;	//iterations 
	double T = 100;   // residual threshold

	int n_sample = 3;
	int max_cnt = 0;
	cv::Mat best_model(3, 1, CV_64FC1);

	for (int i = 0; i<N; i++) {
		//random sampling - 3 point  
		int k[3] = { -1, };
		k[0] = floor((rand() % size + 1)) + 1;

		do
		{
			k[1] = floor((rand() % size + 1)) + 1;
		} while (k[1] == k[0] || k[1]<0);

		do
		{
			k[2] = floor((rand() % size + 1)) + 1;
		} while (k[2] == k[0] || k[2] == k[1] || k[2]<0);

		printf("random sample : %d %d %d\n", k[0], k[1], k[2]);

		//model estimation
		cv::Mat AA(3, 3, CV_64FC1);
		cv::Mat BB(3, 1, CV_64FC1);
		for (int j = 0; j<3; j++)
		{
			AA.at<double>(j, 0) = x[k[j]] * x[k[j]];
			AA.at<double>(j, 1) = x[k[j]];
			AA.at<double>(j, 2) = 1.0;

			BB.at<double>(j, 0) = y[k[j]];
		}

		cv::Mat AA_pinv(3, 3, CV_64FC1);
		invert(AA, AA_pinv, cv::DECOMP_SVD);

		cv::Mat X = AA_pinv * BB;

		//evaluation 
		cv::Mat residual(size, 1, CV_64FC1);
		residual = cv::abs(B1 - A1*X);
		int cnt = 0;
		for (int j = 0; j<size; j++)
		{
			double data = residual.at<double>(j, 0);

			if (data < T)
			{
				cnt++;
			}
		}

		if (cnt > max_cnt)
		{
			best_model = X;
			max_cnt = cnt;
		}
	}

	//------------------------------------------------------------------- optional LS fitting 
	cv::Mat residual = cv::abs(A1*best_model - B1);
	std::vector<int> vec_index;
	for (int i = 0; i<size; i++)
	{
		double data = residual.at<double>(i, 0);
		if (data < T)
		{
			vec_index.push_back(i);
		}
	}

	cv::Mat A2(vec_index.size(), 3, CV_64FC1);
	cv::Mat B2(vec_index.size(), 1, CV_64FC1);

	for (int i = 0; i<vec_index.size(); i++)
	{
		A2.at<double>(i, 0) = x[vec_index[i]] * x[vec_index[i]];
		A2.at<double>(i, 1) = x[vec_index[i]];
		A2.at<double>(i, 2) = 1.0;

		B2.at<double>(i, 0) = y[vec_index[i]];
	}

	cv::Mat A2_pinv(3, vec_index.size(), CV_64FC1);
	invert(A2, A2_pinv, cv::DECOMP_SVD);

	cv::Mat X = A2_pinv * B2;

	//Drawing
	cv::Mat F = A1*X;
	printf("matrix F : cols =%d, rows=%d\n", F.cols, F.rows);

	int y2 = 0;
	for (int x = 0; x < input.cols; x++) {
		y2 = abs(F.at<double>(x, 0));
		outputData[y2 * output.cols + x] = 255;
	}

	delete inputData, x, y;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////
/* *@brief �����߽� ��� �Լ�
@param input �Է� �̹���
@param imgGrayscale ���� �߽��� ������ �Ǵ� RPE���� ����
@param sumOfDot �� ��
@param disanceOfY ��� ���� �߽�
*/
void distanceCalculator(cv::Mat input, cv::Mat RPELine, double* sumOfDot, double* distanceOfY) {
	uchar* imgData = (uchar*)input.data;
	int* RPELineData = maximum(RPELine);
	int imgIdx = 0;
	double dotNum = 0, sumOfX = 0, sumOfY = 0, yIdx = 0;
	for (int x = 0; x < input.cols; x++) {
		for (int y = 0; y < input.rows; y++) {
			imgIdx = imgData[y * input.cols + x];
			if (imgIdx) {
				sumOfX += x;
				sumOfY += y;
				dotNum++;
			}
		}
	}
	if (dotNum) {
		sumOfX = round(sumOfX / dotNum);
		sumOfY = round(sumOfY / dotNum);
	}
	yIdx = RPELineData[(int)sumOfX];
	*distanceOfY = yIdx - sumOfY;
	*sumOfDot = dotNum;
}


