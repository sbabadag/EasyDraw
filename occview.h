#pragma once

#include <QWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

class OccView : public QWidget
{
    Q_OBJECT

public:
    explicit OccView(QWidget* parent = nullptr);
    ~OccView();

    void init();
    void fitAll();

protected:
    virtual QPaintEngine* paintEngine() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
    Handle(V3d_Viewer) m_viewer;
};
