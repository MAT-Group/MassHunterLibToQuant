#include "models/library.hpp"
#include "models/compound.hpp"
#include "models/spectrum.hpp"

namespace LIB_NAMESPACE
{

  Library::Library(boost::property_tree::ptree tree) {

    LibraryID = tree.get<tLibraryID>("LibraryDataSet.Library.LibraryID", NULL);
    AccurateMass = tree.get<bool>("LibraryDataSet.Library.AccurateMass", false);

    for (const auto& [field, subtree] : tree.get_child("LibraryDataSet")) {

      if (field == "Compound") {
        Compound compound(LibraryID, subtree);
        Compounds[compound.CompoundID] = compound;

      } else if (field == "Spectrum") {
        auto spectrum = Spectrum(subtree);

        if (Compounds.find(spectrum.CompoundID) != Compounds.end()) {
          Compounds[spectrum.CompoundID].Spectra[spectrum.SpectrumID] =
              spectrum;
        } else {
          throw std::runtime_error("Compound ID not found for Spectrum: "
                                   + std::to_string(spectrum.CompoundID));
        }

      }
    }

  }

}
