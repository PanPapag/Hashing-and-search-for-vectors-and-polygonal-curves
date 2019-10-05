#ifndef ARGS_UTILS
#define ARGS_UTILS

namespace utils {
  namespace args {
    /**
      ReadArguments - Reads arguments given by user in the command line
      @par int argc The number of arguments
      @par char **argv The array of the arguments' values
      return: ExitCode
    */
    int ReadArguments(int argc, char **argv, struct InputInfo &input_info);
    /**
      ScanArguments - Reads arguments given by user in the stdin
      return: ExitCode
    */
    int ScanArguments(struct InputInfo &input_info);
  }
}

#endif
