#pragma once

#ifndef LIB_MODELS_METHOD_HPP
#define LIB_MODELS_METHOD_HPP

#include <string>
#include <optional>

#include <boost/property_tree/ptree.hpp>

#include <defines.inc.hpp>
#include <types.hpp>

#include "models/library.hpp"

namespace LIB_NAMESPACE
{

struct Parameter
{
  struct attrs_t
  {
    std::string id;
  } attrs;

  std::string DisplayName;
  std::string Value;

  std::optional<std::string> Default;

  struct Limits_T
  {
    struct Limit_T
    {
      struct attrs
      {
        std::string type = "Inclusive";
      } attr;

      std::string value;
    } Minimum, Maximum;
  };

  std::optional<Limits_T> Limits;

  std::optional<std::string> XUnits;
  std::optional<std::string> DataValueType;
  std::optional<std::string> PrecisionType;
  std::optional<std::string> PrecisionDigits;
  std::optional<std::string> ConversionSupport;

  operator boost::property_tree::ptree() const;
};

struct IntegrationParameters
{
  struct ParameterSet_T
  {
    struct attrs_t
    {
      std::string usageKey = "";
      std::string assembly = "";
      std::string class_ =
          "Agilent.MassSpectrometry.DataAnalysis.PSetRTEIntegrator";
    } attrs;

    Parameter DataPointSampling = {
        .attrs = {.id = "DataPointSampling"},
        .DisplayName = "Data point sampling",
        .Value = "1",
        .Default = "1",
        .Limits = std::optional<Parameter::Limits_T>(
            {.Minimum = {.attr = {.type = "Inclusive"}, .value = "1"},
             .Maximum = {.attr = {.type = "Inclusive"}, .value = "9"}}),
        .XUnits = "Unspecified",
        .DataValueType = "Unspecified",
        .PrecisionType = "DigitsAfterDecimal",
        .PrecisionDigits = "2",
        .ConversionSupport = "None"};

    Parameter Smoothing = {
        .attrs = {.id = "Smoothing"},
        .DisplayName = "Smoothing",
        .Value = "false",
        .Default = "false",
    };

    Parameter DetectionFiltering = {
        .attrs = {.id = "DetectionFiltering"},
        .DisplayName = "Detection filtering",
        .Value = "FivePoint",
        .Default = "FivePoint",
    };

    Parameter StartThreshold = {
        .attrs = {.id = "StartThreshold"},
        .DisplayName = "Start threshold",
        .Value = "0.2",
        .Default = "0.2",
        .Limits = std::optional<Parameter::Limits_T>(
            {.Minimum = {.attr = {.type = "Inclusive"}, .value = "0.001"},
             .Maximum = {.attr = {.type = "Inclusive"}, .value = "100"}}),
        .XUnits = "Unspecified",
        .DataValueType = "Unspecified",
        .PrecisionType = "DigitsAfterDecimal",
        .PrecisionDigits = "2",
        .ConversionSupport = "None"};

    Parameter StopThreshold = {
        .attrs = {.id = "StopThreshold"},
        .DisplayName = "Stop threshold",
        .Value = "0",
        .Default = "0",
        .Limits = std::optional<Parameter::Limits_T>(
            {.Minimum = {.attr = {.type = "Inclusive"}, .value = "0"},
             .Maximum = {.attr = {.type = "Inclusive"}, .value = "100"}}),
        .XUnits = "Unspecified",
        .DataValueType = "Unspecified",
        .PrecisionType = "DigitsAfterDecimal",
        .PrecisionDigits = "2",
        .ConversionSupport = "None"};

    Parameter PeakLocation = {
        .attrs = {.id = "PeakLocation"},
        .DisplayName = "Peak Location",
        .Value = "Top",
        .Default = "Top",
    };

    Parameter BaselineReset = {
        .attrs = {.id = "BaselineReset"},
        .DisplayName = "Baseline reset (# points)",
        .Value = "5",
        .Default = "5",
        .Limits = std::optional<Parameter::Limits_T>(
            {.Minimum = {.attr = {.type = "Inclusive"}, .value = "0"},
             .Maximum = {.attr = {.type = "Inclusive"}, .value = "500"}}),
        .XUnits = "Unspecified",
        .DataValueType = "Unspecified",
        .PrecisionType = "DigitsAfterDecimal",
        .PrecisionDigits = "2",
        .ConversionSupport = "None"};

    Parameter BaselineValley = {
        .attrs = {.id = "BaselineValley"},
        .DisplayName = "If leading or trailing edge",
        .Value = "100",
        .Default = "100",
        .Limits = std::optional<Parameter::Limits_T>(
            {.Minimum = {.attr = {.type = "Inclusive"}, .value = "1"},
             .Maximum = {.attr = {.type = "Inclusive"}, .value = "100"}}),
        .XUnits = "Unspecified",
        .DataValueType = "Unspecified",
        .PrecisionType = "DigitsAfterDecimal",
        .PrecisionDigits = "2",
        .ConversionSupport = "None"};

    Parameter BaselinePreference = {
        .attrs = {.id = "BaselinePreference"},
        .DisplayName = "Baseline Preference",
        .Value = "Tangent",
        .Default = "Tangent",
    };

    operator boost::property_tree::ptree() const;

  } ParameterSet;

};  // struct IntegrationParameters

struct TargetCompound
{
  int BatchID = -1;
  int SampleID = -1;
  const unsigned int CompoundID;  // Set from import
  int AccuracyLimitMultiplierLOQ = 1;
  int AccuracyMaximumPercentDeviation = 20;
  std::string CompoundName;  // Set from import
  std::string CompoundType = "Target";
  std::string ConcentrationUnits = "ng/mL";
  std::string CurveFit = "fitLinear";
  std::string CurveFitOrigin = "originIgnore";
  std::string CurveFitWeight = "weightEqual";

  IntegrationParameters IntegrationParameters;

  bool IntegrationParametersModified = false;
  std::string Integrator = "Agile2";
  std::string IonPolarity = "Positive";
  bool ISTDFlag = false;
  float LeftRetentionTimeDelta = 1;
  unsigned int MaximumNumberOfHits = 1;
  float Multiplier = 1;
  float MZ;  // Set from import
  float MZExtractionWindowFilterLeft = 0.3f;
  float MZExtractionWindowFilterRight = 0.7f;
  std::string MZExtractionWindowUnits = "Thomsons";
  std::string NoiseAlgorithmType = "RMS";
  float NoiseOfRawSignal = 35.649809837341309f;
  std::string NoiseReference = "Blank";
  float NoiseStandardDeviationMultiplier = 5;
  std::string PeakFilterThreshold = "PeakAreaPctThreshold";
  float PeakFilterThresholdValue = 5;
  std::string PeakSelectionCriterion = "CloseRTQual";
  unsigned int PrimaryHitPeakID = 0;
  bool QuantitateByHeight = false;
  float RetentionTime;  // Set from import
  std::string RetentionTimeDeltaUnits = "Minutes";
  float RetentionTimeWindow = 10;
  std::string RetentionTimeWindowUnits = "Percent";
  float RightRetentionTimeDelta = 1;
  std::string ScanType = "Ms1Scan";
  float SelectedMZ = 0;
  std::string SignalType = "MS";
  std::string Smoothing = "SavitzkyGolay45";
  float SmoothingFunctionWidth = 15;
  float SmoothingGaussianWidth = 5;
  std::string SpectrumExtractionOverride = "Default";
  std::string SpectrumScanInclusion = "Average";
  unsigned int ThresholdNumberOfPeaks = 100;
  bool TimeReferenceFlag = false;
  unsigned int TimeSegment = 1;
  float Transition;  // Set from import
  std::string UncertaintyRelativeOrAbsolute = "Relative";

  operator boost::property_tree::ptree() const;
};

struct QuantitationDataSet
{
  QuantitationDataSet() = default;
  QuantitationDataSet(const Library& library);

  struct attrs
  {
    std::string SchemaVersion = "65586";
    std::string DataVersion = "0";
    std::string BatchState = "Initial";
    std::string ReferenceWindow = "80";
    std::string ReferenceWindowPercentOrMinutes = "Percent";
    std::string NonReferenceWindow = "200";
    std::string NonReferenceWindowPercentOrMinutes = "Percent";
    std::string CorrelationWindow = "0.5";
    bool ApplyMultiplierTarget = true;
    bool ApplyMultiplierSurrogate = true;
    bool ApplyMultiplierMatrixSpike = true;
    bool ApplyMultiplierISTD = false;
    bool IgnorePeaksNotFound = false;
    bool RelativeISTD = false;
    bool AuditTrail = false;
    std::string LibraryPathFileName = "C:/Database/NIST08.L";
    std::string LibraryMethodPathFileName = "";
    std::string RefLibraryPathFileName = "";
    std::string RefLibraryPatternPathFileName = "";
    std::string CCMaximumElapsedTimeInHours = "0";
    std::string BracketingType = "None";
    std::string StandardAddition = "false";
    bool DynamicBackgroundSubtraction = false;
    std::string BatchName = "";
    std::string BatchDataPathFileName = "";
    std::string DAMethodPathFileNameOrigin = "";
    std::string AnalystName = "";
    std::string ReportGeneratorName = "";
    std::string AnalysisTimeStamp = "0001-01-01T00:00:00+00:00";
    std::string DAMethodLastAppliedTimeStamp = "0001-01-01T00:00:00+00:00";
    std::string CalibrationLastUpdatedTimeStamp = "0001-01-01T00:00:00+00:00";
    std::string ReportGenerationStartedTimeStamp = "0001-01-01T00:00:00+00:00";
    std::string ReportResultsDataPathFileName = "";
    std::string AnalyzeQuantVersion = "";
    std::string ReportQuantVersion = "";
    std::string ComplianceName = "None";
    std::string ComplianceVersion = "";
    std::string ComplianceServer = "";
    std::string FeatureDetection = "false";
    std::string HashCode = "";
    std::string xmlns = "Quantitation";
  } attr;

  std::vector<TargetCompound> Targets;

  void addTarget(const Compound& compound);
  // Qualifiers

  operator boost::property_tree::ptree() const;
};

} // namespace LIB_NAMESPACE

#endif // LIB_MODELS_METHOD_HPP
