#ifndef D_CHANGE_H
#define D_CHANGE_H

#include <QDialog>
#include <string>
#include <QFontDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class d_change;
}

class d_change : public QDialog
{
    Q_OBJECT

public:
    explicit d_change(QString oname, QWidget *parent = nullptr);
    ~d_change();

   std::string getName();

private slots:

   void on_buttonBox_accepted();

   void accept()
   {
       if(!(value.size() > 5 && value.size() < 14))
       {
           QMessageBox messege; messege.setText("Username is not the right size"); messege.exec();
       }
       else
       {
           done(Accepted);
       }
   }

   void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::d_change *ui;
    QString value;
};

#endif // D_CHANGE_H
