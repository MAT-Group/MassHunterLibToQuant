#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include "models/library.hpp"
#include "models/method.hpp"

int main(int argc, char* argv[])
{
  std::string inputFile = "assets/wellcome4.mslibrary.xml";
  std::string outputFile = "test.xml";

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "input,i",
      boost::program_options::value<std::string>(&inputFile),
      "input file (default: stdin)")(
      "output,o",
      boost::program_options::value<std::string>(&outputFile),
      "output file (default: stdout)");

  boost::program_options::variables_map vm;

  try {
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);
  } catch (const boost::program_options::error& e) {
    std::cerr << "Error parsing command line options: " << e.what() << "\n";
    std::cerr << desc << std::endl;
    return 1;
  }

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  // std::istream* in = &std::cin;
  //std::istream* in = &std::cin;
  //std::ifstream fileInput;
  //if (!inputFile.empty()) {
  //  fileInput.open(inputFile);
  //  if (!fileInput) {
  //    std::cerr << "Failed to open input file: " << inputFile << "\n";
  //    return 1;
  //  }
  //  in = &fileInput;
  //}

  boost::property_tree::ptree ptree;

  try {
    boost::property_tree::read_xml(inputFile, ptree);
  } catch (const boost::property_tree::xml_parser_error& e) {
    throw std::runtime_error("Failed to read XML file: "
                             + std::string(e.what()));
  }



  std::ostream* out = &std::cout;
  std::ofstream fileOutput;

  if (outputFile.empty() && !inputFile.empty()) {
    outputFile = inputFile;
  }

  if (!outputFile.empty()) {
    try {
      fileOutput.open(outputFile + ".xml");
    } catch (const boost::filesystem::filesystem_error& e) {
      std::cerr << "Failed to create output directory: " << e.what() << "\n";
      return 1;
    } catch (const std::exception& e) {
      std::cerr << "Error with output file: " << e.what() << "\n";
      return 1;
    }

    if (!fileOutput) {
      std::cerr << "Failed to open output file: " << outputFile + ".m" << "\n";
      return 1;
    }
    out = &fileOutput;
  }

  LIB_NAMESPACE::Library library(ptree);


  try {
    LIB_NAMESPACE::QuantitationDataSet method(library);
    boost::property_tree::write_xml(
        *out,
        (boost::property_tree::ptree)method,
        boost::property_tree::xml_writer_make_settings<std::string>(' ', 4));
  } catch (const std::exception& e) {
    std::cerr << "Error translating or writing method: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
