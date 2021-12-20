#ifndef __LOGGING_H__
#define __LOGGING_H__

#define RUN_TIME_ERROR( message ) std::cerr << "At file '" __FILE__ << "', line " << __LINE__ << " runtime error occured: '" << message << "'." << std::endl;

#endif // !__LOGGING_H__
