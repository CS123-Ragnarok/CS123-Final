#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "scene/SceneviewScene.h"
#include "terrain/terrain.h"
#include "camera/OrbitingCamera.h"


class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();
    Camera *getCamera();

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;
    bool m_isDragging;
    int m_frame_count;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    std::unique_ptr<SceneviewScene> m_scene;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<OrbitingCamera> m_defaultOrbitingCamera;

private slots:
    void tick();

};

#endif // VIEW_H
