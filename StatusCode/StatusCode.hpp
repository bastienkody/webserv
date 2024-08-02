#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include<iostream>
#include<cstdlib>
#include<string>
#include<map>

class StatusCode
{
	public:
		static std::string const & getPhrase(int code);
		static void printAll();
	private:
		static const std::map<int,std::string> _data;
};

#endif
