#pragma once

#include "ActionHub.h"
#include "Action.h"

#include "StackedFormWidget.h"
#include "FormViewer.h"
#include "MeshViewer.h"
#include "PatientTreeWidget.h"
#include "Scanner.h"

CREATE_ACTION(ActionAddNewPatient, {
  widget_->switchTo(StackedFormWidget::PATIENT_FORM);
}, StackedFormWidget* widget_)

CREATE_ACTION(ActionAddNewExamination, {
  form_viewer->newExamination();
}, FormViewer* form_viewer)

CREATE_ACTION(ActionDeleteCurrentItem, {
  widget->removeCurrentItem();
}, PatientTreeWidget* widget)

CREATE_ACTION(ActionShowScanMesh, {
  widget->showScan();
}, PatientTreeWidget* widget)

CREATE_ACTION(ActionStartReconstruction, {
  scanner->startReconstruction();
}, Scanner* scanner)

CREATE_ACTION(ActionStopReconstruction, {
  scanner->stopReconstruction();
}, Scanner* scanner)

CREATE_ACTION(ActionMeshViewerClear, {
  viewer->clearMesh();
}, MeshViewer* viewer);