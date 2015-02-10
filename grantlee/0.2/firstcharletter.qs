var FirstCharLetterFilter = function(input)
{
  var output = input.rawString();
  if (!output[0].match(/\w/)) {
    output = '_' + output;
  }
  return output;
};

FirstCharLetterFilter.filterName = "firstcharletter";

Library.addFilter("FirstCharLetterFilter");

