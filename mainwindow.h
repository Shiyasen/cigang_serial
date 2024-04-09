#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QFont>
#include <QProcess>
#include <QDateTime>
#include <QScrollBar>
#include <QFileDialog>
#include <QRegExp>
#include <QValidator>
#include "global.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
    void initQssStyleSheet();
    long xcount;
    long xrange;

private slots:
    void serialRcvData();
private slots:


    void on_pushButton_scan_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_fuwei_clicked();

    void on_pushButton_zhen_clicked();

    void on_pushButton_qingkong_clicked();

    void on_pushButton_fasong_clicked();

    void on_comboBox_baudrate_currentIndexChanged(int index);

    void on_comboBox_databits_currentIndexChanged(int index);

    void on_comboBox_partity_currentIndexChanged(int index);

    void on_comboBox_stopbits_currentIndexChanged(int index);

    void on_comboBox_flow_currentIndexChanged(int index);

    void on_radioButton_rec_ascii_clicked();

    void on_radioButton_rec_hex_clicked();

    void on_radioButton_send_ascii_clicked();

    void on_radioButton_send_hex_clicked();

    void on_pushButton_qingkong_send_clicked();

    void on_checkBox_dispsend_clicked(bool checked);

    void on_checkBox_disptime_clicked(bool checked);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *terminal;
    QSerialPort *serial;
    QString currentConnectCom;
    QByteArray globalRecvData;
    QValidator *validator_combox_baudrate;

    bool isRoot;
    bool isShowSend;
    bool isShowTime;
    bool recAsciiFormat;
    bool sendAsciiFormat;
    bool repeatSend;
    bool enableDrawFunction;
    bool pauseComOutput;
    void RefreshTheUSBList( void );
};
#endif // MAINWINDOW_H
