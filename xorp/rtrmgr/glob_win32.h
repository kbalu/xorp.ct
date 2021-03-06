/* /////////////////////////////////////////////////////////////////////////////
 * File:    glob.h
 *
 * Purpose: Declaration of the glob() API functions and types for the
 *          Win32 platform.
 *
 * Created  13th November 2002
 * Updated: 10th January 2005
 *
 * Home:    http://synesis.com.au/software/
 *
 * Copyright 2002-2005, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the names of Matthew Wilson and Synesis Software nor the names of
 *   any contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/** \file glob.h
 *
 * Contains the declarations for the glob() API.
 */

#ifndef SYNSOFT_UNIXEM_INCL_H_GLOB
#define SYNSOFT_UNIXEM_INCL_H_GLOB

#ifndef _SYNSOFT_DOCUMENTATION_SKIP_SECTION
# define SYNSOFT_UNIXEM_VER_H_GLOB_MAJOR    2
# define SYNSOFT_UNIXEM_VER_H_GLOB_MINOR    0
# define SYNSOFT_UNIXEM_VER_H_GLOB_REVISION 2
# define SYNSOFT_UNIXEM_VER_H_GLOB_EDIT     21
#endif /* !_SYNSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/** \weakgroup unixem Synesis Software UNIX Emulation for Win32
 * \brief The UNIX emulation library
 */

/** \weakgroup unixem_glob glob() API
 * \ingroup UNIXem unixem
 * \brief This API provides facilities for enumerating the file-system contents
 * @{
 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WIN32
# error This file is only currently defined for compilation on Win32 systems
#endif /* _WIN32 */

/* /////////////////////////////////////////////////////////////////////////////
 * Constants and definitions
 */

/* Error codes */
#define GLOB_NOSPACE    (1)             /*!< \brief (Error result code:) An attempt to allocate memory failed, or if errno was 0 GLOB_LIMIT was specified in the flags and ARG_MAX patterns were matched. */
#define GLOB_ABORTED    (2)             /*!< \brief (Error result code:) The scan was stopped because an error was encountered and either GLOB_ERR was set or (*errfunc)() returned non-zero. */
#define GLOB_NOMATCH    (3)             /*!< \brief (Error result code:) The pattern does not match any existing pathname, and GLOB_NOCHECK was not set int flags. */
#define GLOB_NOSYS      (4)             /*!< \brief (Error result code:) . */
#define GLOB_ABEND      GLOB_ABORTED    /*!< \brief (Error result code:) . */

/* Flags */
#define GLOB_ERR            0x00000001  /*!< \brief Return on read errors.  */
#define GLOB_MARK           0x00000002  /*!< \brief Append a slash to each name.  */
#define GLOB_NOSORT         0x00000004  /*!< \brief Don't sort the names.  */
#define GLOB_DOOFFS         0x00000008  /*!< \brief Insert PGLOB->gl_offs NULLs.  */
#define GLOB_NOCHECK        0x00000010  /*!< \brief If nothing matches, return the pattern.  */
#define GLOB_APPEND         0x00000020  /*!< \brief Append to results of a previous call.  */
#define GLOB_NOESCAPE       0x00000040  /*!< \brief Backslashes don't quote metacharacters.  */
#define GLOB_PERIOD         0x00000080  /*!< \brief Leading `.' can be matched by metachars.  */

# define GLOB_MAGCHAR       0x00000100  /*!< \brief Set in gl_flags if any metachars seen.  */
# define GLOB_ALTDIRFUNC    0x00000200  /*!< \brief Use gl_opendir et al functions.  */
# define GLOB_BRACE         0x00000400  /*!< \brief Expand "{a,b}" to "a" "b".  */
# define GLOB_NOMAGIC       0x00000800  /*!< \brief If no magic chars, return the pattern.  */
# define GLOB_TILDE         0x00001000  /*!< \brief Expand ~user and ~ to home directories. */
# define GLOB_ONLYDIR       0x00002000  /*!< \brief Match only directories.  */
# define GLOB_TILDE_CHECK   0x00004000  /*!< \brief Like GLOB_TILDE but return an error */

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 */

/** \brief Result structure for glob()
 *
 * This structure is used by glob() to return the results of the search.
 */
typedef struct
{
  int   gl_pathc;   /*!< count of total paths so far */
  int   gl_matchc;  /*!< count of paths matching pattern */
  int   gl_offs;    /*!< reserved at beginning of gl_pathv */
  int   gl_flags;   /*!< returned flags */
  char  **gl_pathv; /*!< list of paths matching pattern */
} glob_t;

/* /////////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** \brief Generates pathnames matching a pattern
 *
 * This function is a pathname generator that implements the rules for
 * file name pattern matching used by the UNIX shell.
 *
 * \param pattern The pattern controlling the search
 * \param flags A combination of the <b>GLOB_*</b> flags
 * \param errfunc [Not currently supported. Must be NULL]
 * \param pglob Pointer to a glob_t structure to receive the search results
 * \return 0 on success, otherwise one of the <b>GLOB_*</b> error codes
 */
int glob( char const  *pattern
        , int         flags
        , int       (*errfunc)(char const *, int)
        , glob_t      *pglob);

/** \brief Frees the results of a call to glob
 *
 * This function releases any memory allocated in a call to glob. It must
 * always be called for a successful call to glob.
 *
 * \param pglob Pointer to a glob_t structure to receive the search results
 */
void globfree(glob_t *pglob);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* ////////////////////////////////////////////////////////////////////////// */

/** @} // end of group unixem_glob */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* SYNSOFT_UNIXEM_INCL_H_GLOB */

/* ////////////////////////////////////////////////////////////////////////// */
