#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->WarningLable->setStyleSheet("color : red");

    ui->Certificate_Edit->setText("");
    ui->math_Edit->setText("");
    ui->Rus_Edit->setText("");
    ui->Speciality_Edit->setText("");
    ui->FIO_Edit->setText("");
    ui->Physics_Edit->setText("");
    ui->lineEdit_2->setText("");
    ui->InformationLable->setReadOnly(true);

    file.setFileName("D:\\Список абитуриентов.txt");
    file.open(QIODevice::ReadWrite);
    FIO = "";
    InfText = "";

    Warning = new QTimer();
    connect(Warning, &QTimer::timeout, this, &MainWindow::slotWarningTimer);
    SpecialitiesAmount = 0;
    input();
}

MainWindow::~MainWindow()
{
    delete ui;
    file.close();
}

void MainWindow::on_AddStudent_pushButton_clicked()
{
    bool flag = false;
    int spec = -1;

    if(!getStudentInfo())
        return;

    int i;
    for(i = 0; i < SpecialitiesAmount; i++)
    {
        for(int j = 0; j < Specialities[i].size(); j++)
            if(Specialities[i].AbiturList[j].FIO == Student.FIO)
                flag = true;

        if(Specialities[i].getName() == Student.speciality)
            spec = i;
    }

    if(flag)
        ShowWarning(" * Студент с таким именем уже добавлен * ");
    else
    {
        if(spec < 0)
        {
            spec = SpecialitiesAmount;
            Specialities[SpecialitiesAmount] = Speciality();
            Specialities[SpecialitiesAmount].setName(Student.speciality);
            ui->comboBox->addItem(Student.speciality);
            SpecialitiesAmount++;
        }
        Specialities[spec].addStudent(Student);
        ui->Certificate_Edit->setText("");
        ui->math_Edit->setText("");
        ui->Rus_Edit->setText("");
        ui->Speciality_Edit->setText("");
        ui->FIO_Edit->setText("");
        ui->Physics_Edit->setText("");
        ShowWarning(" * Студент успешно добавлен *");
    }
}


void MainWindow::slotWarningTimer()
{
    ui->WarningLable->setText("");
    Warning->stop();
}

void MainWindow::ShowWarning(QString WarningMessage)
{
    ui->WarningLable->setText(WarningMessage);
    Warning->start(3500);
}

bool MainWindow::getStudentInfo()
{
    int math = getInt(ui->math_Edit->text());
    int physics = getInt(ui->Physics_Edit->text());
    int rus = getInt(ui->Rus_Edit->text());
    int certificate = getInt(ui->Certificate_Edit->text());
    QString spec = getWord(ui->Speciality_Edit->text());

    QString tmp = ui->FIO_Edit->text();
    QString fn = "", sn = "", patr = "";

    int i = 0;

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        sn += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        fn += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        patr += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    if(fn == "" || sn == "" || patr == "" || i < tmp.size() || spec == "#$%^&"
                || math > 100 || math < 0
                || rus > 100 || rus < 0
                || certificate > 100 || certificate < 0
                || physics > 100 || physics < 0)
    {
        ShowWarning(" * Некорректные данные *");
        return false;
    }
    else
    {
        Student = Abiturient(math, physics, rus, certificate, fn, sn, patr);
        Student.speciality = spec;
        return true;
    }
}

bool MainWindow::getFIO()
{
    QString tmp = ui->lineEdit_2->text();
    QString fn = "", sn = "", patr = "";

    int i = 0;

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        sn += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        fn += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    while(i < tmp.size() && tmp[i] != ' ')
    {
        patr += tmp[i];
        i++;
    }

    while(i < tmp.size() && tmp[i] == ' ')
        i++;

    if(fn == "" || sn == "" || patr == "" || i < tmp.size())
    {
        ShowWarning(" * Некорректные данные *");
        return false;
    }
    else
    {
        FIO = sn + " " + fn + " " + patr;
        return true;
    }
}

int MainWindow::getInt(QString str)
{
    QString word = "";
    int i = 0;
    bool ok = true;

    while(i < str.size() && str[i] == ' ')
        i++;

    while(i < str.size() && str[i] != ' ')
    {
        if(str[i] > '9' || str[i] < '0')
            ok = false;
        word += str[i];
        i++;
    }

    while(i < str.size() && str[i] == ' ')
        i++;

    if(word == "" || i < str.size() || !ok)
        return -1e9 - 7;
    else
        return word.toInt();
}

QString MainWindow::getWord(QString str)
{
    QString word = "";
    int i = 0;

    while(i < str.size() && str[i] == ' ')
        i++;

    while(i < str.size() && str[i] != ' ')
    {
        word += str[i];
        i++;
    }

    while(i < str.size() && str[i] == ' ')
        i++;

    if(word == "" || i < str.size())
        return "#$%^&";
    else
        return word;
}
