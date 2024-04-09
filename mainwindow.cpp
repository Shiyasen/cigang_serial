#include "mainwindow.h"
#include "ui_mainwindow.h"

#define VERISON  tr("v1.0")
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    ui(new Ui::MainWindow),
    terminal(new QProcess),
    serial(new QSerialPort)

{

    ui->setupUi(this);
    sendAsciiFormat = false;
    recAsciiFormat = true;
    pauseComOutput = false;
    isRoot = false;
    validator_combox_baudrate = nullptr;

    /*UI init*/
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_fuwei->setEnabled(false);
    ui->pushButton_scan->setEnabled(true);
    ui->pushButton_fasong->setEnabled(false);
    ui->comboBox_baudrate->setCurrentIndex(CONFIG_BAUDRATE_115200_INDEX);
    ui->comboBox_partity->setCurrentIndex(CONFIG_PARITY_NONE_INDEX);
    ui->comboBox_databits->setCurrentIndex(CONFIG_DATABITS_8_INDEX);
    //ui->comboBox_stopbits->setCurrentText(CONFIG_STOPBIT_ONE_INDEX);
    ui->comboBox_flow->setCurrentIndex(CONFIG_FLOWCTRL_NONE_INDEX);
    setWindowTitle("磁钢参数配置上位机 " + VERISON);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRcvData()));
    on_checkBox_dispsend_clicked(false);
    on_checkBox_disptime_clicked(false);
}
void MainWindow::RefreshTheUSBList(void)
{
    QString portName;
    QString uartName;
    QSerialPortInfo info;
    ui->comboBoxlist->clear();

    qDebug()<<"Debug: REfresh the list...";
    foreach(info,QSerialPortInfo::availablePorts())
    {
        //serial->setPort(info);
        portName=info.portName();
        uartName=info.description();
        ui->comboBoxlist->addItem(portName+",(" +uartName+")");
        if(isRoot == false)
        {
            qDebug() << "reset: sudo chmod 777 /dev/" + portName;
            terminal->start("pkexec chmod 777 /dev/"+portName);
            isRoot=true;
        }
        qDebug() << "SYSTEM: Scan the uart device: "+uartName + "("+portName+")"+" has been added to the available list! ";

    }
}
void MainWindow::serialRcvData(void)
{
    QByteArray recvArray;
    QScrollBar *scrollbar = ui->textBrowser_rec->verticalScrollBar();
    QString recvStr;
    recvArray = serial->readAll();
    recvStr = QString(recvArray);
    if (pauseComOutput == false)
    {
        QDateTime local(QDateTime::currentDateTime());
        QString localTime = "<" + local.toString("hh:mm:ss.zzz") + ">";
        QString str;
        if (recAsciiFormat == true)
        {
            qDebug() << recvStr ;
            str = recvStr;
            if (isShowSend)
            {
                str = "rx -> "  + recvStr;
            }
            if (isShowTime)
            {
                str = localTime + str;
            }
            ui->textBrowser_rec->insertPlainText(str);
            scrollbar->setSliderPosition(scrollbar->maximum());
        }
        else
        {
            str = recvStr;
            if (isShowSend)
            {
                str = "rx -> "  + recvArray.toHex();
            }
            if (isShowTime)
            {
                str = localTime + str;
            }
            ui->textBrowser_rec->insertPlainText(str);
        }
    }


}



void MainWindow::on_pushButton_scan_clicked()
{
    RefreshTheUSBList();
}

void MainWindow::on_pushButton_pause_clicked()
{
    if (pauseComOutput == false) {
        pauseComOutput = true;
        ui->pushButton_pause->setText("start");
    } else {
        pauseComOutput = false;
        ui->pushButton_pause->setText("pause");
    }
}

void MainWindow::on_pushButton_open_clicked()
{
    qint8 comboxIndex = 0xff;

    comboxIndex = ui->comboBox_baudrate->currentIndex();
    qDebug() << "COM Config Info: ";
    switch(comboxIndex)
    {
        case CONFIG_BAUDRATE_1200_INDEX:
            serial->setBaudRate(QSerialPort::Baud1200);
            qDebug() << "Baud Rate: 1200; ";
            break;

        case CONFIG_BAUDRATE_2400_INDEX:
            serial->setBaudRate(QSerialPort::Baud2400);
            qDebug() << "Baud Rate: 2400; ";
            break;

        case CONFIG_BAUDRATE_4800_INDEX:
            serial->setBaudRate(QSerialPort::Baud4800);
            qDebug() << "Baud Rate: 4800; ";
            break;

        case CONFIG_BAUDRATE_9600_INDEX:
            serial->setBaudRate(QSerialPort::Baud9600);
            qDebug() << "Baud Rate: 9600; ";
            break;

        case CONFIG_BAUDRATE_19200_INDEX:
            serial->setBaudRate(QSerialPort::Baud19200);
            qDebug() << "Baud Rate: 19200; ";
            break;

        case CONFIG_BAUDRATE_38400_INDEX:
            serial->setBaudRate(QSerialPort::Baud38400);
            qDebug() << "Baud Rate: 38400; ";
            break;

        case CONFIG_BAUDRATE_57600_INDEX:
            serial->setBaudRate(QSerialPort::Baud57600);
            qDebug() << "Baud Rate: 57600; ";
            break;

        case CONFIG_BAUDRATE_115200_INDEX:
            serial->setBaudRate(QSerialPort::Baud115200);
            qDebug() << "Baud Rate: 115200; ";
            break;
        }
    /* set stop bits. */
        comboxIndex = ui->comboBox_stopbits->currentIndex();

        switch (comboxIndex) {
        case CONFIG_STOPBIT_ONE_INDEX:
            serial->setStopBits(QSerialPort::OneStop);
            qDebug() << "stop bits: 1 bit; ";
            break;

        case CONFIG_STOPBIT_ONEANDHALF_INDEX:
            serial->setStopBits(QSerialPort::OneAndHalfStop);
            qDebug() << "stop bits: 1.5 bits;";
            break;

        case CONFIG_STOPBIT_TWO_INDEX:
            serial->setStopBits(QSerialPort::TwoStop);
            qDebug() << "stop bits: 2 bits; ";
            break;
        }

        /* set parity */
            comboxIndex = ui->comboBox_partity->currentIndex();
            switch(comboxIndex)
            {

            case CONFIG_PARITY_NONE_INDEX:
                serial->setParity(QSerialPort::NoParity);
                qDebug() << "partiy set: noParity.";
                break;

            case CONFIG_PARITY_EVEN_INDEX:
                serial->setParity(QSerialPort::EvenParity);
                qDebug() << "partiy set: EvenParity.";
                break;

            case CONFIG_PARITY_ODD_INDEX:
                serial->setParity(QSerialPort::OddParity);
                qDebug() << "partiy set: OddParity.";
                break;

            case CONFIG_PARITY_SPACE_INDEX:
                serial->setParity(QSerialPort::SpaceParity);
                qDebug() << "partiy set: SpaceParity.";
                break;

            case CONFIG_PARITY_MARK_INDEX:
                serial->setParity(QSerialPort::MarkParity);
                qDebug() << "partiy set: MarkParity.";
                break;
            }

        /* set databytes. */
            comboxIndex = ui->comboBox_databits->currentIndex();
            switch (comboxIndex)
            {
            case CONFIG_DATABITS_5_INDEX:
                serial->setDataBits(QSerialPort::Data5);
                qDebug() << "Data bits: 5 bits; ";
                break;

            case CONFIG_DATABITS_6_INDEX:
                serial->setDataBits(QSerialPort::Data6);
                qDebug() << "Data bits: 6 bits; ";
                break;

            case CONFIG_DATABITS_7_INDEX:
                serial->setDataBits(QSerialPort::Data7);
                qDebug() << "Data bits: 7 bits; ";
                break;

            case CONFIG_DATABITS_8_INDEX:
                serial->setDataBits(QSerialPort::Data8);
                qDebug() << "Data bits: 8 bits; ";
                break;
            }
            /* set flowctrl */
            comboxIndex = ui->comboBox_flow->currentIndex();
            switch (comboxIndex)
            {
            case CONFIG_FLOWCTRL_NONE_INDEX:
                serial->setFlowControl(QSerialPort::NoFlowControl);
                qDebug() << "flow ctrl: no flow ctrl; ";
                break;

            case CONFIG_FLOWCTRL_HARD_INDEX:
                serial->setFlowControl(QSerialPort::HardwareControl);
                qDebug() << "flow ctrl: hardware flow ctrl; ";
                break;

            case CONFIG_FLOWCTRL_SOFT_INDEX:
                serial->setFlowControl(QSerialPort::SoftwareControl);
                qDebug() << "flow ctrl: software flow ctrl; ";
                break;
            }
            qDebug() << "--------------------------------; \n";
            QString portInfo = ui->comboBoxlist->currentText();
            QList<QString> infoList = portInfo.split(',');
            currentConnectCom = infoList.at(0);
            qDebug() << currentConnectCom;
            qDebug() << tr("SYSTEM: Serial port ")+portInfo+tr(" ,system is connecting with it.....");
            serial->setPortName(currentConnectCom);
            if (ui->comboBoxlist->currentIndex() == -1)
            {
                    QMessageBox::warning(this,"Warning","Please click firstly the scan button to check your available devices.\nthan connect after selecting one device in the list.");
                    return;
            }
            if (!serial->open(QIODevice::ReadWrite))
            {
                QMessageBox::warning(this,"Warning","Open serial port fail!\n Please see the the information window to solve problem.");
                qDebug() << tr("SYSTEM: The serial port failed to open,Please check as follows: ");
                qDebug() << tr("        1> if the serial port is occupied by other software? ");
                qDebug() << tr("        2> if the serial port connection is normal?");
                qDebug() << tr("        3> if the program is run at root user? You can use the cmd sudo ./(programname) and type your password to be done.");
                ui->comboBoxlist->setEnabled(true);
            } else {
                qDebug() << tr("SYSTEM: The system has been connected with ")+portInfo+" " ;
                ui->pushButton_close->setEnabled(true);
                ui->pushButton_open->setEnabled(false);
                ui->comboBoxlist->setEnabled(false);
                ui->comboBox_flow->setEnabled(false);
                ui->comboBox_partity->setEnabled(false);
                ui->comboBox_baudrate->setEnabled(false);
                ui->comboBox_databits->setEnabled(false);
                ui->comboBox_stopbits->setEnabled(false);
                ui->pushButton_fuwei->setEnabled(true);

                ui->pushButton_scan->setEnabled(false);
                ui->pushButton_fasong->setEnabled(true);
                QMessageBox::information(this,"Information", "UART: "+ portInfo+" has been connected! \n"+"Wait device signals.");

            }
            qDebug() << "The serial has been openned!! \n";


}

void MainWindow::on_pushButton_close_clicked()
{
    serial->close();
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_fasong->setEnabled(false);
    ui->pushButton_scan->setEnabled(true);
    ui->comboBoxlist->setEnabled(true);
    ui->comboBox_flow->setEnabled(true);
    ui->comboBox_partity->setEnabled(true);
    ui->comboBox_baudrate->setEnabled(true);
    ui->comboBox_databits->setEnabled(true);
    ui->comboBox_stopbits->setEnabled(true);
}

void MainWindow::on_pushButton_fuwei_clicked()
{
    QByteArray temp;
    //ui->textEdit_send->clear();

    temp[0]=0xa5;
    temp[1]=0x01;

    switch(ui->comboBox_chx->currentIndex())
    {
        case 0 :temp[2]=0x11; break;
        case 1 :temp[2]=0x01; break;
        case 2 :temp[2]=0x12; break;
        case 3 :temp[2]=0x02; break;

        case 4 :temp[2]=0x13; break;
        case 5 :temp[2]=0x03; break;
        case 6 :temp[2]=0x14; break;
        case 7 :temp[2]=0x04; break;
        default:break;
    }
    temp[3]=0x00;
    temp[4]=0x39;
    temp[5]=temp[1]+temp[2]+temp[3]+temp[4];
    temp[6]=0x5a;
    qDebug() << temp.toHex();
    serial->write(temp);
    qDebug() << "UART SendHex : " << temp.toHex();
}

void MainWindow::on_pushButton_zhen_clicked()
{
    QByteArray temp;
    temp[0]=0xa5;
    temp[1]=0x0d;
    //temp[2]=0x01;
    switch(ui->comboBox_chx->currentIndex())
    {
        case 0 :temp[2]=0x11; break;
        case 1 :temp[2]=0x01; break;
        case 2 :temp[2]=0x12; break;
        case 3 :temp[2]=0x02; break;

        case 4 :temp[2]=0x13; break;
        case 5 :temp[2]=0x03; break;
        case 6 :temp[2]=0x14; break;
        case 7 :temp[2]=0x04; break;
        default:break;
    }
    temp[3]=0x00;
    temp[4]=0x41;
    short int shuju=ui->comboBox_fuzhi->currentText().toInt();
    temp[5]=shuju & 0xFF;
    temp[6]=(shuju >> 8) & 0xFF;
    shuju=ui->comboBox_minT->currentText().toInt();
    temp[7]=shuju & 0xFF;
    temp[8]=(shuju >> 8) & 0xFF;
    shuju=ui->comboBox_maxT->currentText().toInt();
    temp[9]=shuju & 0xFF;
    temp[10]=(shuju >> 8) & 0xFF;
    shuju=ui->comboBox_top_cnt ->currentText().toInt();
    temp[11]=shuju & 0xFF;
    temp[12]=(shuju >> 8) & 0xFF;
    shuju=ui->comboBox_bottom_cnt ->currentText().toInt();
    temp[13]=shuju & 0xFF;
    temp[14]=(shuju >> 8) & 0xFF;
    shuju=ui->comboBox_jiange ->currentText().toInt();
    temp[15]=shuju & 0xFF;
    temp[16]=(shuju >> 8) & 0xFF;
    temp[17]=   temp[1]+temp[2]+temp[3]+temp[4]+temp[5]+
                temp[6]+temp[7]+temp[8]+temp[9]+temp[10]+
                temp[11]+temp[12]+temp[13]+temp[14]+temp[15]+temp[16];
    temp[18]=0x5a;
    qDebug() <<"zhen:"<< temp.toHex();
    ui->textEdit_send->clear();
    ui->textEdit_send->append(temp.toHex());
}

void MainWindow::on_pushButton_qingkong_clicked()
{
    ui->textBrowser_rec->clear();
}

void MainWindow::on_pushButton_fasong_clicked()
{
    QString input = ui->textEdit_send->toPlainText();
    QByteArray temp;
    if (input.isEmpty() == true)
    {
        QMessageBox::warning(this,"Warning","The send text is empty!\n Please input the data...");
        return;
    }
    else
    {
        QDateTime local(QDateTime::currentDateTime());
        QString localTime = "<" + local.toString("hh:mm:ss.zzz") + ">";
        QString str;
        if (sendAsciiFormat == true)
        {
            serial->write(input.toLatin1());
            str += input.toLatin1();
            if (isShowSend) {
                str = "tx -> " + str;
            }
            if (isShowTime) {
                str = localTime + str;
            }
            ui->textBrowser_rec->append(str);
            qDebug() << "UART SendAscii : " << input.toLatin1();
        }
        else
        {
            StringToHex(input, temp);
            serial->write(temp);
            str += input.toLatin1();
            if (isShowSend) {
                str = "tx -> " + str;
            }
            if (isShowTime) {
                str = localTime + str;
            }
            ui->textBrowser_rec->append(str);
            qDebug() << "UART SendHex : " << temp.toHex();
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_baudrate_currentIndexChanged(int index)
{
    ui->comboBox_baudrate->setEditable(false);
    if(validator_combox_baudrate != nullptr){
        delete validator_combox_baudrate;
        validator_combox_baudrate = nullptr;
        qDebug() << "delete and clear validator";
    }

    switch(index) {
    case CONFIG_BAUDRATE_1200_INDEX:
        serial->setBaudRate(QSerialPort::Baud1200);
        qDebug() << "Baud Rate: 1200; ";
        break;

    case CONFIG_BAUDRATE_2400_INDEX:
        serial->setBaudRate(QSerialPort::Baud2400);
        qDebug() << "Baud Rate: 2400; ";
        break;

    case CONFIG_BAUDRATE_4800_INDEX:
        serial->setBaudRate(QSerialPort::Baud4800);
        qDebug() << "Baud Rate: 4800; ";
        break;

    case CONFIG_BAUDRATE_9600_INDEX:
        serial->setBaudRate(QSerialPort::Baud9600);
        qDebug() << "Baud Rate: 9600; ";
        break;

    case CONFIG_BAUDRATE_19200_INDEX:
        serial->setBaudRate(QSerialPort::Baud19200);
        qDebug() << "Baud Rate: 19200; ";
        break;

    case CONFIG_BAUDRATE_38400_INDEX:
        serial->setBaudRate(QSerialPort::Baud38400);
        qDebug() << "Baud Rate: 38400; ";
        break;

    case CONFIG_BAUDRATE_57600_INDEX:
        serial->setBaudRate(QSerialPort::Baud57600);
        qDebug() << "Baud Rate: 57600; ";
        break;

    case CONFIG_BAUDRATE_115200_INDEX:
        serial->setBaudRate(QSerialPort::Baud115200);
        qDebug() << "Baud Rate: 115200; ";
        break;
    case CONFIG_BAUDRATE_CUSTOM_INDEX:
        ui->comboBox_baudrate->setEditable(true);
        ui->comboBox_baudrate->setCurrentText("");
        QRegExp regx("[0-9]{7}");
        validator_combox_baudrate = new QRegExpValidator(regx, ui->comboBox_baudrate);
        ui->comboBox_baudrate->setValidator(validator_combox_baudrate);
        qDebug() << "validator";
        break;
    }
}

void MainWindow::on_comboBox_databits_currentIndexChanged(int index)
{
    switch (index) {
    case CONFIG_DATABITS_5_INDEX:
        serial->setDataBits(QSerialPort::Data5);
        qDebug() << "Data bits: 5 bits; ";
        break;

    case CONFIG_DATABITS_6_INDEX:
        serial->setDataBits(QSerialPort::Data6);
        qDebug() << "Data bits: 6 bits; ";
        break;

    case CONFIG_DATABITS_7_INDEX:
        serial->setDataBits(QSerialPort::Data7);
        qDebug() << "Data bits: 7 bits; ";
        break;

    case CONFIG_DATABITS_8_INDEX:
        serial->setDataBits(QSerialPort::Data8);
        qDebug() << "Data bits: 8 bits; ";
        break;
    }
}

void MainWindow::on_comboBox_partity_currentIndexChanged(int index)
{
    switch(index) {

    case CONFIG_PARITY_NONE_INDEX:
        serial->setParity(QSerialPort::NoParity);
        qDebug() << "partiy set: noParity.";
        break;

    case CONFIG_PARITY_EVEN_INDEX:
        serial->setParity(QSerialPort::EvenParity);
        qDebug() << "partiy set: EvenParity.";
        break;

    case CONFIG_PARITY_ODD_INDEX:
        serial->setParity(QSerialPort::OddParity);
        qDebug() << "partiy set: OddParity.";
        break;

    case CONFIG_PARITY_SPACE_INDEX:
        serial->setParity(QSerialPort::SpaceParity);
        qDebug() << "partiy set: SpaceParity.";
        break;

    case CONFIG_PARITY_MARK_INDEX:
        serial->setParity(QSerialPort::MarkParity);
        qDebug() << "partiy set: MarkParity.";
        break;

    }
}

void MainWindow::on_comboBox_stopbits_currentIndexChanged(int index)
{
    switch (index) {
    case CONFIG_STOPBIT_ONE_INDEX:
        serial->setStopBits(QSerialPort::OneStop);
        qDebug() << "stop bits: 1 bit; ";
        break;

    case CONFIG_STOPBIT_ONEANDHALF_INDEX:
        serial->setStopBits(QSerialPort::OneAndHalfStop);
        qDebug() << "stop bits: 1.5 bits;";
        break;

    case CONFIG_STOPBIT_TWO_INDEX:
        serial->setStopBits(QSerialPort::TwoStop);
        qDebug() << "stop bits: 2 bits; ";
        break;
    }
}

void MainWindow::on_comboBox_flow_currentIndexChanged(int index)
{
    switch (index) {
    case CONFIG_FLOWCTRL_NONE_INDEX:
        serial->setFlowControl(QSerialPort::NoFlowControl);
        qDebug() << "flow ctrl: no flow ctrl; ";
        break;
    case CONFIG_FLOWCTRL_HARD_INDEX:
        serial->setFlowControl(QSerialPort::HardwareControl);
        qDebug() << "flow ctrl: hardware flow ctrl; ";
        break;
    case CONFIG_FLOWCTRL_SOFT_INDEX:
        serial->setFlowControl(QSerialPort::SoftwareControl);
        qDebug() << "flow ctrl: software flow ctrl; ";
        break;
    }
}
void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len;) {
        //char lstr,
        hstr = str[i].toLatin1();
        if (hstr == ' ') {
            i++;
            continue;
        }
        i++ ;
        if (i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if ((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void MainWindow::on_radioButton_rec_ascii_clicked()
{
    recAsciiFormat = true;
    qDebug() << "SYSTEM: Set recv data by ASCII." ;
}

void MainWindow::on_radioButton_rec_hex_clicked()
{
    recAsciiFormat = false;
    qDebug() << "SYSTEM: Set recv data by HEX." ;
}

void MainWindow::on_radioButton_send_ascii_clicked()
{
    sendAsciiFormat = true;
    qDebug() << "SYSTEM: Set send data by ASCII." ;
}

void MainWindow::on_radioButton_send_hex_clicked()
{
    sendAsciiFormat = false;
    qDebug() << "SYSTEM: Set send data by HEX." ;
}

void MainWindow::on_pushButton_qingkong_send_clicked()
{
    ui->textEdit_send->clear();
}

void MainWindow::on_checkBox_dispsend_clicked(bool checked)
{
    isShowSend = checked;
}

void MainWindow::on_checkBox_disptime_clicked(bool checked)
{
    isShowTime = checked;
}

void MainWindow::on_pushButton_clicked()
{
    QString shuoming    =   "[1] OffsetValue:现场直流偏置，对应ADC值（20us采集一次，连续采集50000，及1s，求平均）\n"
                            "[2] TopThreshold:上顶点幅值阈值\n"
                            "[3] BottomThreshold:下顶点幅值阈值\n"
                            "[4] TopQuarThreshold:上顶点幅值阈值的二分之一\n"
                            "[5] BottomQuarThreshold:下顶点幅值阈值的二分之一\n"
                            "[6] AmplThreshold:信号幅值单位，磁钢板内部将这个值转化为AD值，再结合OffsetValue形成TopThreshold/BottomThreshold\n"
                            "[7] DlyTime:同一通道，两次磁钢信号的最小间隔时间，单位：ms"
                            ;
    QMessageBox::information(this, "参数说明", shuoming);
}
