#include"ConfigFile.hpp"

//	Default
ConfigFile::ConfigFile() {}
ConfigFile::~ConfigFile() {}
//	Param constructor
ConfigFile::ConfigFile(const std::string filename) : _filename(filename) {}
//	Copy constructor (+ copy operator)
ConfigFile::ConfigFile(const ConfigFile & src) {*this = src;}
ConfigFile & ConfigFile::operator=(const ConfigFile & rhs)
{
	if(this != &rhs)
	{
		this->_filename = rhs._filename;
		this->_rawData = rhs.getRawData();
	}
	return (*this);
}
//	os stream << redefinition
std::ostream & operator<<(std::ostream& os, const ConfigFile &rhs)
{return (os << rhs.getRawData());} 
//	Getter
std::string const & ConfigFile::getRawData() const {return (_rawData);}

void	ConfigFile::openReadFileData()
{
	_fs.open(_filename.c_str(), std::fstream::in);
	if (_fs.fail())
		throw std::invalid_argument("Confgfile: " + _filename + " failed to open.");

	// test size vs string.max (cf. cpp01/04)
	_fs.seekg(0, std::ios_base::end);
	unsigned long dataSize = static_cast<unsigned long>(_fs.tellg());
	std::cout << "Size:" << dataSize << std::endl;
	_fs.seekg(std::ios_base::beg);
	std::string string = "test";
	if (dataSize == 0 || dataSize >= string.max_size())
		throw std::invalid_argument("Data from " + _filename + " is too big or empty");

	//	read into _rawData
	char *buf = new char [dataSize + 1];
	buf[dataSize] = '\0';
	_fs.read(buf, dataSize);
	if (_fs)
		_rawData = buf;
	else
		throw std::invalid_argument("Problem with read on configfile");
	delete [] buf;
}




