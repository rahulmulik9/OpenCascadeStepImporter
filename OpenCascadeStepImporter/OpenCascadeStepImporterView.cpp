
// OpenCascadeStepImporterView.cpp : implementation of the COpenCascadeStepImporterView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenCascadeStepImporter.h"
#endif

#include "OpenCascadeStepImporterDoc.h"
#include "OpenCascadeStepImporterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCascadeStepImporterView

IMPLEMENT_DYNCREATE(COpenCascadeStepImporterView, CView)

BEGIN_MESSAGE_MAP(COpenCascadeStepImporterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenCascadeStepImporterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// COpenCascadeStepImporterView construction/destruction

COpenCascadeStepImporterView::COpenCascadeStepImporterView() noexcept
{
	// TODO: add construction code here

}

COpenCascadeStepImporterView::~COpenCascadeStepImporterView()
{
	
}

BOOL COpenCascadeStepImporterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void COpenCascadeStepImporterView::ImportFile(CString filname)
{
	m_pViwer->ReadFile(filname);
}

void COpenCascadeStepImporterView::SetView() {

	m_pViwer = std::make_unique<OCCTViewer>();
	m_pViwer->SetWindow(GetSafeHwnd());
}

// COpenCascadeStepImporterView drawing

void COpenCascadeStepImporterView::OnDraw(CDC* /*pDC*/)
{
	COpenCascadeStepImporterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// COpenCascadeStepImporterView printing


void COpenCascadeStepImporterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenCascadeStepImporterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenCascadeStepImporterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenCascadeStepImporterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COpenCascadeStepImporterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenCascadeStepImporterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenCascadeStepImporterView diagnostics

#ifdef _DEBUG
void COpenCascadeStepImporterView::AssertValid() const
{
	CView::AssertValid();
}

void COpenCascadeStepImporterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenCascadeStepImporterDoc* COpenCascadeStepImporterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenCascadeStepImporterDoc)));
	return (COpenCascadeStepImporterDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenCascadeStepImporterView message handlers
