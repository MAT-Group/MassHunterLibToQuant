
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "models/library.hpp"

int main() {

  std::string inputFile = "assets/wellcome4.mslibrary.xml";
  boost::property_tree::ptree ptree;

  try {
    boost::property_tree::read_xml(inputFile, ptree);
  } catch (const boost::property_tree::xml_parser_error& e) {
    throw std::runtime_error(
      "Failed to read XML file: " + std::string(e.what())
    );
  }

  LIB_NAMESPACE::Library library(ptree);
  (void)(0);
}
