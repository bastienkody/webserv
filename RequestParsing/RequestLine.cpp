#include"RequestLine.hpp"

//	Default
RequestLine::RequestLine() {}
RequestLine::~RequestLine() {}
RequestLine::RequestLine(const RequestLine & src) {*this = src;}
RequestLine & RequestLine::operator=(const RequestLine & rhs)
{
	if(this != &rhs)
	{
		this->_fullRequestLine = rhs.getFull();
		this->_verb = rhs.getVerb();
		this->_version = rhs.getVersion();
		this->_isVersionGood = rhs.getIsVersionGood();
		this->_isVerbGood = rhs.getIsVerbGood();
		this->_Url = rhs.getUrl();
	}
	return (*this);
}
// getters
std::string		const & RequestLine::getFull() const {return _fullRequestLine;}
std::string		const & RequestLine::getVerb() const {return _verb;}
std::string		const & RequestLine::getVersion() const {return _version;}
bool			const & RequestLine::getIsVersionGood() const {return _isVersionGood;}
bool			const & RequestLine::getIsVerbGood() const {return _isVerbGood;}
URL				const & RequestLine::getUrl() const {return _Url;}


/*
	Param constructor
	VERB WS URL WS VERSION CRLF
*/
RequestLine::RequestLine(std::string rql)
{
	_fullRequestLine = rql;

	_verb = rql.substr(0, rql.find(" "));
	rql.erase(0, rql.find(" ") + 1);
	_isVerbGood = false;
	if (_verb.compare(GET) == 0 || _verb.compare(POST) == 0 || _verb.compare(DELETE) == 0)
		_isVerbGood = true;

	_Url = URL(rql.substr(0, rql.find(" ")));
	rql.erase(0, rql.find(" ") + 1);

	_version = rql.substr(0, rql.size());
	_isVersionGood = _version.compare(EXPECTED_VERSION) == 0 ? true:false;
}