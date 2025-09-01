#include "pch.h"
#include "OCCTViewer.h"
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