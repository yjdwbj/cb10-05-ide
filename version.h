#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2014";
	static const char UBUNTU_VERSION_STYLE[] =  "14.10";
	
	//Software Status
	static const char STATUS[] =  "Release Candidate";
	static const char STATUS_SHORT[] =  "rc";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 734;
	static const long REVISION  = 3975;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1429;
	#define RC_FILEVERSION 1,7,734,3975
	#define RC_FILEVERSION_STRING "1, 7, 734, 3975\0"
	static const char FULLVERSION_STRING [] = "1.7.734.3975";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 14;
	

}
#endif //VERSION_H
