#include "PID.h"

PID::PID() {}

PID::~PID() {}

// Initialise PID and define initial errors
void PID::Init(double Kp_, double Ki_, double Kd_) {
  Kp     = Kp_;
  Ki     = Ki_;
  Kd     = Kd_;
  pError = 0.0;
  iError = 0.0;
  dError = 0.0;
}

void PID::UpdateError(double cte) {
    dError =  cte - pError;
    pError =  cte;
    iError += cte;
}

double PID::TotalError() {
  return (Kp * pError) + (Ki * iError) + (Kd * dError);
}
