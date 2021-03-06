#ifndef xml_config_h
#define xml_config_h

#include <map>
#include <string>

#include "file.h"

namespace File {
class ConfigParser {
  std::map<std::string, std::map<std::string, std::string>> data;
  std::string nodata;

 public:
  ConfigParser(const std::string &filepath,
               const std::vector<std::string> &targets,
               const std::string &return_on_no_data = "NODATA")
      : nodata(return_on_no_data) {
    File<Operation::Read, Type::Xml> file(filepath);

    const std::filesystem::path relpath{
        std::filesystem::path(filepath).remove_filename()};

    for (auto &key : targets) {
      if (key == std::string{"default"})
        throw std::runtime_error("Cannot give default values in config");

      auto child = file.get_child(key.c_str());
      std::map<std::string, std::string> attr_map;

      for (auto x = child.attributes_begin(); x not_eq child.attributes_end();
           x++) {
        std::string key{x->name()};
        std::string val{x->value()};
        if (key == std::string{"default"}) {
          throw std::runtime_error("Cannot give default values in config");
        } else if (key.substr(0, 4) == "path" or
                   key.substr(0, 6) == "config") {  // Special code for paths
                                                    // and other config files
          if (val.size() and val[0] not_eq '/') {   // relative path
            auto thispath = relpath;
            thispath /= val;
            attr_map[key] = thispath.string();
          } else {
            attr_map[key] = val;
          }
        } else {
          attr_map[x->name()] = val;
        }
      }

      data[key] = attr_map;
      file.leave_child();
    }
    file.close();
  }

  const std::string &operator()(const std::string &childkey,
                                const std::string &attrkey) const {
    auto attr = data.find(childkey);
    if (attr == data.end()) {
      return nodata;
    } else {
      auto val = attr->second.find(attrkey);
      if (val == attr->second.end()) {
        return nodata;
      } else {
        return val->second;
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const ConfigParser &cp) {
    for (auto &cfg : cp.data) {
      os << cfg.first << ":\n";
      for (auto &attr : cfg.second)
        os << '\t' << attr.first << ":\t" << attr.second << '\n';
    }
    return os << "default (all data):\n\t" << cp.nodata;
  }
};  // ConfigParser
}  // namespace File

#endif  // xml_config_h
