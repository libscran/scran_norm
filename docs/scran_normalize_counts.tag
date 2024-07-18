<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="file">
    <name>center_size_factors.hpp</name>
    <path>scran_norm/</path>
    <filename>center__size__factors_8hpp.html</filename>
    <includes id="sanitize__size__factors_8hpp" name="sanitize_size_factors.hpp" local="yes" import="no" module="no" objc="no">sanitize_size_factors.hpp</includes>
    <class kind="struct">scran_norm::CenterSizeFactorsOptions</class>
  </compound>
  <compound kind="file">
    <name>choose_pseudo_count.hpp</name>
    <path>scran_norm/</path>
    <filename>choose__pseudo__count_8hpp.html</filename>
    <class kind="struct">scran_norm::ChoosePseudoCountOptions</class>
  </compound>
  <compound kind="file">
    <name>sanitize_size_factors.hpp</name>
    <path>scran_norm/</path>
    <filename>sanitize__size__factors_8hpp.html</filename>
    <class kind="struct">scran_norm::SizeFactorDiagnostics</class>
    <class kind="struct">scran_norm::SanitizeSizeFactorsOptions</class>
  </compound>
  <compound kind="file">
    <name>scran_norm.hpp</name>
    <path>scran_norm/</path>
    <filename>scran__norm_8hpp.html</filename>
    <includes id="center__size__factors_8hpp" name="center_size_factors.hpp" local="yes" import="no" module="no" objc="no">center_size_factors.hpp</includes>
    <includes id="choose__pseudo__count_8hpp" name="choose_pseudo_count.hpp" local="yes" import="no" module="no" objc="no">choose_pseudo_count.hpp</includes>
    <namespace>scran</namespace>
  </compound>
  <compound kind="struct">
    <name>scran_norm::CenterSizeFactorsOptions</name>
    <filename>structscran__norm_1_1CenterSizeFactorsOptions.html</filename>
    <member kind="variable">
      <type>CenterBlockMode</type>
      <name>block_mode</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a03777fea5bdfc1f386a60a88470e3422</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>ignore_invalid</name>
      <anchorfile>structscran__norm_1_1CenterSizeFactorsOptions.html</anchorfile>
      <anchor>a09be0c48d2af10b0975c95b4f4ffa6cc</anchor>
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
    <name>normalize_counts::hpp</name>
    <filename>namespacenormalize__counts_1_1hpp.html</filename>
  </compound>
  <compound kind="namespace">
    <name>scran</name>
    <filename>namespacescran.html</filename>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Scaling normalization of single-cell count data</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
