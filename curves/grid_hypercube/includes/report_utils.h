#ifndef GRID_HYPERCUBE_REPORT_UTILS
#define GRID_HYPERCUBE_REPORT_UTILS

#include "./utils.h"

namespace utils {
  namespace report {
    /** \brief Given ExitCode report correspodent error and exit
      @par const ExitCode& code - enumerated ExitCode provided from namespace utils
    */
    void ReportError(const ExitCode& code);
  }
}

#endif
