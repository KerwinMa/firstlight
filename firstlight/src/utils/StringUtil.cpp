#include "firstlight.h"
#include <algorithm>

namespace flt
{

	//-----------------------------------------------------------------------
	const stringc StringUtil::BLANK;
	//-----------------------------------------------------------------------
	void StringUtil::trim(stringc& str, bool left, bool right)
	{
		static const stringc delims = " \t\r";
		if(right)
			str.erase(str.find_last_not_of(delims)+1); // trim right
		if(left)
			str.erase(0, str.find_first_not_of(delims)); // trim left
	}

	//-----------------------------------------------------------------------
	std::vector<stringc> StringUtil::split( const stringc& str, const stringc& delims, unsigned int maxSplits)
	{
		std::vector<stringc> ret;
		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

		unsigned int numSplits = 0;

		// Use STL methods 
		size_t start, pos;
		start = 0;
		do 
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == stringc::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back( str.substr(start) );
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != stringc::npos);



		return ret;
	}

	int StringUtil::splitOutFloats(float* &floats, const stringc& str, const stringc& delims)
	{
		std::vector<stringc> strs = split(str, delims);
		int size = (int)strs.size();
		floats = new float[size];
		for(int i=0; i<size; ++i)
		{			
			sscanf(strs[i].c_str(),"%f",&floats[i]);			
		}

		return size;
	}

	//-----------------------------------------------------------------------
	void StringUtil::toLowerCase(stringc& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			tolower);
	}

	//-----------------------------------------------------------------------
	void StringUtil::toUpperCase(stringc& str) 
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			toupper);
	}
	//-----------------------------------------------------------------------
	bool StringUtil::startsWith(const stringc& str, const stringc& pattern, bool lowerCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		stringc startOfThis = str.substr(0, patternLen);
		if (lowerCase)
			StringUtil::toLowerCase(startOfThis);

		return (startOfThis == pattern);
	}
	//-----------------------------------------------------------------------
	bool StringUtil::endsWith(const stringc& str, const stringc& pattern, bool lowerCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		stringc endOfThis = str.substr(thisLen - patternLen, patternLen);
		if (lowerCase)
			StringUtil::toLowerCase(endOfThis);

		return (endOfThis == pattern);
	}
	//-----------------------------------------------------------------------
	stringc StringUtil::standardisePath(const stringc& init)
	{
		stringc path = init;

		std::replace( path.begin(), path.end(), '\\', '/' );
		if( path[path.length() - 1] != '/' )
			path += '/';

		return path;
	}
	//-----------------------------------------------------------------------
	void StringUtil::splitFilename(const stringc& qualifiedName, 
		stringc& outBasename, stringc& outPath)
	{
		stringc path = qualifiedName;
		// Replace \ with / first
		std::replace( path.begin(), path.end(), '\\', '/' );
		// split based on final /
		size_t i = path.find_last_of('/');

		if (i == stringc::npos)
		{
			outPath.clear();
			outBasename = qualifiedName;
		}
		else
		{
			outBasename = path.substr(i+1, path.size() - i - 1);
			outPath = path.substr(0, i+1);
		}

	}
	//-----------------------------------------------------------------------
	void StringUtil::splitBaseFilename(const stringc& fullName, 
		stringc& outBasename, stringc& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == stringc::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i+1);
			outBasename = fullName.substr(0, i);
		}
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------------
	void StringUtil::splitFullFilename(	const stringc& qualifiedName, 
		stringc& outBasename, stringc& outExtention, stringc& outPath )
	{
		stringc fullName;
		splitFilename( qualifiedName, fullName, outPath );
		splitBaseFilename( fullName, outBasename, outExtention );
	}
	//-----------------------------------------------------------------------
	bool StringUtil::match(const stringc& str, const stringc& pattern, bool caseSensitive)
	{
		stringc tmpStr = str;
		stringc tmpPattern = pattern;
		if (!caseSensitive)
		{
			StringUtil::toLowerCase(tmpStr);
			StringUtil::toLowerCase(tmpPattern);
		}

		stringc::const_iterator strIt = tmpStr.begin();
		stringc::const_iterator patIt = tmpPattern.begin();
		stringc::const_iterator lastWildCardIt = tmpPattern.end();
		while (strIt != tmpStr.end() && patIt != tmpPattern.end())
		{
			if (*patIt == '*')
			{
				lastWildCardIt = patIt;
				// Skip over looking for next character
				++patIt;
				if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the string
					strIt = tmpStr.end();
				}
				else
				{
					// scan until we find next pattern character
					while(strIt != tmpStr.end() && *strIt != *patIt)
						++strIt;
				}
			}
			else
			{
				if (*patIt != *strIt)
				{
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
				}
				else
				{
					++patIt;
					++strIt;
				}
			}

		}
		// If we reached the end of both the pattern and the string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
			return true;
		}
		else
		{
			return false;
		}

	}

}
