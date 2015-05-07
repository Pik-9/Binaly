/**
 * \file SettingsDialog.hpp
 * \author Daniel Steinhauer
 * \brief This file contains the application's settings dialog.
 */

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

/**
 * \class ColorButton
 * \brief A push button the user can pick a color with.
 */
class ColorButton : public QPushButton
{
  Q_OBJECT
protected:
  QColorDialog *dialog;
  QColor chosenColor;
  
public:
  /**
   * \brief The constructor.
   * \param color The color the button should have at the beginning.
   * \param parent The parent widget.
   */
  ColorButton (QColor color = Qt::white, QWidget *parent = 0);
  
  /**
   * \brief The copy constructor
   * \param other The \c ColorButton to copy.
   */
  ColorButton (const ColorButton&);
  
  /**
   * \brief The destructor.
   */
  virtual ~ColorButton ();
  
  /**
   * \brief Get the selected color.
   * \return The selected color.
   */
  QColor getColor ();
  
protected:
  /**
   * \brief Overwriting the paint event.
   */
  void paintEvent (QPaintEvent*);
  
public slots:
  /**
   * \brief This slot sets the selected color. It is normally called by the color dialog.
   * \param color The new color to set.
   */
  void setColor (QColor);
};

/**
 * \class SettingsDialog
 * \brief The settings dialog widget.
 */
class SettingsDialog : public QWidget
{
  Q_OBJECT
private:
  /**
   * \var settings
   * \brief The application's global settings.
   */
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
  /**
   * \brief The constructor.
   * \param appSettings A pointer to the application's settings. It should be initialized at the very beginning.
   * \param parent The parent widget.
   */
  SettingsDialog (QSettings*, QWidget *parent = 0);
  
  /**
   * \brief The destructor.
   */
  virtual ~SettingsDialog ();
  
public slots:
  /**
   * \brief This slot writes the new settings to \c settings and hides this window.
   */
  void applySettings ();
};

#endif