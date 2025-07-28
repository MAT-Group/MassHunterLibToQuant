#pragma once

#ifndef LIB_MODELS_LIBRARY_HPP
#define LIB_MODELS_LIBRARY_HPP

#include <map>

#include <boost/property_tree/ptree.hpp>

#include <defines.inc.hpp>
#include <types.hpp>

#include "models/compound.hpp"

namespace LIB_NAMESPACE
{

struct Library
{
  tLibraryID LibraryID = NULL;
  bool AccurateMass = false;
  std::map<tCompoundID, Compound> Compounds = {};

  Library() = default;
  Library(const Library&) = default;
  Library& operator=(const Library&) = default;
  Library(Library&&) = default;
  Library& operator=(Library&&) = default;
  ~Library() = default;

  Library(boost::property_tree::ptree);
};

} // namespace LIB_NAMESPACE

#endif // LIB_MODELS_LIBRARY_HPP
