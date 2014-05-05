#include "player.h"

#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBrowser>
#include <QFile>
#include <QScrollBar>
#include <QMenuBar>
#include <QTimer>
#include <QApplication>
#include <QDesktopServices>
#include <QProcess>
#include <QSettings>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

// 3rd library
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "QxJson.h"

// ui
#include "aboutui.h"
#include "projectconfigui.h"
#include "createprojectui.h"
#include "consoleui.h"
#include "logindialog.h"
#include "preferenceui.h"

#define kPortrait               1
#define kLandscape              2
#define kOpenRecentFiles        "recents"
#define kDefaultMaxRecents      "maxfiles"
#define kDefaultMaxRecentValue  5
#define kRecentItemTitle        "title"
#define kRecentItemArgs         "args"
#define MODULE_NAME_SEPARATOR   "."
#define MODULE_NAME_CORE        "core"
#define MENU_BAR_FIXED_HEIGHT   25

#define EDITOR_CALL_LUA         "LUA_Interface"
#define SETTINGS_DEMO_LIST      "demoList"

#ifdef Q_OS_WIN
static const QString shortcutFileTemplate("%1/%2 %3\n");
#else
static const QString shortcutFileTemplate("#!/bin/sh\n"
                                          "\n"
                                          "cd %1\n"
                                          "./%2 %3 &\n");
#endif

// keyboard shortcuts
// we can change the keyboard shortcuts from here !
enum {
    ShortCut_NEW_PROJECT     = Qt::CTRL + Qt::Key_N,
    ShortCut_NEW_PLAYER      = Qt::CTRL + Qt::SHIFT + Qt::Key_N,
    ShortCut_OPEN            = Qt::CTRL + Qt::Key_O,
    ShortCut_CLOSE           = Qt::CTRL + Qt::Key_W,

    ShortCut_BUILD_IOS       = Qt::CTRL + Qt::Key_B,
    ShortCut_BUILD_ANDROID   = Qt::CTRL + Qt::SHIFT + Qt::Key_B,
#ifdef Q_OS_MAC
    ShortCut_RELAUNCH        = Qt::CTRL + Qt::Key_R,
#else
    ShortCut_RELAUNCH        = Qt::Key_F5,
#endif

    ShortCut_SCALE_100  = Qt::CTRL + Qt::Key_0,
    ShortCut_SCALE_75   = Qt::CTRL + Qt::Key_6,
    ShortCut_SCALE_50   = Qt::CTRL + Qt::Key_5,
    ShortCut_SCALE_25   = Qt::CTRL + Qt::Key_4,

    ShortCut_SNAPSHOT   = Qt::CTRL + Qt::Key_S
};

struct ScaleActionData
{
    QString         text;
    QKeySequence    keySequence;
    QVariant        userData;
};

Player::Player(QObject *parent)
    : QObject(parent)
    , m_renderWidget(0)
    , m_mainMenu(0)
    , m_openRecentMenu(0)
    , m_consoleUI(0)
    , m_demoWidget(0)
#ifdef Q_OS_WIN
    , m_mainWindow(0)
	, m_container(0)
#endif
{
    qRegisterMetaType<const char *>("const char *");
}

Player::~Player()
{
    CC_SAFE_DELETE(m_mainMenu);
    CC_SAFE_DELETE(m_consoleUI);
    CC_SAFE_DELETE(m_demoWidget);
}

Player *Player::instance()
{
    static Player *player = NULL;
    if (!player)
    {
        player = new Player;
    }
    return player;
}

void Player::setProjectConfig(const ProjectConfig& config)
{
    m_projectConfig = config;
    initScreenMenu();

    // console UI
    if (m_projectConfig.isShowConsole() && m_consoleUI)
    {
        onShowConsole();
    }

    if (m_writeDebugAction)
    {
        m_writeDebugAction->setChecked(m_projectConfig.isWriteDebugLogToFile());
    }
}

ProjectConfig Player::getProjectConfig()
{
    return m_projectConfig;
}

int Player::openURL(lua_State *L)
{
    //从栈中读入实部，虚部
    const char *path = lua_tostring(L, 1);
    Player::instance()->onOpenURL(path);
    return 0;
}

int Player::openQuickDemoWithWebView(lua_State * /*L*/)
{
    Player::instance()->onOpenQuickDemoWebview();
    return 0;
}

int Player::newProject(lua_State * /*L*/)
{
    CreateProjectUI ui(0);
    if (ui.exec() == QDialog::Accepted) {
        ui.createNewProject();
    }
    return 0;
}

/**
 * @brief Player::onOpenProject
 * save the recent menu order and restart the player with args
 */
void Player::onOpenProject()
{
    ProjectConfig config = Player::instance()->getProjectConfig();
    ProjectConfigUI ui(config, 0);
    if (ui.exec() == QDialog::Accepted)
    {
        config = ui.getProjectConfig();

        // save recent menu order

        QSettings settings;
        QVariantList recents = settings.value(kOpenRecentFiles, QVariantList()).toList();
        int maxRecent = settings.value(kDefaultMaxRecents, kDefaultMaxRecentValue).toInt();
        QVariantMap newItem;
        newItem[kRecentItemTitle] = config.getProjectDir().data();
        QStringList args = QString::fromStdString(config.makeCommandLine()).split(" ");
        newItem[kRecentItemArgs]  = args;
        recents.removeAll(newItem);
        recents.insert(0, newItem);

        if (recents.size() > maxRecent)
        {
            recents.removeLast();
        }
        settings.setValue(kOpenRecentFiles, recents);

        // restart the player
        Player::instance()->restartWithProjectConfig(config);
    }
}

int Player::openProject(lua_State * /*L*/)
{
    Player::instance()->onOpenProject();
    return 0;
}

int Player::showLoginUI(lua_State * /*L*/)
{
    Player::instance()->onShowLoginUI();
    return 0;
}

int Player::sendMessage(lua_State *L)
{
    int argc = lua_gettop(L);

    if (argc > 0)
    {
        //从栈中读入实部，虚部
        QString messageName(lua_tostring(L, 1));
        QStringList messageList = messageName.split(".");
        QString messageData;

        if (messageList.size() == 2)
        {
            messageData = lua_tostring(L, 2);
        }
        Player::instance()->eventDispatch(messageName, messageData);
    }

    return 0;
}

void Player::onOpenURL(const char *path)
{
    QString tmpPath(path);
    if (tmpPath.startsWith("http://"))
        QDesktopServices::openUrl(QUrl(tmpPath));
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(tmpPath));
}

void Player::onRestartWithArgs(QStringList args)
{
    qApp->setProperty(RESTART_ARGS, args);
    qApp->exit(APP_EXIT_CODE);
}

void Player::openDemoWithArgs(QString cmds)
{
    QStringList args = cmds.split(",");
    Player::instance()->onRestartWithArgs(args);
}

void Player::onSaveQuickRootPath(QString absPath)
{
    QSettings settings;
    settings.setValue(ENV_KEY_QUICK_ROOT_PATH, absPath);
    settings.sync();
}

void Player::onOpenRecentProject()
{
    QAction *action = dynamic_cast<QAction*> (sender());
    if (action)
    {
        int oldIndex = m_recentFileActionList.indexOf(action);
        m_recentFileActionList.move(oldIndex, 0);
        QStringList args = action->data().toStringList();
        std::vector<std::string> argsVector;
        Q_FOREACH(QString item, args)
        {
            argsVector.push_back(item.toStdString());
        }

        // update recents data
        QSettings settings;
        QVariantList recents = settings.value(kOpenRecentFiles, QVariantList()).toList();
        recents.move(oldIndex, 0);
        settings.setValue(kOpenRecentFiles, recents);

        m_projectConfig.parseCommandLine(argsVector);
        applySettingAndRestart();
    }
}

void Player::onClearRecentMenu()
{
    Q_FOREACH(QAction *action, m_recentFileActionList)
    {
        m_openRecentMenu->removeAction(action);
    }

    QSettings setting;
    setting.remove(kOpenRecentFiles);
}

void Player::onEnterBackground()
{
    enterBackgroundDelegate();
}

void Player::onEnterForeground()
{
    enterForegroundDelegate();
}

void Player::onMemoryWarning()
{

}

void Player::registerAllCpp()
{
    lua_State *L = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_register(L, "openURL", &Player::openURL);
    lua_register(L, "openQuickDemoWithWebView", &Player::openQuickDemoWithWebView);
    lua_register(L, "newProject", &Player::newProject);
    lua_register(L, "openProject", &Player::openProject);
    lua_register(L, "showLoginUI", &Player::showLoginUI);
    lua_register(L, "QT_INTERFACE", &Player::sendMessage);
}

void Player::initMainMenu()
{
#ifdef Q_OS_MAC
    m_mainMenu = new QMenuBar(0);
    m_mainMenu->setNativeMenuBar(true);
#else
    m_mainMenu = new QMenuBar(m_renderWidget);
#endif

    //
    // file menu
    //

    QMenu *fileMenu = m_mainMenu->addMenu(QObject::tr("&File"));

    QAction *newProjectAction = fileMenu->addAction(QObject::tr("New Project..."), this, SLOT(onNewProject()), QKeySequence(ShortCut_NEW_PROJECT));
    m_actionMap[newProjectAction->shortcut()] = newProjectAction;

    QAction *newPlayerAction = fileMenu->addAction(QObject::tr("New Player"), this, SLOT(onCreateNewPlayer()), QKeySequence(ShortCut_NEW_PLAYER));
    m_actionMap[newPlayerAction->shortcut()] = newPlayerAction;

    fileMenu->addSeparator();
    QAction *openAction = fileMenu->addAction(QObject::tr("Open"), this, SLOT(on_actionOpen_triggered()), QKeySequence(ShortCut_OPEN));
    m_actionMap[openAction->shortcut()] = openAction;

    m_openRecentMenu = fileMenu->addMenu(tr("Open Recent"));
    QSettings setting;
    QVariantList recents = setting.value(kOpenRecentFiles).toList();
    Q_FOREACH(QVariant recentItem, recents)
    {
        QVariantMap item = recentItem.toMap();
        QString title = item.value(kRecentItemTitle).toString();
        QStringList args = item.value(kRecentItemArgs).toStringList();
        QAction *action = m_openRecentMenu->addAction(title);
        action->setData(args);
        m_recentFileActionList << action;
        connect(action, SIGNAL(triggered()), this, SLOT(onOpenRecentProject()));
    }

    m_openRecentMenu->addSeparator();
    m_openRecentMenu->addAction(tr("Clear"), this, SLOT(onClearRecentMenu()));

    fileMenu->addSeparator();
    fileMenu->addAction(QObject::tr("Welcome"), this, SLOT(onShowWelcome()));

    fileMenu->addSeparator();
    fileMenu->addAction(tr("Snapshot"), this, SLOT(onCreateSnapshot()), QKeySequence(ShortCut_SNAPSHOT));
    fileMenu->addAction(tr("Create Shortcut"), this, SLOT(onCreateShortcut()));

    fileMenu->addSeparator();
    QAction *closeAction = fileMenu->addAction(QObject::tr("Close"), this, SLOT(onClose()), QKeySequence(ShortCut_CLOSE));
    m_actionMap[closeAction->shortcut()] = closeAction;

    //
    // player menu
    //

    QMenu *playerMenu = m_mainMenu->addMenu(QObject::tr("&Player"));
    m_writeDebugAction = playerMenu->addAction(QObject::tr("Write Debug Log to File"));
    m_writeDebugAction->setCheckable(true);
    connect(m_writeDebugAction, SIGNAL(triggered(bool)), this, SLOT(onWriteDebugLog(bool)));


    QAction *openDebugLogAction = playerMenu->addAction(QObject::tr("Open Debug Log"));
    openDebugLogAction->setCheckable(true);
    connect(openDebugLogAction, SIGNAL(triggered(bool)), SLOT(onOpenDebugLog(bool)));

    playerMenu->addAction(tr("Upload to Device..."), this, SLOT(onUploadToDevice()))->setEnabled(false);

    playerMenu->addAction(tr("Create Launcher"), this, SLOT(onCreateLauncher()))->setEnabled(false);

    playerMenu->addAction(tr("Auto Connect Debugger"), this, SLOT(onAutoConnectDebugger()))->setCheckable(true);

    playerMenu->addSeparator();
    QMenu *buildMenu = playerMenu->addMenu(tr("Build"));

    QAction *buildIOSMenu = buildMenu->addAction(tr("iOS..."), this, SLOT(onBuildIOS()), QKeySequence(ShortCut_BUILD_IOS));
    buildIOSMenu->setEnabled(false);
    m_actionMap[buildIOSMenu->shortcut()] = buildIOSMenu;

    QAction *buildAndroidMenu = buildMenu->addAction(tr("Android..."), this, SLOT(onBuildAndroid()));
    buildAndroidMenu->setShortcut(QKeySequence(ShortCut_BUILD_ANDROID));
    buildAndroidMenu->setEnabled(false);
    m_actionMap[buildAndroidMenu->shortcut()] = buildAndroidMenu;

    playerMenu->addSeparator();
    QAction *relaunchAction = playerMenu->addAction(QObject::tr("Relaunch"), this, SLOT(on_actionRelaunch_triggered()));
    relaunchAction->setShortcut(QKeySequence(ShortCut_RELAUNCH));
    m_actionMap[relaunchAction->shortcut()] = relaunchAction;

    playerMenu->addSeparator();
    playerMenu->addAction(tr("Show Project Sandbox"), this, SLOT(onShowProjectSandBox()));

    playerMenu->addAction(tr("Show Porject Files"), this, SLOT(onShowProjectFiles()));

    //
    // screen menu
    //
    m_screenMenu = m_mainMenu->addMenu(QObject::tr("&Screen"));

    //
    // window
    //
    QMenu *windowMenu = m_mainMenu->addMenu(tr("&Window"));
    {
        QAction *onTopAction = windowMenu->addAction(tr("Allways On Top"));
        onTopAction->setCheckable(true);
        connect(onTopAction, SIGNAL(triggered(bool)), this, SLOT(onMainWidgetOnTop(bool)));
    }

    //
    // device
    //
    QMenu *deviceMenu = m_mainMenu->addMenu(tr("&Device"));
    deviceMenu->addAction(tr("Enter Background"), this, SLOT(onEnterBackground()));
    deviceMenu->addAction(tr("Enter Foreground"), this, SLOT(onEnterForeground()));
    deviceMenu->addAction(tr("Memory Warning"), this, SLOT(onMemoryWarning()));


    //
    // more menu
    //

    QMenu *moreMenu = m_mainMenu->addMenu(QObject::tr("&More"));
    moreMenu->addAction(tr("AboutQt"), this, SLOT(on_actionAboutQt_triggered()));
    moreMenu->addAction(tr("About"), this, SLOT(on_actionAbout_triggered()));
    moreMenu->addAction(tr("Simples"), this, SLOT(onOpenQuickDemoWebview()));
    moreMenu->addAction(tr("open.cocoachina.com"), this, SLOT(onShowOpenCocoaChinaWebView()));
    moreMenu->addAction(tr("show console"), this, SLOT(onShowConsole()));
    moreMenu->addAction(tr("Login Test"), this, SLOT(onShowLoginUI()));

    m_mainMenu->show();
    m_mainMenu->raise();

    //
    // Preferences
    //
    QAction *m_preference = moreMenu->addAction(tr("&Preferences"));
    m_preference->setMenuRole(QAction::PreferencesRole);
    connect(m_preference, SIGNAL(triggered()), this, SLOT(onShowPreferences()));
}

void Player::makeMainWindow(QWidget *w, QMenuBar *bar)
{
#ifdef Q_OS_MAC
    Q_UNUSED(bar);

    if (w)
    {
        w->show();
    }

#else
    static bool bInited = false;
    if (bar && w && !bInited)
    {
        bInited = true;
        m_mainWindow = new QWidget();
        m_mainWindow->setContentsMargins(0,0,0,0);
        m_mainWindow->setWindowFlags(m_mainWindow->windowFlags()& ~Qt::WindowMaximizeButtonHint);
        m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);

        QSize glSize = w->size();
        m_container = w;

        bar->setFixedHeight(MENU_BAR_FIXED_HEIGHT);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        layout->addWidget(bar);
        layout->addWidget(m_container);

        m_mainWindow->setLayout(layout);
        m_mainWindow->setFixedSize(glSize + QSize(0, MENU_BAR_FIXED_HEIGHT));
        m_mainWindow->show();

        m_mainWindow->installEventFilter(this);

#if QT_VERSION == QT_VERSION_CHECK(5, 1, 0)
        // fix: shortcuts for Qt5.1 on windows
        w->installEventFilter(this);
#endif
    }

#endif

    checkQuickRootPath();
    updateTitle();
}

QMenuBar *Player::getMenuBar()
{
    return this->m_mainMenu;
}

QString Player::getCreateProjectCommand(QString projectPath, QString packageName, bool isPortrait)
{
    // lua logic
    lua_State *L = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(L, "GET_CREATE_PROJECT_COMMAND");
    lua_pushstring(L, projectPath.toUtf8().data());
    lua_pushstring(L, packageName.toUtf8().data());
    lua_pushboolean(L, isPortrait);
    lua_pcall(L, 3, 1, 0);
    QString cmdString(lua_tostring(L, -1));

    return cmdString;
}

void Player::initScreenMenu()
{
    //
    // create screen size
    //
    QActionGroup *actionGroup = new QActionGroup(this);
    CCSize size = m_projectConfig.getFrameSize();
    for (int i = 0; i < SimulatorConfig::sharedDefaults()->getScreenSizeCount(); i++)
    {
        const SimulatorScreenSize &screenSize = SimulatorConfig::sharedDefaults()->getScreenSize(i);
        QAction *action = m_screenMenu->addAction(QString::fromLocal8Bit(screenSize.title.data()));
        action->setCheckable(true);
        action->setEnabled(true);
        action->setData(i);
        connect(action, SIGNAL(triggered()), this, SLOT(onScreenSizeTriggered()));
        actionGroup->addAction(action);

        if ((size.width == screenSize.width && size.height == screenSize.height) ||
            (size.width == screenSize.height && size.height == screenSize.width))
        {
            action->setChecked(true);
        }
    }

    m_screenMenu->addSeparator();

    //
    // Landscape or Portrait
    //
    actionGroup = new QActionGroup(this);

    m_landscapeAction = m_screenMenu->addAction(tr("Landscape"));
    m_landscapeAction->setData(kLandscape);
    m_landscapeAction->setCheckable(true);
    connect(m_landscapeAction, SIGNAL(triggered()), this, SLOT(onLandscapeTriggered()));

    m_portraitAction = m_screenMenu->addAction(tr("Portrait"));
    m_portraitAction->setData(kPortrait);
    m_portraitAction->setCheckable(true);
    connect(m_portraitAction, SIGNAL(triggered()), this, SLOT(onLandscapeTriggered()));

    actionGroup->addAction(m_landscapeAction);
    actionGroup->addAction(m_portraitAction);

    if (m_projectConfig.isLandscapeFrame())
        m_landscapeAction->setChecked(true);
    else
        m_portraitAction->setChecked(true);

    m_screenMenu->addSeparator();

    //
    // scale
    //
    actionGroup = new QActionGroup(this);
    QList<ScaleActionData> screenScaleDataList;
    {
        ScaleActionData actionData;
        actionData.text = tr("Actual (100%)");
        actionData.keySequence = QKeySequence(ShortCut_SCALE_100);
        actionData.userData = 1.0f;
        screenScaleDataList << actionData;
    }
    {
        ScaleActionData actionData;
        actionData.text = tr("Zoom Out (75%)");
        actionData.keySequence = QKeySequence(ShortCut_SCALE_75);
        actionData.userData = 0.75f;
        screenScaleDataList << actionData;
    }
    {
        ScaleActionData actionData;
        actionData.text = tr("Zoom Out (50%)");
        actionData.keySequence = QKeySequence(ShortCut_SCALE_50);
        actionData.userData = 0.5f;
        screenScaleDataList << actionData;
    }
    {
        ScaleActionData actionData;
        actionData.text = tr("Zoom Out (25%)");
        actionData.keySequence = QKeySequence(ShortCut_SCALE_25);
        actionData.userData = 0.25f;
        screenScaleDataList << actionData;
    }
    Q_FOREACH(ScaleActionData actionData, screenScaleDataList)
    {
        QAction *scaleAction = m_screenMenu->addAction(actionData.text);
        actionGroup->addAction(scaleAction);
        scaleAction->setCheckable(true);
        scaleAction->setShortcut(actionData.keySequence);
        scaleAction->setData(actionData.userData);
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(onScreenScaleTriggered()));
        if (int(m_projectConfig.getFrameScale()*100) == int(actionData.userData.toFloat()*100)) {
            scaleAction->setChecked(true);
        }

        m_actionMap[actionData.keySequence] = scaleAction;
    }
}

void Player::restart()
{
    qApp->exit(APP_EXIT_CODE);
}

void Player::applySettingAndRestart()
{
    QStringList args;

    QString cmd(m_projectConfig.makeCommandLine().data());
    args = cmd.split(" ");
    qApp->setProperty(RESTART_ARGS, args);
    this->restart();
}

void Player::checkQuickRootPath()
{
    if (SimulatorConfig::sharedDefaults()->getQuickCocos2dxRootPath().length() <= 0)
    {
        onShowPreferences();
    }
}

void Player::updateTitle()
{
    QString title = QString("quick-x-player (%1%)").arg(m_projectConfig.getFrameScale()*100);
#ifdef Q_OS_MAC
    CCEGLView::sharedOpenGLView()->setViewName(title.toUtf8().data());
#else
    m_mainWindow->setWindowTitle(title);
#endif
}

bool Player::eventFilter(QObject *o, QEvent *e)
{
#ifdef Q_OS_WIN
    // shortcut
    if (o == m_consoleUI || o == m_demoWidget || o == CCEGLView::sharedOpenGLView()->getGLWidget())
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = (QKeyEvent*) e;
            this->processKeyboardEvent(keyEvent);
        }
    }

    // close console and demo widget when the main window has close
    else if (o == m_mainWindow)
    {
        if (e->type() == QEvent::Close)
        {
            if (m_consoleUI)
            {
                m_consoleUI->close();
            }

            if (m_demoWidget)
            {
                m_demoWidget->close();
            }
        }
    }
#endif


    return QObject::eventFilter(o, e);
}

void Player::processKeyboardEvent(QKeyEvent *e)
{
    QKeySequence shortcut = convertKeyEventToKeySequence(e);

    QAction *action = m_actionMap[shortcut];
    if (action)
    {
        action->trigger();
    }
}

QKeySequence Player::convertKeyEventToKeySequence(QKeyEvent *e)
{
    int keyInt = e->key();

    // check for a combination of user clicks
    Qt::KeyboardModifiers modifiers = e->modifiers();

    if(modifiers & Qt::ShiftModifier)
        keyInt += Qt::SHIFT;
    if(modifiers & Qt::ControlModifier)
        keyInt += Qt::CTRL;
    if(modifiers & Qt::AltModifier)
        keyInt += Qt::ALT;
    if(modifiers & Qt::MetaModifier)
        keyInt += Qt::META;

    return QKeySequence(keyInt);
}

void Player::readSettings(QString data)
{
    m_settings = QxTools::stringToVariant(data).toMap();
}

void Player::eventDispatch(QString messageName, QString data)
{
    QStringList messageList = messageName.split(MODULE_NAME_SEPARATOR);
    // core message
    if (messageList.size() <= 2)
    {
        QString prefix = messageList.at(0);

        // core.openURL
        if (prefix == MODULE_NAME_CORE)
        {
            QString tmpMsgName = messageList.at(1);
            if (tmpMsgName == "openURL")
            {
                this->onOpenURL(data.toLocal8Bit().data());
            }
            else if (tmpMsgName == "openProject")
            {
                this->onOpenProject();
            }
            else if (tmpMsgName == "newProject")
            {
                this->onNewProject();
            }
            else if (tmpMsgName == "openDemo")
            {
                this->onOpenQuickDemoWebview();
            }
            else if (tmpMsgName == "addDemoList")
            {
                this->onAddDemoList(data);
            }
            else if (tmpMsgName == "settings")
            {
                readSettings(data);
            }
            else
            {
                QMetaObject::invokeMethod(this, tmpMsgName.toUtf8().data(), Qt::DirectConnection);
            }
        }
    }
    // others
    else
    {

    }
}

void Player::initConsole()
{
    if (!m_consoleUI)
    {
        m_consoleUI = new ConsoleUI();
        QObject::connect(MsgHandlerWapper::instance(), SIGNAL(message(QtMsgType,QString)),
                         m_consoleUI, SLOT(dealWithMessageOutput(QtMsgType,QString)));
    }
}

void Player::setLogFileName(QString fileName)
{
    Q_ASSERT(m_consoleUI);

    if (!fileName.isEmpty())
    {
        m_consoleUI->initWithLogFile(fileName);
    }
}

void Player::on_actionRelaunch_triggered()
{
    this->applySettingAndRestart();
}

void Player::on_actionOpen_triggered()
{
    this->on_actionConfig_triggered();
    return ;

    // old
    QString dir = qApp->applicationDirPath();
    if (m_projectConfig.getProjectDir().length() > 0)
        dir = QString::fromLocal8Bit(m_projectConfig.getProjectDir().data());

    QFileDialog::Options dialogOptions = QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks;
#ifdef Q_OS_MAC
    // https://bugreports.qt-project.org/browse/QTBUG-2587
    #if (QT_VERSION <= QT_VERSION_CHECK(5, 2, 0))
        dialogOptions |= QFileDialog::DontUseNativeDialog;
    #endif
#endif
    dir = QFileDialog::getExistingDirectory(m_renderWidget, tr("Open Directory"), dir, dialogOptions);

    if (!dir.isEmpty())
    {
        m_projectConfig.setProjectDir(dir.toLocal8Bit().constData());
        this->applySettingAndRestart();
    }
}

void Player::onScreenSizeTriggered()
{
    QAction *action = (QAction*)sender();
    const SimulatorScreenSize &screenSize = SimulatorConfig::sharedDefaults()->getScreenSize(action->data().toInt());

    if (m_projectConfig.isLandscapeFrame())
        m_projectConfig.setFrameSize(CCSize(screenSize.height,screenSize.width));
    else
        m_projectConfig.setFrameSize(CCSize(screenSize.width,screenSize.height));

    applySettingAndRestart();
}

void Player::onLandscapeTriggered()
{
    QAction *action = (QAction*)sender();
    int type = action->data().toInt();
    if (type == kPortrait)
    {
        m_projectConfig.changeFrameOrientationToPortait();
    }
    else if (type == kLandscape)
    {
        m_projectConfig.changeFrameOrientationToLandscape();
    }

    applySettingAndRestart();
}

void Player::onScreenScaleTriggered()
{
    QAction *action = (QAction*)sender();
    float scale = action->data().toFloat();
    m_projectConfig.setFrameScale(scale);

    CCEGLView::sharedOpenGLView()->setFrameZoomFactor(scale);
#ifdef Q_OS_WIN
    if (m_mainWindow)
    {
        m_mainWindow->setFixedSize(m_container->size() + QSize(0,MENU_BAR_FIXED_HEIGHT));
    }
#endif

    updateTitle();
}

void Player::onOpenQuickDemoWebview()
{
    if (!m_demoWidget)
    {
        m_demoWidget = new QuickDemoList();
        m_demoWidget->installEventFilter(this);
        connect(m_demoWidget, SIGNAL(sigOpenDemo(QString)), this, SLOT(onOpenDemo(QString)));

        m_demoWidget->addDemos(m_settings[SETTINGS_DEMO_LIST].toList());
    }

    m_demoWidget->raise();
    m_demoWidget->show();
}

void Player::onAddDemoList(QString data)
{
    if (m_demoWidget)
    {
        m_demoWidget->addDemos(QxTools::stringToVariant(data).toList());
    }
}

void Player::onOpenDemo(QString demoId)
{
    sendMessageToLua("core.openDemo", demoId);
}

void Player::on_actionAbout_triggered()
{
    AboutUI *aboutUI = new AboutUI(m_renderWidget);
    aboutUI->show();
}

void Player::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(0);
}

void Player::on_actionConfig_triggered()
{
    onOpenProject();
}

void Player::onShowOpenCocoaChinaWebView()
{
    const char *url = "http://www.cocoachina.com/";
    onOpenURL(url);
}

void Player::onShowConsole()
{
    Q_ASSERT(m_consoleUI);

    m_consoleUI->show();

    static bool isFirstShow = true;
    if (isFirstShow)
    {
        isFirstShow = false;
        m_consoleUI->installEventFilter(this);
#ifdef Q_OS_MAC
        int y = QGuiApplication::primaryScreen()->availableGeometry().height() - m_consoleUI->height();
#else
        int y = QGuiApplication::primaryScreen()->availableGeometry().height() - m_consoleUI->frameGeometry().height();
#endif
        m_consoleUI->move(0, y);
    }
}

void Player::onShowLoginUI()
{
    LoginDialog *dialog = new LoginDialog();
    connect(dialog, SIGNAL(sigLogin(QString,QString)), this, SLOT(onLogin(QString,QString)));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void Player::restartWithProjectConfig(ProjectConfig &config)
{
    m_projectConfig = config;
    applySettingAndRestart();
}

void Player::onNewProject()
{
    this->newProject(0);
}

void Player::onCreateNewPlayer()
{
    ProjectConfig newPlayerConfig = m_projectConfig;
    QWidget *window = CCEGLView::sharedOpenGLView()->getGLWidget();
    newPlayerConfig.resetToWelcome();
    newPlayerConfig.setWindowOffset(CCPoint(window->pos().x()+50, window->pos().y()+10));

    QString cmd(newPlayerConfig.makeCommandLine().data());
    QStringList args = cmd.split(" ");

#ifdef Q_OS_MAC
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
#else
    QProcess::startDetached(qApp->applicationFilePath(), args);
#endif
}

void Player::onClose()
{
    qApp->exit(0);
}

void Player::onShowWelcome()
{
    m_projectConfig.resetToWelcome();
    qApp->exit(APP_EXIT_CODE);
}

void Player::onCreateSnapshot()
{
    QString timeFormat("yyyy_MM_dd_HH_mm_ss");
    QString suffix(".png");

    QString fileName = QxTools::getHomeDesktopPath();
    fileName += "screenshot";
    fileName += QDateTime::currentDateTime().toString(timeFormat);
    fileName += suffix;

    QxTools::saveScreenshot(fileName);
}

void Player::onCreateShortcut()
{
    QString desktopPath =  QxTools::getHomeDesktopPath();

    QString fileName = QFileDialog::getSaveFileName(0, "",  desktopPath);
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::Text | QFile::WriteOnly | QFile::Truncate))
        {
            QString appFileName = qApp->applicationFilePath().remove(qApp->applicationDirPath()).remove("/");
            QTextStream out(&file);

            out << shortcutFileTemplate.arg(qApp->applicationDirPath())
                                       .arg(appFileName)
                                       .arg(m_projectConfig.makeCommandLine().data());
        }
        file.setPermissions(file.permissions() | QFile::ExeOwner);
    }
}

void Player::onWriteDebugLog(bool checked)
{
    if (m_consoleUI)
    {
        m_consoleUI->setRecordDebugLog(checked);
    }
}

void Player::onOpenDebugLog(bool checked)
{
    if (checked && m_consoleUI)
    {
        m_consoleUI->openLogFile();
    }
}

void Player::onUploadToDevice()
{

}

void Player::onCreateLauncher()
{

}

void Player::onAutoConnectDebugger()
{

}

void Player::onBuildIOS()
{

}

void Player::onBuildAndroid()
{

}

void Player::onShowProjectSandBox()
{
    onOpenURL(m_projectConfig.getProjectDir().c_str());
}

void Player::onShowProjectFiles()
{
    onOpenURL(m_projectConfig.getProjectDir().c_str());
}

void Player::onShowPreferences()
{
    PreferenceUI *widget = new PreferenceUI();
    connect(widget, SIGNAL(sigSaveQuickRootPath(QString)), this, SLOT(onSaveQuickRootPath(QString)));
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void Player::onMainWidgetOnTop(bool checked)
{
#ifdef Q_OS_MAC
    Qt::WindowFlags flags = CCEGLView::sharedOpenGLView()->getGLWidget()->windowFlags();
    QPoint pos = CCEGLView::sharedOpenGLView()->getGLWidget()->pos();
    if (checked)
    {
        flags ^= Qt::WindowStaysOnBottomHint;
        flags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        flags ^= Qt::WindowStaysOnTopHint;
        flags |= Qt::WindowStaysOnBottomHint;
    }
    CCEGLView::sharedOpenGLView()->getGLWidget()->setWindowFlags(flags);
    CCEGLView::sharedOpenGLView()->getGLWidget()->show();
    CCEGLView::sharedOpenGLView()->getGLWidget()->move(pos);
#else
	Qt::WindowFlags flags = m_mainWindow->windowFlags();
	QPoint pos = m_mainWindow->pos();
    if (checked)
    {
        flags ^= Qt::WindowStaysOnBottomHint;
        flags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        flags ^= Qt::WindowStaysOnTopHint;
        flags |= Qt::WindowStaysOnBottomHint;
    }
    m_mainWindow->setWindowFlags(flags);
	m_mainWindow->show();
    m_mainWindow->move(pos);
#endif
}

void Player::onLogin(QString userName, QString password)
{
    QVariantMap data;
    data["user"] = userName;
    data["pwd"] = password;

    sendMessageToLua("core.message", QxTools::variantToString(data));
}

void Player::sendMessageToLua(QString eventId, QString eventData)
{
    cocos2d::CCLuaStack *luaStack = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack();
    luaStack->pushString(eventId.toStdString().data());
    luaStack->pushString(eventData.toStdString().data());
    luaStack->executeGlobalFunction(EDITOR_CALL_LUA, 2);
}
