#include "pch.h"
#include "OCCTViewer.h"
#include <XCAFApp_Application.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <AIS_Shape.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <WNT_Window.hxx>
OCCTViewer::OCCTViewer()
{
	Handle(Aspect_DisplayConnection) aDisplayConnection;
	// create a graphic driver from default connection
	m_pGraphicsDriver = new OpenGl_GraphicDriver(aDisplayConnection);
	m_pV3dViewer = new V3d_Viewer(m_pGraphicsDriver);

	m_pV3dViewer->SetComputedMode(Standard_True);

	// Set default lighting
	m_pV3dViewer->SetDefaultLights();
	m_pV3dViewer->SetLightOn();

	m_pV3dView = m_pV3dViewer->CreateView();

	// Initialize interactive context for the viewer.
	m_pAISContext = new AIS_InteractiveContext(m_pV3dViewer);

	// Default display mode is wireframe, change it to shaded mode.
	m_pAISContext->SetDisplayMode(AIS_Shaded, Standard_True);

	// Default selection mode is wireframe, change it to shaded mode.
	const Handle(Prs3d_Drawer)& selectionStyle = m_pAISContext->SelectionStyle();
	selectionStyle->SetColor(static_cast<Quantity_NameOfColor>(Quantity_NOC_YELLOW));
	selectionStyle->SetDisplayMode(AIS_Shaded);

	const Handle(Prs3d_Drawer)& highlightStyle = m_pAISContext->HighlightStyle();
	highlightStyle->SetDisplayMode(AIS_Shaded);
	//highlightStyle->SetTransparency(0.5);

	m_pAISContext->SetToHilightSelected(Standard_False);
	m_pAISContext->DisableDrawHiddenLine();
}

OCCTViewer::~OCCTViewer()
{
}

void OCCTViewer::SetWindow(HWND hwnd)
{
	// Create a view from a viewer and display in the window whose hande is hwnd.
	Handle(WNT_Window) aWNTWindow = new WNT_Window(hwnd);
	m_pV3dView->SetWindow(aWNTWindow);

	if (!aWNTWindow->IsMapped()) aWNTWindow->Map();

	Graphic3d_RenderingParams& aParams = m_pV3dView->ChangeRenderingParams();
	aParams.IsAntialiasingEnabled = Standard_True;
	aParams.NbMsaaSamples = 8;
	m_bgColor1 = Quantity_NOC_SKYBLUE;
	m_bgColor2 = Quantity_NOC_GRAY;
	m_pV3dView->SetBgGradientColors(m_bgColor1, m_bgColor2, Aspect_GFM_VER);
}

void OCCTViewer::ReadFile(CString fileName)
{
	Handle(TDocStd_Application) app = XCAFApp_Application::GetApplication();
	Handle(TDocStd_Document) doc;
	app->NewDocument("XmlXCAF", doc);

	// Reader
	STEPCAFControl_Reader reader;
	CT2A asciiStr(fileName);  
	IFSelect_ReturnStatus status = reader.ReadFile(asciiStr);
	if (status != IFSelect_RetDone)
	{
		std::cerr << "Error: cannot read STEP file " << fileName << std::endl;
		return ;
	}

	// Transfer into the document
	if (!reader.Transfer(doc))
	{
		std::cerr << "Error: STEP transfer failed" << std::endl;
		return ;
	}
	DisplayModel(doc);
}

void OCCTViewer::DisplayModel(Handle(TDocStd_Document) doc)
{
	/*Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
	if (shapeTool.IsNull())
	{
		std::cerr << "Error: cannot get shape tool" << std::endl;
		return;
	}

	TDF_LabelSequence labels;
	shapeTool->GetFreeShapes(labels);

	for (Standard_Integer i = 1; i <= labels.Length(); ++i)
	{
		TDF_Label label = labels.Value(i);
		TopoDS_Shape shape = shapeTool->GetShape(label);

		if (!shape.IsNull())
		{
			Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
			m_pAISContext->Display(aisShape, Standard_True);
		}
	}

	m_pV3dView->FitAll();
	m_pV3dView->Redraw();*/


	for (XCAFPrs_DocumentExplorer aDocExp(doc, XCAFPrs_DocumentExplorerFlags_None); aDocExp.More(); aDocExp.Next())
	{
		const XCAFPrs_DocumentNode& aNode = aDocExp.Current();
		if (/*theToExplode*/true)
		{
			if (aNode.IsAssembly) { continue; } // handle only leaves
		}
		else
		{
			if (aDocExp.CurrentDepth() != 0) { continue; } // handle only roots
		}

		Handle(XCAFPrs_AISObject) aPrs = new XCAFPrs_AISObject(aNode.RefLabel);
		if (!aNode.Location.IsIdentity()) { aPrs->SetLocalTransformation(aNode.Location); }

		// AIS object's owner is an application-owned property; it is set to string object in this sample
		aPrs->SetOwner(new TCollection_HAsciiString(aNode.Id));

		m_pAISContext->Display(aPrs, AIS_Shaded, 0, false);
	}
	m_pAISContext->UpdateCurrentViewer();
}
