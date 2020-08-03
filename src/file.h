#ifndef file_h
#define file_h

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "../inc/pugixml/pugixml.hpp"

#include "enums.h"

namespace File {  
ENUMCLASS(Operation, unsigned char,
          Read,
          Write,
          Append,
          ReadBinary,
          WriteBinary,
          AppendBinary
)

ENUMCLASS(Type, unsigned char,
          Raw,
          Xml
)

template <class T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& x) {
  if constexpr (std::is_arithmetic<T>::value) {
    for (auto& b: x)
      os << b << ' ';
  } else if constexpr (not std::is_arithmetic<T>::value) {
    for (auto& b: x)
      os << b << '\n';
  }
  return os;
}

template <class T> std::istream& operator>>(std::istream& is, std::vector<T>& x) {
  for (auto& b: x)
    is >> b;
  return is;
}

template <Operation X, Type Y, int PREC=15>
class File {
  std::filesystem::path path;
  std::fstream fil;
  pugi::xml_document doc;
  pugi::xml_parse_result result;
  pugi::xml_node root;
  pugi::xml_node child;
  
public:
  File(const std::string& p) : path(p) {
    
    if constexpr (Y == Type::Xml)
      if (path.extension() not_eq ".xml")
        path = path.replace_extension(".xml");
    
    if constexpr (X == Operation::Read or X == Operation::ReadBinary) {
      if (not std::filesystem::exists(path)) {
        std::ostringstream os;
        os << '"' << path << '"' << " does not exist.  Cannot read it.";
        throw std::runtime_error(os.str());
      }
    }
    
    // FIXME:
    static_assert(Y == Type::Raw or Y == Type::Xml);
    
    if constexpr (Y == Type::Raw) {
      if constexpr (X == Operation::Read) {
        fil.open(path, std::ios::in);
      } else if constexpr (X == Operation::ReadBinary) {
        fil.open(path, std::ios::in | std::ios::binary);
      } else if constexpr (X == Operation::Write) {
        fil.open(path, std::ios::out);
        fil << std::setprecision(PREC);
      } else if constexpr (X == Operation::WriteBinary) {
        fil.open(path, std::ios::out | std::ios::binary);
      } else if constexpr (X == Operation::Append) {
        fil.open(path, std::ios::out | std::ios::app);
        fil << std::setprecision(PREC);
      } else if constexpr (X == Operation::AppendBinary) {
        fil.open(path, std::ios::out | std::ios::binary | std::ios::app);
      }
    } else if constexpr (Y == Type::Xml) {
      if constexpr (X == Operation::Read) {
        result = doc.load_file(path.c_str());
        child = root = doc.document_element();
      } else if constexpr (X == Operation::ReadBinary) {
        result = doc.load_file(path.c_str());
        child = root = doc.document_element();
        fil.open((path.string() + std::string(".bin")).c_str(), std::ios::in | std::ios::binary);
      } else if constexpr (X == Operation::Write) {
        auto declarationNode = doc.append_child(pugi::node_declaration);
        declarationNode.append_attribute("version")= "1.0";
        child = root = doc.append_child("RADCTRL");
      } else if constexpr (X == Operation::WriteBinary) {
        auto declarationNode = doc.append_child(pugi::node_declaration);
        declarationNode.append_attribute("version") = "1.0";
        child = root = doc.append_child("RADCTRL");
        fil.open((path.string() + std::string(".bin")).c_str(), std::ios::out | std::ios::binary);
      } else if constexpr (X == Operation::Append) {
        result = doc.load_file(path.c_str());
        child = root = doc.document_element();
      } else if constexpr (X == Operation::AppendBinary) {
        result = doc.load_file(path.c_str());
        fil.open((path.string() + std::string(".bin")).c_str(), std::ios::out | std::ios::binary | std::ios::app);
        child = root = doc.document_element();
      }
    }
  }
  
  std::string getline(const char limit='\n') noexcept {
    static_assert(X == Operation::Read and
    Y == Type::Raw, "Bad file type and operation");
    std::string line;
    std::getline(fil, line, limit);
    return line;
  }
  
  bool at_end() const noexcept {
    static_assert(Y == Type::Raw);
    return fil.eof();
  }
  
  template <typename T>
  File& operator<<(const T & x) {
    static_assert(X == Operation::Append or X == Operation::Write, "Bad operation");
    if constexpr (Y == Type::Raw)
      fil << x;
    else if constexpr (Y == Type::Xml) {
      std::ostringstream os;
      os << std::setprecision(PREC);
      os << x;
      child.append_child(pugi::node_pcdata).set_value(os.str().c_str());
    }
    return *this;
  }
  
  template <typename T>
  File& operator>>(T & x) {
    static_assert(X == Operation::Read and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    if constexpr (Y == Type::Raw)
      fil >> x;
    else if constexpr (Y == Type::Xml) {
      std::istringstream is(child.text().as_string());
      is >> x;
    }
    return *this;
  }
  
  template <typename T>
  void write(const T & x, size_t n=sizeof(T)) {
    static_assert((X == Operation::WriteBinary or X == Operation::AppendBinary) and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    fil.write(reinterpret_cast<const char *>(&x), n);
  }
  
  template <typename T>
  void write(T * x, size_t n) {
    static_assert((X == Operation::WriteBinary or X == Operation::AppendBinary) and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    fil.write(reinterpret_cast<const char *>(x), n);
  }
  
  template <typename T>
  void write(const std::vector<T> & x) {
    static_assert((X == Operation::WriteBinary or X == Operation::AppendBinary) and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    for (auto& v: x) write(v);
  }
  
  template <typename T>
  void read(T & x, size_t n=sizeof(T)) {
    static_assert(X == Operation::ReadBinary and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    fil.read(reinterpret_cast<char *>(&x), n);
  }
  
  template <typename T>
  void read(T * x, size_t n) {
    static_assert(X == Operation::ReadBinary and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    fil.read(reinterpret_cast<char *>(x), n);
  }
  
  template <typename T>
  void read(std::vector<T> & x) {
    static_assert(X == Operation::ReadBinary and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    for (auto& v: x) read(v);
  }
  
  template <bool relative=true>
  void seek(long n) {
    static_assert(X == Operation::ReadBinary and
    (Y == Type::Raw or Y == Type::Xml), "Bad file type and operation");
    if constexpr (relative)
      fil.seekg(n, std::ios_base::cur);
    else if constexpr (not relative)
      fil.seekg(n);
  }
  
  void close() {
    static_assert(Y == Type::Raw or Y == Type::Xml);
    if constexpr (Y == Type::Raw)
      fil.close();
    else if constexpr (Y == Type::Xml) {
      if constexpr (X == Operation::Write or
                    X == Operation::WriteBinary or
                    X == Operation::Append or
                    X == Operation::AppendBinary) {
        if (not doc.save_file(path.c_str(), "")) {
          std::ostringstream os;
          os << "Cannot save file to:" << '\n' << '\t'
             << path << '\n' << "Exiting";
          throw std::runtime_error(os.str());
        }
        if constexpr (X == Operation::AppendBinary or X == Operation::WriteBinary)
          fil.close();
      }
    }
  }
  
  void new_child(const std::string& name) {
    static_assert(Y == Type::Xml and (X == Operation::Write or
                                      X == Operation::WriteBinary or
                                      X == Operation::Append or
                                      X == Operation::AppendBinary));
    child = root.append_child(name.c_str());
  }
  
  pugi::xml_node get_child(const std::string& name) {
    static_assert(Y == Type::Xml);
    child = root.child(name.c_str());
    if (not child) {
      std::ostringstream os;
      os << "Problem getting child of XML document: " << path << '\n';
      os << "Child does not exist: " << name << '\n';
      throw std::runtime_error(os.str());
    }
    return child;
  }
  
  void leave_child() {
    static_assert(Y == Type::Xml);
    child = root;
  }
  
  template <class T>
  void add_attribute(const std::string& name, const T& value) {
    static_assert(Y == Type::Xml and (X == Operation::Write or
                                      X == Operation::WriteBinary or
                                      X == Operation::Append or
                                      X == Operation::AppendBinary));
    std::ostringstream os;
    os << std::boolalpha << value;
    child.append_attribute(name.c_str()) = os.str().c_str();
  }
  
  pugi::xml_attribute get_attribute(const std::string& name) {
    static_assert(Y == Type::Xml);
    return child.attribute(name.c_str());
  }
  
  void inc_size(const int n=1) {
    static_assert(Y == Type::Xml);
    child.attribute("size") = child.attribute("size").as_int() + n;
  }
  
  int size() {
    static_assert(Y == Type::Xml);
    int x = child.attribute("size").as_int();
    return x ? x : 1;
  }
};  // File

std::vector<std::string> Devices(std::vector<std::string> contains, size_t maxN=9);
}

# endif  // file_h