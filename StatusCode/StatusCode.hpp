#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include<iostream>
#include<cstdlib>
#include<string>
#include<map>

class StatusCode
{
	public:
		StatusCode();
		~StatusCode();
		std::string const & getPhrase(int code) const;
		void printAll() const;
	private:
		std::map<int,std::string> _data;
};

#endif
