#pragma once

#include "scanner_3d.h"

#include <reconstructmesdk/reme.h>

class RemeScanner3D : public Scanner3D
{
public:
  RemeScanner3D();
  ~RemeScanner3D();

  QStringList GetComputingDevices() override;
  bool InitComputingDevice(int device_id) override;

private:
  reme_context_t context_;
};