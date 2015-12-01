#include "FormViewer.h"
#include "PatientData.h"

FormViewer::FormViewer(QWidget* parent /*= nullptr*/)
  : QStackedWidget(parent)
{
  patient_form_ = new PatientForm(this);
  exam_form_ = new ExaminationForm(this);
  addWidget(patient_form_);
  addWidget(exam_form_);
}

FormViewer::~FormViewer()
{
  delete patient_form_;
  delete exam_form_;
}

void FormViewer::showPatient(PatientData data)
{
  patient_form_->clear();
  exam_form_->clear();
  setCurrentIndex(0);
  patient_form_->fill(data);
}

void FormViewer::ShowExamination(ExaminationData data)
{
  patient_form_->clear();
  exam_form_->clear();
  setCurrentIndex(1);
  exam_form_->fill(data);
}

void FormViewer::newPatient()
{
  patient_form_->clear();
  setCurrentIndex(0);
}

void FormViewer::newExamination()
{
  exam_form_->clear();
  setCurrentIndex(1);
}
