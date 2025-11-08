#ifndef haw_conf_h
#define haw_conf_h

// Hawthorn configuration file.
// Manages some things

// ===OS===

// check for unix-style os
#if !defined(_WIN32) &&                                                                            \
	(defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
// posix compliant
#define HAW_UNIX
#define HAW_USE_POSIX

#if defined(__linux) || defined(__linux__)
#define HAW_LINUX
#endif //  defined(__linux) || defined(__linux__)

#if defined(__APPLE__) && defined(__MACH__) && defined(TARGET_OS_MAC)
#define HAW_MACOS
#endif

#endif

#if defined(__CYGWIN__) && !defined(_WIN32)
// Cygwin POSIX under Microsoft Windows.
#define HAW_CYGWIN
#endif

#if defined(_WIN64)
// Microsoft Windows (64-bit)
#define HAW_WINDOWS64
#define HAW_WINDOWS
#elif defined(_WIN32)
// Microsoft Windows (32-bit)
#define HAW_WINDOWS32
#define HAW_WINDOWS
#endif

// ===SYMBOLS===
#define END_OF_LINE ';'

#ifdef HAW_UNIX
#define PATH_SEPARATOR '/'
#endif
#ifdef HAW_WINDOWS
#define PATH_SEPARATOR "\\"
#endif

// ignore value symbol
#define HAW_IGNORE_SYMBOL '_'
// like the `#` in '#include'
#define HAW_FLAG_SYMBOL '~'
// like the `//` this symbol considered doubled
// when checking
#define HAW_COMMENT_START '?'

// ===FORMATS===

#define HAW_FLOAT_FORMAT "%g"
#define HAW_DOUBLE_FORMAT "%g"
#define HAW_STR_FORMAT "%s"
#define HAW_CHAR_FORMAT "%c"

// ===CONVERTS===

#define haw_strtoint(str) stoi(str)
#define haw_strtofloat(str) stof(str)
#define haw_strtodouble(str) stod(str)
#define haw_strtolongdouble(str) stold(str)

// ===PREFIX===
// does we need it?

#define HAW_PREFIX_SIGNED 's'
#define HAW_PREFIX_UNSIGNED 'u'
#define HAW_PREFIX_FLOAT 'f'
#define HAW_PREFIX_DOUBLE 'd'
#define HAW_PREFIX_LONG 'l'
#endif // !haw_conf_h
