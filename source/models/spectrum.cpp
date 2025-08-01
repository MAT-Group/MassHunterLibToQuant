
#include <iostream>
#include <string>

#include "base64.hpp"

#include "models/spectrum.hpp"

namespace LIB_NAMESPACE
{

  namespace detail
  {
    template<typename T>
    std::vector<T> decodeBase64Binary(std::string input)
    {
      std::string decoded = base64::decode(input);
      std::vector<T> decoded_floats;
      decoded_floats.reserve(decoded.size() / sizeof(T));

      for (size_t i = 0; i + sizeof(T) <= decoded.size(); i += sizeof(T)) {
        T value;
        std::memcpy(&value, &decoded[i], sizeof(T));
        decoded_floats.push_back(value);
      }
      return decoded_floats;
    }
  }

Spectrum::Spectrum(boost::property_tree::ptree tree)
{
  LibraryID = tree.get<tLibraryID>("LibraryID", 0);
  CompoundID = tree.get<tCompoundID>("CompoundID", 0);
  SpectrumID = tree.get<tSpectrumID>("SpectrumID", 0);
  BasePeakMZ = tree.get<float>("BasePeakMZ", 0.0f);

  try {
    MzValues =
        detail::decodeBase64Binary<double>(tree.get<std::string>("MzValues"));

    AbundanceValues = detail::decodeBase64Binary<double>(
        tree.get<std::string>("AbundanceValues"));
  
  } catch (const std::exception&) {
    //throw std::runtime_error("Failed to decode MzValues or AbundanceValues: "
    //                         + std::string(e.what()));

    std::cerr << "Failed to decode for " 
              << std::to_string(CompoundID)
              //<< ". " + e.what()
              << std::endl;
  }

}

}
