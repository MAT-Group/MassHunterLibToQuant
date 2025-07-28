#pragma once

#ifndef LIB_MODELS_COMPOUND_HPP
#define LIB_MODELS_COMPOUND_HPP

#include <string>
#include <map>

#include <boost/property_tree/ptree.hpp>

#include <defines.inc.hpp>
#include <types.hpp>

#include "models/spectrum.hpp"

namespace LIB_NAMESPACE
{

struct Compound
{
  tLibraryID LibraryID;
  tCompoundID CompoundID;
  
  std::string CASNumber;
  std::string CompoundName;
  std::string Formula;
  float BoilingPoint;
  float MeltingPoint;
  float MolecularWeight;
  float RetentionIndex;
  float RetentionTimeRTL;

  std::map<tSpectrumID, Spectrum> Spectra;

  Compound(tLibraryID, boost::property_tree::ptree);
  Compound() = default;
  Compound(const Compound&) = default;
  Compound& operator=(const Compound&) = default;
  Compound(Compound&&) = default;
  Compound& operator=(Compound&&) = default;
  ~Compound() = default;

};

} // namespace LIB_NAMESPACE

#endif // LIB_MODELS_COMPOUND_HPP
