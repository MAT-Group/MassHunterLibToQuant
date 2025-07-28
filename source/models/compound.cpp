#include "models/compound.hpp"

namespace LIB_NAMESPACE
{

Compound::Compound(tLibraryID libraryID, boost::property_tree::ptree tree)
  : LibraryID(libraryID)
{
  CompoundID = tree.get<tCompoundID>("CompoundID", 0);
  CASNumber = tree.get<std::string>("CASNumber", "");
  CompoundName = tree.get<std::string>("CompoundName", "");
  Formula = tree.get<std::string>("Formula", "");
  BoilingPoint = tree.get<float>("BoilingPoint", 0.0f);
  MeltingPoint = tree.get<float>("MeltingPoint", 0.0f);
  MolecularWeight = tree.get<float>("MolecularWeight", 0.0f);
  RetentionIndex = tree.get<float>("RetentionIndex", 0.0f);
  RetentionTimeRTL = tree.get<float>("RetentionTimeRTL", 0.0f);

}

}  // namespace LIB_NAMESPACE
