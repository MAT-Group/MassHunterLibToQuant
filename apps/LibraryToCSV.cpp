#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "models/library.hpp"
#include "models/spectrum.hpp"

template<typename T1, typename T2>
std::vector<std::pair<T1, T2>> top_n_zip(const std::vector<T1>& v1,
                                         const std::vector<T2>& v2,
                                         size_t n,
                                         bool descending = true)
{
  size_t len = std::min(v1.size(), v2.size());
  std::vector<std::pair<T1, T2>> zipped;

  for (size_t i = 0; i < len; ++i) {
    zipped.emplace_back(v1[i], v2[i]);
  }

  // Sort by first element of pair
  std::sort(zipped.begin(),
            zipped.end(),
            [descending](const auto& a, const auto& b)
            { return descending ? a.first > b.first : a.first < b.first; });

  // Take top n (or all if n is too big)
  if (n < zipped.size()) {
    zipped.resize(n);
  }

  return zipped;
}

std::string convert(const LIB_NAMESPACE::Library& lib) {
  std::string output;

    for (const auto& [id, compound] : lib.Compounds) {

    // Add compound data to CSV
    std::string csvLine;
    csvLine += std::to_string(compound.CompoundID) + ",";
    csvLine += std::to_string(compound.RetentionIndex) + ",";

    for (const auto& [id, spectrum] : compound.Spectra) {

      auto top_pairs =
          top_n_zip(spectrum.AbundanceValues, spectrum.MzValues, 5);

      for (const auto& p : top_pairs) {
        csvLine += std::to_string(p.second) + ",";
      }

      // csvLine += join(spec.MzValues, ",") + ",";
      // csvLine += std::to_string(spec.LibraryID) + ",";
      // csvLine += std::to_string(spec.CompoundID) + ",";
      // csvLine += std::to_string(spec.SpectrumID) + ",";
      break;
    }

    output += csvLine + "\n";
  }

  return output;

}


int main(int argc, char* argv[])
{
  std::string inputFile = "assets/wellcome4.mslibrary.xml";
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
    std::cout << "Converting Library to CSV..." << std::endl;
    std::cout << convert(library) << std::endl;

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error writing output: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
