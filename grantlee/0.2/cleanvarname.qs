var CleanVarnameFilter = function(input)
{
  var input = input.rawString();
  var output = input.trim().replace(/[^\w\d]/, '_');
  if (!output[0].match(/\w/)) {
    output = '_' + output;
  }
  return output;
};

CleanVarnameFilter.filterName = "cleanvarname";

Library.addFilter("CleanVarnameFilter");

