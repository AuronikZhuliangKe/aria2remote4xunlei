#ifndef WINDOWS7_H
#define WINDOWS7_H
#include "winutils.h"
#include <QIcon>
#include <QString>
#include <QWidget>
#include <QtGui/QApplication>

class Windows7
{
    Q_DECLARE_TR_FUNCTIONS(GeoIP)
public:
    Windows7();
    ~Windows7();
    void initTaskbarButton(QWidget *parent);

    //toolbar overlay icon
    void updateOverlayIcon(bool isConnected);

    //toolbar progress bar
    void updateProgressBarValue(int value);
    void updatePorgressBarState(TBPFLAG state);

    //toolbar button
    void ToolbarButtonsUpdate();
    void ToolbarButtonsInit();
    void ToolbarSetImages();
    int getToolbarButtonState() { return m_thbButtons[0].iBitmap; }
    void ToolbarButtonEnabled(int index, bool state);
private:
    ITaskbarList3* m_taskbarInterface;
    QWidget *m_parent;
    THUMBBUTTON m_thbButtons[TASKBAR_BUTTON_NUMBER];
    void SetOverlayIcon(const QIcon &icon, QString description);
};

#endif // WINDOWS7_H
