#ifndef REGEX_HPP
# define REGEX_HPP

#include <stdio.h>								// printfs
#include <string.h>								// memory operations like memcmp() memset()
#include <cstring>


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


#endif