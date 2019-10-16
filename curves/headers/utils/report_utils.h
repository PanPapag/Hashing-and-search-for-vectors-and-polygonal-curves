#ifndef REPORT_UTILS
#define REPORT_UTILS

namespace utils {
  namespace report {
    /** \brief Given ExitCode report correspodent error and exit
      @par const ExitCode& code - enumerated ExitCode provided from namespace utils
    */
    void ReportError(const ExitCode& code);
  }
}

#endif
