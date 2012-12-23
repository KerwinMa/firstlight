#ifndef FLT_STRING_UTIL_H
#define FLT_STRING_UTIL_H

#include "common.h"

namespace flt
{
	class StringUtil
	{
	public:
		/** Removes any whitespace characters, be it standard space or
		TABs and so on.
		@remarks
		The user may specify wether they want to trim only the
		beginning or the end of the String ( the default action is
		to trim both).
		*/
		static void trim( stringc& str, bool left = true, bool right = true );

		/** Returns a StringVector that contains all the substrings delimited
		by the characters in the passed <code>delims</code> argument.
		@param
		delims A list of delimiter characters to split by
		@param
		maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
		parameters is > 0, the splitting process will stop after this many splits, left to right.
		*/
		static std::vector< stringc > split( const stringc& str, const stringc& delims = "\t\n ", unsigned int maxSplits = 0);

		static int splitOutFloats(float* &floats, const stringc& str, const stringc& delims = ",");

		/** Upper-cases all the characters in the string.
		*/
		static void toLowerCase( stringc& str );

		/** Lower-cases all the characters in the string.
		*/
		static void toUpperCase( stringc& str );


		/** Returns whether the string begins with the pattern passed in.
		@param pattern The pattern to compare with.
		@param lowerCase If true, the end of the string will be lower cased before
		comparison, pattern should also be in lower case.
		*/
		static bool startsWith(const stringc& str, const stringc& pattern, bool lowerCase = true);

		/** Returns whether the string ends with the pattern passed in.
		@param pattern The pattern to compare with.
		@param lowerCase If true, the end of the string will be lower cased before
		comparison, pattern should also be in lower case.
		*/
		static bool endsWith(const stringc& str, const stringc& pattern, bool lowerCase = true);

		/** Method for standardising paths - use forward slashes only, end with slash.
		*/
		static stringc standardisePath( const stringc &init);

		/** Method for splitting a fully qualified filename into the base name
		and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFilename(const stringc& qualifiedName,
			stringc& outBasename, stringc& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const stringc& qualifiedName, 
			stringc& outBasename, stringc& outExtention, 
			stringc& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const stringc& fullName, 
			stringc& outBasename, stringc& outExtention);


		/** Simple pattern-matching routine allowing a wildcard pattern.
		@param str String to test
		@param pattern Pattern to match against; can include simple '*' wildcards
		@param caseSensitive Whether the match is case sensitive or not
		*/
		static bool match(const stringc& str, const stringc& pattern, bool caseSensitive = true);



		/// Constant blank string, useful for returning by ref where local does not exist
		static const stringc BLANK;
	};
}

#endif //FLT_STRING_UTIL_H