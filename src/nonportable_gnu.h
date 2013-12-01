/*
 * nonportable_gnu.h
 *
 * Include this header to use the nonportable
 * GNU extensions to POSIX.
 *
 * Do not include this header in any vera++
 * checks as it defines the reserved word
 * _GNU_SOURCE which is technically an error
 * except in these circumstances
 *
 * See LICENCE.md for Copyright information
 */

#ifndef YIQI_NONPORTABLE_GNU_H
#define YIQI_NONPORTABLE_GNU_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#endif
