#include "windows7.h"
#include <QBitmap>
#include <QPixmap>

Windows7::Windows7():
    m_taskbarInterface(NULL)
{
}

Windows7::~Windows7()
{
    if (m_taskbarInterface)
    {
        m_taskbarInterface->Release();
    }
}

void Windows7::initTaskbarButton(QWidget *parent)
{
    m_parent = parent;
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**> (&(m_taskbarInterface)));

    if (SUCCEEDED(hr))
    {
        hr = m_taskbarInterface->HrInit();

        if (FAILED(hr))
        {
            m_taskbarInterface->Release();
            m_taskbarInterface = NULL;
            return;
        }
        updateOverlayIcon(false);
        ToolbarSetImages();
        ToolbarButtonsInit();
    }
}

void Windows7::updatePorgressBarState(TBPFLAG state)
{
    if (m_taskbarInterface)
    {
         m_taskbarInterface->SetProgressState(m_parent->winId(), state);
    }
}

void Windows7::updateProgressBarValue(int value)
{
    if (m_taskbarInterface)
    {
        m_taskbarInterface->SetProgressValue(m_parent->winId(), value, 100);
    }
}

void Windows7::updateOverlayIcon(bool isConnected)
{
    if (m_taskbarInterface)
    {
        QString icon_res(":/icon/toolbars/" + QString(isConnected ? "connected.png" : "disconnected.png"));
        // a NULL icon will reset the current overlay icon
        SetOverlayIcon(QIcon(icon_res), "Description");
    }
}

void Windows7::SetOverlayIcon(const QIcon& icon, QString description)
{
    if (m_taskbarInterface)
    {

        HICON overlay_icon = icon.isNull() ? NULL : icon.pixmap(48).toWinHICON();
        m_taskbarInterface->SetOverlayIcon(m_parent->winId(), overlay_icon, description.toStdWString().c_str());

        if (overlay_icon)
        {
            DestroyIcon(overlay_icon);
        }
    }
    return;
}

void Windows7::ToolbarButtonsInit()
{
    static const char *tooltips[TASKBAR_BUTTON_NUMBER] = {QT_TR_NOOP("Start all"), QT_TR_NOOP("Stop all"), QT_TR_NOOP("Purge"), QT_TR_NOOP("Power of Aria2c")};

    for (int index = 0; index < TASKBAR_BUTTON_NUMBER; index++)
        {
        wcscpy(m_thbButtons[index].szTip, tr(tooltips[index]).toStdWString().c_str());

        m_thbButtons[index].iId = IDTB_FIRST + index;
        m_thbButtons[index].iBitmap = index;
        m_thbButtons[index].dwMask =(THUMBBUTTONMASK)(THB_BITMAP | THB_FLAGS | THB_TOOLTIP);
        m_thbButtons[index].dwFlags = THBF_DISABLED;

    }

    if (m_taskbarInterface)
                m_taskbarInterface->ThumbBarAddButtons(m_parent->winId(), TASKBAR_BUTTON_NUMBER, m_thbButtons);
}

void Windows7::ToolbarSetImages()
{
    QPixmap img;
    QBitmap mask;
    HIMAGELIST himl = ImageList_Create(20, 20, ILC_COLOR32, TASKBAR_BUTTON_NUMBER, 0);

    img = QIcon(":/icon/toolbars/unpauseall.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/icon/toolbars/pauseall.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/icon/toolbars/purgedownloadresult.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    img = QIcon(":/icon/toolbars/poweroff.png").pixmap(20);
    mask  = img.createMaskFromColor(Qt::transparent);
    ImageList_Add(himl, img.toWinHBITMAP(QPixmap::PremultipliedAlpha), mask.toWinHBITMAP());

    if (m_taskbarInterface)
                m_taskbarInterface->ThumbBarSetImageList(m_parent->winId(), himl);

    ImageList_Destroy(himl);
}

void Windows7::ToolbarButtonsUpdate()
{
    if (m_taskbarInterface)
    {
        m_taskbarInterface->ThumbBarUpdateButtons(m_parent->winId(), TASKBAR_BUTTON_NUMBER, m_thbButtons);
    }
}

void Windows7::ToolbarButtonEnabled(int index, bool state)
{
    if (m_taskbarInterface)
    {
        m_thbButtons[index].dwFlags = state ? THBF_ENABLED : THBF_DISABLED;
        m_taskbarInterface->ThumbBarUpdateButtons(m_parent->winId(), TASKBAR_BUTTON_NUMBER, m_thbButtons);
    }
}
