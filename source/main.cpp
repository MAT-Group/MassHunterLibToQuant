#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "lib.hpp"

int main(int argc, char* argv[])
{
  std::string inputFile;
  std::string outputFile;

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
  std::istream* in = &std::cin;
  std::ifstream fileInput;
  if (!inputFile.empty()) {
    fileInput.open(inputFile);
    if (!fileInput) {
      std::cerr << "Failed to open input file: " << inputFile << "\n";
      return 1;
    }
    in = &fileInput;
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

  MHLTQ_NAMESPACE::Library library;
  try {
    if (!MHLTQ_NAMESPACE::loadLibrary(*in, library)) {
      std::cerr << "Failed to load library.\n";
      return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error loading library: " << e.what() << "\n";
    return 1;
  }

  try {
    MHLTQ_NAMESPACE::QuantitationDataSet method(library);
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
