#include "Hexedit.hpp"

#include <math.h>
#include <stdlib.h>

const unsigned int HexWidget::bases[4] = {2, 8, 10, 16};

HexWidget::HexWidget (QWidget* parent)
  : QWidget (parent), stream (NULL), /*currentBlockData (0),*/ customData (0)
{
  lyt = new QGridLayout (this);
  c_encoding = new QComboBox ();
  hex_table = new QTableWidget ();
  
  c_encoding->addItem (tr ("Binary"));
  c_encoding->addItem (tr ("Octal"));
  c_encoding->addItem (tr ("Decimal"));
  c_encoding->addItem (tr ("Hexadecimal"));
  c_encoding->setCurrentIndex (3);
  hex_table->setColumnCount (33);
  
  lyt->addWidget (c_encoding, 0, 1, 1, 1);
  lyt->addWidget (hex_table, 1, 0, 1, 3);
  
  table_printed = false;
  
  connect (c_encoding, SIGNAL (currentIndexChanged (int)), this, SLOT (encodingChanged (int)));
  connect (hex_table, SIGNAL (itemChanged (QTableWidgetItem*)), this, SLOT (editCell (QTableWidgetItem*)));
}

HexWidget::~HexWidget ()
{}

QChar HexWidget::printableMatch (const char bare)
{
  QChar RET (bare);
  if ((bare < 32) || (bare == 127) || (bare == 255))  {
    RET = QChar ('.');
  }
  return RET;
}

void HexWidget::setFileStream (Hexfile* filestream)
{
  stream = filestream;
  edited = false;
  table_printed = false;
  customData.clear ();
}

void HexWidget::loadData (const uint64_t position)
{
  Blockinfo bi = stream->getBlockAt (position);
  bool need2createBlock = true;
  for (unsigned int ii = 0; ii < customData.size (); ++ii)  {
    if (bi.begin == customData[ii].begin)  {
      currentIndex = ii;
      need2createBlock = false;
      break;
    }
  }
  
  if (need2createBlock)  {
    FilePart fp;
    fp.begin = bi.begin;
    fp.data = stream->getBlockDataAt (position);
    currentIndex = customData.size ();
    customData.push_back (fp);
  }
  
  /* Lock the table widget for user edits. */
  table_printed = false;
  
  edited = false;
  printData ();
  
  /* Unlock the table widget and react on user's changes from now on. */
  table_printed = true;
}

void HexWidget::saveChanges ()
{
  if (stream)  {
    for (unsigned int ii = 0; ii < customData.size (); ++ii)  {
      stream->writeBlock (customData[ii].begin, customData[ii].data);
    }
  }
}

void HexWidget::printData ()
{
  hex_table->clear ();
  hex_table->setRowCount (ceil (customData[currentIndex].data.size () / 16.0));
  for (unsigned int pos = 0; pos < customData[currentIndex].data.size (); ++pos)  {
    div_t tablePos = div (pos, 16);
    if (!tablePos.rem)  {
      QTableWidgetItem *indexfield = new QTableWidgetItem (
        QString ().setNum (customData[currentIndex].begin + pos, bases[c_encoding->currentIndex ()])
      );
      QFont boldFont;
      boldFont.setBold (true);
      indexfield->setFont (boldFont);
      indexfield->setTextAlignment (Qt::AlignCenter);
      indexfield->setFlags (indexfield->flags () & (Qt::ItemIsEditable ^ 255));
      hex_table->setItem (tablePos.quot, 0, indexfield);
    }
    QTableWidgetItem *hexfield = new QTableWidgetItem (QString ().setNum (
      (unsigned int) (customData[currentIndex].data[pos] & 255), bases[c_encoding->currentIndex ()])
    );
    QTableWidgetItem *asciifield = new QTableWidgetItem (QString (printableMatch (customData[currentIndex].data[pos])));
    
    hexfield->setForeground (Qt::black);
    hexfield->setTextAlignment (Qt::AlignCenter);
    asciifield->setForeground (Qt::gray);
    asciifield->setTextAlignment (Qt::AlignCenter);
    
    hex_table->setItem (tablePos.quot, tablePos.rem + 1, hexfield);
    hex_table->setItem (tablePos.quot, tablePos.rem + 17, asciifield);
  }
  
  hex_table->resizeColumnsToContents ();
  hex_table->resizeRowsToContents ();
}

void HexWidget::encodingChanged (int newVal)
{
  if (stream)  {
    printData ();
  }
}

void HexWidget::editCell (QTableWidgetItem* cell)
{
  if (table_printed)  {
    edited = true;
    QFont cellFont;
    QTableWidgetItem *hexCell, *asciiCell;
    if (cell->column () > 16)  {
      QChar cc = (cell->text ().isEmpty() ? QChar (0) : cell->text ()[0]);
      customData[currentIndex].data[cell->row () * 16 + cell->column () - 17] = cc.cell ();
      asciiCell = cell;
      hexCell = hex_table->item (cell->row (), cell->column () - 16);
      cellFont.setBold (true);
    } else  {
      bool setIntOK;
      unsigned int value = cell->text ().toInt (&setIntOK, bases[c_encoding->currentIndex ()]);
      hexCell = cell;
      asciiCell = hex_table->item (cell->row (), cell->column () + 16);
      if ((setIntOK) && (value < 256))  {
        customData[currentIndex].data[cell->row () * 16 + cell->column () - 1] = (char) value;
        cellFont.setBold (true);
      }
    }
    
    /* Lock the table widget. Don't react on cell changes, because they aren't from the user. */
    table_printed = false;
    char curc = customData[currentIndex].data[hexCell->row () * 16 + hexCell->column () - 1];
    hexCell->setText (QString ().setNum(
      (unsigned int) (curc & 255), bases[c_encoding->currentIndex ()]
    ));
    asciiCell->setText (printableMatch (curc));
    hexCell->setFont (cellFont);
    asciiCell->setFont (cellFont);
    
    /* Unlock the table widget. Any further cell changes come from the user. */
    table_printed = true;
    
    emit fileChanged ();
  }
}