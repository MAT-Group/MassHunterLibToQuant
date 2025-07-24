#include "lib.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include "base64.hpp"

#include <algorithm>
#include <utility>
#include <iostream>
#include <cstdint>

template<std::integral T>
constexpr T byteswap(T value) noexcept
{
    static_assert(std::has_unique_object_representations_v<T>, 
                  "T may not have padding bits");
    auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
    std::ranges::reverse(value_representation);
    return std::bit_cast<T>(value_representation);
}

template<typename T>
std::vector<T> decodeBase64Binary(std::string input){
  std::string decoded = base64::from_base64(input);
  std::vector<T> decoded_floats;
  decoded_floats.reserve(decoded.size() / sizeof(T));

  for (size_t i = 0; i + sizeof(T) <= decoded.size(); i += sizeof(T)) {
    T value;
    std::memcpy(&value, &decoded[i], sizeof(T));
    decoded_floats.push_back(value);
  }
  return decoded_floats;
}

bool MHLTQ_NAMESPACE::loadLibrary(std::istream& in, Library& output)
{
  boost::property_tree::ptree ptree;

  try {
    boost::property_tree::read_xml(in, ptree);
  } catch (const boost::property_tree::xml_parser_error& e) {
    throw std::runtime_error("Failed to read XML file: "
                             + std::string(e.what()));
  }

  output.LibraryID =
      ptree.get<unsigned int>("LibraryDataSet.Library.LibraryID");
  output.AccurateMass = ptree.get<bool>("LibraryDataSet.Library.AccurateMass");

  for (const auto& child : ptree.get_child("LibraryDataSet")) {
    if (child.first == "Compound") {
      Compound comp;
      comp.LibraryID = output.LibraryID;
      comp.CompoundID = child.second.get<unsigned int>("CompoundID");
      comp.CASNumber = child.second.get<std::string>("CASNumber", "");
      comp.CompoundName = child.second.get<std::string>("CompoundName", "");
      comp.Formula = child.second.get<std::string>("Formula", "");
      comp.BoilingPoint = child.second.get<float>("BoilingPoint", 0.0f);
      comp.MeltingPoint = child.second.get<float>("MeltingPoint", 0.0f);
      comp.MolecularWeight = child.second.get<float>("MolecularWeight", 0.0f);
      comp.RetentionIndex = child.second.get<float>("RetentionIndex", 0.0f);
      comp.RetentionTimeRTL = child.second.get<float>("RetentionTimeRTL", 0.0f);

      output.Compounds[comp.CompoundID] = comp;
    }

    if (child.first == "Spectrum") {
      Spectrum spec;
      spec.LibraryID = child.second.get<unsigned int>("LibraryID");
      spec.CompoundID = child.second.get<unsigned int>("CompoundID");
      spec.SpectrumID = child.second.get<unsigned int>("SpectrumID");
      spec.BasePeakMZ = child.second.get<float>("BasePeakMZ", 0.0f);

      try {
        spec.MzValues = decodeBase64Binary<double>(
            child.second.get<std::string>("MzValues"));
        spec.AbundanceValues = decodeBase64Binary<double>(
            child.second.get<std::string>("AbundanceValues"));

      } catch (const std::exception& e) {
        //throw std::runtime_error(
        //    "Failed to decode MzValues or AbundanceValues: "
        //    + std::string(e.what()));
        std::cerr << "Failed to decode for " + std::to_string(spec.CompoundID) + ". " + e.what()
                  << std::endl;
        continue;
      }


      unsigned int compoundID = spec.CompoundID;
      if (output.Compounds.find(compoundID) != output.Compounds.end()) {
        output.Compounds[compoundID].Spectra[spec.SpectrumID] = spec;
      } else {
        throw std::runtime_error("Compound ID not found for Spectrum: "
                                 + std::to_string(compoundID));
        return false;
      }
    }
  }

  (void)0;
  return true;
}

void MHLTQ_NAMESPACE::EncodeParameterSet(
    const IntegrationParameters::ParameterSet_T& input, std::string& output)
{
  boost::property_tree::ptree ptree = input;

  std::ostringstream oss;
  boost::property_tree::write_xml(
      oss,
      ptree,
      boost::property_tree::xml_writer_make_settings<std::string>(' ', 4));

  output = oss.str();
}

void MHLTQ_NAMESPACE::EncodeTarget(const TargetCompound& input,
                                   std::string& output)
{
  boost::property_tree::ptree ptree = input;

  std::ostringstream oss;
  boost::property_tree::write_xml(
      oss,
      ptree,
      boost::property_tree::xml_writer_make_settings<std::string>(' ', 4));

  output = oss.str();
}

std::string MHLTQ_NAMESPACE::makeQuantMethodFromLibrary(const Library& library)
{
  QuantitationDataSet method(library);
  boost::property_tree::ptree ptree = QuantitationDataSet(library);

  std::ostringstream oss;
  boost::property_tree::write_xml(
      oss,
      ptree,
      boost::property_tree::xml_writer_make_settings<std::string>(' ', 4));

  return oss.str();
}

MHLTQ_NAMESPACE::Parameter::operator boost::property_tree::ptree() const
{
  boost::property_tree::ptree ptree;
  ptree.put("<xmlattr>.id", attrs.id);
  ptree.put("DisplayName", DisplayName);
  ptree.put("Value", Value);

  if (Default) {
    ptree.put("Default", *Default);
  }

  if (Limits) {
    boost::property_tree::ptree limits_ptree;
    limits_ptree.put("Minimum.<xmlattr>.type", Limits->Minimum.attr.type);
    limits_ptree.put("Minimum", Limits->Minimum.value);
    limits_ptree.put("Maximum.<xmlattr>.type", Limits->Maximum.attr.type);
    limits_ptree.put("Maximum", Limits->Maximum.value);
    ptree.add_child("Limits", limits_ptree);
  }

  if (XUnits) {
    ptree.put("Units", *XUnits);
  }

  if (DataValueType) {
    ptree.put("DataValueType", *DataValueType);
  }

  if (PrecisionType) {
    ptree.put("PrecisionType", *PrecisionType);
  }

  if (PrecisionDigits) {
    ptree.put("PrecisionDigits", *PrecisionDigits);
  }

  if (ConversionSupport) {
    ptree.put("ConversionSupport", *ConversionSupport);
  }

  return ptree;
}

MHLTQ_NAMESPACE::IntegrationParameters::ParameterSet_T::operator boost::
    property_tree::ptree() const
{
  boost::property_tree::ptree ptree;
  ptree.put("ParameterSet.<xmlattr>.usagekey", attrs.usageKey);
  ptree.put("ParameterSet.<xmlattr>.assembly", attrs.assembly);
  ptree.put("ParameterSet.<xmlattr>.class", attrs.class_);

  ptree.add_child("ParameterSet.Parameter", DataPointSampling);
  ptree.add_child("ParameterSet.Parameter", Smoothing);
  ptree.add_child("ParameterSet.Parameter", DetectionFiltering);
  ptree.add_child("ParameterSet.Parameter", StartThreshold);
  ptree.add_child("ParameterSet.Parameter", StopThreshold);
  ptree.add_child("ParameterSet.Parameter", PeakLocation);
  ptree.add_child("ParameterSet.Parameter", BaselineReset);
  ptree.add_child("ParameterSet.Parameter", BaselineValley);
  ptree.add_child("ParameterSet.Parameter", BaselinePreference);
  return ptree;
}

MHLTQ_NAMESPACE::TargetCompound::operator boost::property_tree::ptree() const
{
  boost::property_tree::ptree ptree;

  ptree.put("BatchID", BatchID);
  ptree.put("SampleID", SampleID);
  ptree.put("CompoundID", CompoundID);
  ptree.put("AccuracyLimitMultiplierLOQ", AccuracyLimitMultiplierLOQ);
  ptree.put("AccuracyMaximumPercentDeviation", AccuracyMaximumPercentDeviation);
  ptree.put("CompoundName", CompoundName);
  ptree.put("CompoundType", CompoundType);
  ptree.put("ConcentrationUnits", ConcentrationUnits);
  ptree.put("CurveFit", CurveFit);
  ptree.put("CurveFitOrigin", CurveFitOrigin);
  ptree.put("CurveFitWeight", CurveFitWeight);

  std::string EncodedIntegrationParameters;
  EncodeParameterSet(IntegrationParameters.ParameterSet,
                     EncodedIntegrationParameters);
  ptree.add("IntegrationParameters", EncodedIntegrationParameters);

  ptree.put("IntegrationParametersModified", IntegrationParametersModified);
  ptree.put("Integrator", Integrator);
  ptree.put("IonPolarity", IonPolarity);
  ptree.put("ISTDFlag", ISTDFlag);
  ptree.put("LeftRetentionTimeDelta", LeftRetentionTimeDelta);
  ptree.put("MaximumNumberOfHits", MaximumNumberOfHits);
  ptree.put("Multiplier", Multiplier);
  ptree.put("MZ", MZ);
  ptree.put("MZExtractionWindowFilterLeft", MZExtractionWindowFilterLeft);
  ptree.put("MZExtractionWindowFilterRight", MZExtractionWindowFilterRight);
  ptree.put("MZExtractionWindowUnits", MZExtractionWindowUnits);
  ptree.put("NoiseAlgorithmType", NoiseAlgorithmType);
  ptree.put("NoiseOfRawSignal", NoiseOfRawSignal);
  ptree.put("NoiseReference", NoiseReference);
  ptree.put("NoiseStandardDeviationMultiplier",
            NoiseStandardDeviationMultiplier);
  ptree.put("PeakFilterThreshold", PeakFilterThreshold);
  ptree.put("PeakFilterThresholdValue", PeakFilterThresholdValue);
  ptree.put("PeakSelectionCriterion", PeakSelectionCriterion);
  ptree.put("PrimaryHitPeakID", PrimaryHitPeakID);
  ptree.put("QuantitateByHeight", QuantitateByHeight);
  ptree.put("RetentionTime", RetentionTime);
  ptree.put("RetentionTimeDeltaUnits", RetentionTimeDeltaUnits);
  ptree.put("RetentionTimeWindow", RetentionTimeWindow);
  ptree.put("RetentionTimeWindowUnits", RetentionTimeWindowUnits);
  ptree.put("RightRetentionTimeDelta", RightRetentionTimeDelta);
  ptree.put("ScanType", ScanType);
  ptree.put("SelectedMZ", SelectedMZ);
  ptree.put("SignalType", SignalType);
  ptree.put("Smoothing", Smoothing);
  ptree.put("SmoothingFunctionWidth", SmoothingFunctionWidth);
  ptree.put("SmoothingGaussianWidth", SmoothingGaussianWidth);
  ptree.put("SpectrumExtractionOverride", SpectrumExtractionOverride);
  ptree.put("SpectrumScanInclusion", SpectrumScanInclusion);
  ptree.put("ThresholdNumberOfPeaks", ThresholdNumberOfPeaks);
  ptree.put("TimeReferenceFlag", TimeReferenceFlag);
  ptree.put("TimeSegment", TimeSegment);
  ptree.put("Transition", Transition);
  ptree.put("UncertaintyRelativeOrAbsolute", UncertaintyRelativeOrAbsolute);

  return ptree;
}

MHLTQ_NAMESPACE::QuantitationDataSet::QuantitationDataSet(
    const Library& library)
    : QuantitationDataSet()
{

  for (const auto& compound : library.Compounds) {
    addTarget(compound.second);
  }
}

void MHLTQ_NAMESPACE::QuantitationDataSet::addTarget(const Compound& compound)
{
  if (compound.Spectra.empty()) {
    throw;
  }

  TargetCompound target = {
      .CompoundID = compound.CompoundID,
      .CompoundName = compound.CompoundName,
      .MZ = compound.Spectra.begin()
                ->second.BasePeakMZ,
      .RetentionTime = compound.RetentionTimeRTL,
      .Transition = compound.Spectra.begin()->second.BasePeakMZ,
  };

  Targets.push_back(target);
}

MHLTQ_NAMESPACE::QuantitationDataSet::operator boost::property_tree::ptree()
    const
{
  boost::property_tree::ptree ptree;

  ptree.put("QuantitationDataSet.<xmlattr>.SchemaVersion", attr.SchemaVersion);
  ptree.put("QuantitationDataSet.<xmlattr>.DataVersion", attr.DataVersion);
  ptree.put("QuantitationDataSet.<xmlattr>.BatchState", attr.BatchState);
  ptree.put("QuantitationDataSet.<xmlattr>.ReferenceWindow",
            attr.ReferenceWindow);
  ptree.put("QuantitationDataSet.<xmlattr>.ReferenceWindowPercentOrMinutes",
            attr.ReferenceWindowPercentOrMinutes);
  ptree.put("QuantitationDataSet.<xmlattr>.NonReferenceWindow",
            attr.NonReferenceWindow);
  ptree.put("QuantitationDataSet.<xmlattr>.NonReferenceWindowPercentOrMinutes",
            attr.NonReferenceWindowPercentOrMinutes);
  ptree.put("QuantitationDataSet.<xmlattr>.CorrelationWindow",
            attr.CorrelationWindow);
  ptree.put("QuantitationDataSet.<xmlattr>.ApplyMultiplierTarget",
            attr.ApplyMultiplierTarget);
  ptree.put("QuantitationDataSet.<xmlattr>.ApplyMultiplierSurrogate",
            attr.ApplyMultiplierSurrogate);
  ptree.put("QuantitationDataSet.<xmlattr>.ApplyMultiplierMatrixSpike",
            attr.ApplyMultiplierMatrixSpike);
  ptree.put("QuantitationDataSet.<xmlattr>.ApplyMultiplierISTD",
            attr.ApplyMultiplierISTD);
  ptree.put("QuantitationDataSet.<xmlattr>.IgnorePeaksNotFound",
            attr.IgnorePeaksNotFound);
  ptree.put("QuantitationDataSet.<xmlattr>.RelativeISTD", attr.RelativeISTD);
  ptree.put("QuantitationDataSet.<xmlattr>.AuditTrail", attr.AuditTrail);
  ptree.put("QuantitationDataSet.<xmlattr>.LibraryPathFileName",
            attr.LibraryPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.LibraryMethodPathFileName",
            attr.LibraryMethodPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.RefLibraryPathFileName",
            attr.RefLibraryPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.RefLibraryPatternPathFileName",
            attr.RefLibraryPatternPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.CCMaximumElapsedTimeInHours",
            attr.CCMaximumElapsedTimeInHours);
  ptree.put("QuantitationDataSet.<xmlattr>.BracketingType",
            attr.BracketingType);
  ptree.put("QuantitationDataSet.<xmlattr>.StandardAddition",
            attr.StandardAddition);
  ptree.put("QuantitationDataSet.<xmlattr>.DynamicBackgroundSubtraction",
            attr.DynamicBackgroundSubtraction);
  ptree.put("QuantitationDataSet.<xmlattr>.BatchName", attr.BatchName);
  ptree.put("QuantitationDataSet.<xmlattr>.BatchDataPathFileName",
            attr.BatchDataPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.DAMethodPathFileNameOrigin",
            attr.DAMethodPathFileNameOrigin);
  ptree.put("QuantitationDataSet.<xmlattr>.AnalystName", attr.AnalystName);
  ptree.put("QuantitationDataSet.<xmlattr>.ReportGeneratorName",
            attr.ReportGeneratorName);
  ptree.put("QuantitationDataSet.<xmlattr>.AnalysisTimeStamp",
            attr.AnalysisTimeStamp);
  ptree.put("QuantitationDataSet.<xmlattr>.DAMethodLastAppliedTimeStamp",
            attr.DAMethodLastAppliedTimeStamp);
  ptree.put("QuantitationDataSet.<xmlattr>.CalibrationLastUpdatedTimeStamp",
            attr.CalibrationLastUpdatedTimeStamp);
  ptree.put("QuantitationDataSet.<xmlattr>.ReportGenerationStartedTimeStamp",
            attr.ReportGenerationStartedTimeStamp);
  ptree.put("QuantitationDataSet.<xmlattr>.ReportResultsDataPathFileName",
            attr.ReportResultsDataPathFileName);
  ptree.put("QuantitationDataSet.<xmlattr>.AnalyzeQuantVersion",
            attr.AnalyzeQuantVersion);
  ptree.put("QuantitationDataSet.<xmlattr>.ReportQuantVersion",
            attr.ReportQuantVersion);
  ptree.put("QuantitationDataSet.<xmlattr>.ComplianceName",
            attr.ComplianceName);
  ptree.put("QuantitationDataSet.<xmlattr>.ComplianceVersion",
            attr.ComplianceVersion);
  ptree.put("QuantitationDataSet.<xmlattr>.ComplianceServer",
            attr.ComplianceServer);
  ptree.put("QuantitationDataSet.<xmlattr>.FeatureDetection",
            attr.FeatureDetection);
  ptree.put("QuantitationDataSet.<xmlattr>.HashCode", attr.HashCode);
  ptree.put("QuantitationDataSet.<xmlattr>.xmlns", attr.xmlns);

  for (const auto& target : Targets) {
    ptree.add_child("QuantitationDataSet.TargetCompound", target);
  }

  return ptree;
}

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

std::string MHLTQ_NAMESPACE::Library::tocsv()
{
  // Make header

  std::string output;

  for (const auto& compound : Compounds) {
    const Compound& comp = compound.second;
  

    // Add compound data to CSV
    std::string csvLine;
    csvLine += std::to_string(comp.CompoundID) + ",";
    csvLine += std::to_string(comp.RetentionIndex) + ",";


    for (const auto& spectrum : comp.Spectra) {
      const Spectrum& spec = spectrum.second;

      auto top_pairs = top_n_zip(spec.AbundanceValues, spec.MzValues, 5);

      for (const auto& p : top_pairs) {
        csvLine += std::to_string(p.second) + ",";
      }


      // csvLine += join(spec.MzValues, ",") + ",";
      //csvLine += std::to_string(spec.LibraryID) + ",";
      //csvLine += std::to_string(spec.CompoundID) + ",";
      //csvLine += std::to_string(spec.SpectrumID) + ",";
      break;
    }



    output += csvLine + "\n";

  }

  return output;
}
