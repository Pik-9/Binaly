#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QWidget>
#include <QColorDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QPaintEvent>
#include <QSettings>

class ColorButton : public QPushButton
{
  Q_OBJECT
protected:
  QColorDialog *dialog;
  QColor chosenColor;
  
public:
  ColorButton (QColor color = Qt::white, QWidget *parent = 0);
  ColorButton (const ColorButton&);
  virtual ~ColorButton ();
  
  QColor getColor ();
  
protected:
  void paintEvent (QPaintEvent*);
  
public slots:
  void setColor (QColor);
};

class SettingsDialog : public QWidget
{
  Q_OBJECT
private:
  QSettings *settings;
  
  QGridLayout *lyt;
  
  QGroupBox *colors_box;
  QGridLayout *cb_lyt;
  QLabel *cb_textCol, *cb_homCol, *cb_binCol, *cb_miscCol;
  ColorButton *cb_textCol_btn, *cb_homCol_btn, *cb_binCol_btn, *cb_miscCol_btn;
  
  QGroupBox *lang_box;
  QHBoxLayout *lang_lyt;
  QLabel *lang_l;
  QComboBox *lang_c;
  
  QPushButton *ok_btn, *cancel_btn;
  
public:
  SettingsDialog (QSettings*, QWidget *parent = 0);
  virtual ~SettingsDialog ();
  
public slots:
  void applySettings ();
};

#endif