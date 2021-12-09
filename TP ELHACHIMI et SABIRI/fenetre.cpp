#pragma comment(lib, "ODALID.lib")
#include "fenetre.h"
#include "ui_fenetre.h"
#include "ODALID.h"
#include <QtGui>

Fenetre::Fenetre(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Fenetre)
{
    ui->setupUi(this);


    Fenetre::~Fenetre()
    {
        delete ui;
    }

    ReaderName MonLecteur;
//char pszHost[] = "192.168.1.4";
    uint8_t key_ff[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t key1[6] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
    uint8_t key2[6] = {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5};

    uint8_t key3[6] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5};
    uint8_t key4[6] = {0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5};



    void Fenetre::on_Connect_clicked()
    {
        int16_t status = 0;
        uint8_t atq[2];
        uint8_t sak[1];
        uint8_t uid[12];
        uint16_t uid_len = 12;
        uint8_t data[16];


        MonLecteur.Type = ReaderCDC;
        MonLecteur.device = 0;
        status = OpenCOM1(&MonLecteur);
        qDebug() << "OpenCOM1" << status;


        status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyA, key1, 0);
        qDebug() << "Mf_Classic_LoadKey KEY 1" << status;
        status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyB, key2, 0);
        qDebug() << "Mf_Classic_LoadKey KEY 2" << status;
        status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyA, key3, 0);
        qDebug() << "Mf_Classic_LoadKey KEY 3" << status;
        status = Mf_Classic_LoadKey(&MonLecteur, Auth_KeyB, key4, 0);
        qDebug() << "Mf_Classic_LoadKey KEY 4" << status;

        char version[30];
        uint8_t serial[4];
        char stackReader[20];
        status = Version(&MonLecteur, version, serial, stackReader);
        qDebug() << "Version" << status;
        ui->Affichage->setText(version);
        ui->Affichage->update();

        // RF field ON
        RF_Power_Control(&MonLecteur, TRUE, 0);

        status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
        qDebug() << "ISO14443_3_A_PollCard" << status;

    }



    void Fenetre::on_saisie_clicked(){


        int16_t status = 0;
        uint8_t atq[2];
        uint8_t sak[1];
        uint8_t uid[12];
        uint16_t uid_len = 12;
        uint8_t dataName[16];
        uint8_t dataNickname[16];

        MonLecteur.Type = ReaderCDC;
        MonLecteur.device = 0;

        status = Mf_Classic_Read_Block(&MonLecteur, TRUE, ((2*4)+1), dataNickname, Auth_KeyA, 2);
        qDebug() << "Mf_Classic_Read_Sector_Nickname" << status;

        status = Mf_Classic_Read_Block(&MonLecteur, TRUE, ((2*4)+2), dataName, Auth_KeyA, 2);
        qDebug() << "Mf_Classic_Read_Sector_Name" << status;

        ui->fenetre_saisi->setText((char*)dataName);
        ui->fenetre_saisi->update();

        ui->affiche_prenom->setText((char*)dataNickname);
        ui->affiche_prenom->update();

    }

//Ecriture
    void Fenetre::on_Write_clicked(){


        int16_t status = 0;
        uint8_t atq[2];
        uint8_t sak[1];
        uint8_t uid[12];
        uint16_t uid_len = 12;
        uint8_t dataNom[16];
        uint8_t dataPrenom[16];


        QString Text = ui->fenetre_write->toPlainText();
        QString Text1 = ui->fenetre_prenom->toPlainText();
        strcpy((char*)dataNom, (char*)Text.toLatin1().data());
        strcpy((char*)dataPrenom, (char*)Text1.toLatin1().data());

        status = Mf_Classic_Write_Block(&MonLecteur, TRUE, ((2*4)+1), dataPrenom, Auth_KeyB, 2);
        qDebug() << "Mf_Classic_Write_Sector_Prenom" << status;

        status = Mf_Classic_Write_Block(&MonLecteur, TRUE, ((2*4)+2), dataNom, Auth_KeyB, 2);
        qDebug() << "Mf_Classic_Write_Sector_Nom" << status;
        qDebug() << "Nom : " << Text;
        qDebug() << "Prénom : " << Text1;



    }

    void Fenetre::on_button_in_clicked(){

        uint32_t value =0;


        status = Mf_Classic_Increment_Value(&MonLecteur, TRUE, ((3*4)+2), 1, ((3*4)+1), Auth_KeyB,3);
        status = Mf_Classic_Restore_Value(&MonLecteur, TRUE, ((3*4)+1), ((3*4)+2), Auth_KeyB,3);

        status = Mf_Classic_Read_Value(&MonLecteur, TRUE, ((3*4)+2), &value, Auth_KeyA, 3);
        qDebug() << "Mf_Classic_Read_Value_14" << status;

        status = Mf_Classic_Read_Value(&MonLecteur, TRUE, ((3*4)+1), &value, Auth_KeyA, 3);
        qDebug() << "Mf_Classic_Read_Value_13" << status;


    }





    void Fenetre::on_button_de_clicked(){
        int16_t status = 0;
        uint32_t valueCard = 0;
        uint32_t value = (uint32_t) ui->/*l-ValeurIncrement*/ ->text().toInt();

        status = Mf_Classic_Read_Value(&MonLecteur, TRUE, ((3*4)+2), &valueCard, Auth_KeyA, 3);

        if(valueCard == 0 || valueCard-value > valueCard)
        {
            // On ne peut pas décrémenter plus (unsigned --> on évite d'atteindre 2^32-1)
            // TO DO Status bar
            ui->statusBar->showMessage("Il faut plus de crédit pour pouvoir décrémenter");
        }
        else
        {

            status = Mf_Classic_Decrement_Value(&MonLecteur, TRUE, ((3*4)+2), value, ((3*4)+1), Auth_KeyA, 3);

            if(status != 0)
                qDebug() << GetErrorMessage(status);

            status = Mf_Classic_Restore_Value(&MonLecteur, TRUE, ((3*4)+1), ((3*4)+2), Auth_KeyA, 3);

            if(status != 0)
                qDebug() << GetErrorMessage(status);


            status = Mf_Classic_Read_Value(&MonLecteur, TRUE, ((3*4)+2), &valueCard, Auth_KeyA, 3);


            //ui->l_Credit->setText(QString("Crédit : %1").arg(valueCard));

        }

        //qDebug() << "decrement";
    }

    void Fenetre::on_Quitter_clicked()
    {
        int16_t status = 0;
        RF_Power_Control(&MonLecteur, FALSE, 0);
        status = LEDBuzzer(&MonLecteur, LED_OFF);
        status = CloseCOM1(&MonLecteur);
        qApp->quit();
    }
