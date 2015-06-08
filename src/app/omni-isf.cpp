#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <fstream>

namespace odm
{
  namespace detail
  {
    /// found on http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html
    inline static std::string get_file_contents(const std::string& _filename)
    {
      std::ifstream in(_filename, std::ios::in | std::ios::binary);
      if (in)
      {
        return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
      }
      throw(errno);
    }

    inline static std::string remove_comments(const std::string& _code)
    {
      std::istringstream _is(_code);
      std::ostringstream _os;

      char c;
      while (_is.good())
      {
        _is.get(c);
        if (c=='\'' || c=='"')            /* literal */
        {
          int q=c;
          do
          {
            _os << c;
            if (c=='\\')
            {
              char b;
              _is.get(b);
              _os << b;
            }
            _is.get(c);
          }
          while (c!=q);
          _os << c;
        }
        else if (c=='/')              /* opening comment ? */
        {
          _is.get(c);
          if (c!='*')                  /* no, recover */
          {
            _os << '/';
            _is.unget();
          }
          else
          {
            int p;
            _os << ' ';
            do
            {
              p=c;
              _is.get(c);
            }
            while (c!='/' || p!='*');
          }
        }else
        {
          _os << c;
        }
      }
      return _os.str();
    }

    inline static std::map<std::string,std::string> typeMap()
    {
      static std::map<std::string,std::string> _typeMap;
      if (_typeMap.empty())
      {
        _typeMap = std::map<std::string,std::string>({
            {"vec2" ,"point2D"},
            {"float", "float"},
            {"int", "long"},
            {"bool", "Boolean"},
            {"vec4" , "color"},
            {"sampler2D", "image"},
            {"sampler2DRect", "image"}});
      }
      return _typeMap;
    }

    inline static bool supportedType(std::string const& _str)
    {
      return typeMap().count(_str) > 0;
    }

    struct Uniform
    {
      Uniform() {}
      Uniform(
          const std::string& _type,
          const std::string& _name) : 
        type_(_type),
        name_(_name) {}

      std::string type_;
      std::string name_;
    };

    inline static std::string remove_line_comments(const std::string& _code)
    {
      std::string _out = _code;
      while(_out.find("//") != std::string::npos) 
      {
        size_t Beg = _out.find("//");
        _out.erase(Beg, _out.find("\n", Beg) - Beg);
      }
      return _out;
    }

    inline static std::string remove_preprocessor_lines(const std::string& _code)
    {
      using namespace boost;
      typedef std::vector<std::string> split_vec_type;
      split_vec_type _splitVec;
      split(_splitVec,_code,is_any_of("\n"), token_compress_on);
      for (auto& _s : _splitVec)
        trim(_s);
      auto it = std::remove_if(_splitVec.begin(),_splitVec.end(),[](const std::string& _s)
      {
        if (_s.empty()) return true;
        return _s[0] == '#';
      });
      _splitVec.erase(it,_splitVec.end());
      std::ostringstream _os;
      for (auto& _s : _splitVec)
      {
        _os << _s << '\n';
      }
      return _os.str();
    }

    template<typename LINES, typename UNIFORMS>
    inline static bool get_uniforms(const std::string& _code, LINES& _lines, UNIFORMS& _uniforms)
    {
      using namespace boost;
      typedef std::vector<std::string> split_vec_type;
      split_vec_type _splitVec; 
      split(_splitVec,_code,is_any_of(";}"), token_compress_off);
      for (auto& _split : _splitVec)
      {
        split_vec_type _tokens;
        split(_tokens,_split,is_any_of("\t "), token_compress_on);
        for (auto& _token : _tokens)
        {
          trim(_token);
        }
        auto it = std::remove_if(_tokens.begin(),_tokens.end(),[](const std::string& _str)
        {
          return _str.empty();
        });
        _tokens.erase(it,_tokens.end());
        if (_tokens.empty()) continue;
        
        if (_tokens.front() != "uniform") 
        {
          _lines.push_back(_split);
          continue;
        }
        size_t _s = _tokens.size();
        if (_s < 3 || _s> 4) 
        {
          std::cerr << "Malformed uniform: " << _split << std::endl;  
          return false;
        }
        for (auto& _token : _tokens)
          trim_right_if(_token,is_any_of(";"));
        size_t _typeIdx = _s == 3 ? 1 : 2;
        size_t _nameIdx = _s == 3 ? 2 : 3;
        if (!supportedType(_tokens[_typeIdx])) 
        {
          std::cerr << "Unsupported Type Id:" << _typeIdx << std::endl;
          return false;
        }
        _uniforms.emplace_back(_tokens[_typeIdx],_tokens[_nameIdx]);
      }
      return true;
    }
  }

  inline static std::string generateISF(const std::string& _shader)
  {
    using namespace detail;
    auto&& _stripped = remove_preprocessor_lines(remove_line_comments(remove_comments(_shader))); 
    typedef std::vector<std::string> split_vec_type;
    split_vec_type _lines;
    std::vector<Uniform> _uniforms;
    if (!get_uniforms(_stripped,_lines,_uniforms)) return std::string();
    
    auto&& _typeMap = typeMap();
    using boost::property_tree::ptree;
    ptree _tree;
    _tree.put("DESCRIPTION","Generated by Omnidome.");
    _tree.put("CREDIT","Generated by Omnidome.");
    ptree _categories;
    ptree _category;
    _category.put("","Omnidome");
    _categories.push_back(std::make_pair("",_category));
    _tree.push_back(std::make_pair("CATEGORIES",_categories));

    ptree _inputs;
    for (auto& _uniform : _uniforms)
    {
      ptree _input;
      _input.put("NAME",_uniform.name_);
      _input.put("TYPE",_typeMap[_uniform.type_]);
      _inputs.push_back(std::make_pair("",_input));
    }
    _tree.push_back(std::make_pair("INPUTS",_inputs));

    std::ostringstream _out;
    _out << "/*";
    boost::property_tree::json_parser::write_json(_out,_tree);
    _out << "*/" << std::endl;

    std::istringstream _is(_shader);
    std::string _line, _token;
    while (std::getline(_is, _line))
    {
      std::istringstream _lineIn(_line);
      _lineIn >> _token;
      if (_token != "uniform")
      {
        _out << _line << std::endl;
      }
    }
    return _out.str();
  }
}


namespace po = boost::program_options;
using namespace std;


int main(int ac, char* av[])
{
  cout << "Omnidome ISF generator -- written by Wilston Oreo (C) 2014." << endl;

  stringstream descStr;
  descStr << "Allowed options";

  // Declare the supported options
  po::options_description desc(descStr.str());

  std::string _input, _output;

  desc.add_options()
  ("help,h", "Display help message.")
  ("input,i", po::value<std::string>(&_input), "Input shader file")
  ("output,o", po::value<std::string>(&_output), "Output ISF shader")
  ;



  // Parse the command line arguments for all supported options
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);
  
  if (vm.count("help"))
  {
    cout << desc << endl;
    return EXIT_FAILURE;
  }



  auto&& _isf = odm::generateISF(odm::detail::get_file_contents(_input));
  if (_isf.empty()) return EXIT_FAILURE;

  std::ofstream _of(_output);
  _of << _isf;

  return EXIT_SUCCESS;
}

