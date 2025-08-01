#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include "models/library.hpp"
#include "models/method.hpp"

double score(const LIB_NAMESPACE::Compound& compound,
             std::vector<LIB_NAMESPACE::Spectrum::tMzValue> kernel)
{

  double score = 0;

  for (const auto& [id, spectrum] : compound.Spectra) {

    for (const auto& mz : kernel) {

      for (size_t i = 0;
           i < spectrum.AbundanceValues.size() && i < spectrum.MzValues.size();
           ++i)
      {
        if (abs(spectrum.MzValues[i] - mz) < 0.1) {
          score += (spectrum.AbundanceValues[i] / 10000);
        }

      }
    }

  }

  return score / compound.Spectra.size();
}

int main(int argc, char* argv[])
{
  std::string inputFile = "assets/wellcome4.mslibrary.xml";

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "input,i",
      boost::program_options::value<std::string>(&inputFile),
      "input file (default: stdin)");

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

  LIB_NAMESPACE::Library library(ptree);

  std::vector<std::vector<LIB_NAMESPACE::Spectrum::tMzValue>> kernels = {
    {29, 43, 57, 71, 85, 99, 113, 127, 141}, // Alkane
    {31, 45, 59, 73, 87, 101, 115, 129, 143},  // Alcohol
    {43, 60, 74, 88, 102, 116, 130, 144},  // Ester
    {30, 44, 58, 72, 86, 100, 114, 128, 142},  // Amine
    {29, 44, 58, 72, 86, 100, 114, 128},  // Aldehyde
    {43, 58, 72, 86, 100, 114, 128, 142},  // Ketone
    {49, 63, 77, 91, 105, 119, 133, 147},  // Chloroalkane
    {152, 154, 156},  // Chlorobiphenyl
    {50, 80, 94, 108, 122, 136},  // Halogenated
    {47, 61, 75, 89, 103, 117, 131, 145},  // Sulphur
    {68, 82, 96, 110, 124, 138},  // Furan Ether
    {45, 60, 74, 88, 102, 116, 130, 144},  // Carboxylic acid
    {77, 91, 105, 119, 133, 147}  // Aromatic
  };

  std::cout << "ID, "
            << "Name, "
            << "Alkane, "
            << "Alcohol, "
            << "Ester, "
            << "Amine, "
            << "Aldehyde, "
            << "Ketone, "
            << "Chloroalkane, "
            << "Chlorobiphenyl, "
            << "Halogenated, "
            << "Sulphur, "
            << "Furan Ether, "
            << "Carboxylic acid, "
            << "Aromatic, "
            << std::endl;

  for (const auto& [id, compound] : library.Compounds) {
    
    std::string name = compound.CompoundName;

    std::cout << compound.CompoundID << ", ";
    std::cout << boost::algorithm::replace_all_copy(compound.CompoundName, ",", "-") << ", ";

    for (const auto& kernel : kernels) {
      std::cout << score(compound, kernel) << ", ";
    }

    std::cout << std::endl;

  }

  return 0;
}
