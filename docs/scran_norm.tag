<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>center_size_factors.hpp</name>
    <path>scran_norm/</path>
    <filename>center__size__factors_8hpp.html</filename>
    <includes id="sanitize__size__factors_8hpp" name="sanitize_size_factors.hpp" local="yes" import="no" module="no" objc="no">sanitize_size_factors.hpp</includes>
    <class kind="struct">scran_norm::ComputeMeanSizeFactorOptions</class>
    <class kind="struct">scran_norm::CenterSizeFactorsOptions</class>
    <class kind="struct">scran_norm::CenterSizeFactorsBlockedOptions</class>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="file">
    <name>center_spike_in_factors.hpp</name>
    <path>scran_norm/</path>
    <filename>center__spike__in__factors_8hpp.html</filename>
    <includes id="center__size__factors_8hpp" name="center_size_factors.hpp" local="yes" import="no" module="no" objc="no">center_size_factors.hpp</includes>
    <class kind="struct">scran_norm::CenterSpikeInFactorsOptions</class>
    <class kind="struct">scran_norm::CenterSpikeInFactorsBlockedOptions</class>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="file">
    <name>choose_pseudo_count.hpp</name>
    <path>scran_norm/</path>
    <filename>choose__pseudo__count_8hpp.html</filename>
    <class kind="struct">scran_norm::ChoosePseudoCountOptions</class>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="file">
    <name>normalize_counts.hpp</name>
    <path>scran_norm/</path>
    <filename>normalize__counts_8hpp.html</filename>
    <class kind="class">scran_norm::DelayedLogNormalizeHelper</class>
    <class kind="struct">scran_norm::NormalizeCountsOptions</class>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="file">
    <name>sanitize_size_factors.hpp</name>
    <path>scran_norm/</path>
    <filename>sanitize__size__factors_8hpp.html</filename>
    <class kind="struct">scran_norm::SizeFactorDiagnostics</class>
    <class kind="struct">scran_norm::SanitizeSizeFactorsOptions</class>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="file">
    <name>scran_norm.hpp</name>
    <path>scran_norm/</path>
    <filename>scran__norm_8hpp.html</filename>
    <includes id="center__size__factors_8hpp" name="center_size_factors.hpp" local="yes" import="no" module="no" objc="no">center_size_factors.hpp</includes>
    <includes id="center__spike__in__factors_8hpp" name="center_spike_in_factors.hpp" local="yes" import="no" module="no" objc="no">center_spike_in_factors.hpp</includes>
    <includes id="choose__pseudo__count_8hpp" name="choose_pseudo_count.hpp" local="yes" import="no" module="no" objc="no">choose_pseudo_count.hpp</includes>
    <includes id="normalize__counts_8hpp" name="normalize_counts.hpp" local="yes" import="no" module="no" objc="no">normalize_counts.hpp</includes>
    <namespace>scran_norm</namespace>
  </compound>
  <compound kind="struct">
    <name>scran_norm::CenterSizeFactorsBlockedOptions</name>
    <filename>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</anchorfile>
      <anchor>ac2d6fe94f34c91c48b18eb52013fbad3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SizeFactorDiagnostics *</type>
      <name>diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</anchorfile>
      <anchor>ae500637d1e5287f08067adc30b21d316</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CenterBlockMode</type>
      <name>block_mode</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</anchorfile>
      <anchor>a1f4f48369ecd42c68133beb469af8d23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::optional&lt; std::vector&lt; double &gt; &gt;</type>
      <name>custom_centers</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</anchorfile>
      <anchor>a1dd3a6c3050f3d7b6f7b6f0d7ab2b458</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>report_final</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsBlockedOptions.html</anchorfile>
      <anchor>af405df200388a16da7499e39287ca76f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::CenterSizeFactorsOptions</name>
    <filename>structscran__norm_1_1CenterSizeFactorsOptions.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a09be0c48d2af10b0975c95b4f4ffa6cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>center</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a2dfa25bb5094e18b536180a27efed78b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SizeFactorDiagnostics *</type>
      <name>diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a3201cbdb4dccf6a151c9cfa93f2f3950</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>report_final</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a260b0a1709fc79318ea1b16b81e11abe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::CenterSpikeInFactorsBlockedOptions</name>
    <filename>structscran__norm_1_1CenterSpikeInFactorsBlockedOptions.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsBlockedOptions.html</anchorfile>
      <anchor>a84cdc01f67e9f4db7f5bf10bbba19ff0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SizeFactorDiagnostics *</type>
      <name>endogenous_diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsBlockedOptions.html</anchorfile>
      <anchor>a18e9f7472f6da44577de08d5e34c9378</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; SizeFactorDiagnostics * &gt;</type>
      <name>spike_in_diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsBlockedOptions.html</anchorfile>
      <anchor>a1ab5500edb8fa47740d9383159b5ad2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CenterBlockMode</type>
      <name>block_mode</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsBlockedOptions.html</anchorfile>
      <anchor>a9b9321549ac031a2c1b61d91055bdc48</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::CenterSpikeInFactorsOptions</name>
    <filename>structscran__norm_1_1CenterSpikeInFactorsOptions.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsOptions.html</anchorfile>
      <anchor>ad0b361d6c2352c7049a43cc1bf865ff1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SizeFactorDiagnostics *</type>
      <name>endogenous_diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsOptions.html</anchorfile>
      <anchor>ab941e327bdfc194f0ce9cd0d11d4f2da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; SizeFactorDiagnostics * &gt;</type>
      <name>spike_in_diagnostics</name>
      <anchorfile>structscran__norm_1_1CenterSpikeInFactorsOptions.html</anchorfile>
      <anchor>a720ee8eb759293bf22eccb0f25f5905d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::ChoosePseudoCountOptions</name>
    <filename>structscran__norm_1_1ChoosePseudoCountOptions.html</filename>
    <member kind="variable">
      <type>double</type>
      <name>quantile</name>
      <anchorfile>structscran__norm_1_1ChoosePseudoCountOptions.html</anchorfile>
      <anchor>aa87301acfa85174d8e349c95122730de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>max_bias</name>
      <anchorfile>structscran__norm_1_1ChoosePseudoCountOptions.html</anchorfile>
      <anchor>ac5d2757a9c6bdf790ee4a9007cba4938</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>min_value</name>
      <anchorfile>structscran__norm_1_1ChoosePseudoCountOptions.html</anchorfile>
      <anchor>acc819b9b63b9f1781a8bc70173912df2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::ComputeMeanSizeFactorOptions</name>
    <filename>structscran__norm_1_1ComputeMeanSizeFactorOptions.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1ComputeMeanSizeFactorOptions.html</anchorfile>
      <anchor>a5f1d2ca0728cc73f406acddcdc5c9f63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SizeFactorDiagnostics *</type>
      <name>diagnostics</name>
      <anchorfile>structscran__norm_1_1ComputeMeanSizeFactorOptions.html</anchorfile>
      <anchor>aa107dbf4abb80fc9ffa76e3df36cee15</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>scran_norm::DelayedLogNormalizeHelper</name>
    <filename>classscran__norm_1_1DelayedLogNormalizeHelper.html</filename>
    <templarg>typename OutputValue_</templarg>
    <templarg>typename InputValue_</templarg>
    <templarg>typename Index_</templarg>
    <templarg>typename ReciprocalSizeFactors_</templarg>
    <base>tatami::DelayedUnaryIsometricOperationHelper&lt; OutputValue_, InputValue_, Index_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>DelayedLogNormalizeHelper</name>
      <anchorfile>classscran__norm_1_1DelayedLogNormalizeHelper.html</anchorfile>
      <anchor>afda033fbeb0d3fe4bf2ca3c4261c0d94</anchor>
      <arglist>(ReciprocalSizeFactors_ reciprocal_size_factors, OutputValue_ log_base, OutputValue_ pseudo_count)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::NormalizeCountsOptions</name>
    <filename>structscran__norm_1_1NormalizeCountsOptions.html</filename>
    <member kind="variable">
      <type>double</type>
      <name>pseudo_count</name>
      <anchorfile>structscran__norm_1_1NormalizeCountsOptions.html</anchorfile>
      <anchor>a3744450b615ed684b641ab552fa289db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>preserve_sparsity</name>
      <anchorfile>structscran__norm_1_1NormalizeCountsOptions.html</anchorfile>
      <anchor>ace588a078064faab63a5bef8fc665077</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>log</name>
      <anchorfile>structscran__norm_1_1NormalizeCountsOptions.html</anchorfile>
      <anchor>a69a9473da69f8b86ba73b4b800a1268b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>log_base</name>
      <anchorfile>structscran__norm_1_1NormalizeCountsOptions.html</anchorfile>
      <anchor>a041c9ac7344203670564b9cdf99523da</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::SanitizeSizeFactorsOptions</name>
    <filename>structscran__norm_1_1SanitizeSizeFactorsOptions.html</filename>
    <member kind="variable">
      <type>SanitizeAction</type>
      <name>handle_zero</name>
      <anchorfile>structscran__norm_1_1SanitizeSizeFactorsOptions.html</anchorfile>
      <anchor>a341805c7650ee93db13050621d1505d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SanitizeAction</type>
      <name>handle_negative</name>
      <anchorfile>structscran__norm_1_1SanitizeSizeFactorsOptions.html</anchorfile>
      <anchor>a7ac9d4c7ee89bc43920abfb1bf34dfdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SanitizeAction</type>
      <name>handle_nan</name>
      <anchorfile>structscran__norm_1_1SanitizeSizeFactorsOptions.html</anchorfile>
      <anchor>a50a3b186d5c6a430dfa4dce137c88372</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SanitizeAction</type>
      <name>handle_infinite</name>
      <anchorfile>structscran__norm_1_1SanitizeSizeFactorsOptions.html</anchorfile>
      <anchor>acaf0c77542462c508c14c30994e8d07d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>scran_norm::SizeFactorDiagnostics</name>
    <filename>structscran__norm_1_1SizeFactorDiagnostics.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>has_negative</name>
      <anchorfile>structscran__norm_1_1SizeFactorDiagnostics.html</anchorfile>
      <anchor>a3832be287c812e130bb4e54683c08dc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>has_zero</name>
      <anchorfile>structscran__norm_1_1SizeFactorDiagnostics.html</anchorfile>
      <anchor>aa4e63c1414f0365451ab0ff0d5f2e060</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>has_nan</name>
      <anchorfile>structscran__norm_1_1SizeFactorDiagnostics.html</anchorfile>
      <anchor>aa901ac740808328911adc4ad4f35ce14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>has_infinite</name>
      <anchorfile>structscran__norm_1_1SizeFactorDiagnostics.html</anchorfile>
      <anchor>a64441d7a301e3abe3dc3d09500fe5054</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>scran_norm</name>
    <filename>namespacescran__norm.html</filename>
    <class kind="struct">scran_norm::CenterSizeFactorsBlockedOptions</class>
    <class kind="struct">scran_norm::CenterSizeFactorsOptions</class>
    <class kind="struct">scran_norm::CenterSpikeInFactorsBlockedOptions</class>
    <class kind="struct">scran_norm::CenterSpikeInFactorsOptions</class>
    <class kind="struct">scran_norm::ChoosePseudoCountOptions</class>
    <class kind="struct">scran_norm::ComputeMeanSizeFactorOptions</class>
    <class kind="class">scran_norm::DelayedLogNormalizeHelper</class>
    <class kind="struct">scran_norm::NormalizeCountsOptions</class>
    <class kind="struct">scran_norm::SanitizeSizeFactorsOptions</class>
    <class kind="struct">scran_norm::SizeFactorDiagnostics</class>
    <member kind="enumeration">
      <type></type>
      <name>CenterBlockMode</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a47460312428a74877116e789da012aba</anchor>
      <arglist></arglist>
      <enumvalue file="namespacescran__norm.html" anchor="a47460312428a74877116e789da012abaa9e73aef66b430ace3684865eb71cbbf3">PER_BLOCK</enumvalue>
      <enumvalue file="namespacescran__norm.html" anchor="a47460312428a74877116e789da012abaa664626f39fdbfdb9cfe98285a9e161f9">LOWEST</enumvalue>
      <enumvalue file="namespacescran__norm.html" anchor="a47460312428a74877116e789da012abaa72baef04098f035e8a320b03ad197818">CUSTOM</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>SanitizeAction</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a946ecb96a14f7df7cd0d5be4599766d2</anchor>
      <arglist></arglist>
      <enumvalue file="namespacescran__norm.html" anchor="a946ecb96a14f7df7cd0d5be4599766d2aa2e843feab94ef623fea888f07c28696">IGNORE</enumvalue>
      <enumvalue file="namespacescran__norm.html" anchor="a946ecb96a14f7df7cd0d5be4599766d2abb1ca97ec761fc37101737ba0aa2e7c5">ERROR</enumvalue>
      <enumvalue file="namespacescran__norm.html" anchor="a946ecb96a14f7df7cd0d5be4599766d2a5c9ebeb2c005fca935ce86e3de8c7142">SANITIZE</enumvalue>
    </member>
    <member kind="function">
      <type>SizeFactor_</type>
      <name>compute_mean_size_factor</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a6683f56e7b34b29bfcf1c373b332fe0f</anchor>
      <arglist>(const std::size_t num_cells, const SizeFactor_ *const size_factors, const ComputeMeanSizeFactorOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; SizeFactor_ &gt;</type>
      <name>compute_mean_size_factor_blocked</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a82a42100f46ec5f59f0eb508f131ef3e</anchor>
      <arglist>(const std::size_t num_cells, const SizeFactor_ *const size_factors, const Block_ *const block, const std::size_t num_blocks, const ComputeMeanSizeFactorOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>SizeFactor_</type>
      <name>center_size_factors</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a353713e8b5bdf4efad1c678cc2307516</anchor>
      <arglist>(const std::size_t num_cells, SizeFactor_ *const size_factors, const CenterSizeFactorsOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; SizeFactor_ &gt;</type>
      <name>center_size_factors_blocked</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a36ca8a8e37f0ddd646f1dfc508208e68</anchor>
      <arglist>(const std::size_t num_cells, SizeFactor_ *const size_factors, const Block_ *const block, const std::size_t num_blocks, const CenterSizeFactorsBlockedOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>center_spike_in_factors</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a422c682014542530ce017311cabb11dd</anchor>
      <arglist>(const std::size_t num_cells, SizeFactor_ *const endogenous, const std::vector&lt; SizeFactor_ * &gt; &amp;spike_ins, const CenterSpikeInFactorsOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; SizeFactor_ &gt;</type>
      <name>center_spike_in_factors_blocked</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>ab3a00b25a3333303fde65aa27a182ba8</anchor>
      <arglist>(const std::size_t num_cells, SizeFactor_ *const endogenous, const std::vector&lt; SizeFactor_ * &gt; &amp;spike_ins, const Block_ *const block, const std::size_t num_blocks, const CenterSpikeInFactorsBlockedOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>Float_</type>
      <name>choose_pseudo_count_raw</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a9dd11205d7cc41b69a8f9fbc16d3081b</anchor>
      <arglist>(const std::size_t num, Float_ *const size_factors, const ChoosePseudoCountOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>Float_</type>
      <name>choose_pseudo_count</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a37205f65b305106aeb9902b1ba4e4043</anchor>
      <arglist>(const std::size_t num, const Float_ *const size_factors, const ChoosePseudoCountOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::shared_ptr&lt; tatami::Matrix&lt; OutputValue_, Index_ &gt; &gt;</type>
      <name>normalize_counts</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a141ebdc51024210f4c27902446fa4169</anchor>
      <arglist>(std::shared_ptr&lt; const tatami::Matrix&lt; InputValue_, Index_ &gt; &gt; counts, SizeFactors_ size_factors, const NormalizeCountsOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>SizeFactorDiagnostics</type>
      <name>check_size_factor_sanity</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a80034f4d92ab790c59098c46b5d4f1b2</anchor>
      <arglist>(const std::size_t num, const SizeFactor_ *const size_factors)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sanitize_size_factors</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a57131d1a4d08f8b505a5597f44827c85</anchor>
      <arglist>(const std::size_t num, SizeFactor_ *const size_factors, const SizeFactorDiagnostics &amp;status, const SanitizeSizeFactorsOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>SizeFactorDiagnostics</type>
      <name>sanitize_size_factors</name>
      <anchorfile>namespacescran__norm.html</anchorfile>
      <anchor>a2e7ace5b998fc48cf816d1aa0552f41d</anchor>
      <arglist>(const std::size_t num, SizeFactor_ *const size_factors, const SanitizeSizeFactorsOptions &amp;options)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Scaling normalization of single-cell count data</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
