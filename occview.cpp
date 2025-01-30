#include "occview.h"
#include <OpenGl_GraphicDriver.hxx>
#include <QMouseEvent>
#include <Aspect_Window.hxx>
#include <WNT_Window.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_TypeOfTriedronPosition.hxx>
#include <Graphic3d_GraphicDriver.hxx>

OccView::OccView(QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    init();
}

OccView::~OccView()
{
}

void OccView::init()
{
    Handle(Aspect_DisplayConnection) display_connection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphic_driver = new OpenGl_GraphicDriver(display_connection);

#ifdef _WIN32
    Handle(WNT_Window) window = new WNT_Window((Aspect_Handle)winId(), Quantity_NOC_MATRAGRAY);
#else
    Handle(Xw_Window) window = new Xw_Window(display_connection, (Window)winId());
#endif

    m_viewer = new V3d_Viewer(graphic_driver);
    m_view = m_viewer->CreateView();
    m_view->SetWindow(window);
    if (!window->IsMapped()) {
        window->Map();
    }

    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_view->SetBackgroundColor(Quantity_NOC_BLACK);
    m_view->MustBeResized();
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);

    fitAll();
}

void OccView::fitAll()
{
    m_view->FitAll();
    m_view->ZFitAll();
}

QPaintEngine* OccView::paintEngine() const
{
    return nullptr;
}

void OccView::paintEvent(QPaintEvent* event)
{
    m_view->Redraw();
}

void OccView::resizeEvent(QResizeEvent* event)
{
    if (!m_view.IsNull())
    {
        m_view->MustBeResized();
    }
}

void OccView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        m_view->StartRotation(event->pos().x(), event->pos().y());
}

void OccView::mouseReleaseEvent(QMouseEvent* event)
{
}

void OccView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        m_view->Rotation(event->pos().x(), event->pos().y());
    }
}

void OccView::wheelEvent(QWheelEvent* event)
{
    Standard_Integer x = event->position().x();
    Standard_Integer y = event->position().y();
    
    m_view->StartZoomAtPoint(x, y);
    const Standard_Integer delta = event->angleDelta().y();
    if (delta > 0) {
        m_view->ZoomAtPoint(x, y, x + delta, y + delta);
    } else {
        m_view->ZoomAtPoint(x, y, x + std::abs(delta), y + std::abs(delta));
    }
}
