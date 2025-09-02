#pragma once
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
class OCCTViewer
{
private:
	Handle_OpenGl_GraphicDriver m_pGraphicsDriver;
	Handle_V3d_Viewer m_pV3dViewer;
	Handle_AIS_InteractiveContext m_pAISContext;
	Handle_V3d_View m_pV3dView;
	Quantity_Color m_bgColor1, m_bgColor2;
public:
	OCCTViewer();
	~OCCTViewer();
	void SetWindow(HWND hwnd);
	void ReadFile(CString fileName);
	void DisplayModel(Handle(TDocStd_Document) doc);

};
