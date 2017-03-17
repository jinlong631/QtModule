#include "frmmain.h"
#include "ui_frmmain.h"
#include "./Api/iconhelper.h"
#include "./Api/myhelper.h"
#include "./AppConfig/myapp.h"
#include "QsLog/QsLog.h"
FrmMain::FrmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    QLOG_INFO()<<"app start...";
    myHelper::FormInCenter(this);
    this->InitStyle();
}

FrmMain::~FrmMain()
{
    delete ui;
}

void FrmMain::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    changSkinMenu=NULL;
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    //IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    ui->lab_Ico->setPixmap(QPixmap(myApp::AppPath+"images/logo.ico"));
    ui->lab_Title->setText(myApp::SoftTitle);
    connect(ui->btnMenu,SIGNAL(clicked(bool)),this,SLOT(slotChangeDisplay()));

    menuBar = new QMenuBar(ui->widget);
    menuBar->setGeometry(QRect(0,0,3000,40));

    QMenu* menuFile = menuBar->addMenu(tr("&文件"));
    QMenu* menuEdit = menuBar->addMenu(tr("&编辑"));
    QMenu* menuBuild = menuBar->addMenu(tr("&构建"));
    QMenu* menuDebug = menuBar->addMenu(tr("&调试"));
    QMenu* menuTool = menuBar->addMenu(tr("&工具"));
    QMenu* menuHelp = menuBar->addMenu(tr("&帮助"));

    QAction *openFile = menuFile->addAction(QIcon(":/images/image/open_file.ico"),tr("打开"));
    QAction *newFile = menuFile->addAction(QIcon(":/images/image/new_file..ico"),tr("新建"));
    QAction *filePrint = menuFile->addAction(QIcon(":/images/image/print.ico"),tr("打印"));
    QAction *appExit = menuFile->addAction(QIcon(":/images/image/close.png"),tr("退出"));

    //帮助菜单下的项
    QAction *softwareInfo = menuHelp->addAction(QIcon(":/images/image/info.ico"),tr("关于本软件"));
    QAction *contactUs = menuHelp->addAction(QIcon(":/images/image/contact us.ico"),tr("软件帮助"));

    ui->gridLayout_2->setMenuBar(menuBar);

    toolBar = new QToolBar(ui->widget_2);
    toolBar->addAction(openFile);
    toolBar->addAction(newFile);
    toolBar->addAction(filePrint);
    toolBar->addAction(appExit);
    ui->gridLayout_3->setMenuBar(toolBar);
    //toolBar->setGeometry(0,0,3000,30);
    //toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//图标在前 文字在后
    //toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//图标在上 文字在下
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    connect(ui->btnInfo,SIGNAL(clicked(bool)),this,SLOT(slotShowInfobox()));
    connect(ui->btnQuestion,SIGNAL(clicked(bool)),this,SLOT(slotShowQuestionbox()));
    connect(ui->btnError,SIGNAL(clicked(bool)),this,SLOT(slotShowErrorbox()));
}

bool FrmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmMain::on_btnMenu_Close_clicked()
{
    qApp->exit();
}

void FrmMain::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void FrmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmMain::slotChangeDisplay()
{
    static QAction *gray;
    static QAction *navy;
    static QAction *black;
    static QAction *blue;

    if (changSkinMenu == NULL)
    {
        changSkinMenu = new QMenu(this);

        gray = new QAction(changSkinMenu);
        navy = new QAction(changSkinMenu);
        black = new QAction(changSkinMenu);
        blue = new QAction(changSkinMenu);

        gray->setText(tr("灰色"));
        navy->setText(tr("天蓝色"));
        black->setText(tr("黑色"));
        blue->setText(tr("蓝色"));

        changSkinMenu->addAction(gray);
        changSkinMenu->addAction(navy);
        changSkinMenu->addAction(black);
        changSkinMenu->addAction(blue);

        changSkinMenu->setStyleSheet("border: 1px solid black");
        connect(gray, SIGNAL(triggered()),this, SLOT(slotchangedToGray()));
        connect(navy, SIGNAL(triggered()),this, SLOT(slotchangedToNavy()));
        connect(black, SIGNAL(triggered()),this, SLOT(slotchangedToBlack()));
        connect(blue, SIGNAL(triggered()),this, SLOT(slotchangedToBlue()));

    }
    QPoint pos;
    pos.setX(0);
    pos.setY(ui->btnMenu->height());
    changSkinMenu->exec(ui->btnMenu->mapToGlobal(pos));
}
void FrmMain::slotchangedToGray()
{
    myHelper::SetStyle("gray");//灰色风格
}
void FrmMain::slotchangedToBlue()
{
    myHelper::SetStyle("blue");//蓝色色风格
}
void FrmMain::slotchangedToBlack()
{
    myHelper::SetStyle("black");//黑色色风格
}
void FrmMain::slotchangedToNavy()
{
    myHelper::SetStyle("navy");//天蓝色色风格
}
void FrmMain::slotShowInfobox()
{
    myHelper::ShowMessageBoxInfo("今天是个好天气。");
}
void FrmMain::slotShowErrorbox()
{
    myHelper::ShowMessageBoxError("程序出现错误！");
}
void FrmMain::slotShowQuestionbox()
{
    myHelper::ShowMessageBoxQuesion("今天工作顺利吗？");
}
