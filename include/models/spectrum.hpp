#pragma once

#ifndef LIB_MODELS_SPECTRUM_HPP
#define LIB_MODELS_SPECTRUM_HPP

#include <vector>

#include <boost/property_tree/ptree.hpp>

#include <defines.inc.hpp>
#include <types.hpp>

namespace LIB_NAMESPACE
{

// TODO: Documentation
// TODO: Check cache miss for structure layout
struct Spectrum
{
  tLibraryID LibraryID;
  tCompoundID CompoundID;
  tSpectrumID SpectrumID;
  float BasePeakMZ;

  typedef double tMzValue;
  std::vector<tMzValue> MzValues;

  typedef double tAbundanceValue;
  std::vector<tAbundanceValue> AbundanceValues;

  Spectrum(boost::property_tree::ptree tree);
  Spectrum() = default;
  Spectrum(const Spectrum&) = default;
  Spectrum& operator=(const Spectrum&) = default;
  Spectrum(Spectrum&&) = default;
  Spectrum& operator=(Spectrum&&) = default;
  ~Spectrum() = default;

};

} // namespace LIB_NAMESPACE

#endif  // LIB_MODELS_SPECTRUM_HPP
