#ifndef VENUS_QUILL_PRECOMPILED_HPP
#define VENUS_QUILL_PRECOMPILED_HPP

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogFunctions.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/RotatingJsonFileSink.h"
#include "quill/sinks/RotatingSink.h"

// If you are using Clangd you will see several "unused-includes" warnings.
// These are false warnings as this is a pre-compiled-header file to cut compile time.
// You should ignore these warnings, DO NOT directly include these files into "VN_logger.cpp" OR "VN_logger.hpp".

#endif  // VENUS_QUILL_PRECOMPILED_HPP