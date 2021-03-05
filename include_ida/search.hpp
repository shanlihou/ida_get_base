/*
 *      Interactive disassembler (IDA).
 *      ALL RIGHTS RESERVED.
 *      Copyright (c) 1990-2015 Hex-Rays
 *
 */

#ifndef __SEARCH_HPP
#define __SEARCH_HPP

/*! \file search.hpp

  \brief Middle-level search functions

  They all are controlled by \ref SEARCH_
*/

/// \defgroup SEARCH_ Search flags
//@{
#define SEARCH_UP       0x000           ///< search towards lower addresses
#define SEARCH_DOWN     0x001           ///< search towards higher addresses
#define SEARCH_NEXT     0x002           ///< useful only for search() and find_binary().
                                        ///< for other find_.. functions it is implicitly set
#define SEARCH_CASE     0x004           ///< case-sensitive search (case-insensitive otherwise)
#define SEARCH_REGEX    0x008           ///< regular expressions  in search string (only supported for txt search)
#define SEARCH_NOBRK    0x010           ///< don't test for ctrl-break to interrupt the search
#define SEARCH_NOSHOW   0x020           ///< don't display the search progress/refresh screen
#define SEARCH_UNICODE  0x040           ///< treat literal strings as unicode (find_binary() only)
#define SEARCH_IDENT    0x080           ///< search for an identifier (text search).
                                        ///< it means that the characters before
                                        ///< and after the match can not be is_visible_char().
#define SEARCH_BRK      0x100           ///< return #BADADDR if Ctrl-Break wass pressed during search
//@}


/// Is the #SEARCH_DOWN bit set?

inline bool search_down(int sflag)      { return (sflag & SEARCH_DOWN) != 0; }


/// \name find_... functions
/// \param ea          start ea
/// \param sflag       combination of \ref SEARCH_
/// \param[out] opnum  filled with operand number whenever relevant
/// \return first ea at which the search criteria is met
//@{


/// Find next error or problem

idaman ea_t ida_export find_error(ea_t ea, int sflag, int *opnum=NULL);


/// Find next operand without any type info

idaman ea_t ida_export find_notype(ea_t ea, int sflag, int *opnum=NULL);


/// Find next unexplored address

idaman ea_t ida_export find_unknown(ea_t ea, int sflag);


/// Find next ea that is the start of an instruction or data

idaman ea_t ida_export find_defined(ea_t ea, int sflag);


/// Find next suspicious operand

idaman ea_t ida_export find_suspop(ea_t ea, int sflag, int *opnum=NULL);


/// Find next data address

idaman ea_t ida_export find_data(ea_t ea,int sflag);


/// Find next code address

idaman ea_t ida_export find_code(ea_t ea,int sflag);


/// Find next code address that does not belong to a function

idaman ea_t ida_export find_not_func(ea_t ea,int sflag);


/// Find next immediate operand with the given value

idaman ea_t ida_export find_imm(ea_t newEA, int sflag, uval_t srchValue, int *opnum=NULL);


/// Find next address that denotes the start of the specified binary sequence (specified in text form).
/// \param startea,endea   range to search
/// \param ubinstr,radix   see user2bin()

idaman ea_t ida_export find_binary(
        ea_t startea,
        ea_t endea,
        const char *ubinstr,
        int radix,
        int sflag);


/// See search()

idaman ea_t ida_export find_text(ea_t start_ea, int y, int x, const char *ustr, int sflag);

//@}

class place_t;


/// Search for a text substring (low level function).
/// \param ud              line array parameter
/// \param[in,out] start   pointer to starting place:
///                          - start->ea:    starting address
///                          - start->lnnum: starting Y coordinate
/// \param end             pointer to ending place:
///                          - end->ea:       ending address
///                          - end->lnnum:    ending Y coordinate
/// \param[in,out] startx  pointer to starting X coordinate
/// \param str             substring to search for.
/// \param sflag           \ref SEARCH_
/// \retval 0  substring not found
/// \retval 1  substring found. The matching position is returned in:
///              - start->ea:       address
///              - start->lnnum:    Y coordinate
///              - *startx:         X coordinate
/// \retval 2  search was cancelled by ctrl-break.
///            The farthest searched address is
///            returned in the same manner as in the successful return (1).
/// \retval 3  the input regular expression is bad.
///            The error message was displayed.

idaman int ida_export search(
        void *ud,
        place_t *start,
        const place_t *end,
        int *startx,
        const char *str,
        int sflag);


/// Convert user-specified binary string to internal representation.
/// \param [out] out   buffer for the output sequence of bytes
/// \param [out] mask  buffer for the output comparison mask.
///                    if mask[0] == 0xFF upon return, then there were question marks in
///                    the input text string.
/// \param ea          linear address to convert for (the conversion depends on the
///                    address, because the number of bits in a byte depend on the
///                    segment type)
/// \param in          input text string. contains space-separated:
///                      - numbers (numeric base is determined by 'radix')
///                          - if value of number fits a byte, it is considered as a byte
///                          - if value of number fits a word, it is considered as 2 bytes
///                          - if value of number fits a dword,it is considered as 4 bytes
///                      - "..." string constants
///                      - 'x'  single-character constants
///                      - ?    variable bytes
/// \note Examples of search strings (assuming base 16):
///  CD 21          - bytes 0xCD, 0x21
///  21CD           - bytes 0xCD, 0x21 (little endian ) or 0x21, 0xCD (big-endian)
///  "Hello", 0     - the null terminated string "Hello"
///  L"Hello"       - 'H', 0, 'e', 0, 'l', 0, 'l', 0, 'o', 0
///  B8 ? ? ? ? 90  - byte 0xB8, 4 bytes with any value, byte 0x90
///
/// \param radix       numeric base of numbers (8,10,16)
/// \param unicode     treat strings as unicode
///                    \note L"string" is another way to enter unicode strings
/// \return length of output string.
///         -1 if the input string has bad format (warning is displayed).
/// \note the output buffers are assumed to be #MAXSTR bytes

idaman int ida_export user2bin(
        uchar *out,
        uchar *mask,
        ea_t ea,
        const char *in,
        int radix,
        bool unicode);

#endif // __SEARCH_HPP
