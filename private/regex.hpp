#ifndef REGEX_HPP
# define REGEX_HPP

#include <stdio.h>
#include <string.h>
#include <cstring>

/****************************************************************************/
// Basic definitions of types. Feel free to remove if you have them already defined.

#ifndef FALSE
	#define	FALSE		(0)
#endif
#ifndef TRUE
	#define	TRUE		(1)
#endif
#ifndef MAX
	#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif
typedef unsigned char		MYBOOL, UCHAR;		// Feel free to change boolean to 'int' or C++ 'bool'. 

typedef signed int INT32;			// Linux
#define nullptr NULL				// Linux

/****************************************************************************/
// Fast Comparison of 8,16,32 bits int - without branching. Use in loops where speed is crucial. 
#define		   is_int_negative(       num1     ) ((((INT32) (num1)        )&0x80000000)>>31)	// (num1<0   ) ? 1 : 0
inline INT32 is_int_notZero( INT32  num1     ){	return is_int_negative(num1|-num1); }			// (num1!=0  ) ? 1 : 0, Since (-X)|X is 0 for X==0 and negative for every other X.
#define		   is_int_notEqual(	   num1,num2)	is_int_notZero(num1^num2)						// (num1!=num2)? 1 : 0
#define		   is_int_equal(	   num1,num2)	(is_int_notZero(num1^num2)^0x1)					// (num1==num2)? 1 : 0
#define        is_int_inRange(i,L,U)  ((((INT32((i)-(L)))|(INT32((U)-(i))))^0x80000000)>>31)	// ((i>=L)&&(i<=U) ? 1 : 0
#define        isDigit(               chr)       is_int_inRange(chr,'0','9')					// ('9'>=chr>='0') ? 1 : 0

#define STRCPY_S_SUCCESS 0
#define STRCPY_S_INVALID_ARGUMENT 1
#define STRCPY_S_BUFFER_TOO_SMALL 2

/****************************************************************************/
// Define the types of special regex character commands (as bits). 
#define		TYPE_CHAR				(0)			// Default 0 -  Single character. Like: A b 7 .
#define     TYPE_PREFIX				(1)			// Sepcial '\\' command for Abbreviations and using special characters. like \\?. This is the only command that actually uses 2 characters
#define     TYPE_SUFFIX				(2)			// Command has iteration suffix like a*, z+, b{3,5} 
#define     TYPE_OPEN				(4)			// Left Parentheses: On of the following { ( [ . This rule opens a sub expression
#define     TYPE_CLOSE				(8)			// Right parentheses: } ) ]
#define     TYPE_RECURSION			(16)		// Termination of recursive call. (command is a suffix of previous one. Like previous is 'A' and current is '{2}') Todo: Not used yet

#define		NO_MATCH				(-1)		// Returned when regular expression cannot be matched to the string.

/****************************************************************************/
// Define a command structure (single language rule)
typedef struct{
    char	id;			// The character which represennts the command. Like: * ? [
    char	attr;		// Type of the command. Can be combination of above types
    void*	f;			// Pointer to function which proccesses the current command. Polymorphism (C style)
} Cmd;

#define cmdLength(  cmd)		(1 + ((cmd)->attr&TYPE_PREFIX))		// All commands take 1 character + optional prefix character
#define    isSuffix(cmd)		(     (cmd)->attr&TYPE_SUFFIX)		// Does current command is a suffix of previous one. Like previous is 'A' and current is '{2}'
#define    isOpen(  cmd)        (     (cmd)->attr&TYPE_OPEN  )		// Does this command opens a sub expression
#define get_cmd(c)               get_cmd_byChar[(c)&0x7F]			// Get the command strucutre by character. For '(' will return '(' command
#define isReservedSymbol(c)      (get_cmd(c)->id != 0)				// Is the given character a reserved symbol (used as regex command)

/**
 * Define pointers to functions that process the special commands.
 * Standard functions and characters like () . [] \\ A 7 and son one ...	
*/
typedef int (*StandFunc)(const char* pattern, const char* sample);	

/**
 * Define pointers to functions that process the special commands.
 * Suffix functions for multiple occurences like * + ? {}
*/
typedef int (*SuffixFunc)(const char* pattern, const char* sample,const char* endpattern);

/**
 * Struct: tCompiledRegex
 * 
 * Structures for compilation of regex pattern (faster execution of regex, at cost of initial calculation and memory usage). 
 * The faster execution is achieved by precalculating for each expression its length
 * 
 * Private Members:
 *   start - A pointer to the start of the compiled pattern.
 *   end - A pointer to the end of the compiled pattern.
 *   exprLen - An array storing the lengths of each sub-expression within the pattern, for fast navigation. 
 *             Supports maximum expression length of 256 bytes.
 *   unionLen - An array storing the lengths of each union set within the pattern (e.g., in 'aab|ccd|ef'), 
 *              facilitating quicker jumps between union sets.
 * 
 * Public Methods:
 *   getExpressionEnd - Given a pointer to the beginning of a sub-expression, returns a pointer to its end.
 *   getExpressionEnd_UnionSet - Similar to getExpressionEnd, but for union sets within the pattern.
 *   compile - Compiles the provided regular expression pattern. Assumes the pattern is a legal expression
 *             no longer than 256 bytes. Returns a pointer to the end of the pattern on success, or NULL on failure.
 *   search - Searches for the compiled pattern in a provided sample string. Returns a pointer to the beginning
 *            of the matched part and updates resLen with the length of the matched part.
 * 
 * Note: 
 *   This structure is designed for use with regular expressions of limited size and complexity. It is optimized 
 *   for performance, leveraging pre-computed lengths of sub-expressions and union sets.
 */
struct tCompiledRegex
{
private:
    const char* start;
	const char* end;
	unsigned char exprLen[256];
	unsigned char unionLen[256];

public:
	const char* getExpressionEnd(         const char *ex) const { return ex + exprLen[ (int)(ex-start)]; }
	const char* getExpressionEnd_UnionSet(const char *ex) const { return ex + unionLen[(int)(ex-start)]; } 
	
	const char* compile(const char *pattern);
	const char* search( const char *sampleString, int* resLen) const;
};

const char* regex_search(const char* pattern, const char* sampleString, int* resLen);

/**
 * There are 6 predicate functions.
 * 
 * - First parameter is the pattern. 
 * - Second is the sample string and optional third is pointer to the end of the pattern 
 * - Each one returns the number of consumed characters in sample string or NO_MATCH if inapplicable 
 * (like 'a+' was applied on 'bbb'). Note 'a*' can be applied on 'bbb' and it consumes zero characters.
 * */ 
// 
int c_achar(	 const char* pattern, const char* sample);
int c_any(	 const char* pattern, const char* sample);
int c_extended(const char* pattern, const char* sample);
int c_group(	 const char* pattern, const char* sample);
int c_option(	 const char* pattern, const char* sample);
int c_multi(	 const char* pattern, const char* sample, const char* endpattern);

void regex_debug(const char* pattern, const char* sampleString, const char* trueAnswer);
int regex_test(const char* pattern, const char* sampleString, const char* trueAnswer);

#endif
