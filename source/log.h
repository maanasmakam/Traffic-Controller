/****************************************************************************
 * Filename : log.h
 * Description : File to enable printf in debug mode only.
 * Author : Maanas Makam Dileep Kumar
 * Reference : Logic taken from Prof. Howdy's slides.
 * Tools Used : MCUXpresso IDE
 ****************************************************************************/
#ifndef LOG_H_
#define LOG_H_
#include <stdio.h>
#ifdef DEBUG
#	define log printf
#else
#  define log(...)
#endif
#endif /* LOG_H_ */
