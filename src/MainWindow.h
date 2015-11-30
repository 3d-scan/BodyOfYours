#pragma once

#include <QMainWindow>
#include <QStackedLayout>
#include <QGridLayout>
#include <QTabWidget>

#include "MeshViewer.h"
#include "PatientTreeWidget.h"
#include "Scanner.h"
#include "FormViewer.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

public slots:
  void openScan(QString filename);
  void calculateDiff();
  void calculateMirror();

private:
  MeshViewer* viewer_;
  Scanner* scanner_;
  QGridLayout* main_layout;
  PatientTreeWidget* patient_widget_;
  QTabWidget* viewport_tabs_;
  FormViewer* form_viewer_;
};
