var UppercaseFilter = function(input)
{
  var input = input.rawString();
  return input.toUpperCase();
};

UppercaseFilter.filterName = "uppercase";

Library.addFilter("UppercaseFilter");

