#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>

namespace Ui {
    class Fenetre;
}

class Fenetre : public QWidget
{
    Q_OBJECT

public:
    explicit Fenetre(QWidget *parent = 0);
    ~Fenetre();

private slots:
            void on_Connect_clicked();
    void on_saisie_clicked();
    void on_Write_clicked();
    void on_Quitter_clicked();
    void on_button_in_clicked();
    void on_button_de_clicked();


private:
    Ui::Fenetre *ui;
};

#endif // FENETRE_H
